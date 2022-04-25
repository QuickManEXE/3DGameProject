#include "CollisionTaskManager.h"

#include <assert.h>

CollisionTaskManager* CollisionTaskManager::mp_Instance;

CollisionTaskManager* CollisionTaskManager::GetInstance()
{
	if (!mp_Instance)
	{
		mp_Instance = new CollisionTaskManager();
	}
	return mp_Instance;
}

void CollisionTaskManager::ReleaseInstance()
{
	// タスクが削除されていない可能性があります。(TaskManagerの方でタスクを削除するときに一緒に外しているはず)
	assert(!mp_Instance->m_FirstList);

	delete mp_Instance;
	mp_Instance = nullptr;
}

CollisionTaskManager::CollisionTaskManager()
{
	for (int i = CollisionTask::eLayer_System; i < CollisionTask::eLayer_Max; i++)
	{
		m_FirstList[i] = nullptr;
	}
}

CollisionTaskManager::~CollisionTaskManager()
{
}

void CollisionTaskManager::CollisionCheckAll()
{
	// 更新(衝突判定)

	for (int i = CollisionTask::eLayer_System; i < CollisionTask::eLayer_Max; i++) {

		CollisionTask* currentTask = m_FirstList[i];


		while (currentTask)
		{
			CollisionTask* nextTask = currentTask->mp_Next;

			// 一時停止していない場合は衝突判定
			if (!currentTask->m_IsPause)
			{
				while (nextTask) {

					if (!nextTask->m_IsPause) {
						//別のTaskと衝突判定
						currentTask->CollisionCheck(nextTask);

						nextTask->CollisionCheck(currentTask);
					}

					nextTask = nextTask->mp_Next;
				}


			}

			// 次のタスク
			currentTask = currentTask->mp_Next;
		}
	}
}

void CollisionTaskManager::AddCollisionTask(int _layerID,CollisionTask* _task)
{
	int i = _layerID;
	// 先頭ポインタがnullの場合は先頭ポインタに設定して追加終了
	if (!m_FirstList[i])
	{
		m_FirstList[i] = _task;
		return;
	}

	// 指定のグループと優先度の場所に追加
	CollisionTask* currentTask = m_FirstList[i];
	CollisionTask* nextTask = currentTask->mp_Next;

	// グループ
	while (currentTask)
	{
		// 優先度がリストのタスクより低い場合はその前のところにタスクを追加
		if (_task->m_Priority < currentTask->m_Priority)
		{
			// 追加したタスクの前後を設定
			_task->mp_Prev = currentTask->mp_Prev;
			_task->mp_Next = currentTask;

			// リストのタスクを繋ぎ変える
			if (currentTask->mp_Prev)
			{
				currentTask->mp_Prev->mp_Next = _task;
			}

			currentTask->mp_Prev = _task;

			// 先頭に追加された場合は先頭ポインタ変更
			if (m_FirstList[i] == currentTask)
			{
				m_FirstList[i] = _task;
			}

			return;
		}

		// 次のタスクがない場合は、最後に追加する
		if (!currentTask->mp_Next)
		{
			currentTask->mp_Next = _task;
			_task->mp_Prev = currentTask;
			return;
		}

		// 次のタスク
		currentTask = currentTask->mp_Next;
	}
}

void CollisionTaskManager::RemoveTask(int _layerID, CollisionTask* _removeTask)
{
	int i = _layerID;
	CollisionTask* prev = _removeTask->mp_Prev;
	CollisionTask* next = _removeTask->mp_Next;

	// タスクのグループの前後
	if (prev)
	{
		prev->mp_Next = next;

	}
	if (next)
	{
		next->mp_Prev = prev;
	}

	// 先頭ポインタを入れ替える場合更新
	if (_removeTask == m_FirstList[i])
	{
		m_FirstList[i] = _removeTask->mp_Next;
	}

	// 前後左右のつながりを初期化
	_removeTask->mp_Prev = nullptr;
	_removeTask->mp_Next = nullptr;
}

void CollisionTaskManager::RemoveTask(Task* _removeTask)
{
	for (int i = CollisionTask::eLayer_System; i < CollisionTask::eLayer_Max; i++) {
		CollisionTask* currentTask = m_FirstList[i];

		// タスク検索(複数登録があるかもしれないので全部確認する)
		while (currentTask)
		{
			CollisionTask* nextTask = currentTask->mp_Next;

			// タスクポインタが一致している場合はリストから解除
			if (currentTask->mp_Task == _removeTask)
			{
				RemoveTask(i,currentTask);
			}

			// 次のタスク
			currentTask = nextTask;
		}
	}
}

void CollisionTaskManager::ChangePriority(int _layerID, CollisionTask* _task)
{
	// 当たり判定リストから外す
	RemoveTask(_layerID, _task);

	// 当たり判定リストに追加
	AddCollisionTask(_layerID, _task);
}

std::vector<CollisionTask*> CollisionTaskManager::GetCollisionList()
{
	// ゲームレイヤーの
	int i = CollisionTask::eLayer_Game;

	CollisionTask* currentTask = m_FirstList[i];

	std::vector<CollisionTask*> task_vector;

	while (currentTask)
	{
		// 一時停止していない場合
		if (!currentTask->m_IsPause)
		{
			//リストに入れる
			task_vector.push_back(currentTask);

		}

		// 次のタスク
		currentTask = currentTask->mp_Next;
	}

	return task_vector;
}
