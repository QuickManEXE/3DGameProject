#include "CModelField.h"
#include "CCollision.h"
#include "CCamera.h"
#ifndef USE_THREAD_CLASS
#include <process.h>
#endif
#include <functional>
template <typename T> static T _clamp(T n,T min, T max) {
	if (n < min) n = min;
	if (n > max) n = max;
	return n;
}
CFieldPoly::CFieldPoly() : m_pVertex(NULL), m_colmask(NULL), m_colIdx(NULL), colNormal(NULL),m_buffer(0), m_vao(0){

}
CFieldPoly::~CFieldPoly() {
	if (m_colIdx) {
		delete[] m_colIdx;
		m_colIdx = NULL;
	}
	if (colNormal) {
		delete[] colNormal;
		colNormal = NULL;
	}
	if (m_buffer > 0) {
		glDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
	if (m_vao > 0) {
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_pVertex) delete[] m_pVertex;
	m_pVertex = NULL;
	if (m_colmask) delete m_colmask;
}

void CFieldPoly::Load(FILE *fp){
	fread(&m_vertexCnt, sizeof(unsigned long), 1, fp);
	if (m_vertexCnt <= 0) return;
	m_pVertex = new SFieldVertex[m_vertexCnt];
	fread(m_pVertex, sizeof(SFieldVertex), m_vertexCnt, fp);
	fread(&m_cut, sizeof(unsigned long), 1, fp);
	m_colIdx = new CFieldColIndex[m_cut*m_cut];
	
	colNormal = new CVector3D[m_vertexCnt/3];
	SFieldVertex *v = m_pVertex;
	for (unsigned int i = 0; i < m_vertexCnt / 3; i++,v+=3) {
		colNormal[i] = CVector3D::Cross(v[1].m_vertex - v[0].m_vertex, v[2].m_vertex - v[0].m_vertex);
		if (colNormal[i].LengthSq() <= 0) continue;
		colNormal[i].Normalize();

	
	}
	for (unsigned int i = 0; i < m_cut*m_cut; i++) {

		fread(&m_colIdx[i].m_size, sizeof(unsigned long), 1, fp);
		if (m_colIdx[i].m_size > 0) {
			m_colIdx[i].m_idx = new unsigned long[m_colIdx[i].m_size];
			fread(m_colIdx[i].m_idx, sizeof(unsigned long), m_colIdx[i].m_size, fp);
		}
	}
	m_colmask = new unsigned long[(m_vertexCnt / 3 / 32) + 1];

	

}
void CFieldPoly::Render(CShader *shader){
	if (m_vertexCnt <= 0) return;
	int offSet=0;
	if (m_buffer==0) {
		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SFieldVertex) * m_vertexCnt, m_pVertex, GL_STATIC_DRAW);
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		
		GLint vertLoc = glGetAttribLocation(shader->GetProgram(), "Vertex");
		glEnableVertexAttribArray(vertLoc);
		glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(SFieldVertex), (const void*)offSet);
		offSet += sizeof(CVector3D);

		GLint normalLoc = glGetAttribLocation(shader->GetProgram(), "Normal");
		glEnableVertexAttribArray(normalLoc);
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(SFieldVertex), (const void*)offSet);
		offSet += sizeof(CVector3D);

		GLint texLoc = glGetAttribLocation(shader->GetProgram(), "TexCoord");
		glEnableVertexAttribArray(texLoc);
		glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(SFieldVertex), (const void*)offSet);
		offSet += sizeof(CVector2D);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexCnt);

	glBindVertexArray(0);

}

bool CFieldPoly::CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e, float *pLength, int raws, int rawe, int cols, int cole){

	if (m_buffer == 0) return false;
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	SFieldVertex *v = (SFieldVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	//SFieldVertex *v = m_pVertex;
	memset(m_colmask, 0, sizeof(unsigned long)*((m_vertexCnt / 3 / 32) + 1));
	

	bool hit = false;

	for (int lineZ = cols; lineZ <= cole; lineZ++) {
		for (int lineX = raws; lineX <= rawe; lineX++) {
			int k = lineZ*m_cut + lineX;
			for (unsigned int i = 0; i < m_colIdx[k].m_size; i++){
				int idx = m_colIdx[k].m_idx[i];
				if (m_colmask[idx / 32] & (1 << (idx % 32))) continue;
				if (colNormal[idx].LengthSq() <= 0) continue;		
				if (CCollision::IntersectTriangleRay(c, s, e, v[idx * 3].m_vertex, v[idx * 3 + 1].m_vertex, v[idx * 3 + 2].m_vertex, colNormal[idx],pLength)){

					
					*n = colNormal[idx];
					hit = true;
				}	

				m_colmask[idx / 32] |= 1 << (idx % 32);
			}

		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return hit;

}
int CFieldPoly::CollisionSphere(CCollTriangle *out, const CVector3D &center, float radius, int maxcnt, int raws, int rawe, int cols, int cole, int matNo){
	if (m_buffer == 0) return 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	SFieldVertex *v = (SFieldVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	//SFieldVertex *v = m_pVertex;
	memset(m_colmask, 0, sizeof(unsigned long)*((m_vertexCnt / 3 / 32) + 1));
	
	int cnt = 0;
	float dist;
	for (int lineZ = cols; lineZ <= cole; lineZ++) {
		for (int lineX = raws; lineX <= rawe; lineX++) {
			int k = lineZ*m_cut + lineX;
			for (unsigned int i = 0; i < m_colIdx[k].m_size; i++){
				int idx = m_colIdx[k].m_idx[i];
				if (m_colmask[idx / 32] & (1 << (idx % 32))) continue;
				if (colNormal[idx].LengthSq() <= 0) continue;
				if (CCollision::CollisionTriangleSphere(v[idx * 3].m_vertex, v[idx * 3 + 1].m_vertex, v[idx * 3 + 2].m_vertex, colNormal[idx], center, radius, NULL, &dist)) {
					out->m_dist = dist;
					out->m_material_id = matNo;
					out->m_normal = colNormal[idx];
					out->m_vertex[0] = v[idx * 3].m_vertex;
					out->m_vertex[1] = v[idx * 3+1].m_vertex;
					out->m_vertex[2] = v[idx * 3+2].m_vertex;

					out++;
					cnt++;
					if (cnt >= maxcnt) {
						return cnt;
						glUnmapBuffer(GL_ARRAY_BUFFER);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
					}
				}
				m_colmask[idx / 32] |= 1 << (idx % 32);
			}
		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return cnt;
}

int CFieldPoly::CollisionCupsel(CCollTriangle *out, const CVector3D &top, const CVector3D &bottom, float radius, int maxcnt, int raws, int rawe, int cols, int cole, int matNo){
	if (m_buffer == 0) return 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	SFieldVertex *v = (SFieldVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	//SFieldVertex *v = m_pVertex;
	
	int cnt = 0;
	memset(m_colmask, 0, sizeof(unsigned long)*((m_vertexCnt / 3 / 32) + 1));
	

	float dist;
	for (int lineZ = cols; lineZ <= cole; lineZ++) {
		for (int lineX = raws; lineX <= rawe; lineX++) {
			int k = lineZ*m_cut + lineX;
			for (unsigned int i = 0; i < m_colIdx[k].m_size; i++){
				int idx = m_colIdx[k].m_idx[i];
				if (m_colmask[idx / 32] & (1 << (idx % 32))) continue;
				if (colNormal[idx].LengthSq() <= 0) continue;
				if (CCollision::CollisionTriangleCapsule(v[idx * 3].m_vertex, v[idx * 3 + 1].m_vertex, v[idx * 3 + 2].m_vertex, colNormal[idx], top, bottom, radius, NULL, &dist)) {
					out->m_dist = dist;
					out->m_material_id = matNo;
					out->m_normal = colNormal[idx];
					out->m_vertex[0] = v[idx * 3].m_vertex;
					out->m_vertex[1] = v[idx * 3 + 1].m_vertex;
					out->m_vertex[2] = v[idx * 3 + 2].m_vertex;

					out++;
					cnt++;
					if (cnt >= maxcnt) {
						return cnt;
						glUnmapBuffer(GL_ARRAY_BUFFER);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
					}
				}
				m_colmask[idx / 32] |= 1 << (idx % 32);
			}
		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return cnt;

}
void CFieldMesh::Load(FILE *fp){
	fread(&m_min, sizeof(CVector3D), 1, fp);
	fread(&m_max, sizeof(CVector3D), 1, fp);
	fread(&m_polyCnt, sizeof(unsigned long), 1, fp);
	m_poly = new CFieldPoly[m_polyCnt];
	for (unsigned int i = 0; i < m_polyCnt; i++)
		m_poly[i].Load(fp);
	m_load = true;
//	printf("---load\n");
}
void CFieldMesh::Render(CShader *shader, int matNo){
	if (!m_load) return;
	m_poly[matNo].Render(shader);

}

bool CFieldMesh::CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e, const CVector3D &dir,const CVector3D &box_min, const CVector3D &box_max, float *pLength, const SFieldModelHeader &header){
	if (!m_load) return false;
	
	if (!CCollision::CollisionAABBRay(NULL,NULL,m_min, m_max,s, dir)) return false;
	bool hit = false;



	int raws = (int)((box_min.x - m_min.x) / header.colCutLength);
	int rawe = (int)((box_max.x - m_min.x) / header.colCutLength);
	int cole = (int)((box_max.z - m_min.z) / header.colCutLength);
	int cols = (int)((box_min.z - m_min.z) / header.colCutLength);

	raws = _clamp(raws, 0, (int)header.colCut-1);
	rawe = _clamp(rawe, 0, (int)header.colCut-1);
	cols = _clamp(cols, 0, (int)header.colCut-1);
	cole = _clamp(cole, 0, (int)header.colCut-1);

	for (unsigned int i = 0; i < m_polyCnt; i++) {

		hit |= m_poly[i].CollisionRay(c, n, s, e, pLength, raws, rawe, cols, cole);
	}
	return hit;

}
int CFieldMesh::CollisionSphere(CCollTriangle *out, const CVector3D &center, float radius, int maxcnt, const SFieldModelHeader &header) {
	if (!m_load) return 0;
	float l = center.x - radius;
	float r = center.x + radius;
	float t = center.z + radius;
	float b = center.z - radius;

	int raws = (int)((l - m_min.x) / header.colCutLength);
	int rawe = (int)((r - m_min.x) / header.colCutLength);

	int cole = (int)((t - m_min.z) / header.colCutLength);
	int cols = (int)((b - m_min.z) / header.colCutLength);


	raws = _clamp(raws, 0, (int)header.colCut - 1);
	rawe = _clamp(rawe, 0, (int)header.colCut - 1);
	cols = _clamp(cols, 0, (int)header.colCut - 1);
	cole = _clamp(cole, 0, (int)header.colCut - 1);

	int cnt = 0;
	for (unsigned int i = 0; i < m_polyCnt; i++) {
		int c = m_poly[i].CollisionSphere(out, center, radius, maxcnt, raws, rawe, cols, cole,i);
		out += c;
		cnt += c;
		if (cnt >= maxcnt) break;
	}
	return cnt;

}
int CFieldMesh::CollisionCupsel(CCollTriangle *out, const CVector3D &top, const  CVector3D &bottom, float radius, int maxcnt, const SFieldModelHeader &header){
	if (!m_load) return 0;
	int cnt = 0;
	float l = min(top.x, bottom.x) - radius;
	float r = max(top.x, bottom.x) + radius;
	float t = max(top.z, bottom.z) + radius;
	float b = min(top.z, bottom.z) - radius;

	int raws = (int)((l - m_min.x) / header.colCutLength);
	int rawe = (int)((r - m_min.x) / header.colCutLength);
	int cole = (int)((t - m_min.z) / header.colCutLength);
	int cols = (int)((b - m_min.z) / header.colCutLength);


	raws = _clamp(raws, 0, (int)header.colCut - 1);
	rawe = _clamp(rawe, 0, (int)header.colCut - 1);
	cols = _clamp(cols, 0, (int)header.colCut - 1);
	cole = _clamp(cole, 0, (int)header.colCut - 1);

	for (unsigned int i = 0; i < m_polyCnt; i++) {
		int c = m_poly[i].CollisionCupsel(out, top, bottom, radius, maxcnt, raws, rawe, cols, cole,i);
		out += c;
		cnt += c;
		if (cnt >= maxcnt) break;
	}
	return cnt;


}
void CFieldMesh::Release(){
	if (!m_load) return;
	if (m_poly) delete[] m_poly;
	m_poly = NULL;
	m_load = false;
	//printf("---release\n");
}
CModelField::CModelField() : m_pMaterial(NULL), m_pMesh(NULL){
	
}


CModelField::~CModelField(){
	if (m_pLoadThread) {
		m_isDestroy = true;
#ifdef USE_THREAD_CLASS
		m_pLoadThread->join();
		delete m_pLoadThread;
		m_pLoadThread = nullptr;
#endif
	}
}

bool CModelField::Load(const char *path){
//	FILE* fp;
	strcpy_s(m_fileName, 256, path);
	MakePath(path);
	fopen_s(&m_fp, m_fileName, "rb");
	if (m_fp == NULL) {
		printf("ファイルがありません%s\n", m_fileName);
		return false;
	}
	fread(&m_header, sizeof(SFieldModelHeader), 1, m_fp);
	if (m_header.fileType[0] != 'F' || m_header.fileType[1] != 'M') {
		fclose(m_fp);
		printf("フィールドファイルではありません%s\n", path);
		return false;
	}
	if (m_header.verversion != 0x100) {
		fclose(m_fp);
		printf("バージョンが違います%s\n", path);
		return false;
	}
	
	m_offSet.offSet = new unsigned long[m_header.cutX*m_header.cutZ];
	fread(m_offSet.offSet, sizeof(unsigned long), m_header.cutX*m_header.cutZ, m_fp);

	m_materialCnt = m_header.materialCnt;
	SFieldMaterial *material;
	material = new SFieldMaterial[m_header.materialCnt];
	m_pMaterial = new CMaterial[m_header.materialCnt];
	fread(material, sizeof(SFieldMaterial), m_header.materialCnt, m_fp);
	for (unsigned int i = 0; i < m_header.materialCnt; i++) {
		SFieldMaterial	*m = &material[i];
		CMaterial *mat = &m_pMaterial[i];
		strcpy_s(mat->m_name, 128, m->m_name);
		mat->m_alpha = 1.0f;
		mat->m_ambient = m->m_ambient;
		mat->m_diffuse = m->m_diffuse;
		mat->m_specular = m->m_specular;
		mat->m_shininess = m->m_shininess;
		mat->m_emissive = m->m_emissive;
		strcpy_s(mat->m_texture_name, NAME_STR_SIZE, m->m_texName);
		if (strlen(m->m_texName)) {
			char str[PATH_SIZE];
			strcpy_s(str, PATH_SIZE, m_filePath);
			strcat_s(str, PATH_SIZE,m->m_texName);

			mat->m_pTex = new CTexture();
			mat->m_pTex->Load(str);
		} else {
			mat->m_pTex = NULL;
		}
		//printf("%s\n", m->m_name);
		//printf("%s\n", m->m_texName);
	}
	


	m_pMesh = new CFieldMesh[m_header.cutX*m_header.cutZ];
	delete[] material;


	m_shader = CShader::GetInstance("StaticMesh");

	m_posX = -1;
	m_posZ = -1;
	m_minX = 0;
	m_minZ = 0;
	m_maxX = -1;
	m_maxZ = -1;
	m_length = 200.0f;
	m_isLoading = false;
	m_isDestroy = false;
#ifdef USE_THREAD_CLASS
	m_pLoadThread = new std::thread(std::bind(&CModelField::LoadMeshProc, this));
#else
	m_threadHandle = (HANDLE)_beginthread(
		&CModelField::_loadThread,	// ランチャを起動
		0,
		this);
#endif

	return true;
}

CModelField::CModelField(const CModelField &f) {
	memcpy(this, &f, sizeof(f));
	m_pLoadThread = new std::thread(std::bind(&CModelField::LoadMeshProc, this));
}
bool CModelField::LoadMesh(int x, int z){
	if (!m_pMesh) return false;
	int idx = GetIdx(x, z);
	if(m_pMesh[idx].m_load) return false;
//	FILE* fp;
//	fopen_s(&fp, m_fileName, "rb");
//	if (fp == NULL) {
//		printf("ファイルがありません%s\n", m_fileName);
//		return false;
//	}
	fseek(m_fp, m_offSet.offSet[idx], SEEK_SET);
	m_pMesh[idx].Load(m_fp);
//	fclose(fp);

	return true;
}
void CModelField::LoadMeshProc(){
	while (!m_isDestroy) {
		Sleep(5);
		for (int x = m_minX; x <= m_maxX; x++){
			for (int z = m_minZ; z <= m_maxZ; z++){
				m_mutex.lock();
				int idx = GetIdx(x, z);
				LoadMesh(x, z);
				m_mutex.unlock();
			}
		}
	}

}

void CModelField::LoadMesh(CVector3D pos) {
	CalcRenderRange(pos);
	m_mutex.lock();
	for (int x = m_minX; x <= m_maxX; x++) {
		for (int z = m_minZ; z <= m_maxZ; z++) {
			int idx = GetIdx(x, z);
			LoadMesh(x, z);
		}
	}
	m_mutex.unlock();
}
CVector3D GetVertexN(const CVector3D &normal, CVector3D &min, CVector3D &max){
	CVector3D res = min;
	//軸境界ボックスの中で最小値を持つの頂点
	if (normal.x < 0)
		res.x = max.x;
	if (normal.y < 0)
		res.y = max.y;
	if (normal.z < 0)
		res.z = max.z;
	return(res);
}
CVector3D GetVertexP(const CVector3D &normal, CVector3D &min, CVector3D &max){
	CVector3D res = min;
	if (normal.x > 0)
		res.x = max.x;
	if (normal.y > 0)
		res.y = max.y;
	if (normal.z > 0)
		res.z = max.z;
	return(res);
}
int boxInFrustum(CVector3D &pos, CVector3D &normal,CVector3D &min, CVector3D &max) {


	CVector3D minv = min - pos;
	CVector3D maxv = max - pos;
	float t = CVector3D::Dot(minv, normal);
	float t2 = CVector3D::Dot(maxv, normal);
	if (t < 0 && t2 < 0) return false;
	return true;
}

void CModelField::CalcRenderRange(CVector3D center) {
	float showLength = m_length / m_scale.z;

	m_center = m_matrix.GetInverse() * CVector4D(center, 1);

	CVector3D c = m_center - m_header.min;
	m_posX = (int)(c.x / m_header.cutLength);
	m_posZ = (int)(c.z / m_header.cutLength);
	if (m_posX < 0) m_posX = 0;
	if (m_posZ < 0) m_posZ = 0;
	int minX = (int)((c.x - showLength) / m_header.cutLength);
	int maxX = (int)((c.x + showLength) / m_header.cutLength);
	int minZ = (int)((c.z - showLength) / m_header.cutLength);
	int maxZ = (int)((c.z + showLength) / m_header.cutLength);

	m_minX = _clamp(minX, 0, (int)m_header.cutX - 1);
	m_minZ = _clamp(minZ, 0, (int)m_header.cutZ - 1);
	m_maxX = _clamp(maxX, 0, (int)m_header.cutX - 1);
	m_maxZ = _clamp(maxZ, 0, (int)m_header.cutZ - 1);


}

void CModelField::Render() {
	CModel::UpdateMatrix();
	Render(m_matrix);
}
void CModelField::Render(CMatrix &m){
	m_mutex.lock();
//	while (m_isLoading) { Sleep(1); }
	m_shader->enable();
	SendShaderParam(m, CCamera::GetCurrent()->GetViewMatrix() * m, CCamera::GetCurrent()->GetProjectionMatrix());



	m_OldminX = m_minX;
	m_OldmaxX = m_maxX;
	m_OldminZ = m_minZ;
	m_OldmaxZ = m_maxZ;



	CalcRenderRange(CCamera::GetCurrent()->GetPos());
	CVector3D dir = m_matrix.GetInverse() * CCamera::GetCurrent()->GetDir();
	CVector3D c = m_center - m_header.min;
//	m_minX = 0;
//	m_minZ = 0;
//	m_maxX = m_header.cutX - 1;
//	m_maxZ = m_header.cutZ - 1;


	if (m_minX > m_OldminX) {
		for (int x = m_OldminX - 1; x >= 0 ; x--) {
			for (int z = m_OldminZ; z <= m_OldmaxZ; z++){
				int idx = GetIdx(x, z);
				m_pMesh[idx].Release();
			}
			break;//一列消したら終わり
		}
	}
	if (m_maxX < m_OldmaxX) {
		for (int x = m_OldmaxX + 1; x <(int)m_header.cutX; x++) {
			for (int z = m_OldmaxZ; z <= m_OldmaxZ; z++){
				int idx = GetIdx(x, z);
				m_pMesh[idx].Release();
			}
			break;//一列消したら終わり
		}
	}

	if (m_minZ > m_OldminZ) {
		for (int z = m_OldminZ - 1; z >= 0; z--) {
			for (int x = m_OldminX; x <= m_OldmaxX; x++){
				int idx = GetIdx(x, z);
				m_pMesh[idx].Release();
			}
			break;//一列消したら終わり
		}
	}
	if (m_maxZ < m_OldmaxZ) {
		for (int z = m_OldmaxZ + 1; z < (int)m_header.cutZ; z++) {
			for (int x = m_OldmaxX; x <= m_OldmaxX; x++){
				int idx = GetIdx(x, z);
				m_pMesh[idx].Release();
			}
			break;//一列消したら終わり
		}
	}


	//thr_main.join();
	
	/*
	for (int x = m_minX; x <= m_maxX; x++){
		for (int z = m_minZ; z <= m_maxZ; z++){
			int idx = GetIdx(x, z);
			LoadMesh(x, z);
		}
	}
	*/

	for (int i = 0; i < m_materialCnt; i++) {
		m_pMaterial[i].Map(m_shader);
		for (int x = m_minX; x <= m_maxX; x++){
			for (int z = m_minZ; z <= m_maxZ; z++){
				CVector3D max = GetVertexP(dir, CVector3D(x*m_header.cutLength, 0, z*m_header.cutLength), CVector3D((x+1)*m_header.cutLength, 1000, (z+1)*m_header.cutLength));
				CVector3D min = GetVertexN(dir, CVector3D(x*m_header.cutLength, 0, z*m_header.cutLength), CVector3D((x + 1)*m_header.cutLength, 1000, (z + 1)*m_header.cutLength));
				if(boxInFrustum(c,dir,min,max)) {
					int idx = GetIdx(x, z);
					m_pMesh[idx].Render(m_shader, i);
				}
			}
		}
		m_pMaterial[i].Unmap();
	}

	m_shader->disable();
	m_mutex.unlock();
}


void CModelField::Release(){
	m_isDestroy = true;
#ifdef USE_THREAD_CLASS
	m_pLoadThread->join();
	delete m_pLoadThread;
	m_pLoadThread = nullptr;
#else
	WaitForSingleObject(m_threadHandle, INFINITE); /* スレッドが終了するまで待つ。 */
#endif
	fclose(m_fp);

	//if (m_shader) { delete m_shader; m_shader = NULL; }
	for (int i = 0; i < m_materialCnt; i++) {
		m_pMaterial[i].Release();
	}
	delete[] m_pMaterial;
	delete[] m_pMesh;
	delete	m_offSet.offSet;
}

bool CModelField::CollisionRay(CVector3D *pCross, CVector3D *pNormal, const CVector3D &s, const CVector3D &e)const {
	CVector3D cross, normal;
	float length = 1e10;
	bool hit = false;
	CVector3D c, n;

	CMatrix inv = m_matrix.GetInverse();
	CVector3D s2 = CVector4D(s, 1)*inv;
	CVector3D e2 = CVector4D(e, 1)*inv;
	CVector3D dir = (e2 - s2).GetNormalize();



	CVector3D minV(fminf(s2.x, e2.x), 0, fminf(s2.z, e2.z));
	CVector3D maxV(fmaxf(s2.x, e2.x), 0, fmaxf(s2.z, e2.z));
	int minX = (int)((minV.x - m_header.min.x) / m_header.cutLength);
	int maxX = (int)((maxV.x  - m_header.min.x) / m_header.cutLength);
	int minZ = (int)((minV.z- m_header.min.z) / m_header.cutLength);
	int maxZ = (int)((maxV.z  - m_header.min.z) / m_header.cutLength);
	minX = _clamp(minX, 0, (int)m_header.cutX - 1);
	minZ = _clamp(minZ, 0, (int)m_header.cutZ - 1);
	maxX = _clamp(maxX, 0, (int)m_header.cutX - 1);
	maxZ = _clamp(maxZ, 0, (int)m_header.cutZ - 1);


	CVector3D box_max(max(s2.x, e2.x), max(s2.y, e2.y), max(s2.z, e2.z));
	CVector3D box_min(min(s2.x, e2.x), min(s2.y, e2.y), min(s2.z, e2.z));

	for (int x = minX; x <= maxX; x++) {
		for (int z = minZ; z <= maxZ; z++) {
			int idx = GetIdx(x, z);
			if (m_pMesh[idx].CollisionRay(&c, &n, s2, e2, dir,box_min, box_max,&length,m_header))
				hit = true;	
		}
	}
	if (hit){
		if (pCross) {
			CVector4D cross = m_matrix*CVector4D(c.x, c.y, c.z, 1);
			*pCross = CVector3D(cross.x, cross.y, cross.z);
		}
		if (pNormal) {
			CVector4D normal = m_matrix*CVector4D(n.x, n.y, n.z, 0);
			*pNormal = CVector3D(normal.x, normal.y, normal.z).GetNormalize();
		}
	}
	return hit;
}

int CModelField::CollisionSphere(CCollTriangle *out, const CVector3D &center, float radius, int maxcnt)const {

	float scale = m_matrix.GetFront().Length();
	int cnt = 0;
	CCollTriangle *o = out;
	CMatrix inv = m_matrix.GetInverse();
	CVector3D p = CVector4D(center,1)*inv;
	float r = radius / scale;

	CVector3D p2 = p - m_header.min;
	
	int minX = (int)((p2.x - r) / m_header.cutLength);
	int maxX = (int)((p2.x + r) / m_header.cutLength);
	int minZ = (int)((p2.z - r) / m_header.cutLength);
	int maxZ = (int)((p2.z + r) / m_header.cutLength);
	minX = _clamp(minX, 0, (int)m_header.cutX - 1);
	minZ = _clamp(minZ, 0, (int)m_header.cutZ - 1);
	maxX = _clamp(maxX, 0, (int)m_header.cutX - 1);
	maxZ = _clamp(maxZ, 0, (int)m_header.cutZ - 1);


	
	for (int x = minX; x <= maxX; x++) {
		for (int z = minZ; z <= maxZ; z++) {
			int idx = GetIdx(x, z);
			int c = m_pMesh[idx].CollisionSphere(o, p, r, maxcnt, m_header);
			o += c;
			cnt += c;
			if (cnt > maxcnt) break;
		}
	}

	o = out;
	for (int i = 0; i < cnt; i++, o++) {
		o->m_dist *= scale;

		o->Transform(m_matrix);

	}
	return cnt;

}

int CModelField::CollisionCupsel(CCollTriangle *out, const CVector3D &top, const CVector3D &bottom, float radius, int maxcnt)const {

	float scale = CVector3D(m_matrix.m00, m_matrix.m10, m_matrix.m20).Length();
	int cnt = 0;
	CCollTriangle *o = out;
	CMatrix inv = m_matrix.GetInverse();
	CVector3D t = CVector4D(top , 1) * inv;
	CVector3D b = CVector4D(bottom, 1) * inv;
	float r = radius / scale;

	CVector3D minV(fminf(t.x, b.x), 0, fminf(t.z, b.z));
	CVector3D maxV(fmaxf(t.x, b.x), 0, fmaxf(t.z, b.z));
	int minX = (int)(((minV.x - r) - m_header.min.x) / m_header.cutLength);
	int maxX = (int)(((maxV.x + r) - m_header.min.x) / m_header.cutLength);
	int minZ = (int)(((minV.z - r) - m_header.min.z) / m_header.cutLength);
	int maxZ = (int)(((maxV.z + r) - m_header.min.z) / m_header.cutLength);
	minX = _clamp(minX, 0, (int)m_header.cutX - 1);
	minZ = _clamp(minZ, 0, (int)m_header.cutZ - 1);
	maxX = _clamp(maxX, 0, (int)m_header.cutX - 1);
	maxZ = _clamp(maxZ, 0, (int)m_header.cutZ - 1);



	for (int x = minX; x <= maxX; x++) {
		for (int z = minZ; z <= maxZ; z++) {
			int idx = GetIdx(x, z);
			int c = m_pMesh[idx].CollisionCupsel(o, t, b, r, maxcnt, m_header);
			o += c;
			cnt += c;
			if (cnt > maxcnt) break;
		}
	}

	o = out;
	for (int i = 0; i < cnt; i++, o++) {
		o->m_dist *= scale;
		o->m_normal = (m_matrix*o->m_normal).GetNormalize();
		o->m_vertex[0] = (m_matrix*CVector4D(o->m_vertex[0], 1.0f));
		o->m_vertex[1] = (m_matrix*CVector4D(o->m_vertex[1], 1.0f));
		o->m_vertex[2] = (m_matrix*CVector4D(o->m_vertex[2], 1.0f));

	}
	return cnt;

}

