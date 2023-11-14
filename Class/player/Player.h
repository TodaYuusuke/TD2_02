#pragma once
#include <Adapter.h>
#include "Lantern.h"

// 前方宣言
class Stage;

class Player final {
public: // ** メンバ関数 ** //

	// 初期化
	void Init(LWP::Object::Camera* camera);
	// 更新
	void Update(Stage* stage);

	// カメラの追従切り替え
	void ChangeFollowCamera();

private: // ** プライベートな定数 ** //

	// プレイヤーの速度
	const float kPlayerSpeed = 0.04f;
	// 追従カメラの速度
	const float kFollowCameraSpeed = 0.033f;

private: // ** メンバ変数 ** //

	// モデル
	LWP::Primitive::Mesh* model_ = nullptr;

	// ランタン
	Lantern lantern_;
	// ランタンを持つ場所
	LWP::Object::WorldTransform grabPosition_;

	// 追従させるカメラのポインタ
	LWP::Object::Camera* camera_ = nullptr;
	// 追従カメラのゴール地点
	LWP::Math::Vector3 cameraGoalTranslation_ = { 0.0f, 0.0f, 0.0f };
	LWP::Math::Vector3 cameraGoalRotation_ = { 0.0f, 0.0f, 0.0f };
	// カメラのオフセット
	LWP::Math::Vector3 cameraOffset_ = { 0.0f, 0.0f, -8.0f };
	// カメラを追従させるかのフラグ
	bool isFollowingCamera_ = true;

private: // ** プライベートな関数 ** //

	// 移動
	void Move();
	// ランタンに関する処理

	// カメラの追従処理
	void FollowCameraTurn();

	// 線形補完
	LWP::Math::Vector3 Lerp(const LWP::Math::Vector3& v1, const LWP::Math::Vector3& v2, float t);
	// 球面線形補間（あとでエンジンのUtilityに持ってく）
	LWP::Math::Vector3 Slerp(const LWP::Math::Vector3& v1, const LWP::Math::Vector3& v2, float t);

};