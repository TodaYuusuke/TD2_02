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
	model_->commonColor = new Color(ColorPattern::RED);
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
	modelLeaf_[0]->material.enableLighting = true;
	modelLeaf_[0]->commonColor = new Color(0x00FF07FF);
	modelLeaf_[1] = LWP::Resource::LoadModel("Flower/FlowerLeaf.obj");
	modelLeaf_[1]->transform.translation.x = position.x;
	modelLeaf_[1]->transform.translation.z = position.y;
	modelLeaf_[1]->transform.rotation.y = 3.14f;
	modelLeaf_[1]->material.enableLighting = true;
	modelLeaf_[1]->commonColor = new Color(0x00FF07FF);

	// 親子関係をセット
	model_->transform.Parent(&modelStem_->transform);

	lightingTime_ = 0;
}

void Flower::Update() {
	// 照らされている時間をマイナス
	lightingTime_--;
	// 幅は超えないように
	lightingTime_ = std::clamp<int>(lightingTime_, 0, 20);
	
	ImGui::Begin("Flower");
	if (ImGui::TreeNode("Hontai")) {
		model_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Stem")) {
		modelStem_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Leaf0")) {
		modelLeaf_[0]->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Leaf1")) {
		modelLeaf_[1]->DebugGUI();
		ImGui::TreePop();
	}
	ImGui::End();

	// 茎が生えるアニメーション
	if (lightingTime_ <= 10) {
		modelStem_->transform.scale.y = easeOut((float)lightingTime_ / 60.0f, 0.0f, 1.0f, 10.0f / 60.0f);
	}
	// 花が生えるアニメーション
	else if(lightingTime_ <= 20) {
		float value = easeOut((float)(lightingTime_ - 10) / 60.0f, 0.0f, 1.0f, 10.0f / 60.0f);
		model_->transform.scale = { value,value,value };
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