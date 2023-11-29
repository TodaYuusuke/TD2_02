#pragma once
#include <Adapter.h>
#include "Lantern.h"

// 前方宣言
class Stage;

class Player {
public: // ** メンバ関数 ** //

	// 初期化
	void Init(LWP::Math::Vector3 startPosition, LWP::Object::Camera* camera);
	// 更新
	void Update(Stage* stage);

	// カメラの追従切り替え
	void ChangeFollowCamera();

protected: // ** プライベートな定数 ** //

	// プレイヤーの速度
	const float kPlayerSpeed = 0.02f;
	// 追従カメラの速度
	const float kFollowCameraSpeed = 0.033f;

	// 重力加速度
	const float kGravities = -9.8f / 60.0f / 100.0f;


protected: // ** メンバ変数 ** //

	// モデル
	LWP::Primitive::Mesh* model_ = nullptr;

	LWP::Primitive::Mesh* headModel_ = nullptr;				//頭
	LWP::Primitive::Mesh* bodyModel_ = nullptr;				//上半身
	LWP::Primitive::Mesh* leftForeArmModel_ = nullptr;		//左腕　手から肘
	LWP::Primitive::Mesh* leftUpperArmModel_ = nullptr;		//左腕　肘から肩
	LWP::Primitive::Mesh* rightForeArmModel_ = nullptr;		//右腕　手から肘
	LWP::Primitive::Mesh* rightUpperArmModel_ = nullptr;	//右腕　肘から肩
	LWP::Primitive::Mesh* footModel_ = nullptr;				//下半身


	// プレイヤーのスタート地点
	LWP::Math::Vector3 startPosition_;
	// 重力による加速度
	float gravitiesAT = 0.0f;

	// ランタン
	Lantern lantern_;
	// ランタンを持つ場所
	LWP::Object::WorldTransform grabPosition_;
	// 振るまい
	enum class Behavior {
		GrabLantern, // ランタンを持っている状態
		ReadyToThrow,	// ランタンを投げる構え
		Throwing,	// 投げている最中
		NoHave,	 // 何も持っていない状態
	};
	Behavior behavior_ = Behavior::GrabLantern;

	// 追従させるカメラのポインタ
	LWP::Object::Camera* camera_ = nullptr;
	// 追従カメラのゴール地点
	LWP::Math::Vector3 cameraGoalTranslation_ = { 0.0f, 0.0f, 0.0f };
	LWP::Math::Vector3 cameraGoalRotation_ = { 0.0f, 0.0f, 0.0f };
	// カメラのオフセット
	LWP::Math::Vector3 cameraOffset_ = { 0.0f, 0.0f, -8.0f };
	int cameraOffsetMultiply = 3;
	// カメラを追従させるかのフラグ
	bool isFollowingCamera_ = true;


protected: // ** プライベートな関数 ** //

	// 移動
	void Move();
	// ランタンに関する処理
	virtual void Action();

	// カメラの追従処理
	void FollowCameraTurn();

	// 派生クラス用更新処理
	virtual void UpdateDerved(Stage* stage);

	// 線形補完
	LWP::Math::Vector3 Lerp(const LWP::Math::Vector3& v1, const LWP::Math::Vector3& v2, float t);
	// 球面線形補間（あとでエンジンのUtilityに持ってく）
	LWP::Math::Vector3 Slerp(const LWP::Math::Vector3& v1, const LWP::Math::Vector3& v2, float t);

};