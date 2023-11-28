#include "Derved.h"

void Wall::Init(LWP::Math::Vector3 position, float scale) {
#if _DEBUG
	model_ = LWP::Resource::LoadModel("Wall/LowPolyWall.obj");
#else
	model_ = LWP::Resource::LoadModel("Wall/Wall.obj");
#endif
	model_->transform.translation = position;
	model_->transform.rotation.y = static_cast<float>(LWP::Utility::GenerateRandamNum<int>(0, 3)) * 1.57f;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void Wall::Update() {
	// 特になし
}

bool Wall::IsMapChipCollision() {
	return true;
}
bool Wall::IsGroundCollision() {
	return true;
}