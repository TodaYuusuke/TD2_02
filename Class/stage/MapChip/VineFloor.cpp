#include "Derved.h"

using namespace LWP::Utility;

void VineFloor::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 茎のモデル読み込み
	stemModel_ = LWP::Resource::LoadModel("VineFloor/VineFloorStem.obj");
	stemModel_->transform.translation = position;
	stemModel_->transform.scale = { scale,scale,scale };
	stemModel_->commonColor = new Color(0x021f03FF);
	stemModel_->material.enableLighting = true;
	// 花のモデル読み込み
	flowerModel_ = LWP::Resource::LoadModel("VineFloor/VineFloorFlower.obj");
	flowerModel_->transform.Parent(&stemModel_->transform);
	flowerModel_->commonColor = new Color(ColorPattern::RED);
	flowerModel_->material.enableLighting = true;
}

void VineFloor::Update() {
	isGrew_--;
}

bool VineFloor::IsMapChipCollision() {
	return false;
}
bool VineFloor::IsGroundCollision() {
	return true;
}

void VineFloor::GrawUp() {
	isGrew_ = 2;
}

void VineFloor::OnActive() {
	model_->isActive = true;
	stemModel_->isActive = true;
	flowerModel_->isActive = true;
};
void VineFloor::OffActive() {
	model_->isActive = false;
	stemModel_->isActive = false;
	flowerModel_->isActive = false;
};