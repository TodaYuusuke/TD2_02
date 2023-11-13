#include "Derved.h"

void VineFloor::Init(LWP::Math::Vector3 position) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->material.enableLighting = true;
}

void VineFloor::Update() {
	// “Á‚É‚È‚µ
}