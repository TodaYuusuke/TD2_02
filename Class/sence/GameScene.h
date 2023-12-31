#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"

#include "../stage/Stage.h"
#include "../player/Player.h"

class GameScene final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	GameScene(int level, LWP::Math::Vector3 prevScenePosition);
	GameScene() = default;

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
		UPBUTTON,				// 上ボタン
		START,					// スタートボタン
		FONT_THROW,				// 文字_投げ
		FONT_PARALLEL,			// 文字_平行移動
		FONT_ZOOM,				// 文字_ズーム
		FONT_RESET,				// 文字_リセット
		TUTORIAL_CANDLE,		// チュートリアル_ろうそく
		TUTORIAL_GROWLEAF,		// チュートリアル_成長する草
		TUTORIAL_WITHERLEAF,	// チュートリアル_枯れる草
		CANDLE,					// ろうそく
		X,						// 掛ける
		UI_NUM					// UIの数
	};

	// UI
	LWP::Primitive::Surface* operationUI_[UI_NUM];
	// ろうそくの残り本数
	LWP::Primitive::Surface* candleNumUI_[4];

	// 戻るボタン
	LWP::Primitive::Surface* backButton_;
	// ステージ選択に戻るUI
	LWP::Primitive::Surface* selectStageUI_;

	// ステージのレベル
	const int level_;
	// ステージデータ
	Stage stage_;

	// プレイヤー
	Player player_;
	// ステージ選択シーンでの最後の位置を保持
	LWP::Math::Vector3 prevScenePosition_;

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