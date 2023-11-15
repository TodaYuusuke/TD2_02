#include "Derved.h"

void Candle::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
}

void Candle::Update() {
	// 特になし
}

bool Candle::IsMapChipCollision() {
	return false;
}
bool Candle::IsGroundCollision() {
	return true;
}