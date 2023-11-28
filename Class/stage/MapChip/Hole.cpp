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
}

void Hole::Update() {
	isGrew_--;
	if (isGrew_ > 0) {
		leafModel_->transform.scale = { 1.0f,1.0f,1.0f };
	}
	else {
		leafModel_->transform.scale = { 0.0f,0.0f,0.0f };
	}
}

bool Hole::IsMapChipCollision() {
	return false;
}
bool Hole::IsGroundCollision() {
	// もし葉っぱがあるなら当たり判定あり
	if (isGrew_ > 0) {
		return true;
	}

	return false;
}

void Hole::GrawUp() {
	isGrew_ = 2;
}

void Hole::OnActive() {
	model_->isActive = true;
	leafModel_->isActive = true;
}
void Hole::OffActive() {
	model_->isActive = false;
	leafModel_->isActive = false;
}