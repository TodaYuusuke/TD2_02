#include "Derved.h"

void DarkVineLeaf::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void DarkVineLeaf::Update() {
	// 特になし
}

bool DarkVineLeaf::IsMapChipCollision() {
	return false;
}
bool DarkVineLeaf::IsGroundCollision() {
	return !isDead_;
}