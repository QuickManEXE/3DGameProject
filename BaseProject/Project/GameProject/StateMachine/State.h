#pragma once
#include<stdio.h>
#include"../TaskSystem/CollisionTask.h"

//�X�e�[�g�i��ԁj�̊��N���X
//�n�܂�
//���s��
//�I���
//�̏�Ԃɕʂ��
template <typename T>
class State {
public:

	T* owner;

	State<T>(T* owner) {

		this->owner = owner;

	}

	//�X�e�[�g�J�n���̏���
	virtual void Enter() {

		//printf("�X�e�[�g�̊J�n\n");

	}

	//�X�e�[�g���̏���
	virtual void Execute() {

		//printf("�X�e�[�g�̎��s��\n");
	}

	//�X�e�[�g�I�����̏���
	virtual void Exit() {

		//printf("�X�e�[�g�̏I��\n");

	}

	//�X�e�[�g�ʂ̕`�揈��
	virtual void Render() {

		//printf("�X�e�[�g�ʂ̕`�揈��\n");

	}

	//�X�e�[�g�ʂ̓����蔻�菈��
	virtual void CollisionCheck(CollisionTask* _task) {

		//printf("�X�e�[�g�ʂ̓����蔻�菈��\n");

	}

};