#include "StageSelectScene.h"
#include "GameScene.h"

using namespace LWP::Utility;

StageSelectScene::StageSelectScene(Color startTransitionColor) {
	transitionColor_ = startTransitionColor;
}

void StageSelectScene::Initialize() {
	backGround_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	backGround_->vertices[0].position = { 0.0f,0.0f,1000.0f };
	backGround_->vertices[1].position = { 1280.0f,0.0f,1000.0f };
	backGround_->vertices[2].position = { 1280.0f,720.0f,1000.0f };
	backGround_->vertices[3].position = { 0.0f,720.0f,1000.0f };
#if _DEBUG
	backGround_->commonColor = new LWP::Utility::Color(WHITE);	// 背景色をセット
#else
	backGround_->commonColor = new LWP::Utility::Color(BLACK);	// 背景色をセット
#endif
	backGround_->isUI = true;

	stageSelect_.Init(&player_);
	player_.Init(stageSelect_.GetPlayerStartPosition(), mainCamera);

	transition_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	transition_->vertices[0].position = { 0.0f,0.0f,0.0f };
	transition_->vertices[1].position = { 1280.0f,0.0f,0.0f };
	transition_->vertices[2].position = { 1280.0f,720.0f,0.0f };
	transition_->vertices[3].position = { 0.0f,720.0f,0.0f };
	transition_->commonColor = new Color(transitionColor_);	// 背景色をセット
	transition_->isUI = true;
}
// 更新
void StageSelectScene::Update() {

	// トランジション更新
	if (isStart_) {
		stageSelect_.Update();
		player_.Update(&stageSelect_);
		transitionFrame_--;
		transition_->commonColor->A = static_cast<char>(static_cast<int>(easeOut((float)(transitionFrame_) / 60.0f, 0.0f, 255.0f, 30.0f / 60.0f)));

		if (transitionFrame_ <= 0) {
			isStart_ = false;
		}
	}
	else if (isEnd_) {
		transitionFrame_++;
		transition_->commonColor->A = static_cast<char>(static_cast<int>(easeOut((float)(transitionFrame_) / 60.0f, 0.0f, 255.0f, 30.0f / 60.0f)));

		if (transitionFrame_ >= 30) {
			nextScene_ = next_;
		}
	}
	else {
		stageSelect_.Update();
		player_.Update(&stageSelect_);

		// 再読み込み
		if (LWP::Input::Keyboard::GetTrigger(DIK_R)) {
			next_ = new StageSelectScene(BLACK);
			transition_->commonColor = new Color(BLACK);	// 背景色をセット
			isEnd_ = true;
		}
		// もしステージが選択されたならステージを進める
		int level = player_.GetSelectedStageLevel();
		if (level != -1) {
			next_ = new GameScene(level);
			transition_->commonColor = new Color(BLACK);	// 背景色をセット
			isEnd_ = true;
		}
	}
}