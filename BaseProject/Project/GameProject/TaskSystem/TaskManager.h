#ifndef INCLUDE_TASK_MANAGER
#define INCLUDE_TASK_MANAGER

#include "Task.h"

// シングルトン

class TaskManager
{
private:
	static TaskManager* mp_Instance;		// インスタンス(実行中に1個しかインスタンスを生成させない)

	Task* mp_ListFirst;					// 更新リストの先端ポインタ

	Task* mp_DeleteListFirst;			// 削除リストの先端ポインタ

public:
	// インスタンス取得
	static TaskManager* GetInstance();	
	// インスタンス解放
	static void ReleaseInstance();		

	// タスク即全消去(後片付けに使用する)
	void AllKill();						

	// Task更新
	void UpdateAll();

	// タスクをリストに追加
	void AddTask(Task* _addTask);

	//予約削除の実行
	void PreKill();

	// 削除リストに追加
	void AddKill(Task* _killTask);

	Task* FindTask(int _priorty);
private:
	TaskManager();
	~TaskManager();

	// タスク削除
	void KillTask();

	// Taskをリストから外す
	void RemoveUpdateTaskList(Task* _removeTask);
	void RemoveKillTaskList(Task* _removeTask);

	// Taskをリストに追加する
	void AddTaskList(Task* _firstTask, Task* _removeTask);

};

#endif // !INCLUDE_TASK_MANAGER
