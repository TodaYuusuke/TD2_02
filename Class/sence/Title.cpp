#include "Title.h"
#include "GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void Title::Initialize() {
	sphere = LWP::Primitive::CreateInstance<Sphere>();

	// BGMの読み込み
	bgm_[LEAFSWAY] = LWP::Resource::LoadAudio("BGM_leafWindow.wav");
	bgm_[BIRD] = LWP::Resource::LoadAudio("BGM_bird.wav");
	bgm_[PIANO] = LWP::Resource::LoadAudio("BGM_piano.wav");

	// 音の再生(全てループ再生をさせる)
	for (int i = 0; i < BGM_NUM; i++) {
		bgm_[i]->SetLoopCount(255);
		bgm_[i]->Play();
	}
	// 音量調整
	bgm_[LEAFSWAY]->SetVolume(0.07f);
	bgm_[BIRD]->SetVolume(0.04f);
	bgm_[PIANO]->SetVolume(0.05f);
}

// 更新
void Title::Update() {
	// ENTERキーを押すとシーン切り替え
	if (Keyboard::GetTrigger(DIK_RETURN)) {
		nextScene_ = new GameScene(1);
	}
}