#include "Lantern.h"
#include <random>

void Lantern::Init() {
	handleModel_ = LWP::Resource::LoadModel("Lantern/Lantern_Handle.obj");
	handleModel_->transform.scale = { 0.1f,0.1f,0.1f };
	handleModel_->name = "Handle";
	model_ = LWP::Resource::LoadModel("Lantern/Lantern.obj");
	model_->name = "Hontai";
	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->color = LWP::Utility::Color::KelvinToRGB(2300);
	light_->radius = 3.0f;
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

void Lantern::Update() {
	// ランタンの揺れる処理
	SwingUpdate();

	// ライトのふるまい
	LightBehavior();

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
	ImGui::End();
#endif
}

LWP::Object::WorldTransform* Lantern::Grab() {
	return &handleModel_->transform;
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
	const float kCycleSpeed = M_PI / intensityCycleFrame_;
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