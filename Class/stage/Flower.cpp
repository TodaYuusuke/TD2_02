#pragma once
#include "Flower.h"

using namespace LWP::Math;
using namespace LWP::Utility;

void Flower::Init(Vector2 position) {
	// 花
	model_ = LWP::Resource::LoadModel("Flower/Flower.obj");
	model_->transform.translation.y = 0.05f;
	model_->transform.scale = { 0.0f,0.0f,0.0f };
	model_->material.enableLighting = true;
	// 茎
	modelStem_ = LWP::Resource::LoadModel("Flower/FlowerStem.obj");
	modelStem_->transform.translation.x = position.x;
	modelStem_->transform.translation.z = position.y;
	modelStem_->material.enableLighting = true;
	modelStem_->commonColor = new Color(0x00FF07FF);
	// 双葉
	modelLeaf_[0] = LWP::Resource::LoadModel("Flower/FlowerLeaf.obj");
	modelLeaf_[0]->transform.translation.x = position.x;
	modelLeaf_[0]->transform.translation.z = position.y;
	modelLeaf_[0]->transform.scale = { 0.5f,0.5f,0.5f };
	modelLeaf_[0]->material.enableLighting = true;
	modelLeaf_[0]->commonColor = new Color(0x00FF07FF);
	modelLeaf_[1] = LWP::Resource::LoadModel("Flower/FlowerLeaf.obj");
	modelLeaf_[1]->transform.translation.x = position.x;
	modelLeaf_[1]->transform.translation.z = position.y;
	modelLeaf_[1]->transform.scale = { 0.5f,0.5f,0.5f };
	modelLeaf_[1]->transform.rotation.y = 3.14f;
	modelLeaf_[1]->material.enableLighting = true;
	modelLeaf_[1]->commonColor = new Color(0x00FF07FF);

	// 親子関係をセット
	model_->transform.Parent(&modelStem_->transform);

	lightingTime_ = 0;


	// 色をランダムに
	switch (GenerateRandamNum<int>(0, 5)) {
	case 0:
		model_->commonColor = new Color(RED);
		break;
	case 1:
		model_->commonColor = new Color(ColorPattern::BLUE);
		break;
	case 2:
		model_->commonColor = new Color(ColorPattern::CYAN);
		break;
	case 3:
		model_->commonColor = new Color(ColorPattern::MAGENTA);
		break;
	case 4:
		model_->commonColor = new Color(ColorPattern::YELLOW);
		break;
	case 5:
		model_->commonColor = new Color(0xD40090FF);
		break;
	}

	swayFrame_ = GenerateRandamNum<int>(0, swayEndFrame_);
	maxAmplitude_ = (float)M_PI / 16.0f;

	float rad = static_cast<float>(GenerateRandamNum<int>(0, 628) / 100.0f);
	modelStem_->transform.rotation.y = rad;
	modelLeaf_[0]->transform.rotation.y += rad;
	modelLeaf_[1]->transform.rotation.y += rad;
}

void Flower::Update() {
	// 照らされている時間をマイナス
	lightingTime_--;
	// 幅は超えないように
	lightingTime_ = std::clamp<int>(lightingTime_, 0, 20);

	// 茎が生えるアニメーション
	if (lightingTime_ <= 10) {
		modelStem_->transform.scale.y = easeOut((float)lightingTime_ / 60.0f, 0.0f, 0.5f, 10.0f / 60.0f);
	}
	// 花が生えるアニメーション
	else if (lightingTime_ <= 20) {
		float value = easeOut((float)(lightingTime_ - 10) / 60.0f, 0.0f, 0.5f, 10.0f / 60.0f);
		model_->transform.scale = { value,value,value };
		isSway_ = true;
	}

	// 花が非表示ならアニメーションをしない
	if (!model_->isActive) {
		isSway_ = false;
	}

	// 揺れるアニメーション
	if (isSway_) {
		if (swayFrame_ >= swayEndFrame_) {
			swayFrame_ = 0;
		}

		modelStem_->transform.rotation.x = maxAmplitude_ * sinf(swayFrame_ * kCycleSpeed_);
		modelStem_->transform.rotation.z = maxAmplitude_ * sinf(swayFrame_ * kCycleSpeed_);

		model_->transform.rotation.x = (maxAmplitude_ + 0.175f) * sinf(swayFrame_ * kCycleSpeed_);
		model_->transform.rotation.z = (maxAmplitude_ + 0.175f) * sinf(swayFrame_ * kCycleSpeed_);

		for (int i = 0; i < 2; i++) {
			modelLeaf_[i]->transform.rotation.x = modelStem_->transform.rotation.x;
			modelLeaf_[i]->transform.rotation.z = modelStem_->transform.rotation.z;
		}

		swayFrame_++;
	}
}

void Flower::OnActive() {
	modelLeaf_[0]->isActive = true;
	modelLeaf_[1]->isActive = true;
	modelStem_->isActive = true;
	model_->isActive = true;
}
// 花をOFFに
void Flower::OffActive() {
	modelLeaf_[0]->isActive = false;
	modelLeaf_[1]->isActive = false;
	modelStem_->isActive = false;
	model_->isActive = false;
}