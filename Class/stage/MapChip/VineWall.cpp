#include "Derved.h"

void VineWall::Init(LWP::Math::Vector3 position, float scale) {
	model_ = LWP::Resource::LoadModel("Floor/Floor.obj");
	model_->transform.translation = position;
	model_->transform.scale = { scale,scale,scale };
	model_->material.enableLighting = true;
	
#if _DEBUG
    treeModel_ = LWP::Resource::LoadModel("Wall/LowPolyWall.obj");
#else
    treeModel_ = LWP::Resource::LoadModel("Floor/Floor.obj");
#endif
	treeModel_->transform.translation = position;
	treeModel_->transform.translation.y += 1.0f;
	treeModel_->transform.scale = { scale,scale,scale };
	treeModel_->material.enableLighting = true;
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
        float value = easeOut((float)(lightingTime_) / 60.0f, 0.01f, 1.0f, 10.0f / 60.0f);
        treeModel_->transform.scale = { value,value,value };
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