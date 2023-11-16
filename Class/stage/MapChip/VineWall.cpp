#include "Derved.h"

void VineWall::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void VineWall::Update() {
	// 特になし
}

bool VineWall::IsMapChipCollision() {
	return false;
}
bool VineWall::IsGroundCollision() {
	return true;
}