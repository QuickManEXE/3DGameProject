#pragma once
#include"StateMachine.h"
#include<vector>
#include"../TaskSystem/CollisionTask.h"

template <typename T,typename TEnum>
class StateAI {
public:
	//�o�^�X�e�[�g�̃��X�g
	std::vector<State<T>*> state_vector;

	//�X�e�[�g�}�V���̓o�^
	StateMachine<T>* stateMachine;

	~StateAI() {
		
		
		for (State<T>* s : state_vector) {
			delete s;
		}
		
		state_vector.clear();

		delete stateMachine;

	};

	void ChangeState(TEnum state) {

		//�X�e�[�g�}�V�����Ȃ��ꍇ
		if (stateMachine == nullptr) {
			return;//�e��
		}

		//���݃X�e�[�g��ς���
		stateMachine->ChangeState(state_vector[(int)state]);

	}

	virtual bool IsCurrentState(TEnum state)
	{
		///�X�e�[�g�}�V�[������null�̏ꍇ
		if (stateMachine == nullptr){
			return false;
		}
		///���݂̃X�e�[�g�}�V���ŉғ����Ă���X�e�[�g�ƁA�w�肵���X�e�[�g���������ǂ�����Bool�l�ŕԂ�
		return stateMachine->currentState == state_vector[(int)state];
	}


	//���݂̃X�e�[�gUpdate�����s
	void StateUpdate(){

		if (stateMachine != nullptr) {

			stateMachine->Update();

		}
	}

	//�X�e�[�g�ʂ�Render�����s
	void StateRender() {

		if (stateMachine != nullptr) {

			stateMachine->Render();

		}
	}

	//�X�e�[�g�ʂ�CollisionCheck�����s
	void StateCollisionCheck(CollisionTask* _task) {

		if (stateMachine != nullptr) {

			stateMachine->CollisionCheck(_task);

		}

	}

};