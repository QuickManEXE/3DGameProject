#pragma once
#include"State.h"

//ステート（状態）の基底クラス
//始まり
//実行中
//終わり
//の状態に別れる
template <typename T>
class StateMachine {
public:
	//現在のステート
	State<T>* currentState;

	//コンストラクタ
	StateMachine() {
		//現在のステートをnullに
		currentState = nullptr;

	};

	//現在のステートを取得
	State<T>* GetCurrentState() {
		return currentState;
	}

	void Update() {

		//現在のステートが存在するなら
		if (currentState != nullptr) {

			//実行中のステートを呼び出す
			currentState->Execute();

		}

	}


	void ChangeState(State<T>* state) {

		//現在のステートがあるなら
		if (currentState != nullptr) {

			//終了処理を呼び出す
			currentState->Exit();
		}

		//現在のステートの変更
		currentState = state;

		//現在のステートの開始処理を呼ぶ
		currentState->Enter();
	}

	//現在のステートの描画関数を呼ぶ
	void Render() {

		//現在のステートがあるなら
		if (currentState != nullptr) {

			//描画処理を呼び出す
			currentState->Render();
		}

	}

	//現在のステートの衝突判定関数を呼ぶ
	void CollisionCheck(CollisionTask* _task) {

		//現在のステートがあるなら
		if (currentState != nullptr) {

			//当たり判定処理を呼び出す
			currentState->CollisionCheck(_task);
		}

	}



};