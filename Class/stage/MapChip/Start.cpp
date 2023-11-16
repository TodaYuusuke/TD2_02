#include "Derved.h"

void Start::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void Start::Update() {
	// 特になし
}

bool Start::IsMapChipCollision() {
	return false;
}
bool Start::IsGroundCollision() {
	return true;
}