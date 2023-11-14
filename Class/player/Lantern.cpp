#include "Lantern.h"
#include "../stage/Stage.h"

void Lantern::Init() {
	handleModel_ = LWP::Resource::LoadModel("Lantern/Lantern_Handle.obj");
	handleModel_->transform.scale = { 0.1f,0.1f,0.1f };
	handleModel_->name = "Handle";
	model_ = LWP::Resource::LoadModel("Lantern/Lantern.obj");
	model_->name = "Hontai";
	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->transform.translation.y = -1.2f;
	light_->color = LWP::Utility::Color::KelvinToRGB(2000);
	light_->radius = 3.0f;
	light_->decay = 1.5f;
	light_->isActive = true;
	light_->name = "Lantern";
	
	// 親子関係をセット
	model_->transform.Parent(&handleModel_->transform);
	light_->transform.Parent(&model_->transform);
}

void Lantern::Update(Stage* stage) {
#if _DEBUG
	ImGui::Begin("Lantern");
	if (ImGui::TreeNode("Handle")) {
		handleModel_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Hontai")) {
		model_->DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Light")) {
		light_->DebugGUI();
		ImGui::TreePop();
	}
	ImGui::End();
#endif
}

void Lantern::Grab(LWP::Object::WorldTransform* trasnform) {
	handleModel_->transform.Parent(trasnform);
}