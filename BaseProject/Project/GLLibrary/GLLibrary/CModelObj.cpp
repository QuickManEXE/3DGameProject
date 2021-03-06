#include "CModelObj.h"
#include "CCollision.h"
#include "CCamera.h"
#include <string>

CMeshObj::CMeshObj() :m_buffer(0), mp_vertex_array(NULL), m_vertexNum(0), m_colmask(NULL), m_colmin(NULL), m_colmax(NULL), m_colIdex(NULL),m_max(-1e10, -1e10, -1e10),m_min(1e10, 1e10, 1e10){
}
void CMeshObj::Render(CShader *shader) {

	if (!m_buffer) {

		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * m_vertexNum, mp_vertex_array, GL_STATIC_DRAW);
		delete[] mp_vertex_array;
		mp_vertex_array = nullptr;

	}
	m_material.Map(shader);
	///glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glEnableVertexAttribArray(CShader::eVertexLocation);
	glEnableVertexAttribArray(CShader::eNormalLocation);
	glEnableVertexAttribArray(CShader::eTexcoordlLocation);



	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CMeshObj::SVertex), 0);
	glVertexAttribPointer(CShader::eNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CMeshObj::SVertex), (void*)sizeof(CVector3D));
	glVertexAttribPointer(CShader::eTexcoordlLocation, 2, GL_FLOAT, GL_FALSE, sizeof(CMeshObj::SVertex), (void*)(sizeof(CVector3D) + sizeof(CVector3D)));

	glDrawArrays(GL_TRIANGLES,0,m_vertexNum);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	m_material.Unmap();

	
}
void CMeshObj::Release() {
	if(mp_vertex_array)  {delete[] mp_vertex_array; mp_vertex_array = nullptr;}
	if (m_colmask) {		delete m_colmask; m_colmask = nullptr; }
	if (m_colmin) {		delete[] m_colmin; m_colmin = nullptr;	}
	if (m_colmax) {		delete[] m_colmax; m_colmax = nullptr;}
	if (m_colIdex) {		delete[] m_colIdex; m_colIdex = nullptr;}
	m_material.Release();
	if (m_buffer) {
		glDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
	//glDeleteVertexArrays(1, &m_vao);
}
void CMeshObj::CreatCollsionIndex(CVector3D cut){


	SVertex *v = mp_vertex_array;
	m_cut = cut;

	
	m_colmask = new unsigned long[(m_vertexNum / 3 / 32)+1];

	m_length.x = max(0.0f, (m_max.x - m_min.x) / m_cut.ix);
	m_length.y = max(0.0f, (m_max.y - m_min.y) / m_cut.iy);
	m_length.z = max(0.0f, (m_max.z - m_min.z) / m_cut.iz);
	//printf("cut_length(%f %f %f)\n", m_length.x, m_length.y, m_length.z);
	int xyz = m_cut.ix*m_cut.iy*m_cut.iz;
	m_colmin = new CVector3D[xyz];
	m_colmax = new CVector3D[xyz];
	m_colIdex = new std::vector<int>[xyz];
	for (int k = 0; k < m_cut.iy; k++) {
		for (int i = 0; i < m_cut.iz; i++) {
			for (int j = 0; j < m_cut.ix; j++) {
				int idx = GetIndex(j,k,i);
				m_colmin[idx] = CVector3D(m_min.x + j * m_length.x, m_min.y + k * m_length.y, m_min.z + i * m_length.z);
				m_colmax[idx] = CVector3D(m_min.x + (j + 1) * m_length.x, m_min.y + (k + 1)*m_length.y, m_min.z + (i + 1)*m_length.z);

			}
		}
	}

	for (int i = 0; i < m_vertexNum; i += 3, v += 3) {
		
		float l = fminf(fminf(v[0].v.x, v[1].v.x), v[2].v.x);
		float r = fmaxf(fmaxf(v[0].v.x, v[1].v.x), v[2].v.x);
		float t = fmaxf(fmaxf(v[0].v.z, v[1].v.z), v[2].v.z);
		float b = fminf(fminf(v[0].v.z, v[1].v.z), v[2].v.z);
		float u = fmaxf(fmaxf(v[0].v.y, v[1].v.y), v[2].v.y);
		float d = fminf(fminf(v[0].v.y, v[1].v.y), v[2].v.y);

		CVector3D e1 = v[1].v - v[0].v;
		CVector3D e2 = v[2].v - v[0].v;

		v[0].n = CVector3D::Cross(e1, e2).GetNormalize();
		CVector3D s, e;
		s.ix = max(0, min(m_cut.ix-1,(int)((l - m_min.x) / m_length.x)));
		e.ix = max(0, min(m_cut.ix - 1, (int)((r - m_min.x) / m_length.x)));

		s.iz = max(0, min(m_cut.iz - 1, (int)((b - m_min.z) / m_length.z)));
		e.iz = max(0, min(m_cut.iz - 1, (int)((t - m_min.z) / m_length.z)));

		s.iy = max(0, min(m_cut.iy - 1, (int)((d - m_min.y) / m_length.y)));
		e.iy = max(0, min(m_cut.iy - 1, (int)((u - m_min.y) / m_length.y)));

		for (int lineY = s.iy; lineY <= e.iy; lineY++) {
			for (int lineZ = s.iz; lineZ <= e.iz; lineZ++) {
				for (int lineX = s.ix; lineX <= e.ix; lineX++) {
					int idx = GetIndex(lineX, lineY, lineZ);
					m_colIdex[idx].push_back(i / 3);
				}
			}
		}

	}
	
	for (int k = 0; k < m_cut.iy; k++) {
		for (int i = 0; i < m_cut.iz; i++) {
			for (int j = 0; j < m_cut.ix; j++) {
				int idx = GetIndex(j, k, i);
			//	printf("%d ", m_colIdex[idx].size());
			}
		//	printf("\n");
		}
	//	printf("\n");
	}
				

}
bool CMeshObj::CollisionRay(CVector3D *c, CVector3D *n, const CVector3D &s, const CVector3D &e,const CVector3D& dir, float *pLength){
	SVertex* v = nullptr;
	if (m_buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		v = (SVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	}else {
		v = mp_vertex_array;
	}

	//	SVertex *v = mp_vertex_array;
	memset(m_colmask, 0, sizeof(unsigned long)*((m_vertexNum / 3 / 32) + 1));
	float l = min(s.x, e.x);
	float r = max(s.x, e.x);
	float t = max(s.z, e.z);
	float b = min(s.z, e.z);
	float u = max(s.y, e.y);
	float d = min(s.y, e.y);

	CVector3D ss, ee;
	ss.ix = max(0,min(m_cut.ix - 1, (int)((l - m_min.x) / m_length.x)));
	ee.ix = max(0, min(m_cut.ix - 1, (int)((r - m_min.x) / m_length.x)));

	ss.iz = max(0, min(m_cut.iz - 1, (int)((b - m_min.z) / m_length.z)));
	ee.iz = max(0, min(m_cut.iz - 1, (int)((t - m_min.z) / m_length.z)));

	ss.iy = max(0, min(m_cut.iy - 1, (int)((d - m_min.y) / m_length.y)));
	ee.iy = max(0, min(m_cut.iy - 1, (int)((u - m_min.y) / m_length.y)));


	bool hit = false;

	for (int lineY = ss.iy; lineY <= ee.iy; lineY++) {
		for (int lineZ = ss.iz; lineZ <= ee.iz; lineZ++) {
			for (int lineX = ss.ix; lineX <= ee.ix; lineX++) {
				int ceil = GetIndex(lineX, lineY, lineZ);
				if (!CCollision::CollisionAABBRay(NULL, NULL, m_colmin[ceil], m_colmax[ceil], s, dir)) continue;
				for (unsigned int i = 0; i < m_colIdex[ceil].size(); i++) {
					int idx = m_colIdex[ceil][i];
					if (m_colmask[idx / 32] & (1 << (idx % 32))) continue;
					if (CCollision::IntersectTriangleRay(c, s, e, v[idx * 3].v, v[idx * 3 + 1].v, v[idx * 3 + 2].v, pLength)) {

						CVector3D e1 = v[idx * 3 + 1].v - v[idx * 3].v;
						CVector3D e2 = v[idx * 3 + 2].v - v[idx * 3].v;

						*n = CVector3D::Cross(e1, e2).GetNormalize();
						hit = true;
					}
					m_colmask[idx / 32] |= 1 << (idx % 32);
				}

			}
		}
	}
	/*
	for (int i = 0; i < m_vertexNum; i += 3, v += 3) {
		if (CCollision::IntersectTriangleRay(c, s, e, v[0], v[1], v[2], pLength)){

			CVector3D e1 = v[1] - v[0];
			CVector3D e2 = v[2] - v[0];

			*n = CVector3D::Cross(e1, e2).GetNormalize();
			hit = true;
		}
	}
	*/
	if (m_buffer) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return hit;
}


int CMeshObj::CollisionSphere(CCollTriangle *out, const CVector3D &center, float radius, int maxcnt,int matNo){
	SVertex* v = nullptr;
	if (m_buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		v = (SVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	}
	else {
		v = mp_vertex_array;
	}

	//	SVertex *v = mp_vertex_array;
	memset(m_colmask, 0, sizeof(unsigned long)*((m_vertexNum / 3 / 32) + 1));
	float l = center.x-radius;
	float r = center.x + radius;
	float t = center.z + radius;
	float b = center.z - radius;
	float u = center.y + radius;
	float d = center.y - radius;

	CVector3D ss, ee;
	ss.ix = max(0, min(m_cut.ix - 1, (int)((l - m_min.x) / m_length.x)));
	ee.ix = max(0, min(m_cut.ix - 1, (int)((r - m_min.x) / m_length.x)));

	ss.iz = max(0, min(m_cut.iz - 1, (int)((b - m_min.z) / m_length.z)));
	ee.iz = max(0, min(m_cut.iz - 1, (int)((t - m_min.z) / m_length.z)));

	ss.iy = max(0, min(m_cut.iy - 1, (int)((d - m_min.y) / m_length.y)));
	ee.iy = max(0, min(m_cut.iy - 1, (int)((u - m_min.y) / m_length.y)));

	int cnt = 0;
	float dist;
	for (int lineY = ss.iy; lineY <= ee.iy; lineY++) {
		for (int lineZ = ss.iz; lineZ <= ee.iz; lineZ++) {
			int ceil_x = GetIndex(0, lineY, lineZ);
			for (int lineX = ss.ix; lineX <= ee.ix; lineX++) {
				int ceil = ceil_x + lineX;
				for (unsigned int i = 0, s = m_colIdex[ceil].size(); i < s; i++) {
					int idx = m_colIdex[ceil][i];
					if (m_colmask[idx / 32] & (1 << (idx % 32))) continue;
					if (CCollision::CollisionTriangleSphere(v[idx * 3].v, v[idx * 3 + 1].v, v[idx * 3 + 2].v,v[idx*3].n, center, radius, NULL, &dist)) {
						out->m_dist = dist;
						out->m_material_id = matNo;
						CVector3D e1 = v[idx * 3 + 1].v - v[idx * 3].v;
						CVector3D e2 = v[idx * 3 + 2].v - v[idx * 3].v;

						out->m_normal = CVector3D::Cross(e1, e2);
						out->m_vertex[0] = v[idx * 3 + 0].v;
						out->m_vertex[1] = v[idx * 3 + 1].v;
						out->m_vertex[2] = v[idx * 3 + 2].v;
						if (out->m_normal.LengthSq() == 0) continue;
						out->m_normal.Normalize();
						out++;
						cnt++;
						if (cnt >= maxcnt) {
							glUnmapBuffer(GL_ARRAY_BUFFER);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							return cnt;
						}
					}
					m_colmask[idx / 32] |= 1 << (idx % 32);
				}
			}
		}
	}
	/*
	int cnt = 0;
	for (int i = 0; i < m_vertexNum; i+=3,v+=3)
	{
		float dist;
		if (CCollision::CollisionTriangleSphere(v[0], v[1], v[2], center, radius, NULL, &dist)) {
			out->m_dist = dist;

			CVector3D e1 = v[1] - v[0];
			CVector3D e2 = v[2] - v[0];

			out->m_normal = CVector3D::Cross(e1, e2).GetNormalize();
			out++;
			cnt++;
		}
	}
	*/
	if (m_buffer) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return cnt;
}


int CMeshObj::CollisionCupsel(CCollTriangle *out, const CVector3D &top, const CVector3D &bottom, float radius, int maxcnt,int matNo){
	SVertex* v = nullptr;
	if (m_buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		v = (SVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	}
	else {
		v = mp_vertex_array;
	}


	//	SVertex *v = mp_vertex_array;
	
	memset(m_colmask, 0, sizeof(unsigned long)*((m_vertexNum / 3 / 32) + 1));
	float l = min(top.x,bottom.x) - radius;
	float r = max(top.x, bottom.x) + radius;
	float t = max(top.z, bottom.z) + radius;
	float b = min(top.z, bottom.z) - radius;
	float u = max(top.y, bottom.y) + radius;
	float d = min(top.y, bottom.y) - radius;

	CVector3D ss, ee;
	ss.ix = max(0, min(m_cut.ix - 1, (int)((l - m_min.x) / m_length.x)));
	ee.ix = max(0, min(m_cut.ix - 1, (int)((r - m_min.x) / m_length.x)));

	ss.iz = max(0, min(m_cut.iz - 1, (int)((b - m_min.z) / m_length.z)));
	ee.iz = max(0, min(m_cut.iz - 1, (int)((t - m_min.z) / m_length.z)));

	ss.iy = max(0, min(m_cut.iy - 1, (int)((d - m_min.y) / m_length.y)));
	ee.iy = max(0, min(m_cut.iy - 1, (int)((u - m_min.y) / m_length.y)));

	int cnt = 0;
	float dist;
	for (int lineY = ss.iy; lineY <= ee.iy; lineY++) {
		for (int lineZ = ss.iz; lineZ <= ee.iz; lineZ++) {
			for (int lineX = ss.ix; lineX <= ee.ix; lineX++) {
				int ceil = GetIndex(lineX, lineY, lineZ);
				for (unsigned int i = 0; i < m_colIdex[ceil].size(); i++) {
					int idx = m_colIdex[ceil][i];
					if (m_colmask[idx / 32] & (1 << (idx % 32))) continue;
					if (CCollision::CollisionTriangleCapsule(v[idx * 3].v, v[idx * 3 + 1].v, v[idx * 3 + 2].v, top, bottom, radius, NULL, &dist)) {
						out->m_dist = dist;
						out->m_material_id = matNo;
						CVector3D e1 = v[idx * 3 + 1].v - v[idx * 3].v;
						CVector3D e2 = v[idx * 3 + 2].v - v[idx * 3].v;


						out->m_normal = CVector3D::Cross(e1, e2);
						if (out->m_normal.LengthSq() == 0) continue;
						out->m_normal.Normalize();
						out->m_vertex[0] = v[idx * 3 + 0].v;
						out->m_vertex[1] = v[idx * 3 + 1].v;
						out->m_vertex[2] = v[idx * 3 + 2].v;

						out++;
						cnt++;
						if (cnt >= maxcnt) {

							glUnmapBuffer(GL_ARRAY_BUFFER);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							return cnt;
						}
					}
					m_colmask[idx / 32] |= 1 << (idx % 32);
				}
			}
		}
	}
	/*
	int cnt = 0;
	for (int i = 0; i < m_vertexNum; i += 3, v += 3)
	{
		float dist;
		if (CCollision::CollisionTriangleCapsule(v[0], v[1], v[2], top, bottom, radius, NULL, &dist)) {
			out->m_dist = dist;

			CVector3D e1 = v[1] - v[0];
			CVector3D e2 = v[2] - v[0];

			out->m_normal = CVector3D::Cross(e1, e2).GetNormalize();
			out++;
			cnt++;
		}
	}
	*/
	if (m_buffer) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return cnt;
}

void CMeshObj::LoadBinary(FILE *fp, const char *filePath) {
	fread(&m_vertexNum, sizeof(int), 1, fp);
	if (m_vertexNum <= 0) return;
	struct SMaterial {
		char m_name[128];
		char m_texName[128];
		CVector4D m_ambient;	//?A???r?G???g?J???[
		CVector4D m_diffuse;	//?f?t???[?Y?J???[
		CVector3D m_specular;	//?X?y?L?????[?J???[
		CVector3D m_emissive;	//????????
		float m_shininess;		//?X?y?L?????[?W??
		SMaterial() {
			memset(this, 0, sizeof(SMaterial));
			m_shininess = 1.0f;
		}
	}mat;
	fread(&mat, sizeof(SMaterial), 1, fp);
	fread(&m_max, sizeof(CVector3D), 1, fp);
	fread(&m_min, sizeof(CVector3D), 1, fp);
	fread(&m_min, sizeof(CVector3D), 1, fp);
	fread(&m_length, sizeof(CVector3D), 1, fp);
	mp_vertex_array = new SVertex[m_vertexNum];
	fread(mp_vertex_array, sizeof(SVertex), m_vertexNum, fp);
	fread(&m_cut, sizeof(CVector3D), 1, fp);

	
	m_colmask = new unsigned long[(m_vertexNum / 3 / 32) + 1];

	for(int k=0;k<m_cut.y;k++){
		for (int i = 0; i < m_cut.z; i++) {
			for (int j = 0; j < m_cut.x; j++) {
				int idx = GetIndex(j, k, i);
				int size;
				fread(&size, sizeof(unsigned long), 1, fp);
				int *idx_p = new int[size];
				fread(idx_p, sizeof(int), size, fp);
				for (int j = 0; j < size; j++) {
					m_colIdex[idx].push_back(idx_p[j]);
				}
				delete idx_p;
			}
		}
	}
	strcpy_s(m_material.m_name, NAME_STR_SIZE, mat.m_name);
	m_material.m_ambient = mat.m_ambient;
	m_material.m_diffuse = mat.m_diffuse;
	m_material.m_specular = mat.m_specular;
	m_material.m_shininess = mat.m_shininess;
	m_material.m_emissive = mat.m_emissive;
	if (mat.m_texName[0]) {

		char str[256];
		strcpy_s(str, 256, filePath);
		strcat_s(str,256, mat.m_texName);
		if (m_material.m_pTex) {
			m_material.Release();
		}
		m_material.m_pTex = new CTexture();
		if (!m_material.m_pTex->Load(str)) {
			if (m_material.m_pTex->Load(mat.m_texName)) {
				delete m_material.m_pTex;
				m_material.m_pTex = nullptr;
			}
		}
		
	}
	m_material.m_alpha = 1.0f;
}
CModelObj::CModelObj() :m_mesh(NULL){
}
CModelObj::~CModelObj() {
}

bool CModelObj::LoadBinary(const char *path) {
	MakePath(path);
	SObjModelHeader h;
	FILE *fp=nullptr;
	fopen_s(&fp, path, "rb");
	if (!fp) return false;
	fread(&h, sizeof(SObjModelHeader), 1, fp);
	if (h.fileType[0] == 'B' && h.fileType[1] == 'O') {
		m_materialCnt = h.m_materialCnt;
		m_cut = h.m_cut;
		m_mesh = new CMeshObj[m_materialCnt];
		for (int i = 0; i < m_materialCnt; i++) {
			m_mesh[i].LoadBinary(fp,m_filePath);
		}
	} else {
		return false;
	}
	fclose(fp);
	m_shader = CShader::GetInstance("StaticMesh");

	return true;

}
bool CModelObj::Load(const char *path) {

	const char *p = strrchr(path, '.');
	if (strcmp(p, ".obb") == 0) {
		return LoadBinary(path);
	}
	return Load(path, 1, 1, 1);
}
bool CModelObj::Load(const char *path, int cut_x, int cut_y, int cut_z) {
	MakePath(path);
	m_cut.ix = cut_x;
	m_cut.iy = cut_y;
	m_cut.iz = cut_z;
	m_max = CVector3D(FLT_MIN, FLT_MIN, FLT_MIN);
	m_min = CVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	//?t?@?C???|?C???^(fp)?????????J?????t?@?C?????n???h?????n???B
	FILE* fp;
	fopen_s(&fp,path,"r");
	//?t?@?C?????J?????????????????G???[???\????false??????
	if(fp==NULL) {
		printf("?t?@?C??????????????%s\n",path);
		return false;
	}
	//???W?f?[?^?p?J?E???^?[
	int	VCnt=0;
	//?@???f?[?^?p?J?E???^?[
	int	NCnt=0;
	//?e?N?X?`???[?R?[?h?p?J?E???^?[
	int	TCnt=0;
	//???f?[?^?p?J?E???^?[
	int	FCnt=0;
	//?L?[???[?h?i?[?p
	char key[128];
	//?t?@?C???????????????????J??????
	CMeshObj *m=NULL;
	while(!feof(fp)) {
		//key???N???A
		key[0]=0;
		//1?P???@????????????????
		fscanf_s(fp,"%s",key,128);
		//???????????L?[???[?h???uv?v???????W?f?[?^?J?E???^?[???J?E???g?A?b?v
		if(strcmp(key,"v")==0) {
			VCnt++;
		} else 
		if(strcmp(key,"usemtl")==0) {
			char name[128];
			fscanf_s(fp,"%s",name,128);
			for(int i=0;i<m_materialCnt;i++) {
				if(strcmp(name,m_mesh[i].m_material.m_name)==0) {
					m = &m_mesh[i];
				}
			}
		}
		//???????????L?[???[?h???uf?v???????f?[?^?J?E???^?[???J?E???g?A?b?v
		if(strcmp(key,"f")==0) {
			//???_?????J?E???g?A?b?v
			if(m) m->m_vertexNum+=3;
		} else 
		//???????????L?[???[?h???uvn?v?????@???f?[?^?J?E???^?[???J?E???g?A?b?v
		if(strcmp(key,"vn")==0) {
			NCnt++;
		} else 
		//???????????L?[???[?h???uvt?v?????e?N?X?`???[?R?[?h?f?[?^?J?E???^?[???J?E???g?A?b?v
		if(strcmp(key,"vt")==0) {
			TCnt++;
		} else 
		//???????????L?[???[?h???umtllib?v?????}?e???A???t?@?C????????????
		if(strcmp(key,"mtllib")==0) {
			char str[PATH_SIZE];
			strcpy_s(str, PATH_SIZE, m_filePath);
			int l = strlen(str);
			fscanf_s(fp, "%s", &str[l], PATH_SIZE-l);
			LoadMaterial(str);
		}
	}
	//printf("VCnt %d\n",VCnt);
	

	//???W?f?[?^?????????z????????
	CVector3D *pVertex = new CVector3D[VCnt];
	//?@???f?[?^?????????z????????
	CVector3D *pNormal = new CVector3D[NCnt];
	//?e?N?X?`???[?R?[?h?f?[?^?????????z????????
	CVector2D *pTexCoord = new CVector2D[TCnt];
	
	for(int i=0;i<m_materialCnt;i++) {
		//?`???p???_?z????????
		m_mesh[i].mp_vertex_array = new CMeshObj::SVertex[m_mesh[i].m_vertexNum];
		m_mesh[i].m_vertexNum=0;

	}
	//?????????????W?f?[?^??0???????????i?[?????????A?J?E???^?[??0??????
	VCnt=0;
	NCnt=0;
	TCnt=0;
	//?????????????u???t?@?C??????????????
	fseek(fp,0,SEEK_SET);
	//?`???p???_?z?????????W?f?[?^?i?[??????
	int idx = 0;
	//?t?@?C???????????????????J??????
	while(!feof(fp)) {
		//key???N???A
		key[0]=0;
		//?L?[???[?h??????????
		fscanf_s(fp,"%s",key,128);
		//?u???v???????????????A???W?f?[?^??????????
		if(strcmp(key,"v")==0) {
			CVector3D*vv = &pVertex[VCnt];
			fscanf_s(fp,"%f %f %f",&vv->x,&vv->y,&vv->z);
			m_min.x = min(m_min.x, vv->x);
			m_max.x = max(m_max.x, vv->x);
			m_max.z = max(m_max.z, vv->z);
			m_min.z = min(m_min.z, vv->z);
			m_max.y = max(m_max.y, vv->y);
			m_min.y = min(m_min.y, vv->y);

			//?????????????????f?[?^??
			VCnt++;
		}else//?uvn?v???????????????A?@???f?[?^??????????
		if(strcmp(key,"vn")==0) {
			fscanf_s(fp,"%f %f %f",&pNormal[NCnt].x,&pNormal[NCnt].y,&pNormal[NCnt].z);
			//?????????????????f?[?^??
			NCnt++;
		}else//?uvt?v???????????????A?e?N?X?`???[?R?[?h?f?[?^??????????
		if(strcmp(key,"vt")==0) {
			fscanf_s(fp,"%f %f",&pTexCoord[TCnt].x,&pTexCoord[TCnt].y);
			pTexCoord[TCnt].y = 1.0f - pTexCoord[TCnt].y;
			//?????????????????f?[?^??
			TCnt++;
		} else 
		if(strcmp(key,"usemtl")==0) {
			char name[NAME_STR_SIZE];
			fscanf_s(fp,"%s",name, NAME_STR_SIZE);
			for(int i=0;i<m_materialCnt;i++) {
				if(strcmp(name,m_mesh[i].m_material.m_name)==0) {
					m = &m_mesh[i];
				}
			}
		}
		if(strcmp(key,"f")==0) {

			int d[3][3] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			char data[128];
			fgets(data, 255, fp);
			char *p = data + 1;
			char str[16];
			int strIdx = 0;
			int t = 0, c = 0;
			CVector3D v[3];
			while (*p) {
				if (*p == '/' || *p == ' ' || *p == '\n') {

					str[strIdx] = 0;
					if (strIdx != 0) d[t][c] = atoi(str);
					strIdx = 0;
					if (*p == '/') t++;
					else{
						c++;
						t = 0;
						if (c >= 3) {
							if (*(p + 1) != '\0') printf("?O?p?`???O???|???S????????????");
							break;
						}
					}
				}
				else {
					str[strIdx] = *p;
					strIdx++;
				}
				p++;
			}
			m->mp_vertex_array[m->m_vertexNum].v = v[0] = pVertex[d[0][0] - 1];			//???_?P?????W?f?[?^
			m->mp_vertex_array[m->m_vertexNum + 1].v = v[1]  = pVertex[d[0][1] - 1];		//???_?Q?????W?f?[?^
			m->mp_vertex_array[m->m_vertexNum + 2].v= v[2]  = pVertex[d[0][2] - 1];

			m->mp_vertex_array[m->m_vertexNum].n = pNormal[d[2][0] - 1];			//???_?P???@???f?[?^
			m->mp_vertex_array[m->m_vertexNum + 1].n = pNormal[d[2][1] - 1];		//???_?Q???@???f?[?^
			m->mp_vertex_array[m->m_vertexNum + 2].n = pNormal[d[2][2] - 1];

			m->mp_vertex_array[m->m_vertexNum].t = pTexCoord[d[1][0] - 1];			//???_?P???e?N?X?`???[?R?[?h?f?[?^
			m->mp_vertex_array[m->m_vertexNum + 1].t = pTexCoord[d[1][1] - 1];		//???_?Q???e?N?X?`???[?R?[?h?f?[?^
			m->mp_vertex_array[m->m_vertexNum + 2].t = pTexCoord[d[1][2] - 1];
			/*
			int v1,v2,v3;
			int n1,n2,n3;
			int t1,t2,t3;
			fscanf_s(fp,"%d/%d/%d %d/%d/%d %d/%d/%d",&v1,&t1,&n1,&v2,&t2,&n2,&v3,&t3,&n3);
			m->m_pVertexAry[m->m_vertexNum] = pVertex[v1-1];			//???_?P?????W?f?[?^
			m->m_pVertexAry[m->m_vertexNum+1] = pVertex[v2-1];		//???_?Q?????W?f?[?^
			m->m_pVertexAry[m->m_vertexNum+2] = pVertex[v3-1];		
			
			m->m_pNormalAry[m->m_vertexNum] = pNormal[n1-1];			//???_?P???@???f?[?^
			m->m_pNormalAry[m->m_vertexNum+1] = pNormal[n2-1];		//???_?Q???@???f?[?^
			m->m_pNormalAry[m->m_vertexNum+2] = pNormal[n3-1];		
			
			m->m_pTexCoordAry[m->m_vertexNum] = pTexCoord[t1-1];			//???_?P???e?N?X?`???[?R?[?h?f?[?^
			m->m_pTexCoordAry[m->m_vertexNum+1] = pTexCoord[t2-1];		//???_?Q???e?N?X?`???[?R?[?h?f?[?^
			m->m_pTexCoordAry[m->m_vertexNum+2] = pTexCoord[t3-1];		
			*/
			//?f?[?^???i?[????3??????????
			m->m_vertexNum+=3;
		}
	}
	fclose(fp);

	m_shader = CShader::GetInstance("StaticMesh");


	for (int i = 0; i<m_materialCnt; i++) {
		m_mesh[i].m_min = m_min;
		m_mesh[i].m_max = m_max;
		m_mesh[i].CreatCollsionIndex(m_cut);

	}



	
	//?????f?[?^????
	delete[] pVertex;
	delete[] pNormal;
	delete[] pTexCoord;
	return true;
}

bool CModelObj::LoadMaterial(char *path) {
	char key[128];
	FILE* fp;
	fopen_s(&fp, path, "r");
	//?t?@?C?????J?????????????????G???[???\????false??????
	if (fp == NULL) {
		printf("?t?@?C??????????????%s\n", path);
		return false;
	}

	m_materialCnt = 0;
	while (!feof(fp)) {
		fscanf_s(fp, "%s", key, 128);
		if (strcmp(key, "newmtl") == 0)
			m_materialCnt++;
	}
	m_mesh = new CMeshObj[m_materialCnt];
	m_materialCnt = 0;
	CMeshObj *m = m_mesh;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp)) {
		key[0] = '\0';
		fscanf_s(fp, "%s", key, 128);
		if (strcmp(key, "newmtl") == 0) {
			m = &m_mesh[m_materialCnt];
			char name[NAME_STR_SIZE];
			fscanf_s(fp, "%s", name, NAME_STR_SIZE);
			strcpy_s(m->m_material.m_name, NAME_STR_SIZE, name);

			m_materialCnt++;
		}else
		if (strcmp(key, "Ka") == 0) {
			fscanf_s(fp, "%f %f %f", &m->m_material.m_ambient.x, &m->m_material.m_ambient.y, &m->m_material.m_ambient.z);
			m->m_material.m_ambient.a = 1.0f;
		}else
		if (strcmp(key, "Kd") == 0) {
			fscanf_s(fp, "%f %f %f", &m->m_material.m_diffuse.x, &m->m_material.m_diffuse.y, &m->m_material.m_diffuse.z);
			m->m_material.m_diffuse.a = 1.0f;
		}else
		if (strcmp(key, "Ks") == 0) {
			fscanf_s(fp, "%f %f %f", &m->m_material.m_specular.x, &m->m_material.m_specular.y, &m->m_material.m_specular.z);
		}else
		if (strcmp(key, "Ns") == 0) {
			fscanf_s(fp, "%f", &m->m_material.m_shininess);
		}else
		if (strcmp(key, "d") == 0) {
			float aplha;
			fscanf_s(fp, "%f", &aplha);
			m->m_material.m_ambient.a = aplha;
			m->m_material.m_diffuse.a = aplha;
		}else
		if (strcmp(key, "map_Kd") == 0) {


			char tex_file[PATH_SIZE];
			fscanf_s(fp, "%s", tex_file, PATH_SIZE);
			strcpy_s(m->m_material.m_texture_name, NAME_STR_SIZE, tex_file);

			char str[PATH_SIZE];
			strcpy_s(str, PATH_SIZE, m_filePath);
			int l = strlen(str);
			strcat_s(str,PATH_SIZE,tex_file);

			if (m->m_material.m_pTex) {
				m->m_material.Release();
			}
			m->m_material.m_pTex = new CTexture();
			if (!m->m_material.m_pTex->Load(str)) {
				delete m->m_material.m_pTex;
				m->m_material.m_pTex = NULL;
			}
		}
	}
	fclose(fp);
	return true;
}
void CModelObj::Render(CMatrix &m) {
	m_shader->enable();
	SendShaderParam(m,CCamera::GetCurrent()->GetViewMatrix() * m, CCamera::GetCurrent()->GetProjectionMatrix());


	for(int i=0;i<m_materialCnt;i++) {
		m_mesh[i].Render(m_shader);
	}
	m_shader->disable();


}
void CModelObj::Render() {
	CModel::UpdateMatrix();
	Render(m_matrix);
}
void CModelObj::Release() {
	for(int i=0;i<m_materialCnt;i++) {
		m_mesh[i].Release();
	}
	if (m_mesh) delete[] m_mesh;
	//if (m_shader) delete m_shader;
}

bool CModelObj::CollisionRay(CVector3D *pCross, CVector3D *pNormal, const CVector3D &s, const CVector3D &e)const {
	CVector3D cross, normal;
	float length = 1e10;
	bool hit = false;
	CVector3D c, n;

	CMatrix inv = m_matrix.GetInverse();
	CVector3D s2 = inv * CVector4D(s, 1);
	CVector3D e2 = inv * CVector4D(e, 1);
	CVector3D dir = (e2 - s2).GetNormalize();

	for (int i = 0; i<m_materialCnt; i++) {
		if (m_mesh[i].CollisionRay(&c, &n, s2,e2, dir,&length))
			hit = true;
	}
	if (hit){
		if (pCross) {
			*pCross = m_matrix*CVector4D(c , 1);
		}
		if (pNormal) {
			*pNormal = CVector3D(m_matrix*CVector4D(n, 0)).GetNormalize();
		}
	}
	return hit;

}


int CModelObj::CollisionSphere(CCollTriangle *out, const CVector3D &center, float radius, int maxcnt)const {
	float scale = CVector3D(m_matrix.m00, m_matrix.m10, m_matrix.m20).Length();
	int cnt = 0;
	CCollTriangle *o = out;
	CVector3D p = m_matrix.GetInverse() * CVector4D(center, 1);
	float r = radius / scale;

	for (int i = 0; i<m_materialCnt; i++) {
		int c = m_mesh[i].CollisionSphere(o, p, r, maxcnt,i);
		o += c;
		cnt += c;
		if (cnt > maxcnt) break;

	}
	o = out;
	for (int i = 0; i < cnt; i++, o++) {
		o->m_dist *= scale;
		o->Transform(m_matrix);

	}
	return cnt;
}


int CModelObj::CollisionCupsel(CCollTriangle *out, const CVector3D &top, const CVector3D &bottom, float radius, int maxcnt)const {
	float scale = CVector3D(m_matrix.m00, m_matrix.m10, m_matrix.m20).Length();
	int cnt = 0;
	CCollTriangle *o = out;
	CMatrix inv = m_matrix.GetInverse();
	CVector3D t = inv * CVector4D(top, 1);
	CVector3D b = inv * CVector4D(bottom, 1);
	float r = radius / scale;

	for (int i = 0; i<m_materialCnt; i++) {
		int c = m_mesh[i].CollisionCupsel(o, t, b, r,maxcnt,i);
		o += c;
		cnt += c;
		if (cnt > maxcnt) break;

	}
	o = out;
	for (int i = 0; i < cnt; i++, o++) {
		o->m_dist *= scale;
		o->Transform(m_matrix);

	}
	return cnt;
}
CModelObj* CModelObj::CreateModel(char *path, int cut_x, int cut_y, int cut_z) {
	CModelObj *o = new CModelObj();
	if (o->Load(path, cut_x, cut_y, cut_z)) {
		return o;
	}
	delete o;
	return nullptr;
}

CVector3D CModelObj::GetMax() const
{
	CVector4D v_max = m_matrix * CVector4D(m_max, 1);
	CVector4D v_min = m_matrix * CVector4D(m_min, 1);
	CVector3D ret_max;
	ret_max.x = max(v_max.x, v_min.x);
	ret_max.y = max(v_max.y, v_min.y);
	ret_max.z = max(v_max.z, v_min.z);
	return ret_max;
}

CVector3D CModelObj::GetMin() const
{

	CVector4D v_max = m_matrix * CVector4D(m_max, 1);
	CVector4D v_min = m_matrix * CVector4D(m_min, 1);
	CVector3D ret_min;
	ret_min.x = min(v_max.x, v_min.x);
	ret_min.y = min(v_max.y, v_min.y);
	ret_min.z = min(v_max.z, v_min.z);
	return ret_min;
}
