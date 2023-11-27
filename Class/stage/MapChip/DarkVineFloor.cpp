#include "Derved.h"

void DarkVineFloor::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void DarkVineFloor::Update() {
	if (isGrew_ > 0 && isGrew_ <= kFrameUntilDeath) {
		isGrew_--;
	}
}

bool DarkVineFloor::IsMapChipCollision() {
	return false;
}
bool DarkVineFloor::IsGroundCollision() {
	return true;
}

void DarkVineFloor::GrawUp() {
	isGrew_ += 2;
}