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
	StageSelectScene(LWP::Utility::Color startTransitionColor, LWP::Math::Vector3 startPosition);

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//

	// 背景
	LWP::Primitive::Surface* backGround_;

	// UIの種類
	enum UIType {
		RB,						// RBボタン
		LB,						// LBボタン
		FONT_START,				// 文字_ゲーム開始
		FONT_PARALLEL,			// 文字_平行移動
		UI_NUM					// UIの数
	};

	// UI
	LWP::Primitive::Surface* operationUI_[UI_NUM];

	StageSelect stageSelect_;
	StageSelectPlayer player_;

	// 開始地点を指定するフラグ
	bool isSelectStartPosition_ = false;
	LWP::Math::Vector3 startPosition_;

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