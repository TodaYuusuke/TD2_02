#include "Derved.h"
#include "../../player/Player.h"
#include <string>

using namespace LWP::Math;
using namespace LWP::Utility;

void StageMap::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;

	// 看板読み込み
	signModel_ = LWP::Resource::LoadModel("StageSelect/Sign.obj");
	signModel_->transform.translation = position;
	//signModel_->transform.translation.y -= 0.f;
	//signModel_->transform.rotation.z = 0.56f;
	signModel_->texture = LWP::Resource::LoadTextureLongPath("resources/obj/StageSelect/Sign" + std::to_string(stageNum) + ".png");
	signModel_->material.enableLighting = true;

	float sign[2] = { 1, -1 };

	for (int i = 0; i < kMaxTree; i++) {
		// 幹のモデル読み込み
		trunkModel_[i] = LWP::Resource::LoadModel("Tree/Trunk.obj");
		trunkModel_[i]->transform.Parent(&model_->transform);
		trunkModel_[i]->transform.translation.x = static_cast<float>(GenerateRandamNum<int>(5, 20)) / 100.0f * sign[GenerateRandamNum<int>(0, 1)];
		trunkModel_[i]->transform.translation.z = static_cast<float>(GenerateRandamNum<int>(5, 20)) / 100.0f * sign[GenerateRandamNum<int>(0, 1)];
		float s = static_cast<float>(GenerateRandamNum<int>(15, 25)) / 100.0f;
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
}

void StageMap::Update() {
	// 特になし
	Vector3 dir = signModel_->transform.GetWorldPosition();
	dir.y -= 0.1f;
	dir -= player->GetWorldPosition();

	// 向きを求める
	Vector3 goalRotation = { 0.0f, 0.0f, 0.0f };
	// Y軸周りの角度
	goalRotation.y = std::atan2f(dir.x, dir.z);
	// X軸周りの角度
	goalRotation.x = std::atan2f(-dir.y, Vector3{ dir.x, 0.0f, dir.z }.Length());

	// 現在の角度と目標の角度を比較し、逆回転の場合に調整
	if (std::abs(signModel_->transform.rotation.y - goalRotation.y) > M_PI) {
		if (signModel_->transform.rotation.y > goalRotation.y) {
			signModel_->transform.rotation.y -= static_cast<float>(2.0f * M_PI);
		}
		else {
			signModel_->transform.rotation.y += static_cast<float>(2.0f * M_PI);
		}
	}
	// 角度を適応
	signModel_->transform.rotation = Slerp(signModel_->transform.rotation, goalRotation, 0.3f);
	signModel_->transform.rotation.x += 0.01f;	// ほんのちょびっと追加で傾ける


	// 照らされている時間をマイナス
	lightingTime_--;
	// 幅は超えないように
	lightingTime_ = std::clamp<int>(lightingTime_, 0, 10);

	// 花が生えるアニメーション
	float value = easeOut((float)(lightingTime_) / 60.0f, 0.3f, 0.1f, 10.0f / 60.0f);
	signModel_->transform.scale = { value,value,value };
}

bool StageMap::IsMapChipCollision() {
	return false;
}
bool StageMap::IsGroundCollision() {
	return true;
}

void StageMap::OnActive() {
    model_->isActive = true;
	for (int i = 0; i < kMaxTree; i++) {
		trunkModel_[i]->isActive = true;
		for (int n = 0; n < 3; n++) {
			leavesModel_[i][n]->isActive = true;
		}
	}
}
void StageMap::OffActive() {
    model_->isActive = false;
	for (int i = 0; i < kMaxTree; i++) {
		trunkModel_[i]->isActive = false;
		for (int n = 0; n < 3; n++) {
			leavesModel_[i][n]->isActive = false;
		}
	}
}

void StageMap::NearPlayer() {
	// アニメーション用のフレームでもカウントアップさせる予定
	lightingTime_ += 2;
}