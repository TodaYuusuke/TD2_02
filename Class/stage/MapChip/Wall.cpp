#include "Derved.h"

void Wall::Init(LWP::Math::Vector3 position) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.translation.y += 1.0f;
	model_->material.enableLighting = true;
}

void Wall::Update() {
	// “Á‚É‚È‚µ
}