#include "Derved.h"

void Hole::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Hole/Hole.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void Hole::Update() {
	// 特になし
}

bool Hole::IsMapChipCollision() {
	return false;
}
bool Hole::IsGroundCollision() {
	return false;
}