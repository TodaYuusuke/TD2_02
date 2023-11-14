#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

#include"../effect/Fire.h"
#include"../collision/CollisionDetector.h"

#include<random>

class GameScene final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	GameScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	//炎パーティクル
	Fire* fire;
	Fire* fire2;

	CollisionDetector* collision;

	
	LWP::Primitive::Mesh* cubeModel;
	LWP::Primitive::Sphere* sphere;

	bool isHit;

};