#include "Derved.h"

void Hole::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Hole/Hole.obj");
	model_->transform.translation = position;
	model_->transform.rotation.y = static_cast<float>(LWP::Utility::GenerateRandamNum<int>(0, 3)) * 1.57f;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 足場になる葉っぱのモデル
	leafModel_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	leafModel_->transform.translation = position;
	leafModel_->transform.scale = { scale,scale,scale };
	leafModel_->material.enableLighting = true;
	leafModel_->isActive = false;
}

void Hole::Update() {
	isGrew_--;
	if (isGrew_ > 0) {
		leafModel_->isActive = true;
	}
	else {
		leafModel_->isActive = false;
	}
}

bool Hole::IsMapChipCollision() {
	return false;
}
bool Hole::IsGroundCollision() {
	// もし葉っぱがあるなら当たり判定あり
	if (leafModel_->isActive) {
		return true;
	}

	return false;
}

void Hole::GrawUp() {
	isGrew_ = 2;
}