#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

class TestTitle final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	TestTitle() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	// 地面
	LWP::Primitive::Surface* ground;

	LWP::Primitive::Triangle* tri[2];
	LWP::Primitive::Surface* surface;
	LWP::Primitive::Sphere* sphere;
	
	LWP::Primitive::Mesh* cubeModel;

	LWP::Resource::Texture* uvTexture;
	LWP::Resource::Texture* monsterBall;

	// SEデータ
	LWP::Resource::Audio* audio;

	bool useMonsterBall = true;
};