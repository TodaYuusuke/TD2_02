#pragma once
#include <Adapter.h>
#include "../Engine/scene/IScene.h"
#include "../sence/StageSelectScene.h"
#include"../stage/MapChip/Derved.h"
#include"../effect/Fire.h"

class Title final
	: public IScene {
public:

	// コンストラクタの定義も自由に可
	Title(bool isPlayAllow);

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// scene遷移したい場合、ISceneポインタ型のnextSceneに次のシーンの実体を代入

private: //*** これより先に必要な処理や変数を記述 ***//
	enum BGMType {
		LEAFSWAY, // 葉の揺れる音
		BIRD,	  // 鳥の鳴き声
		PIANO,	  // ピアノ    (使用したサイトは甘茶の音楽工房、曲名は夢。利用規約を見たところ著作権表示は必須ではない)
		BGM_NUM  // これは使用しないで
	};
	// BGM
	LWP::Resource::Audio* bgm_[BGM_NUM];
	// BGMを止めるかのフラグ
	bool isPlayAllow_ = false;

	// 背景
	LWP::Primitive::Surface* backGround_;

	//タイトル
	LWP::Primitive::Surface* logo_omote_;
	LWP::Primitive::Surface* logo_ura;


	// モデル
	LWP::Primitive::Mesh* model_ = nullptr;

	LWP::Primitive::Mesh* headModel_ = nullptr;				//頭
	LWP::Primitive::Mesh* bodyModel_ = nullptr;				//上半身
	LWP::Primitive::Mesh* leftForeArmModel_ = nullptr;		//左腕　手から肘
	LWP::Primitive::Mesh* leftUpperArmModel_ = nullptr;		//左腕　肘から肩
	LWP::Primitive::Mesh* rightForeArmModel_ = nullptr;		//右腕　手から肘
	LWP::Primitive::Mesh* rightUpperArmModel_ = nullptr;	//右腕　肘から肩
	LWP::Primitive::Mesh* footModel_ = nullptr;				//下半身

	LWP::Primitive::Mesh* bonfire_ = nullptr;

	LWP::Object::PointLight* light_;

	// プレイヤーのスタート地点
	LWP::Math::Vector3 startPosition_;
	// カメラのオフセット
	LWP::Math::Vector3 cameraOffset_;

	//床
	Floor floor;
	OutLineWall wall[5];
	Fire fire;


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