#ifndef INCLUDE_TASK_COLLISION
#define INCLUDE_TASK_COLLISION

#include "Task.h"

class CollisionTask;

typedef void(Task::*CollisionFunc)(CollisionTask*);

class CollisionTask
{
	friend class CollisionTaskManager;

public:
	CollisionTask();
	~CollisionTask();
	
	enum {
		eLayer_System,
		eLayer_Game,
		eLayer_HUD,
		eLayer_Max,
	};
private:

	CollisionTask* mp_Prev;			// 優先順位の前のポインタ
	CollisionTask* mp_Next;			// 優先順位の後のポインタ

	int m_Priority;				// 優先順位
	const char* m_Name;			// デバッグ用名前
	int m_LayerID;				//レイヤーの識別番号

	Task* mp_Task;				// タスクポインタ
	CollisionFunc m_Func;			// 関数ポインタ

	bool m_IsPause;				// 一時停止フラグ

public:
	// 衝突判定関数登録
	void RegistCollision(int _layerID,Task* _task, CollisionFunc _func, int _priority, const char* _name);
	// 登録解除
	void Remove();
	// 優先順位変更
	void ChangePriority(int _priority);

	// 一時停止
	void SetPause(bool _isPause);
	// 登録されているかどうか
	bool IsRegist()const;
	// 優先度取得
	int GetPriority()const;
	// 一時停止しているかどうか
	bool IsPause()const;
	// 衝突判定タスクの名前
	const char* GetName()const;

	Task* GetTask();
private:
	// 衝突判定(関数ポインタ呼び出し)
	void CollisionCheck(CollisionTask* _task);

};

#endif // !INCLUDE_TASK_COLLISION