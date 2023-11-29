#include "Derved.h"

using namespace LWP::Math;
using namespace LWP::Utility;

void VineWall::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 幹のモデル読み込み
	trunkModel_ = LWP::Resource::LoadModel("Tree/Trunk.obj");
	trunkModel_->transform.Parent(&model_->transform);
	trunkModel_->transform.rotation.y = static_cast<float>(GenerateRandamNum<int>(0, 628)) / 100.0f;
	trunkModel_->commonColor = new Color(0x080804FF);
	trunkModel_->material.enableLighting = true;

	// 葉のモデル読み込み
	leavesModel_[0] = LWP::Resource::LoadModel("Tree/LeavesL.obj");
	leavesModel_[1] = LWP::Resource::LoadModel("Tree/LeavesM.obj");
	leavesModel_[2] = LWP::Resource::LoadModel("Tree/LeavesS.obj");
	leavesModel_[0]->transform.Parent(&trunkModel_->transform);
	leavesModel_[1]->transform.Parent(&leavesModel_[0]->transform);
	leavesModel_[2]->transform.Parent(&leavesModel_[1]->transform);
	for (int n = 0; n < 3; n++) {
		leavesModel_[n]->transform.Parent(&trunkModel_->transform);
		leavesModel_[n]->transform.rotation.y = static_cast<float>(GenerateRandamNum<int>(0, 628)) / 100.0f;
		//float ss = static_cast<float>(GenerateRandamNum<int>(80, 120)) / 100.0f;
		//leavesModel_[i][n]->transform.scale = {s,s,s};
		leavesModel_[n]->commonColor = new Color(0x2b992bFF);
		leavesModel_[n]->material.enableLighting = true;
	}
}

void VineWall::Update() {
    isGrew_--;
    if (isGrew_ > 0) {
        isAnimation_ = true;    
    }

    // 照らされている時間をマイナス
    lightingTime_--;
    // 幅は超えないように
    lightingTime_ = std::clamp<int>(lightingTime_, 0, 10);

    // 花が生えるアニメーション
    if (lightingTime_ <= 10) {
		float value = easeOut((float)(lightingTime_) / 60.0f, 0.1f, 2.90f, 10.0f / 60.0f);
		float value2 = easeOut((float)(lightingTime_) / 60.0f, 0.1f, 4.90f, 10.0f / 60.0f);
		trunkModel_->transform.scale = { value2,value,value2 };
    }
    else if (lightingTime_ == 0) {
        isAnimation_ = false;
    }
}

bool VineWall::IsMapChipCollision() {
	if (isGrew_ > 0) {
		return true;
	}

	return false;
}
bool VineWall::IsGroundCollision() {
	return true;
}

void VineWall::GrawUp() {
	isGrew_ = 2;
    lightingTime_ += 2;
}

void VineWall::OnActive() {
    model_->isActive = true;
	trunkModel_->isActive = true;
	for (int n = 0; n < 3; n++) {
		leavesModel_[n]->isActive = true;
	}
}
void VineWall::OffActive() {
    model_->isActive = false;
	trunkModel_->isActive = false;
	for (int n = 0; n < 3; n++) {
		leavesModel_[n]->isActive = false;
	}
}