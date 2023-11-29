#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

#include "../stage/Stage.h"
#include "../player/Player.h"

class GameScene final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	GameScene(int level);
	GameScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	LWP::Resource::Audio* bgm_[3];
	// 背景
	LWP::Primitive::Surface* backGround_;

	// ステージのレベル
	const int level_;
	// ステージデータ
	Stage stage_;

	// プレイヤー
	Player player_;

	
	// シーントランジション
	bool isStart_ = true;
	bool isEnd_ = false;
	int transitionFrame_ = 30;
	LWP::Primitive::Surface* transition_;
	// 次のシーンの予約
	IScene* next_ = nullptr;

	
	// t = 
	// b = 開始の値
	// c = 開始との差分
	// d = 
	float easeOut(float t, float b, float c, float d) {
		t /= d;
		return -c * t * (t - 2.0f) + b;
	}
};