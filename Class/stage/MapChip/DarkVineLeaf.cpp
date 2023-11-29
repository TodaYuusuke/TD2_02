#include "Derved.h"

using namespace LWP::Utility;

void DarkVineLeaf::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("VineFloor/VineFloorLeaf.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->commonColor = new Color(0xFF1408FF);
	model_->material.enableLighting = true;
	model_->texture = LWP::Resource::LoadTextureLongPath("resources/obj/VineFloor/DarkVineLeaf.png");

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