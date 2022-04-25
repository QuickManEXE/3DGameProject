#ifndef INCLUDE_TASK_RENDER_MANAGER
#define INCLUDE_TASK_RENDER_MANAGER

#include "RenderTask.h"

class RenderTaskManager
{
private:
	static RenderTaskManager* mp_Instance;		// インスタンス

	RenderTask* mp_FirstList;					// 描画リストの先端ポインタ

public:
	// インスタンス取得
	static RenderTaskManager* GetInstance();	
	// インスタンス解放
	static void ReleaseInstance();			

	// 描画
	void RenderAll();
	// 追加
	void AddRenderTask(RenderTask* _task);
	// 登録解除
	void RemoveTask(RenderTask* _removeTask);
	// 登録解除(Taskから検索)
	void RemoveTask(Task* _removeTask);

	// 優先度変更
	void ChangePriority(RenderTask* _task);

private:
	RenderTaskManager();
	~RenderTaskManager();


};
#endif // !INCLUDE_TASK_RENDER_MANAGER
