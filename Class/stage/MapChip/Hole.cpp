#include "Derved.h"

void Hole::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Hole/Hole.obj");
	model_->transform.translation = position;
	model_->transform.rotation.y = static_cast<float>(LWP::Utility::GenerateRandamNum<int>(0, 3)) * 1.57f;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 足場になる葉っぱのモデル
	leafModel_ = LWP::Resource::LoadModel("VineFloor/VineFloorLeaf.obj");
	leafModel_->transform.translation = position;
	leafModel_->transform.scale = { scale,scale,scale };
	leafModel_->material.enableLighting = true;

	maxAmplitude_ = 0.01f;
}

void Hole::Update() {
	isGrew_--;
	if (isGrew_ > 0) {
		isAnimation_ = true;
	}

	if (swayFrame_ >= swayEndFrame_) {
		isAnimation_ = false;
		swayFrame_ = startAnimationFrame_;
	}
	// 上下に揺れるアニメーション
	if (isAnimation_) {

		leafModel_->transform.translation.y = maxAmplitude_ * sinf(swayFrame_ * kCycleSpeed_);

		swayFrame_++;
	}
	if (!isAnimation_) {
		waitFrame_++;
	}
	if (waitFrame_ >= startAnimationFrame_) {
		isAnimation_ = true;
	}

	// 照らされている時間をマイナス
	lightingTime_--;
	// 幅は超えないように
	lightingTime_ = std::clamp<int>(lightingTime_, 0, 2);
	// 花が生えるアニメーション
	if (lightingTime_ <= 2) {
		float value = easeOut((float)(lightingTime_) / 60.0f, 0.00f, 1.0f, 2.0f / 60.0f);
		leafModel_->transform.scale = { value,value,value };
	}
	else if (lightingTime_ == 0) {
		isAnimation_ = false;
	}
}

bool Hole::IsMapChipCollision() {
	return false;
}
bool Hole::IsGroundCollision() {
	// もし葉っぱがあるなら当たり判定あり
	if (isGrew_ > 0) {
		return true;
	}

	return false;
}

void Hole::GrawUp() {
	isGrew_ = 2;
	lightingTime_ += 2;
}

void Hole::OnActive() {
	model_->isActive = true;
	leafModel_->isActive = true;
}
void Hole::OffActive() {
	model_->isActive = false;
	leafModel_->isActive = false;
}