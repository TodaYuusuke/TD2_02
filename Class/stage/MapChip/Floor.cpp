#include "Derved.h"

void Floor::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void Floor::Update() {
	// 特になし
}

bool Floor::IsMapChipCollision() {
	return false;
}
bool Floor::IsGroundCollision() {
	return true;
}