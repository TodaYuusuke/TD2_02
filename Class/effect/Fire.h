#pragma once
#include <Adapter.h>
#include<random>

struct Particle
{
	float velocity;
	LWP::Math::Vector3 bithPosition;
	LWP::Primitive::Surface* surface;
};

/// <summary>
/// 火が灯っている演出
/// </summary>
class Fire
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="Position">生成位置</param>
	void Initialize(LWP::Math::Vector3 Position);
	void Update();
	void Draw();


private:

	void ResetParticle(std::vector<Particle>::iterator it, std::mt19937& randomEngine, LWP::Math::Vector3 position);

	Particle CreateParticle(std::mt19937& randomEngine, LWP::Math::Vector3 position);

	// 乱数生成器を受け取る関数
	float generateRandomValue(std::mt19937& generation, float min, float max);


private:

	std::random_device seedGenerator;
	std::mt19937 randomEngine;
	std::uniform_real_distribution<float> distribution;
	std::uniform_real_distribution<float> distTime;

	static const int kMaxParticles = 40;
	int kNumParticles = 0;	//描画されている個数

	const float heightLimit = 1.2f;

	LWP::Resource::Texture* Texture;
	std::vector<Particle> particles;

	float LifeTime[kMaxParticles];	//生きていられる時間

	LWP::Math::Vector3 position_;

};