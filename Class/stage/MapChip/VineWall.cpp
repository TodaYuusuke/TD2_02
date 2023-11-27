#include "Derved.h"

void VineWall::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
	
	treeModel_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	treeModel_->transform.translation = position;
	treeModel_->transform.translation.y += 1.0f;
	treeModel_->transform.scale = { scale,scale,scale };
	treeModel_->material.enableLighting = true;
	treeModel_->isActive = false;
}

void VineWall::Update() {
	isGrew_--;
	if (isGrew_ > 0) {
		treeModel_->isActive = true;
	}
	else {
		treeModel_->isActive = false;
	}
}

bool VineWall::IsMapChipCollision() {
	if (isGrew_ > 0) {
		return true;
	}

	return false;
}
bool VineWall::IsGroundCollision() {
	return true;
}

void VineWall::GrawUp() {
	isGrew_ = 2;
}