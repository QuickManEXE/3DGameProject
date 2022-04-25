#pragma once
#include"StateMachine.h"
#include<vector>
#include"../TaskSystem/CollisionTask.h"

template <typename T,typename TEnum>
class StateAI {
public:
	//登録ステートのリスト
	std::vector<State<T>*> state_vector;

	//ステートマシンの登録
	StateMachine<T>* stateMachine;

	~StateAI() {
		
		
		for (State<T>* s : state_vector) {
			delete s;
		}
		
		state_vector.clear();

		delete stateMachine;

	};

	void ChangeState(TEnum state) {

		//ステートマシンがない場合
		if (stateMachine == nullptr) {
			return;//弾く
		}

		//現在ステートを変える
		stateMachine->ChangeState(state_vector[(int)state]);

	}

	virtual bool IsCurrentState(TEnum state)
	{
		///ステートマシーン内がnullの場合
		if (stateMachine == nullptr){
			return false;
		}
		///現在のステートマシンで稼働しているステートと、指定したステートが同じかどうかをBool値で返す
		return stateMachine->currentState == state_vector[(int)state];
	}


	//現在のステートUpdateを実行
	void StateUpdate(){

		if (stateMachine != nullptr) {

			stateMachine->Update();

		}
	}

	//ステート別のRenderを実行
	void StateRender() {

		if (stateMachine != nullptr) {

			stateMachine->Render();

		}
	}

	//ステート別のCollisionCheckを実行
	void StateCollisionCheck(CollisionTask* _task) {

		if (stateMachine != nullptr) {

			stateMachine->CollisionCheck(_task);

		}

	}

};