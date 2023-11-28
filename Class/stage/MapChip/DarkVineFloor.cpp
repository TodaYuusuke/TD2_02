#include "Derved.h"

void DarkVineFloor::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 光源
	//light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	//light_->transform.translation = position;
	//light_->transform.translation.y = 0.4f;
	//light_->color = LWP::Utility::Color(0x0085FFFF);
	//light_->radius = 1.0f;
	//light_->decay = 5.0f;
	//light_->isActive = true;
}

void DarkVineFloor::Update() {
	if (isGrew_ > 0 && isGrew_ <= kFrameUntilDeath) {
		isGrew_--;
	}
	else if(isGrew_ > kFrameUntilDeath){
		//light_->isActive = false;
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