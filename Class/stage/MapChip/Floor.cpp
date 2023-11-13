#include "Derved.h"

void Floor::Init(LWP::Math::Vector3 position) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->material.enableLighting = true;
}

void Floor::Update() {
	// “Á‚É‚È‚µ
}