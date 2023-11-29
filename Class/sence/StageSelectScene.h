#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

#include "../stage/StageSelect.h"
#include "../player/StageSelectPlayer.h"

class StageSelectScene final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	StageSelectScene(LWP::Utility::Color startTransitionColor);

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	// 背景
	LWP::Primitive::Surface* backGround_;

	StageSelect stageSelect_;
	StageSelectPlayer player_;

	// シーントランジション
	LWP::Utility::Color transitionColor_;
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