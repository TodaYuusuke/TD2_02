#include "GameScene.h"
#include"Title.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;


// 初期化
void GameScene::Initialize() {

	fire = new Fire;
	fire2 = new Fire;
	collision = new CollisionDetector;

	LWP::Math::Vector3 position = { 1.0f,0.0f,5.0f };
	LWP::Math::Vector3 position2 = { -0.1f,0.0f,0.0f };

	fire->Initialize(position);
	fire2->Initialize(position2);


	// 球
	sphere = LWP::Primitive::CreateInstance<Sphere>();
	sphere->Radius(0.2f);
	sphere->material.enableLighting = true;


	// モデル読み込み
	cubeModel = LWP::Resource::LoadModel("cube/cube.obj");
	cubeModel->material.enableLighting = true;
	cubeModel->isActive = true;

	isHit = false;
}

// 更新
void GameScene::Update() {

	fire->Update();
	fire2->Update();


	LWP::Math::Vector2 circleCenter = { sphere->transform.translation.x,sphere->transform.translation.y };
	LWP::Math::Vector2 rectCenter = { cubeModel->transform.translation.x,cubeModel->transform.translation.y };
	LWP::Math::Vector2 rectSide = { cubeModel->transform.scale.x,cubeModel->transform.scale.y };

	if (collision->isCollision(circleCenter,sphere->Radius(), rectCenter, rectSide))
	{
		isHit = true;
	}
	else
	{
		isHit = false;
	}

#ifdef _DEBUG
	ImGui::Begin("collision");
	if (isHit)
	{
		ImGui::Text("Hit!!");
	}
	ImGui::End();
#endif // _DEBUG

	


#if _DEBUG
	// カメラ操作
	ImGui::Begin("Camera");
	mainCamera->DebugGUI();
	ImGui::End();
#endif

	// ENTERキーを押すとシーン切り替え
	if (Keyboard::GetTrigger(DIK_RETURN)) {
		nextScene_ = new TestTitle;
	}


}

// 描画
void GameScene::Draw() {}