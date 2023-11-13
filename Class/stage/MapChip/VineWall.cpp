#include "Derved.h"

void VineWall::Init(LWP::Math::Vector3 position) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->material.enableLighting = true;
}

void VineWall::Update() {
	// “Á‚É‚È‚µ
}