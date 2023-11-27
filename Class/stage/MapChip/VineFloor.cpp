#include "Derved.h"

void VineFloor::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void VineFloor::Update() {
	isGrew_--;
}

bool VineFloor::IsMapChipCollision() {
	return false;
}
bool VineFloor::IsGroundCollision() {
	return true;
}

void VineFloor::GrawUp() {
	isGrew_ = 2;
}