#include "Derved.h"

void Candle::Init(LWP::Math::Vector3 position) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->material.enableLighting = true;
}

void Candle::Update() {
	// “Á‚É‚È‚µ
}