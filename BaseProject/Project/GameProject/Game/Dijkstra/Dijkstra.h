#pragma once
#include "../../stdafx.h"


//ノードクラス
class Node;
struct Route {
	Node* node;
	float length;
};
#define MAX_NEXT_ROUTE	4
struct NodeData {
	int	id;
	CVector3D pos;
	int next_id[MAX_NEXT_ROUTE];
};


class Node {
public:
	int	m_id;
	CVector3D m_pos;							//座標
	std::list<Route*> m_next;					//次のノード
	float m_score;								//スタート地点からの距離
	Node* m_prev;								//親Nodeのポインター
public:

	
	Node(const NodeData* data);
	//ノードの情報を構築する（ノードの繋がりを作成）
	void SetUp(const NodeData* data, std::map<int,Node*>& list);
	~Node();
};

/*!
		@brief	ダイクストラ　経路探索クラス
**/
class Dijkstra {
private:
	std::map<int,Node*> m_list;
	float m_MaxScore;							//スコアの最大値
public:
	static CVector3D center_pos;
	/*!
		@brief	コンストラクタ
		@retval	無し
	**/
	Dijkstra();

	/*!
		@brief	コンストラクタ
		@retval	無し
	**/
	Dijkstra(NodeData* node_data);

	/*!
		@brief	デストラクタ
		@retval	無し
	**/
	~Dijkstra();

	/*!
	@brief	ノードデータを登録する
	@param	node_data　ノードの配置データ
	**/
	void SetUpNode(NodeData* node_data);

	/*!
		@brief	スタート地点からスコアマップをつくる
		@param	node　出発地点のノード
		@retval	ゴールへ着くノード
	**/
	void ScoreFromNode(Node* node);


	/*!
		@brief	指定地点から最も近いノードを探す
		@param	pos　座標
		@retval	無し
	**/
	Node* FindNearNode(const CVector3D& pos);
	/*!
		@brief	経路探索　目的地から出発地点までのノードの情報を返す
		@param	start_pos　出発地点の座標
		@param	goal_pos　目標地点の座標
		@retval	無し
	**/
	std::list<Node*> FindShortestPath(const CVector3D& start_pos, const CVector3D& goal_pos);
	

	/*!
		@brief	スコアの値をMap上に描画　緑＝壁　赤＝通行可　青＝最短距離　
		@retval	無し
	**/
	void Draw();


	/*!
		@brief	エディタサンプル　
		@retval	無し
	**/
	void Edit();

	/*!
	    @brief ノードデータの読み込み（テキストデータから読み込む）
		@retval 読み込んだノードのデータ
	**/

	NodeData* ReadNodeData(const char* file_name);
};