#include "Dijkstra.h"



//  各ノードの繋がり
//  データ打ちより、エディタを作成し、
//  ファイルから読み込みのが良い
//  2   -   1   -   8
//  |       |       |
//  3   -   0   -   7
//  |       |       |
//  4   -   5   -   6
//　

CVector3D Dijkstra::center_pos(30, 0, 24);

NodeData node_data[] = {
    {0,CVector3D(-3,10,58),1,-1,-1,-1},           //0
    {1,CVector3D(9,10,43),0,1,9,10},       //1
    {2,CVector3D(20,10,28),1,3,-1,-1},     //2
    {3,CVector3D(21,9,22),2,4,-1,-1},       //3
    {4,CVector3D(21,7,20),3,5,-1,-1},    //4
    {5,CVector3D(16,5,20),4,6,-1,-1},      //5
    {6,CVector3D(6,3,20),7,5,-1,-1},   //6
    {7,CVector3D(-20,0,18),8,6,-1,-1},      //7
    {8,CVector3D(-20,0,42),7,-1,-1,-1},    //8
    {9,CVector3D(-3,10,28),1,-1,-1,-1},
    {10,CVector3D(20,10,58),1,-1,-1,-1},
    {11,CVector3D(-33,10,59),12,-1,-1,-1},
    {12,CVector3D(-40,10,45),11,13,14,15},
    {13,CVector3D(-33,10,31),12,-1,-1,-1},
    {14,CVector3D(-49,10,31),12,-1,-1,-1},
    {15,CVector3D(-49,10,53),12,16,-1,-1},
    {16,CVector3D(-49,12,55),15,17,-1,-1},
    {17,CVector3D(-49,14,58),16,18,-1,-1},
    {18,CVector3D(-53,16,56),17,19,-1,-1},
    {19,CVector3D(-56,18,56),18,20,-1,-1},
    {20,CVector3D(-55,20,48),19,-1,-1,-1},
};



Node::Node(const NodeData* data):m_id(data->id), m_pos(data->pos), m_score(0),  m_prev(nullptr)
{
}
void Node::SetUp(const NodeData* data, std::map<int,Node*>& list)
{
    for (int i = 0; i < MAX_NEXT_ROUTE; i++) {
        if (data->next_id[i] < 0) continue;
        Node* next = list[data->next_id[i]];
        if (!next) continue;
        Route* r = new Route();
        r->node = next;
        //ノード同士の距離を測る（ここは、初めからデータに含まれていると手間が省ける）
        r->length = (r->node->m_pos - m_pos).Length();
        m_next.push_back(r);

    }

}

Node::~Node()
{
    for (auto n : m_next)
        delete n;
}

Dijkstra::Dijkstra()
{
    int node_data_size = sizeof(node_data) / sizeof(node_data[0]);
    //全てのノードをデータから作成
    for (int i = 0; i < node_data_size; i++) {
        m_list[node_data[i].id]=new Node(&node_data[i]);
    }
    //繋がりを作成
    for (int i = 0; i < node_data_size; i++) {
        m_list[node_data[i].id]->SetUp(&node_data[i],m_list);
    }

}

Dijkstra::Dijkstra(NodeData* node_data)
{
    SetUpNode(node_data);
}

Dijkstra::~Dijkstra()
{

    for (auto v : m_list)
        delete v.second;
}

void Dijkstra::SetUpNode(NodeData* node_data)
{

    int node_data_size = sizeof(node_data) / sizeof(node_data[0]);
    //全てのノードをデータから作成
    for (int i = 0; i < node_data_size; i++) {
        m_list[node_data[i].id] = new Node(&node_data[i]);
    }
    //繋がりを作成
    for (int i = 0; i < node_data_size; i++) {
        m_list[node_data[i].id]->SetUp(&node_data[i], m_list);
    }

}

void Dijkstra::ScoreFromNode(Node* node)
{
   
    //次のノードへ探索
    for (auto next : node->m_next) {
        if (node->m_score + next->length < next->node->m_score) {
            next->node->m_score = node->m_score + next->length;
            next->node->m_prev = node;
            ScoreFromNode(next->node);
          
        }
    }

}

Node* Dijkstra::FindNearNode(const CVector3D& pos)
{
    Node* ret_node = nullptr;
    float length = FLT_MAX;
    for (auto v : m_list) {
        Node* n = v.second;
        //長さの大小だけ分かればいいので、平方根まで計算しなくてよい
        float l = (n->m_pos - pos).LengthSq();
        if (length > l) {
            length = l;
            ret_node = n;
        }
    }
    return ret_node;
}

std::list<Node*> Dijkstra::FindShortestPath(const CVector3D& start_pos, const CVector3D& goal_pos)
{

    //最短ルートのリスト
    std::list<Node*> routeList;

    //スタート地点と最も近いノード
    Node* startNode = FindNearNode(start_pos);

    if (!startNode) return routeList;

    //探索用データの初期化
    m_MaxScore = 0;
    for (auto v : m_list) {
        Node* n = v.second;
        n->m_score = FLT_MAX;
        n->m_prev = nullptr;
    }
    startNode->m_score = 0;

    Node* goalNode = FindNearNode(goal_pos);

    //ノードデータから探索を行う
    ScoreFromNode(startNode);

    
    //最短ルートのリスト作成（スタック）
    //スタート→ゴール
    while (goalNode != nullptr) {
        routeList.push_back(goalNode);
        goalNode = goalNode->m_prev;
    }
    return  routeList;
}

void Dijkstra::Draw()
{
    for (auto v : m_list) {
        Node* n = v.second;
        Utility::DrawSphere(n->m_pos, 1.0f, CVector4D(1, 0, 1, 1));
        for (auto v_n : n->m_next) {
            Node* next = v_n->node;
            Utility::DrawLine(n->m_pos, next->m_pos, CVector4D(1, 0, 0, 1));
        }
    }
}



void Dijkstra::Edit()
{
    //前に選択したノード
    static Node* old_node = nullptr;
    //マウスの座標
    CVector2D p = CInput::GetMousePoint();
    //ワールド座標に変換（画面から　近い→遠いの線分）
    CVector3D n, f;
    Utility::ScreenToWorld(&n, CVector3D(p.x, p.y, 0));
    Utility::ScreenToWorld(&f, CVector3D(p.x, p.y, 1));

    //マウスとの接触ノード
    Node* hit_node = nullptr;
    for (auto v : m_list) {
        Node* node = v.second;
        if (CCollision::CollisionCapsuleShpere(n, f, 0, node->m_pos, 1.0f)) {
            hit_node = node;
        }
    }
    //右クリックで消去
    if (PULL(CInput::eMouseR)) {
        if (hit_node) {
            auto it = m_list.begin();
            while (it != m_list.end()) {
                if (it->second == hit_node) break;
                it++;
            }
            if (it != m_list.end()) {
                //接続ノードのリンク解除
                for (auto v : hit_node->m_next) {
                    Node* next = v->node;
                    for (auto it = next->m_next.begin(); it != next->m_next.end(); ) {
                        if ((*it)->node == hit_node) {
                            delete (*it);
                            it = next->m_next.erase(it);
                            continue;
                        }
                        it++;
                    }

                }
                delete it->second;
                m_list.erase(it);
            }
        }
    }
    //左クリックでノード生成
    if (PUSH(CInput::eMouseL)) {
        if (!hit_node) {
            //マウスの線分と面の衝突点で、地面に置く
            CVector3D v = CVector3D(0, 0, 0) - n;
            CVector3D d = (f - n).GetNormalize();
            CVector3D c = n + d * (CVector3D::Dot(v, CVector3D(0, 1, 0)) / CVector3D::Dot(d, CVector3D(0, 1, 0)));
            if (m_list.size() > 0) {
                //idが連番になるよう最後の番号を取得
                auto it = m_list.end();
                it--;
                int id = it->second->m_id + 1;
                NodeData data = { id,c,-1,-1,-1,-1 };
                m_list[id] = new Node(&data);
            }
            else {
                NodeData data = { 0,c,-1,-1,-1,-1 };
                m_list[0] = new Node(&data);

            }

        }
        //クリックしたノードを保存
        old_node = hit_node;
    }
    //マウスを離すとドラッグ＆ドロップで接続
    if (PULL(CInput::eMouseL)) {
        if (old_node && hit_node && old_node != hit_node) {
            //2点の距離(の2乗)
            float length = (old_node->m_pos - hit_node->m_pos).LengthSq();
            //相互リンク
            old_node->m_next.push_back(new Route({ hit_node,length }));
            hit_node->m_next.push_back(new Route({ old_node,length }));
        }
        old_node = nullptr;
    }
    if (old_node)
        Utility::DrawSphere(old_node->m_pos, 1.2f, CVector4D(1, 1, 0, 1));
    if (hit_node)
        Utility::DrawSphere(hit_node->m_pos, 1.2f, CVector4D(1, 1, 0, 1));

    FONT_T()->Draw(0, 32, 1, 0, 0, "左クリック：ノード生成　ドラッグ：ノード接続　右クリック：ノード削除");
}

NodeData* Dijkstra::ReadNodeData(const char* file_name)
{
    //FILE* open_file;
    std::ifstream file(file_name);
    std::string line;
    
    int cnt = 0;

    while (std::getline(file, line)) {

        std::cout << line << std::endl;

        cnt++;

    }

    NodeData* node_data = new NodeData;

    return node_data;
}
