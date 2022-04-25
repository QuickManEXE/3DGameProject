#ifndef INCLUDE_RENDER_TASK
#define	INCLUDE_RENDER_TASK

#include "Task.h"

typedef void(Task::*RenderFunc)();

class RenderTask
{
	friend class RenderTaskManager;

public:
	RenderTask();
	~RenderTask();

private:
	
	RenderTask* mp_Prev;			// 優先順位の前のポインタ
	RenderTask* mp_Next;			// 優先順位の後のポインタ

	int m_Priority;				// 優先順位
	const char* m_Name;			// デバッグ用名前

	Task* mp_Task;				// タスクポインタ
	RenderFunc m_Func;			// 関数ポインタ

	bool m_IsPause;				// 一時停止フラグ

public: 
	// 描画関数登録
	void RegistRender(Task* _task, RenderFunc _func, int _priority,  const char * _name);
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
	// 描画タスクの名前
	const char* GetName()const;

private:
	// 描画(関数ポインタ呼び出し)
	void Render();
};




#endif // !INCLUDE_RENDER_TASK