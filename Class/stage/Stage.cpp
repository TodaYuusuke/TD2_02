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

	// 花を生成
	flower_.Init({ 3.5f,3.5f });
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

	flower_.Update();
}

bool Stage::CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector, bool isPlayer) {

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


	// どのマップチップと検証するかチェック
	//int y = static_cast<int>(checkPos.z >= -1.0f ? checkPos.z / commonScale : -1);
	int y = static_cast<int>(checkPos.z / commonScale);
	//int x = static_cast<int>(checkPos.x >= -1.0f ? checkPos.x / commonScale : -1);
	int x = static_cast<int>(checkPos.x / commonScale);

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

void Stage::CheckLightCollision(LWP::Math::Vector3 center, float radius) {
	// 光源に対してアクションがある当たり判定をチェックする
	for (int y = 0; y < mapChip_.size(); y++) {
		for (int x = 0; x < mapChip_[y].size(); x++) {

			// マップチップが反応のあるものでない場合continue
			if (!mapChip_[y][x]->IsToGrow()) {
				continue;
			}

			// 4頂点に対してのray（3次元）
			Vector3 ray3[4] = {
				(mapChip_[y][x]->GetModel()->transform.translation + Vector3{-0.5f,0.0f,-0.5f}) - center,
				(mapChip_[y][x]->GetModel()->transform.translation + Vector3{-0.5f,0.0f, 0.5f}) - center,
				(mapChip_[y][x]->GetModel()->transform.translation + Vector3{ 0.5f,0.0f,-0.5f}) - center,
				(mapChip_[y][x]->GetModel()->transform.translation + Vector3{ 0.5f,0.0f, 0.5f}) - center
			};

			// 4頂点に対してのrayをチェックする
			for (int i = 0; i < 4; i++) {
				// まず灯りの範囲内かをチェックする
				if ((center - ray3[i]).Length() > radius) {
					continue;	// 範囲外なのでこの判定は終了
				}

				// rayを二次元に
				Vector2 ray = { ray3[i].x,ray3[i].z };
				// 壁などにヒットした場合用のフラグ
				bool isHit = false;

				// rayと重なっている可能性のあるマップチップを洗い出す
				std::vector<IMapChip*> mapChipArray;
				int mapChipRangeY1 = static_cast<int>(center.z / commonScale);
				int mapChipRangeX1 = static_cast<int>(center.x / commonScale);
				int mapChipRangeY2 = static_cast<int>(ray.y / commonScale);
				int mapChipRangeX2 = static_cast<int>(ray.x / commonScale);
				for (int yy = min(mapChipRangeY1, mapChipRangeY2); yy <= max(mapChipRangeY1, mapChipRangeY2); yy++) {
					for (int xx = min(mapChipRangeX1, mapChipRangeX2); xx <= max(mapChipRangeX1, mapChipRangeX2); xx++) {
						mapChipArray.push_back(mapChip_[yy][xx]);
					}
				}

				// rayと交差しているかチェック
				for (IMapChip* m : mapChipArray) {
					// マップチップの範囲とベクトルの範囲が交差するかどうかを判定
					if (IsIntersecting({ center.x,center.z }, ray, m->GetModel()->transform.translation)) {
						// もし当たり判定があるマップチップの場合、このrayの当たり判定チェックをやめる
						if (m->IsMapChipCollision()) {
							isHit = true;
							break;
						}
					}
				}

				// ヒットしていた場合戻る
				if (isHit) {
					continue;
				}

				// ヒットしていなかったらしいので成長の処理を呼び出す
				mapChip_[y][x]->GrawUp();
				break;	// このマップチップに対する処理を終了する
			}
		}
	}

	// 花に対しても行う
	ImGui::Begin("CollTest");
	for (int i = 0; i < 1; i++) {
		// 地点に対してのray（3次元）
		Vector3 ray3 = flower_.GetWorldPosition();

		// まず灯りの範囲内かをチェックする
		ImGui::Text("Length %f", (ray3 - center).Length());
		if ((ray3 - center).Length() > radius) {
			continue;	// 範囲外なのでこの判定は終了
		}

		// rayを二次元に
		Vector2 ray = { ray3.x,ray3.z };
		// 壁などにヒットした場合用のフラグ
		bool isHit = false;

		// rayと重なっている可能性のあるマップチップを洗い出す
		std::vector<IMapChip*> mapChipArray;
		int mapChipRangeY1 = static_cast<int>(center.z / commonScale);
		int mapChipRangeX1 = static_cast<int>(center.x / commonScale);
		int mapChipRangeY2 = static_cast<int>(ray3.z / commonScale);
		int mapChipRangeX2 = static_cast<int>(ray3.x / commonScale);
		ImGui::Text("mapChipRangeY1 %d", mapChipRangeY1);
		ImGui::Text("mapChipRangeX1 %d", mapChipRangeX1);
		ImGui::Text("mapChipRangeY2 %d", mapChipRangeY2);
		ImGui::Text("mapChipRangeX2 %d", mapChipRangeX2);
		for (int yy = min(mapChipRangeY1, mapChipRangeY2); yy <= max(mapChipRangeY1, mapChipRangeY2); yy++) {
			for (int xx = min(mapChipRangeX1, mapChipRangeX2); xx <= max(mapChipRangeX1, mapChipRangeX2); xx++) {
				mapChipArray.push_back(mapChip_[yy][xx]);
			}
		}

		// rayと交差しているかチェック
		for (IMapChip* m : mapChipArray) {
			// マップチップの範囲とベクトルの範囲が交差するかどうかを判定
			if (IsIntersecting({ center.x,center.z }, ray, m->GetModel()->transform.translation)) {
				// もし当たり判定があるマップチップの場合、このrayの当たり判定チェックをやめる
				if (m->IsMapChipCollision()) {
					isHit = true;
					break;
				}
			}
		}

		// ヒットしていた場合戻る
		if (isHit) {
			continue;
		}

		// ヒットしていなかったらしいので成長の処理を呼び出す
		flower_.AddLightingTime();
		break;	// この花に対する処理を終了する
	}
	ImGui::End();
}

// ベクトルと線分の交差判定
bool Stage::IsIntersecting(Vector2 start, Vector2 end, Vector3 mapChipTransform) {
	Vector2 mapChipPos = { mapChipTransform.x,mapChipTransform.z };
	float v1 = CrossProduct(end - start, (mapChipPos + Vector2{ -0.5f,-0.5f }) - start);
	float v2 = CrossProduct(end - start, (mapChipPos + Vector2{  0.5f,-0.5f }) - start);
	float v3 = CrossProduct(end - start, (mapChipPos + Vector2{  0.5f, 0.5f }) - start);
	float v4 = CrossProduct(end - start, (mapChipPos + Vector2{ -0.5f, 0.5f }) - start);

	// 線分と矩形の辺が交差するかどうかを判定
	return ((v1 * v2 < 0) || (v3 * v4 < 0) || (v1 * v4 < 0) || (v2 * v3 < 0));
}

// Vector2の内積を求める関数
float Stage::CrossProduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.y - v2.x * v1.y;
}