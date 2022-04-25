#pragma once
#include "../../stdafx.h"


//�m�[�h�N���X
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
	CVector3D m_pos;							//���W
	std::list<Route*> m_next;					//���̃m�[�h
	float m_score;								//�X�^�[�g�n�_����̋���
	Node* m_prev;								//�eNode�̃|�C���^�[
public:

	
	Node(const NodeData* data);
	//�m�[�h�̏����\�z����i�m�[�h�̌q������쐬�j
	void SetUp(const NodeData* data, std::map<int,Node*>& list);
	~Node();
};

/*!
		@brief	�_�C�N�X�g���@�o�H�T���N���X
**/
class Dijkstra {
private:
	std::map<int,Node*> m_list;
	float m_MaxScore;							//�X�R�A�̍ő�l
public:
	static CVector3D center_pos;
	/*!
		@brief	�R���X�g���N�^
		@retval	����
	**/
	Dijkstra();

	/*!
		@brief	�R���X�g���N�^
		@retval	����
	**/
	Dijkstra(NodeData* node_data);

	/*!
		@brief	�f�X�g���N�^
		@retval	����
	**/
	~Dijkstra();

	/*!
	@brief	�m�[�h�f�[�^��o�^����
	@param	node_data�@�m�[�h�̔z�u�f�[�^
	**/
	void SetUpNode(NodeData* node_data);

	/*!
		@brief	�X�^�[�g�n�_����X�R�A�}�b�v������
		@param	node�@�o���n�_�̃m�[�h
		@retval	�S�[���֒����m�[�h
	**/
	void ScoreFromNode(Node* node);


	/*!
		@brief	�w��n�_����ł��߂��m�[�h��T��
		@param	pos�@���W
		@retval	����
	**/
	Node* FindNearNode(const CVector3D& pos);
	/*!
		@brief	�o�H�T���@�ړI�n����o���n�_�܂ł̃m�[�h�̏���Ԃ�
		@param	start_pos�@�o���n�_�̍��W
		@param	goal_pos�@�ڕW�n�_�̍��W
		@retval	����
	**/
	std::list<Node*> FindShortestPath(const CVector3D& start_pos, const CVector3D& goal_pos);
	

	/*!
		@brief	�X�R�A�̒l��Map��ɕ`��@�΁��ǁ@�ԁ��ʍs�@���ŒZ�����@
		@retval	����
	**/
	void Draw();


	/*!
		@brief	�G�f�B�^�T���v���@
		@retval	����
	**/
	void Edit();

	/*!
	    @brief �m�[�h�f�[�^�̓ǂݍ��݁i�e�L�X�g�f�[�^����ǂݍ��ށj
		@retval �ǂݍ��񂾃m�[�h�̃f�[�^
	**/

	NodeData* ReadNodeData(const char* file_name);
};