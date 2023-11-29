#include "Derved.h"

using namespace LWP::Math;
using namespace LWP::Utility;

void StageMap::Init(LWP::Math::Vector3 position, float scale) {
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

	// 看板読み込み
	signModel_ = LWP::Resource::LoadModel("StageSelect/Sign.obj");
	signModel_->transform.translation = position;
	signModel_->transform.scale = { scale,scale,scale };
	signModel_->material.enableLighting = true;

	LWP::Math::Vector3 vec[4] = {
		{0.3f,0.0f,0.3f},
		{-0.3f,0.0f,0.3f},
		{0.3f,0.0f,-0.3f},
		{-0.3f,0.0f,-0.3f}
	};

	for (int i = 0; i < kMaxTree; i++) {
		// 幹のモデル読み込み
		trunkModel_[i] = LWP::Resource::LoadModel("Tree/Trunk.obj");
		trunkModel_[i]->transform.Parent(&model_->transform);
		trunkModel_[i]->transform.translation = vec[i];
		trunkModel_[i]->transform.translation.x += static_cast<float>(GenerateRandamNum<int>(-10, 10)) / 100.0f;
		trunkModel_[i]->transform.translation.z += static_cast<float>(GenerateRandamNum<int>(-10, 10)) / 100.0f;
		float s = static_cast<float>(GenerateRandamNum<int>(80, 120)) / 100.0f;
		//float s = 2.5f;
		trunkModel_[i]->transform.scale = { s,s,s };
		trunkModel_[i]->transform.rotation.y = static_cast<float>(GenerateRandamNum<int>(0, 628)) / 100.0f;
		trunkModel_[i]->commonColor = new Color(0x0a0a09FF);
		trunkModel_[i]->material.enableLighting = true;

		// 葉のモデル読み込み
		leavesModel_[i][0] = LWP::Resource::LoadModel("Tree/LeavesL.obj");
		leavesModel_[i][1] = LWP::Resource::LoadModel("Tree/LeavesM.obj");
		leavesModel_[i][2] = LWP::Resource::LoadModel("Tree/LeavesS.obj");
		leavesModel_[i][0]->transform.Parent(&trunkModel_[i]->transform);
		leavesModel_[i][1]->transform.Parent(&leavesModel_[i][0]->transform);
		leavesModel_[i][2]->transform.Parent(&leavesModel_[i][1]->transform);
		for (int n = 0; n < 3; n++) {
			leavesModel_[i][n]->transform.Parent(&trunkModel_[i]->transform);
			leavesModel_[i][n]->transform.rotation.y = static_cast<float>(GenerateRandamNum<int>(0, 628)) / 100.0f;
			//float ss = static_cast<float>(GenerateRandamNum<int>(80, 120)) / 100.0f;
			//leavesModel_[i][n]->transform.scale = { ss,ss,ss };
			leavesModel_[i][n]->commonColor = new Color(0x021f03FF);
			leavesModel_[i][n]->material.enableLighting = true;
		}
	}
#endif
}

void StageMap::Update() {
	// 特になし
}

bool StageMap::IsMapChipCollision() {
	return false;
}
bool StageMap::IsGroundCollision() {
	return true;
}

void StageMap::OnActive() {
    model_->isActive = true;
#if _DEBUG
#else
	for (int i = 0; i < kMaxTree; i++) {
		trunkModel_[i]->isActive = true;
		for (int n = 0; n < 3; n++) {
			leavesModel_[i][n]->isActive = true;
		}
	}
#endif
}
void StageMap::OffActive() {
    model_->isActive = false;
#if _DEBUG
#else
	for (int i = 0; i < kMaxTree; i++) {
		trunkModel_[i]->isActive = false;
		for (int n = 0; n < 3; n++) {
			leavesModel_[i][n]->isActive = false;
		}
	}
#endif
}

void StageMap::NearPlayer(LWP::Math::Vector3 playerPosition) {
	// アニメーション用のフレームでもカウントアップさせる予定
	playerPosition;
}