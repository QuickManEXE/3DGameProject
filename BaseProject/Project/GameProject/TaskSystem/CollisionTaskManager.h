#ifndef INCLUDE_TASK_COLLISION_MANAGER
#define INCLUDE_TASK_COLLISION_MANAGER

#include "CollisionTask.h"
#include<vector>


class CollisionTaskManager
{
private:
	static CollisionTaskManager* mp_Instance;		// インスタンス

	CollisionTask* m_FirstList[CollisionTask::eLayer_Max];	// 衝突判定リストの先端ポインタ

public:
	// インスタンス取得
	static CollisionTaskManager* GetInstance();
	// インスタンス解放
	static void ReleaseInstance();

	// 衝突判定
	void CollisionCheckAll();
	// 追加
	void AddCollisionTask(int _layerID,CollisionTask* _task);
	// 登録解除
	void RemoveTask(int _layerID,CollisionTask* _removeTask);
	// 登録解除(Taskから検索)
	void RemoveTask(Task* _removeTask);

	// 優先度変更
	void ChangePriority(int _layerID,CollisionTask* _task);


	std::vector<CollisionTask*> GetCollisionList();

private:
	CollisionTaskManager();
	~CollisionTaskManager();

	
};
#endif // !INCLUDE_TASK_COLLISION_MANAGER
