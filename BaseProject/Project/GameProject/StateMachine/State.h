#pragma once
#include<stdio.h>
#include"../TaskSystem/CollisionTask.h"

//ステート（状態）の基底クラス
//始まり
//実行中
//終わり
//の状態に別れる
template <typename T>
class State {
public:

	T* owner;

	State<T>(T* owner) {

		this->owner = owner;

	}

	//ステート開始時の処理
	virtual void Enter() {

		//printf("ステートの開始\n");

	}

	//ステート中の処理
	virtual void Execute() {

		//printf("ステートの実行中\n");
	}

	//ステート終了時の処理
	virtual void Exit() {

		//printf("ステートの終了\n");

	}

	//ステート別の描画処理
	virtual void Render() {

		//printf("ステート別の描画処理\n");

	}

	//ステート別の当たり判定処理
	virtual void CollisionCheck(CollisionTask* _task) {

		//printf("ステート別の当たり判定処理\n");

	}

};