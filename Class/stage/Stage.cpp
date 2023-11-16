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
					mapChip_[y].push_back(new Floor());
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

bool Stage::CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector) {
	// どのマップチップと検証するかチェック
	int y = checkPos.z >= 0.0f ? checkPos.z / commonScale : -1;
	int x = checkPos.x >= 0.0f ? checkPos.x / commonScale : -1;

	// 場外でなければ当たり判定をチェック
	if (y > 0 && y < mapChip_.size()) {
		if (x > 0 && x < mapChip_[y].size()) {

			Vector3 cPos = checkPos;
			cPos.z -= (y * commonScale);
			cPos.x -= (x * commonScale);

			bool b = mapChip_[y][x]->CheckCollision(cPos, fixVector);
			
			return b;
		}
	}

	return false;
}
