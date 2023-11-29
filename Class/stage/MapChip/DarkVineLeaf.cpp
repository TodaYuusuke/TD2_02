#include "Derved.h"

using namespace LWP::Utility;

void DarkVineLeaf::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("VineFloor/VineFloorLeaf.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	//model_->commonColor = new Color(ColorPattern::RED);
	model_->material.enableLighting = true;

	maxAmplitude_ = 0.01f;
}

void DarkVineLeaf::Update() {
	// 上下に揺れるアニメーション
	model_->transform.translation.y = maxAmplitude_ * sinf(swayFrame_ * kCycleSpeed_);

	swayFrame_++;
}

bool DarkVineLeaf::IsMapChipCollision() {
	return false;
}
bool DarkVineLeaf::IsGroundCollision() {
	return !isDead_;
}

//void DarkVineLeaf::OnActive() {
//	model_->isActive = true;
//};
//void DarkVineLeaf::OffActive() {
//	model_->isActive = false;
//	isAnimation_ = false;
//};