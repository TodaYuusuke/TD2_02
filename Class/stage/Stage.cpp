#include "Stage.h"
#include <fstream>
#include <sstream>

using namespace LWP::Math;
using namespace LWP::Utility;

void Stage::Init(int level) {
	// レベルのステージデータを読み込む
	std::ifstream ifs("resources/stage/level" + std::to_string(level) + ".csv");
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
			switch (static_cast<Mapchip>(std::atoi(field.c_str()))) {
				case Mapchip::Floor:
				default:
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
					kCandleCount++;
					break;
				case Mapchip::OutLine:
					mapChip_[y].push_back(new OutLineWall());
					break;
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

	// 枯れ草の初期化
	for (int y = 0; y < mapChip_.size(); y++) {
		for (int x = 0; x < mapChip_[y].size(); x++) {
			if (dynamic_cast<DarkVineFloor*>(mapChip_[y][x])) {
				DarkVineLeaf* leave{ nullptr };	// 葉っぱのポインタ
				// 回りの葉っぱを検知
				for (int plus = 1; plus <= 3; plus++) {
					if (y + plus < mapChip_.size() && dynamic_cast<DarkVineLeaf*>(mapChip_[y + plus][x])) {
						leave = dynamic_cast<DarkVineLeaf*>(mapChip_[y + plus][x]);
						leave->SetBeginAnimationFrame(24 - 8 * plus);
						leave->SetRotateY(0);
					}
					else { break; }
				}
				for (int plus = 1; plus <= 3; plus++) {
					if (y - plus >= 0 && dynamic_cast<DarkVineLeaf*>(mapChip_[y - plus][x])) {
						leave = dynamic_cast<DarkVineLeaf*>(mapChip_[y - plus][x]);
						leave->SetBeginAnimationFrame(24 - 8 * plus);
						leave->SetRotateY(1.57f * 2);
					}
					else { break; }
				}
				for (int plus = 1; plus <= 3; plus++) {
					if (x + plus < mapChip_[0].size() && dynamic_cast<DarkVineLeaf*>(mapChip_[y][x + plus])) {
						leave = dynamic_cast<DarkVineLeaf*>(mapChip_[y][x + plus]);
						leave->SetBeginAnimationFrame(24 - 8 * plus);
						leave->SetRotateY(1.57f);
					}
					else { break; }
				}
				for (int plus = 1; plus <= 3; plus++) {
					if (x - plus >= 0 && dynamic_cast<DarkVineLeaf*>(mapChip_[y][x - plus])) {
						leave = dynamic_cast<DarkVineLeaf*>(mapChip_[y][x - plus]);
						leave->SetBeginAnimationFrame(24 - 8 * plus);
						leave->SetRotateY(1.57f * 3);
					}
					else { break; }
				}
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
			mapChip_[y][x]->OffActive();
		}
	}

#if _DEBUG
#else
	// 花更新
	for (int i = 0; i < flowers_.size(); i++) {
		flowers_[i].Update();
		flowers_[i].OffActive();
	}
#endif

	// ろうそくの当たり判定を取る
	for (int y = 0; y < mapChip_.size(); y++) {
		for (int x = 0; x < mapChip_[y].size(); x++) {
			if (dynamic_cast<Candle*>(mapChip_[y][x])) {
				Candle* candle = dynamic_cast<Candle*>(mapChip_[y][x]);
				if (candle->isIgnited) {
					CheckLightCollision(candle->GetModel()->transform.GetWorldPosition(), 2.5f);
				}
			}
		}
	}
}

bool Stage::CheckCollision(LWP::Math::Vector3 checkPos, LWP::Math::Vector3* fixVector, bool isPlayer, bool isGrabLantern) {

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
				// ただし葉っぱが育っているならば通常の当たり判定として扱う
				Hole* hole = dynamic_cast<Hole*>(mapChip_[y][x]);
				if (hole->isGrew_ > 0) {
					result = mapChip_[y][x]->CheckCollision(cPos, fixVector);
				}
				else {
					Wall* wall = new Wall();
					wall->SetModel(mapChip_[y][x]->GetModel());
					result = wall->CheckCollision(cPos, fixVector);
				}
			}
			else {
				result = mapChip_[y][x]->CheckCollision(cPos, fixVector);
			}

			// ヒットしていてろうそくの場合かつY軸が下過ぎない場合
			if (isGrabLantern && result && checkPos.y >= -0.1f && dynamic_cast<Candle*>(mapChip_[y][x])) {
				Candle* candle = dynamic_cast<Candle*>(mapChip_[y][x]);
				// 未点火なら点火
				if (!candle->isIgnited) {
					
					candle->isIgnited = true;
					candle->FireSound();
					ignitedCandle++;
				}
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

			// 中心に対してのray（3次元）
			Vector3 ray3 = mapChip_[y][x]->GetModel()->transform.GetWorldPosition();

			// まず灯りの範囲内かをチェックする
			if ((ray3 - center).Length() > radius) {
				// かなり離れてる場合わけじゃないならactiveにする
				if ((ray3 - center).Length() < 3.25f + 0.5f) {
					mapChip_[y][x]->OnActive();
				}
				continue;	// 範囲外なのでこの判定は終了
			}
			mapChip_[y][x]->OnActive();

			// そも光源がY-ならば判定をとらない
			if (center.y < 0.0f) { continue; }
			// マップチップが反応のあるものでない場合continue
			if (!mapChip_[y][x]->IsToGrow()) { continue; }

			// rayを二次元に
			Vector2 ray = { ray3.x,ray3.z };
			// 壁などにヒットした場合用のフラグ
			bool isHit = false;

			// もしプレイヤーの影になっているならば -> 処理を行わない
			if (IsPlayerIntersecting({ center.x,center.z }, ray)) { continue; }

			// rayと重なっている可能性のあるマップチップを洗い出す
			std::vector<IMapChip*> mapChipArray;
			int mapChipRangeY1 = static_cast<int>(center.z / commonScale);
			int mapChipRangeX1 = static_cast<int>(center.x / commonScale);
			int mapChipRangeY2 = static_cast<int>(ray3.z / commonScale);
			int mapChipRangeX2 = static_cast<int>(ray3.x / commonScale);
			for (int yy = min(mapChipRangeY1, mapChipRangeY2); yy <= max(mapChipRangeY1, mapChipRangeY2); yy++) {
				for (int xx = min(mapChipRangeX1, mapChipRangeX2); xx <= max(mapChipRangeX1, mapChipRangeX2); xx++) {
					if (!(yy == y && xx == x)) {
						mapChipArray.push_back(mapChip_[yy][xx]);
					}
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
			if (isHit) { continue; }

			// ヒットしていなかったらしいので成長の処理を呼び出す
			mapChip_[y][x]->GrawUp();
			// このマップチップがVineFloorの場合の処理
			if (dynamic_cast<VineFloor*>(mapChip_[y][x])) {
				Hole* holes[12]{ nullptr };	// 穴のポインタ
				int i = 0;
				// 回りの穴を検知
				for (int plus = 1; plus <= 3; plus++) {
					if (y + plus < mapChip_.size() && dynamic_cast<Hole*>(mapChip_[y + plus][x])) {
						holes[i++] = dynamic_cast<Hole*>(mapChip_[y + plus][x]);
						//if (holes[i - 1]->isGrew_ > 1) {
						//	holes[i - 1] = nullptr;
						//	break;
						//}
						holes[i - 1]->SetFlowerRotateY(0);
						holes[i - 1]->SetBeginAnimationFrame(6 - 2 * i);
					}
					else { break; }
				}
				for (int plus = 1; plus <= 3; plus++) {
					if (y - plus >= 0 && dynamic_cast<Hole*>(mapChip_[y - plus][x])) {
						holes[i++] = dynamic_cast<Hole*>(mapChip_[y - plus][x]);
						//if (holes[i - 1]->isGrew_ > 1) {
						//	holes[i - 1] = nullptr;
						//	break;
						//}
						holes[i - 1]->SetFlowerRotateY(1.57f * 2);
						holes[i - 1]->SetBeginAnimationFrame(6 - 2 * i);
					}
					else { break; }
				}
				for (int plus = 1; plus <= 3; plus++) {
					if (x + plus < mapChip_[0].size() && dynamic_cast<Hole*>(mapChip_[y][x + plus])) {
						holes[i++] = dynamic_cast<Hole*>(mapChip_[y][x + plus]);
						/*if (holes[i - 1]->isGrew_ > 1) {
							holes[i - 1] = nullptr;
							break;
						}*/
						holes[i - 1]->SetFlowerRotateY(1.57f * 1);
						holes[i - 1]->SetBeginAnimationFrame(6 - 2 * i);
					}
					else { break; }
				}
				for (int plus = 1; plus <= 3; plus++) {
					if (x - plus >= 0 && dynamic_cast<Hole*>(mapChip_[y][x - plus])) {
						holes[i++] = dynamic_cast<Hole*>(mapChip_[y][x - plus]);
						/*if (holes[i - 1]->isGrew_ > 1) {
							holes[i - 1] = nullptr;
							break;
						}*/
						holes[i - 1]->SetFlowerRotateY(1.57f * 3);
						holes[i - 1]->SetBeginAnimationFrame(6 - 2 * i);
					}
					else { break; }
				}


				// 穴の葉をOnに
				for (int n = 0; n < 12; n++) {
					if (holes[n] != nullptr) {
						holes[n]->GrawUp();
					}
				}
			}
			// 照らしたら死ぬ床の場合
			else if (dynamic_cast<DarkVineFloor*>(mapChip_[y][x])) {
				// 死亡するフレームに到達しているかどうかを検証
				DarkVineFloor* floor = dynamic_cast<DarkVineFloor*>(mapChip_[y][x]);
				if (floor->isGrew_ >= floor->kFrameUntilDeath) {
					DarkVineLeaf* leaves[12]{ nullptr };	// 葉っぱのポインタ
					int i = 0;
					// 回りの穴を検知
					for (int plus = 1; plus <= 3; plus++) {
						if (y + plus < mapChip_.size() && dynamic_cast<DarkVineLeaf*>(mapChip_[y + plus][x])) {
							leaves[i++] = dynamic_cast<DarkVineLeaf*>(mapChip_[y + plus][x]);
						}
						else { break; }
					}
					for (int plus = 1; plus <= 3; plus++) {
						if (y - plus >= 0 && dynamic_cast<DarkVineLeaf*>(mapChip_[y - plus][x])) {
							leaves[i++] = dynamic_cast<DarkVineLeaf*>(mapChip_[y - plus][x]);
						}
						else { break; }
					}
					for (int plus = 1; plus <= 3; plus++) {
						if (x + plus < mapChip_[0].size() && dynamic_cast<DarkVineLeaf*>(mapChip_[y][x + plus])) {
							leaves[i++] = dynamic_cast<DarkVineLeaf*>(mapChip_[y][x + plus]);
						}
						else { break; }
					}
					for (int plus = 1; plus <= 3; plus++) {
						if (x - plus >= 0 && dynamic_cast<DarkVineLeaf*>(mapChip_[y][x - plus])) {
							leaves[i++] = dynamic_cast<DarkVineLeaf*>(mapChip_[y][x - plus]);
						}
						else { break; }
					}

					// 穴の葉をOnに
					for (int n = 0; n < 12; n++) {
						if (leaves[n] != nullptr) {
							leaves[n]->isDead_ = true;
							leaves[n]->GetModel()->transform.scale = { 0.0f,0.0f,0.0f };
						}
					}

				}
			}
		}
	}

#if _DEBUG
#else
	// 花に対しても行う
	for (int i = 0; i < flowers_.size(); i++) {
		// 地点に対してのray（3次元）
		Vector3 ray3 = flowers_[i].GetWorldPosition();

		// まず灯りの範囲内かをチェックする
		if ((ray3 - center).Length() > radius) {
			// かなり離れてる場合わけじゃないならactiveにする
			if ((ray3 - center).Length() < 3.25f) {
				flowers_[i].OnActive();
			}
			continue;	// 範囲外なのでこの判定は終了
	}
		flowers_[i].OnActive();

		// rayを二次元に
		Vector2 ray = { ray3.x,ray3.z };
		// 壁などにヒットした場合用のフラグ
		bool isHit = false;


		// もしプレイヤーの影になっているならば -> 処理を行わない
		if (IsPlayerIntersecting({ center.x,center.z }, ray)) { continue; }


		// rayと重なっている可能性のあるマップチップを洗い出す
		std::vector<IMapChip*> mapChipArray;
		int mapChipRangeY1 = static_cast<int>(center.z / commonScale);
		int mapChipRangeX1 = static_cast<int>(center.x / commonScale);
		int mapChipRangeY2 = static_cast<int>(ray3.z / commonScale);
		int mapChipRangeX2 = static_cast<int>(ray3.x / commonScale);
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
		if (isHit) { continue; }

		// ヒットしていなかったらしいので成長の処理を呼び出す
		flowers_[i].AddLightingTime();
}
#endif
}

// ベクトルと線分の交差判定
bool Stage::IsIntersecting(Vector2 start, Vector2 end, Vector3 mapChipTransform) {
	Vector2 mapChipPos = { mapChipTransform.x,mapChipTransform.z };
	float v1 = CrossProduct(end - start, (mapChipPos + Vector2{ -0.5f,-0.5f }) - start);
	float v2 = CrossProduct(end - start, (mapChipPos + Vector2{ 0.5f,-0.5f }) - start);
	float v3 = CrossProduct(end - start, (mapChipPos + Vector2{ 0.5f, 0.5f }) - start);
	float v4 = CrossProduct(end - start, (mapChipPos + Vector2{ -0.5f, 0.5f }) - start);

	// 線分と矩形の辺が交差するかどうかを判定
	return ((v1 * v2 < 0) || (v3 * v4 < 0) || (v1 * v4 < 0) || (v2 * v3 < 0));
}
// プレイヤーver
bool Stage::IsPlayerIntersecting(Vector2 start, Vector2 end) {
	// ランタンからターゲットへの方向ベクトル
	Vector3 sToe = Vector3{ end.x,0.0f,end.y } - Vector3{ start.x, 0.0f, start.y };
	// ランタンからプレイヤーの方向ベクトル
	Vector3 sTop = playerCurrentPosition_ - Vector3{ start.x, 0.0f, start.y };
	// 内積が-のとき判定しない
	if (Vector3::Dot(sToe, sTop) < 0.0f) { return false; }
	// プレイヤーの方が遠いとき判定しない
	if (sToe.Length() < sTop.Length()) { return false; }

	Vector3 playerPos[4] = {
		playerCurrentPosition_ + Vector3{-0.05f,0.0f,-0.05f,} *Matrix4x4::CreateRotateXYZMatrix(playerCurrentRotation_),
		playerCurrentPosition_ + Vector3{ 0.05f,0.0f,-0.05f,} *Matrix4x4::CreateRotateXYZMatrix(playerCurrentRotation_),
		playerCurrentPosition_ + Vector3{ 0.05f,0.0f, 0.05f,} *Matrix4x4::CreateRotateXYZMatrix(playerCurrentRotation_),
		playerCurrentPosition_ + Vector3{-0.05f,0.0f, 0.05f,} *Matrix4x4::CreateRotateXYZMatrix(playerCurrentRotation_),
	};

	float v1 = CrossProduct(end - start, Vector2{ playerPos[0].x,playerPos[0].z } - start);
	float v2 = CrossProduct(end - start, Vector2{ playerPos[1].x,playerPos[1].z } - start);
	float v3 = CrossProduct(end - start, Vector2{ playerPos[2].x,playerPos[2].z } - start);
	float v4 = CrossProduct(end - start, Vector2{ playerPos[3].x,playerPos[3].z } - start);

	// 線分と矩形の辺が交差するかどうかを判定
	return ((v1 * v2 < 0) || (v3 * v4 < 0) || (v1 * v4 < 0) || (v2 * v3 < 0));
}

// Vector2の内積を求める関数
float Stage::CrossProduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.y - v2.x * v1.y;
}

int Stage::CheckNearStageSelectMapChip(LWP::Math::Vector3 playerPosition) {
	// 継承先のためだけに作られた関数
	playerStartPosition;
	return -1;
}
