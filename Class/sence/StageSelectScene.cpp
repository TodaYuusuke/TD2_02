#include "StageSelectScene.h"
#include "GameScene.h"

// 初期化
void StageSelectScene::Initialize() {
	surface_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
}
// 更新
void StageSelectScene::Update() {
	ImGui::Begin("");
	ImGui::SliderInt("StageLevel", &stageLevel, 1, 14);
	if (ImGui::Button("StartGame")) {
		nextScene_ = new GameScene(stageLevel);
	}
	ImGui::End();
}