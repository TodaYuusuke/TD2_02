#pragma once
#include <Adapter.h>
#include <optional>

// 前方宣言
class Stage;

/// <summary>
/// ランタン
/// </summary>
class Lantern final {
public: // ** メンバ関数 ** //

	// 初期化
	void Init();
	// 更新
	void Update(Stage* stage);


	/// <summary>
	/// ランタンを掴む
	/// </summary>
	/// <returns>ランタンを追従させる用のペアレントポインタ</returns>
	void Grab(LWP::Object::WorldTransform* transform);
	/// <summary>
	/// ランタンを投げる
	/// </summary>
	/// <param name="direction">投げる方向のベクトル（正規化済みを送ること）</param>
	void Throw(LWP::Math::Vector3 rotation);

	// 歩いていないときの揺れ幅
	void WaitSwingAmplitude();
	// 歩いているときの揺れ幅
	void MoveSwingAmplitude();

	// ランタンのワールド座標を受け取る関数
	LWP::Math::Vector3 GetWorldPosition() { return handleModel_->transform.GetWorldPosition(); }


private: // ** プライベートな定数 ** //

	// 重力加速度
	const float kGravities = -9.8f / 60.0f / 100.0f;


private: // ** メンバ変数 ** //

	// モデル
	LWP::Primitive::Mesh* handleModel_;
	LWP::Primitive::Mesh* model_;
	// 光源
	LWP::Object::PointLight* light_;

	// 投げられたとき用の速度
	float gravitiesAT = 0.0f;

	// 掴まれているフラグ
	bool isGrabed = true;

	
private: // ** アニメーション関連 ** //

	// 投げてる最中の更新処理
	float throwingTimeCount_ = 99.0f;
	LWP::Math::Vector3 throwingPositionStart_;
	LWP::Math::Vector3 throwingPositionDiff_;
	void ThrowingAnimationUpdate();

	// ライトの振るまい
	void LightBehavior();

	// 通常時の初期化
	void LightBehaviorNormalInit();
	// 通常時の更新
	void LightBehaviorNormalUpdate();
	// 光の点滅初期化
	void LightBehaviorFlickerInit();
	// 光の点滅更新
	void LightBehaviorFlickerUpdate();

	// ランタンの揺れる挙動
	void SwingUpdate();


	// 振るまい
	enum class Behavior {
		kNormal,	 // 通常状態
		kFlicker // 光がチカチカ状態
	};
	Behavior behavior_ = Behavior::kNormal;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 各ふるまいで扱う時間
	struct BehaviorFrame {
		int currentFrame = 0;  // 現在のフレーム
		int EndFrame;		   // 終了フレーム
	};
	// 通常状態のライト
	BehaviorFrame lightNormal_;
	// 点滅時のライト
	BehaviorFrame lightFlicker_;

	// ライトの点滅の振幅
	float lightAmplitude_;
	// ライトの点滅の周期
	float intensityCycle_;
	// 1往復を何フレームで行うか
	int intensityCycleFrame_;
	// ライトの点滅回数(通常状態では使用しない)
	int flickerCount_;

	// ランタンの揺れ幅
	float hontaiMaxAmplitude_;
	// 揺れの周期
	float hontaiRotateCycle_;
	// 1往復するまでの時間
	float hontaiRotateCycleFrame_;

	// ランタンの揺れの係数
	float factorX_;
	float factorZ_;
};