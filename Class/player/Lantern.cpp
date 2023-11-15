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

	LightBehaviorNormalInit();
}

void Lantern::Update() {
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
	lightNormal_.EndFrame = static_cast<int>(distribution(randomEngine));
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
	intensityCycleFrame_ = 8;

	// 乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 終了フレームを乱数で生成
	std::uniform_int_distribution<> distribution(1,3);
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
	ImGui::Begin(" ");
	ImGui::Text("FlickerCount:%d", flickerCount_);
	ImGui::End();
}