#include "Stage.h"
#include <fstream>
#include <sstream>

using namespace LWP::Math;

void Stage::Init(int level) {
	// レベルのステージデータを読み込む
	std::ifstream ifs("resources/stage/level" + std::to_string(level) + ".csv");
	std::string line;	// 1行分のデータ
	int y = 0;

	// 全行読むまでループ
	while (std::getline(ifs, line)) {
		// 1列生成
		mapChip_.push_back(std::vector<IMapChip*>());
		// ','で一列の情報を区切り、１つずつインスタンス生成
		std::istringstream stream(line);
		std::string field;
		while (std::getline(stream, field, ',')) {
			switch (static_cast<Mapchip>(std::atoi(field.c_str()))) {
				case Mapchip::Floor:
				default:
					mapChip_[y].push_back(new Floor());
					break;
				case Mapchip::Hole:
					mapChip_[y].push_back(new Hole());
					break;
				case Mapchip::Wall:
					mapChip_[y].push_back(new Wall());
					break;
				case Mapchip::VineWall:
					mapChip_[y].push_back(new VineWall());
					break;
				case Mapchip::VineFloor:
					mapChip_[y].push_back(new VineFloor());
					break;
				case Mapchip::Start:
					mapChip_[y].push_back(new Start());
					break;
				case Mapchip::Candle:
					mapChip_[y].push_back(new Candle());
					break;
			}
		}
		// 列をずらす
		y++;
	}

	// 最後にYを反転させる
	std::reverse(mapChip_.begin(), mapChip_.end());
	
	// 初期化
	for (int y = 0; y < mapChip_.size(); y++) {
		for (int x = 0; x < mapChip_[y].size(); x++) {
			Vector3 position = {
				static_cast<float>(x) + 0.5f,
				0.0f,
				static_cast<float>(y) + 0.5f
			};
			mapChip_[y][x]->Init(position, commonScale);

			// スタートのとき
			if (dynamic_cast<Start*>(mapChip_[y][x])) {
				playerStartPosition = mapChip_[y][x]->GetModel()->transform.translation;
			}
		}
	}
}

void Stage::Update() {
#if _DEBUG
	ImGui::Begin("Stage");
	ImGui::DragFloat("commonScale", &commonScale, 0.01f);
	ImGui::End();
#endif

	for (int y = 0; y < mapChip_.size(); y++) {
		for (int x = 0; x < mapChip_[y].size(); x++) {
#if _DEBUG
			Vector3 position = {
				(static_cast<float>(x) + 0.5f) * commonScale,
				0.0f,
				(static_cast<float>(y) + 0.5f) * commonScale
			};
			mapChip_[y][x]->SetPosition(position);
			mapChip_[y][x]->SetScale(commonScale);
#endif
			mapChip_[y][x]->Update();
		}
	}
}

bool Stage::CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector, bool isPlayer) {

	// どのマップチップと検証するかチェック
	//int y = static_cast<int>(checkPos.z >= -1.0f ? checkPos.z / commonScale : -1);
	int y = static_cast<int>(checkPos.z / commonScale);
	//int x = static_cast<int>(checkPos.x >= -1.0f ? checkPos.x / commonScale : -1);
	int x = static_cast<int>(checkPos.x / commonScale);


	// 場外の場合の処理
	if (checkPos.z < 0.0f) {
		fixVector->z = -checkPos.z;
		return true;
	}
	else if (checkPos.z > static_cast<float>(mapChip_.size())) {
		fixVector->z = static_cast<float>(mapChip_.size()) - checkPos.z;
		return true;
	}
	if (checkPos.x < 0.0f) {
		fixVector->x = -checkPos.x;
		return true;
	}
	else if (checkPos.x > static_cast<float>(mapChip_[0].size())) {
		fixVector->x = static_cast<float>(mapChip_[0].size()) - checkPos.x;
		return true;
	}

	// 配列の外でなければ当たり判定をチェック
	if (y >= 0 && y < mapChip_.size()) {
		if (x >= 0 && x < mapChip_[y].size()) {

			Vector3 cPos = checkPos;
			cPos.z -= (y * commonScale);
			cPos.x -= (x * commonScale);

			bool result;
			// もしプレイヤーの当たり判定で、かつYが0以上、かつ検証する地点がHoleの場合 -> 壁として扱う
			if (isPlayer && checkPos.y >= 0.0f && dynamic_cast<Hole*>(mapChip_[y][x])) {
				Wall* wall = new Wall();
				wall->SetModel(mapChip_[y][x]->GetModel());
				result = wall->CheckCollision(cPos, fixVector);
			}
			else {
				result = mapChip_[y][x]->CheckCollision(cPos, fixVector);
			}

			return result;
		}
	}

	return false;
}
