#include "Derved.h"

void Candle::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 火をつけるモデル
	candleModel_ = LWP::Resource::LoadModel("Candle/Candle.obj");
	candleModel_->transform.translation = position;
	candleModel_->transform.scale = { scale,scale,scale };
	candleModel_->material.enableLighting = false;
	// 光源
	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->transform.translation = position;
	light_->transform.translation.y = 0.4f;
	light_->color = LWP::Utility::Color(0x0085FFFF);
	light_->radius = 1.0f;
	light_->decay = 5.0f;
	light_->isActive = true;
}

void Candle::Update() {
	if (isIgnited) {
		light_->color = LWP::Utility::Color::KelvinToRGB(1700);
		light_->radius = 2.5f;
		light_->decay = 2.5f;
	}
	// 特になし
}

bool Candle::IsMapChipCollision() {
	return false;
}
bool Candle::IsGroundCollision() {
	return true;
}