#include "StageSelectScene.h"
#include "GameScene.h"

// 初期化
void StageSelectScene::Initialize() {
	backGround_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	backGround_->vertices[0].position = { 0.0f,0.0f,1000.0f };
	backGround_->vertices[1].position = { 1280.0f,0.0f,1000.0f };
	backGround_->vertices[2].position = { 1280.0f,720.0f,1000.0f };
	backGround_->vertices[3].position = { 0.0f,720.0f,1000.0f };
#if _DEBUG
	backGround_->commonColor = new LWP::Utility::Color(LWP::Utility::WHITE);	// 背景色をセット
#else
	backGround_->commonColor = new LWP::Utility::Color(LWP::Utility::BLACK);	// 背景色をセット
#endif
	backGround_->isUI = true;

	stageSelect_.Init();
	player_.Init(stageSelect_.GetPlayerStartPosition(), mainCamera);
}
// 更新
void StageSelectScene::Update() {
	stageSelect_.Update();
	player_.Update(&stageSelect_);

	// 再読み込み
	if (LWP::Input::Keyboard::GetTrigger(DIK_R)) {
		nextScene_ = new StageSelectScene();
	}
	// 前のシーンへ戻る
	if (LWP::Input::Keyboard::GetTrigger(DIK_ESCAPE)) {

	}
}