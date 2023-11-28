#pragma once
#include <Adapter.h>

class Flower final {
public:	// ** メンバ関数 ** //

	// 初期化（座標）
	void Init(LWP::Math::Vector2 position);
	// 更新
	void Update();

	// 照らされている時間を+1（毎フレームマイナスされるので+2）
	void AddLightingTime() { lightingTime_ += 2; }
	// ワールド座標を受け取る
	LWP::Math::Vector3 GetWorldPosition() { return modelLeaf_[0]->transform.GetWorldPosition(); }

	// 花をONに
	void OnActive();
	// 花をOFFに
	void OffActive();

private: // ** メンバ変数 ** //

	// 花
	LWP::Primitive::Mesh* model_;
	// 茎
	LWP::Primitive::Mesh* modelStem_;
	// 双葉
	LWP::Primitive::Mesh* modelLeaf_[2];

	// 照らされている時間
	int lightingTime_ = 0;

	// 揺れるアニメーション
	bool isSway_ = false;
	// アニメーション時間
	int swayFrame_;

	// 振幅
	float maxAmplitude_;

	// 定数
	// 1往復するまでの時間
	const int swayEndFrame_ = 120;
	// 1フレーム当たりの変化量
	const float kCycleSpeed_ = 2 * M_PI / swayEndFrame_;

private: // ** プライベートな関数 ** //
	// t = 
	// b = 開始の値
	// c = 開始との差分
	// d = 
	float easeOut(float t, float b, float c, float d) {
		t /= d;
		return -c * t * (t - 2.0f) + b;
	}
};