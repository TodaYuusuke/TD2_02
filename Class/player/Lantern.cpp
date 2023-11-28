#include "Lantern.h"
#include <random>
#include "../stage/Stage.h"

using namespace LWP::Math;

void Lantern::Init() {
	handleModel_ = LWP::Resource::LoadModel("Lantern/Lantern_Handle.obj");
	handleModel_->transform.scale = { 0.1f,0.1f,0.1f };
	handleModel_->name = "Handle";
	model_ = LWP::Resource::LoadModel("Lantern/Lantern.obj");
	model_->name = "Hontai";
	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->transform.translation.y = -1.2f;
	light_->color = LWP::Utility::Color::KelvinToRGB(1700);
	light_->radius = 3.25f;
	light_->decay = 1.5f;
	light_->isActive = true;
	light_->name = "Lantern";
	// 親子関係をセット
	model_->transform.Parent(&handleModel_->transform);
	light_->transform.Parent(&model_->transform);

	// ライトのふるまいの初期化
	LightBehaviorNormalInit();
	// ランタンの揺れの初期設定
	WaitSwingAmplitude();
}

void Lantern::Update(Stage* stage) {
#if _DEBUG
	ImGui::Begin("Lantern");
	if (ImGui::TreeNode("Handle")) {
		handleModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Hontai")) {
		model_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Light")) {
		light_->DebugGUI();
		ImGui::TreePop();
	}
	ImGui::DragFloat("cycleSpeed", &hontaiRotateCycleFrame_, 1);
	ImGui::DragFloat("amplitude", &hontaiMaxAmplitude_, 0.01f);
	ImGui::DragFloat("factorX", &factorX_, 0.01f);
	ImGui::DragFloat("factorZ", &factorZ_, 0.01f);
	ImGui::Text("%f", throwingTimeCount_);
	ImGui::End();
#endif

	// ライトのふるまい
	LightBehavior();

	// 掴まれているときの処理
	if (isGrabed) {
		// ランタンの揺れる処理
		SwingUpdate();
	}
	else {
		// 投げられている最中のアニメーション
		ThrowingAnimationUpdate();
		// 速度と重力加速度を足す
		gravitiesAT += kGravities;
		handleModel_->transform.translation.y += gravitiesAT;


		// 当たり判定を取る座標8点
		Matrix4x4 rotation = Matrix4x4::CreateRotateXYZMatrix(model_->transform.rotation);
		Vector3 checkPos[8] = {
			handleModel_->transform.translation - (Vector3{handleModel_->transform.scale.x,0.0f,handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{-handleModel_->transform.scale.x,0.0f,handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{handleModel_->transform.scale.x,0.0f,-handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{-handleModel_->transform.scale.x,0.0f,-handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{handleModel_->transform.scale.x,0.2f,handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{-handleModel_->transform.scale.x,0.2f,handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{handleModel_->transform.scale.x,0.2f,-handleModel_->transform.scale.z} / 2.0f) * rotation,
			handleModel_->transform.translation - (Vector3{-handleModel_->transform.scale.x,0.2f,-handleModel_->transform.scale.z} / 2.0f) * rotation
		};

#if _DEBUG	// 当たり判定表示用の球
		//static LWP::Primitive::Sphere* s[8] = {
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>(),
		//	LWP::Primitive::CreateInstance<LWP::Primitive::Sphere>()
		//};
#endif

	// 判定がなくなるまで無限ループ
		bool isHit = true;

		// 回りの判定をとる
		while (isHit) {
			isHit = false;
			for (int i = 0; i < 8; i++) {
#if _DEBUG	// 当たり判定表示用の球
				//s[i]->transform = checkPos[i];
				//s[i]->Radius(0.02f);
#endif
				Vector3 fixVector = { 0.0f,0.0f,0.0f };
				bool result = stage->CheckCollision(checkPos[i], &fixVector, false, false);
				// ヒットしていればフラグをtrueに
				if (result) {
					isHit = result;

					// もしY軸補正がされた場合
					if (fixVector.y > 0.0f) {
						// 座標がある程度下に落ちてしまっているならばY補正を無視する（崖に落ちてるのに上に復帰してしまうので）
						if (handleModel_->transform.translation.y < -0.1f) {
							fixVector.y = 0.0f;
						}
						else {
							fixVector.x = 0.0f;
							fixVector.z = 0.0f;
							gravitiesAT = 0.0f;
						}
					}

					for (int j = 0; j < 8; j++) {
						checkPos[j] += fixVector;
					}

					// もし投げるアニメーション中に、当たった場合 -> 着地点をほぼ真下に
					if (throwingTimeCount_ < 1.0f) {
						throwingTimeCount_ = 1.0f;
					}

					handleModel_->transform.translation += fixVector;
				}
			}
		}
	}

	// 一定以上下に落ちたとき -> 
	if (handleModel_->transform.translation.y < -2.5f) { 
		/* 後で追加 */ 
		handleModel_->transform.translation.y = -2.5f;
	}

	// 光源の当たり判定をチェックする
	stage->CheckLightCollision(light_->transform.GetWorldPosition(), 2.5f);
}

void Lantern::Grab(LWP::Object::WorldTransform* transform) {
	handleModel_->transform.Parent(transform);
	handleModel_->transform.translation = { 0.0f, 0.0f, 0.0f };
	handleModel_->transform.scale = { 0.1f, 0.1f, 0.1f };
	isGrabed = true;
}
void Lantern::Throw(Vector3 rotation) {
	handleModel_->transform = handleModel_->transform.GetWorldPosition();
	handleModel_->transform.Parent(nullptr);
	handleModel_->transform.scale = { 0.05f, 0.05f, 0.05f };
	throwingTimeCount_ = 0.0f;
	throwingPositionStart_ = handleModel_->transform.GetWorldPosition();
	throwingPositionDiff_ = Vector3{ 0.0f,0.0f,3.0f } * Matrix4x4::CreateRotateXYZMatrix(rotation);;
	gravitiesAT = 0.033f;
	isGrabed = false;
}

void Lantern::ThrowingAnimationUpdate() {
	if (throwingTimeCount_ >= 1.0f) {
		return;
	}
	throwingTimeCount_ += 1.0f / 60.0f;
	if (throwingTimeCount_ > 1.0f) {
		throwingTimeCount_ = 1.0f;
	}
	
	// b = 開始の値
	// c = 開始との差分
	auto easeOut = [&](float b, float c) {
		float t = throwingTimeCount_ / 1.0f;
		float t2 = (throwingTimeCount_ - 1.0f / 60.0f) / 1.0f;
		float result = -c * t * (t - 2.0f) + b;
		float result2 = -c * t2 * (t2 - 2.0f) + b;
		return result - result2;
	};

	handleModel_->transform.translation.x += easeOut(throwingPositionStart_.x, throwingPositionDiff_.x);
	handleModel_->transform.translation.z += easeOut(throwingPositionStart_.z, throwingPositionDiff_.z);
}

void Lantern::LightBehavior() {
	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kNormal:
		default:
			LightBehaviorNormalInit();
			break;
			// 攻撃行動
		case Behavior::kFlicker:
			LightBehaviorFlickerInit();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 更新処理
	switch (behavior_) {
	case Behavior::kNormal:
	default:
		LightBehaviorNormalUpdate();
		break;
		// 攻撃行動
	case Behavior::kFlicker:
		LightBehaviorFlickerUpdate();
		break;
	}
}

void Lantern::LightBehaviorNormalInit() {
	// 光の強さを戻す
	light_->intensity = 1.0f;
	// 振幅
	lightAmplitude_ = 0;
	// 周期
	intensityCycle_ = 0;
	// 1往復何フレーム
	intensityCycleFrame_ = 60;

	// 乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 現在のフレーム
	lightNormal_.currentFrame = 0;
	// 終了フレームを乱数で生成
	std::uniform_real_distribution<float> distribution(600.0f, 900.0f);
	lightNormal_.EndFrame = 60;//static_cast<int>(distribution(randomEngine));
}

void Lantern::LightBehaviorNormalUpdate() {
	if (lightNormal_.currentFrame >= lightNormal_.EndFrame) {
		// 点滅に移行
		behaviorRequest_ = Behavior::kFlicker;
	}

	// 振幅の最大値&最小値
	const float kMaxAmplitude = 0.2f;
	// 周期(2秒で1往復)
	const float kCycleSpeed = static_cast<float>(M_PI / intensityCycleFrame_);
	intensityCycle_ += kCycleSpeed;

	//振幅を計算
	lightAmplitude_ = kMaxAmplitude * sinf(intensityCycle_);
	// lightに計算結果を反映
	light_->intensity = 1.0f - lightAmplitude_;

	lightNormal_.currentFrame++;
}

void Lantern::LightBehaviorFlickerInit() {
	// 光の強さを戻す
	light_->intensity = 1.0f;
	lightAmplitude_ = 0;
	intensityCycle_ = 0;
	// 1往復何フレーム
	intensityCycleFrame_ = 16;

	// 乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 終了フレームを乱数で生成
	std::uniform_int_distribution<> distribution(1,2);
	flickerCount_ = distribution(randomEngine);

	// 現在のフレーム
	lightFlicker_.currentFrame = 0;
	// 何回点滅させるかを設定
	lightFlicker_.EndFrame = intensityCycleFrame_ / 2 * flickerCount_;
}

void Lantern::LightBehaviorFlickerUpdate() {
	if (lightFlicker_.currentFrame >= lightFlicker_.EndFrame) {
		// 通常状態に移行
		behaviorRequest_ = Behavior::kNormal;
	}

	// 振幅の最大値&最小値
	const float kMaxAmplitude = 0.5f;
	// 8フレームで1往復
	const float kCycleSpeed = M_PI / (intensityCycleFrame_ / 2);
	intensityCycle_ += kCycleSpeed;

	//振幅を計算
	lightAmplitude_ = kMaxAmplitude * sinf(intensityCycle_);
	// lightに計算結果を反映
	light_->intensity = 1.0f - fabs(lightAmplitude_);

	lightFlicker_.currentFrame++;
}

void Lantern::SwingUpdate() {
	if (hontaiRotateCycle_ >= 2 * M_PI) {
		hontaiRotateCycle_ = 0.0f;
	}

	// 周期(2秒で1往復)
	const float kCycleSpeed = 2 * M_PI / hontaiRotateCycleFrame_;
	hontaiRotateCycle_ += kCycleSpeed;

	model_->transform.rotation.x = factorX_ * hontaiMaxAmplitude_ * sinf(hontaiRotateCycle_);
	model_->transform.rotation.z = factorZ_ * hontaiMaxAmplitude_ / 2 * sinf(hontaiRotateCycle_ + (M_PI / 2));
}

void Lantern::WaitSwingAmplitude() {
	// 振幅の最大値&最小値
	hontaiMaxAmplitude_ = M_PI / 32.0f;
	hontaiRotateCycleFrame_ = 180;
	// 振幅の係数
	factorX_ = 1;
	factorZ_ = 1;
}

void Lantern::MoveSwingAmplitude() {
	// 振幅の最大値&最小値
	hontaiMaxAmplitude_ = M_PI / 4.0f;
	hontaiRotateCycleFrame_ = 60;
	// 振幅の係数
	factorX_ = 1;
	factorZ_ = 1;
}