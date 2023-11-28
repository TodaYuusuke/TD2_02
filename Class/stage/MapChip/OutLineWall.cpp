#include "Derved.h"

using namespace LWP::Math;
using namespace LWP::Utility;

void OutLineWall::Init(LWP::Math::Vector3 position, float scale) {
#if _DEBUG
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.translation.y += 1.0f;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
#else
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	LWP::Math::Vector3 vec[4] = {
		{0.3f,0.0f,0.3f},
		{-0.3f,0.0f,0.3f},
		{0.3f,0.0f,-0.3f},
		{-0.3f,0.0f,-0.3f}
	};

	for (int i = 0; i < kMaxTree; i++) {
		// 幹のモデル読み込み
		trunkModel_[i] = LWP::Resource::LoadModel("OutLineWall/Trunk.obj");
		trunkModel_[i]->transform.Parent(&model_->transform);
		trunkModel_[i]->transform.translation = vec[i];
		trunkModel_[i]->transform.rotation.y = static_cast<float>(GenerateRandamNum<int>(0, 628)) / 100.0f;
		trunkModel_[i]->transform.scale = { scale,scale,scale };
		trunkModel_[i]->material.enableLighting = true;

		// 葉のモデル読み込み
		leavesModel_[i] = LWP::Resource::LoadModel("OutLineWall/Leaves.obj");
		leavesModel_[i]->transform.Parent(&trunkModel_[i]->transform);
		trunkModel_[i]->transform.rotation.y = static_cast<float>(GenerateRandamNum<int>(0, 628)) / 100.0f;
		leavesModel_[i]->transform.scale = { scale,scale,scale };
		leavesModel_[i]->material.enableLighting = true;
	}
#endif
}

void OutLineWall::Update() {
	// 特になし
}

bool OutLineWall::IsMapChipCollision() {
	return true;
}
bool OutLineWall::IsGroundCollision() {
	return true;
}

void OutLineWall::OnActive() {
    model_->isActive = true;
#if _DEBUG
#else
	for (int i = 0; i < kMaxTree; i++) {
		trunkModel_[i]->isActive = true;
		leavesModel_[i]->isActive = true;
	}
#endif
}
void OutLineWall::OffActive() {
    model_->isActive = false;
#if _DEBUG
#else
	for (int i = 0; i < kMaxTree; i++) {
		trunkModel_[i]->isActive = false;
		leavesModel_[i]->isActive = false;
	}
#endif
}