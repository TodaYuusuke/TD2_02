#include "Derved.h"

void Hole::Init(LWP::Math::Vector3 position, float scale) {
	model_ = nullptr;	// モデル無し
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