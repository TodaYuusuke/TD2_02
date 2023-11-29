#include "StageSelect.h"
#include <fstream>
#include <sstream>

using namespace LWP::Math;
using namespace LWP::Utility;

void StageSelect::Init() {
	// レベルのステージデータを読み込む
	std::ifstream ifs("resources/stage/StageSelect.csv");
	std::string line;	// 1行分のデータ
	int y = 0;

	int flowerCount = 0;

	// 全行読むまでループ
	while (std::getline(ifs, line)) {
		// 1列生成
		mapChip_.push_back(std::vector<IMapChip*>());
		// ','で一列の情報を区切り、１つずつインスタンス生成
		std::istringstream stream(line);
		std::string field;
		while (std::getline(stream, field, ',')) {
			Mapchip chipNum = static_cast<Mapchip>(std::atoi(field.c_str()));
			switch (chipNum) {
				case Mapchip::Floor:
					mapChip_[y].push_back(new Floor());
					flowerCount += 3;
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
				case Mapchip::DarkVineFloor:
					mapChip_[y].push_back(new DarkVineFloor());
					break;
				case Mapchip::DarkVineLeaf:
					mapChip_[y].push_back(new DarkVineLeaf());
					break;
				case Mapchip::Start:
					mapChip_[y].push_back(new Start());
					break;
				case Mapchip::Candle:
					mapChip_[y].push_back(new Candle());
					break;
				default:
					StageMap* stageMap = new StageMap();
					stageMap->stageNum = static_cast<int>(chipNum) - 100;
					mapChip_[y].push_back(new StageMap());
			}
		}
		// 列をずらす
		y++;
	}

	// 最後にYを反転させる
	std::reverse(mapChip_.begin(), mapChip_.end());
	// 外周2マスに壁を追加
	int sizeX = mapChip_[0].size();
	for (int i = 0; i < 4; i++) {
		mapChip_.insert(mapChip_.begin(), std::vector<IMapChip*>());	// 一番手前にデータを追加
		mapChip_.push_back(std::vector<IMapChip*>());	// 一番奥にデータを追加
		// Xの長さ分ループしてWallをセットする
		for (int x = 0; x < sizeX; x++) {
			mapChip_[0].push_back(new OutLineWall());
			mapChip_.back().push_back(new OutLineWall());
		}
	}

	// 初期化
	for (int y = 0; y < mapChip_.size(); y++) {
		for (int i = 0; i < 4; i++) {
			mapChip_[y].insert(mapChip_[y].begin(), new OutLineWall());
			mapChip_[y].push_back(new OutLineWall());
		}
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

#if _DEBUG
#else
	// 花を生成
	Vector2 pos = { 0.0f,0.0f };
	//flowerCount = flowerCount / 3 * 2;
	for (int i = 0; i < flowerCount; i++) {
		flowers_.push_back(Flower());

		// 座標を決定
		while (true) {
			pos.x = static_cast<float>(GenerateRandamNum<int>(0, mapChip_[0].size() * 100 - 1)) / 100.0f;
			pos.y = static_cast<float>(GenerateRandamNum<int>(0, mapChip_.size() * 100 - 1)) / 100.0f;

			// 指定した地点が通常の床じゃなければ再生成
			int x = static_cast<int>(pos.x / commonScale);
			int y = static_cast<int>(pos.y / commonScale);
			if (dynamic_cast<Floor*>(mapChip_[y][x])) {
				break;
			}
		}

		flowers_.back().Init(pos);
		flowers_.back().OffActive();
	}
#endif
}

int StageSelect::CheckNearStageSelectMapChip(LWP::Math::Vector3 playerPosition) {
	// 全マップチップ網羅
	for (int y = 0; y < mapChip_.size(); y++) {
		for (int x = 0; x < mapChip_[y].size(); x++) {
			// ｽﾃｰｼﾞ選択マップチップじゃなきゃ戻す
			if (!dynamic_cast<StageMap*>(mapChip_[y][x])) {
				continue;
			}
			StageMap* stageMap = dynamic_cast<StageMap*>(mapChip_[y][x]);

			// 中心に対してのray（3次元）
			Vector3 ray3 = mapChip_[y][x]->GetModel()->transform.GetWorldPosition();

			// 範囲内かをチェックする
			if ((ray3 - playerPosition).Length() > 1.5f) {
				continue;	// 範囲外なのでこの判定は終了
			}

			// 近くにいる際の呼び出し
			stageMap->NearPlayer(playerPosition);
			return stageMap->stageNum;
		}
	}

	return -1;
}