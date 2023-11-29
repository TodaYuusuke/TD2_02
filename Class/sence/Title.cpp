#include "Title.h"
#include "StageSelectScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

// 初期化
void Title::Initialize() {
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

	//背景

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


	startPosition_ = { 0.44f,0.0f,-1.6f };

	model_ = LWP::Resource::LoadModel("Player/LowPolyPlayer.obj");
	headModel_ = LWP::Resource::LoadModel("Player/player_head.obj");
	bodyModel_ = LWP::Resource::LoadModel("Player/player_body.obj");
	footModel_ = LWP::Resource::LoadModel("Player/player_foot.obj");
	rightUpperArmModel_ = LWP::Resource::LoadModel("Player/player_right_upperArm.obj");
	leftUpperArmModel_ = LWP::Resource::LoadModel("Player/player_left_upperArm.obj");
	rightForeArmModel_ = LWP::Resource::LoadModel("Player/player_right_foreArm.obj");
	leftForeArmModel_ = LWP::Resource::LoadModel("Player/player_left_foreArm.obj");

	bonfire_ = LWP::Resource::LoadModel("Bonfire/bonfire.obj");

	headModel_->material.enableLighting = true;
	bodyModel_->material.enableLighting = true;
	footModel_->material.enableLighting = true;
	rightUpperArmModel_->material.enableLighting = true;
	leftUpperArmModel_->material.enableLighting = true;
	rightForeArmModel_->material.enableLighting = true;
	leftForeArmModel_->material.enableLighting = true;

	bonfire_->material.enableLighting = true;

	model_->transform.scale = { 0.5f,0.5f,0.5f };
	model_->material.enableLighting = true;
	model_->commonColor = new LWP::Utility::Color(LWP::Utility::ColorPattern::BLUE);
	model_->isActive = false;

	bodyModel_->transform.Parent(&model_->transform);
	headModel_->transform.Parent(&bodyModel_->transform);
	footModel_->transform.Parent(&bodyModel_->transform);
	leftUpperArmModel_->transform.Parent(&bodyModel_->transform);
	rightUpperArmModel_->transform.Parent(&bodyModel_->transform);

	leftForeArmModel_->transform.Parent(&leftUpperArmModel_->transform);
	rightForeArmModel_->transform.Parent(&rightUpperArmModel_->transform);

	model_->transform.translation = { startPosition_ };
	model_->transform.rotation.y = 4.0f;

	//右腕
	rightUpperArmModel_->transform.translation.x = 0.1f;
	rightUpperArmModel_->transform.translation.y = 0.05f;
	rightUpperArmModel_->transform.rotation.z = 0.5f;

	//左腕
	leftUpperArmModel_->transform.translation.x = -0.1f;
	leftUpperArmModel_->transform.translation.y = 0.05f;
	leftUpperArmModel_->transform.rotation.z = -0.5f;


	cameraOffset_ = { 0.02f, 0.0f, -0.0f };
	mainCamera->transform.translation = { 0.2f,0.1f,-2.6f };
	mainCamera->transform.rotation = { cameraOffset_ };
	mainCamera->transform.scale = { 2.0f,2.0f,2.0f };

	floor.Init({ 0.0f,0.0f,0.0f }, 10.0f);
	wall[0].Init({ -0.5f,0.0f,-0.1f }, 0.6f);
	wall[1].Init({ 0.3f,0.0f,0.2f }, 0.6f);
	wall[2].Init({ 1.0f,0.0f,-0.1f }, 0.6f);
	wall[3].Init({ -0.2f,0.0f,-1.0f }, 0.6f);
	wall[4].Init({ -0.3f,0.0f,-2.0f }, 0.6f);


	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->transform.translation = { -0.1f,4.0f,-0.2f };
	light_->color = LWP::Utility::Color::KelvinToRGB(1700);
	light_->radius = 1.9f;
	light_->decay = 1.5f;
	light_->isActive = true;
	light_->name = "Fire";

	bonfire_->transform.scale = { 0.08f,0.08f,0.08f };
	bonfire_->transform.translation = { 0.27f,0.0f,-1.8f };
	bonfire_->commonColor = new Color(0x110909FF);
	light_->transform.Parent(&bonfire_->transform);

	fire.Initialize(bonfire_->transform.translation);

	logo_omote_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	logo_ura = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();


	logo_omote_->texture = LWP::Resource::LoadTexture("honoka_logo_hontai.png");
	logo_omote_->vertices[0].position = { 0.0f,0.0f,0.0f };
	logo_omote_->vertices[1].position = { 510.0f,0.0f,0.0f };
	logo_omote_->vertices[2].position = { 510.0f,210.0f,0.0f };
	logo_omote_->vertices[3].position = { 0.0f,210.0f,0.0f };


	logo_ura->texture = LWP::Resource::LoadTexture("honoka_logo_ura.png");
	logo_ura->vertices[0].position = { 0.0f,0.0f,0.0f };
	logo_ura->vertices[1].position = { 510.0f,0.0f,0.0f };
	logo_ura->vertices[2].position = { 510.0f,210.0f,0.0f };
	logo_ura->vertices[3].position = { 0.0f,210.0f,0.0f };



	logo_omote_->transform.translation = { 50, 40, 0 };
	logo_ura->transform.translation = { 50, 52, 0 };

	logo_omote_->isUI = true;
	logo_ura->isUI = true;


	transition_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	transition_->vertices[0].position = { 0.0f,0.0f,0.0f };
	transition_->vertices[1].position = { 1280.0f,0.0f,0.0f };
	transition_->vertices[2].position = { 1280.0f,720.0f,0.0f };
	transition_->vertices[3].position = { 0.0f,720.0f,0.0f };
	transition_->commonColor = new Color(BLACK);	// 背景色をセット
	transition_->isUI = true;
}

// 更新
void Title::Update() {
	// トランジション更新
	if (isStart_) {
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

	// ENTERキーを押すとシーン切り替え
	if (Keyboard::GetTrigger(DIK_RETURN)) {
		next_ = new StageSelectScene(BLACK);
		isEnd_ = true;
	}

	fire.Update();
}