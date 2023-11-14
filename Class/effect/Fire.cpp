#include "Fire.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void Fire::Initialize(LWP::Math::Vector3 Position)
{
	// テクスチャ読み込み
	Texture = LWP::Resource::LoadTextureLongPath("resources/system/texture/Fire.png");
	
	//乱数生成器の初期化
	std::mt19937 randomEngine(seedGenerator());

	position_ = Position;
}

void Fire::Update() {

	// 新しいパーティクルを追加
	if (particles.size() < kMaxParticles)
	{
		particles.push_back(CreateParticle(randomEngine, position_));
	}

	// 上へあげる処理
	for (std::vector<Particle>::iterator it = particles.begin(); it != particles.end();) 
	{
		if (it->surface->isActive == true) 
		{
			const float kDeltaTime = 1.0f / 60.0f;
			it->surface->transform.translation.y += it->velocity * kDeltaTime;
			it->surface->transform.scale.x -= 0.01f;
			it->surface->transform.scale.y -= 0.01f;

			// スケールが0なったら消す
			if (it->surface->transform.scale.x <= 0.0f)
			{
				it->surface->isActive = false;
				ResetParticle(it, randomEngine,position_);
			}
			else 
			{
				++it;
			}
		}
	}
}


void Fire::Draw()
{
}


Particle Fire::CreateParticle(std::mt19937& randomEngine,LWP::Math::Vector3 position)
{
	Particle particle;
	particle.velocity = generateRandomValue(randomEngine, 0.1f, 2.0f);
	particle.bithPosition.x = generateRandomValue(randomEngine, position.x - 0.1f, position.x + 0.1f);
	particle.bithPosition.y = position.y;
	particle.bithPosition.z = position.z;

	particle.surface = LWP::Primitive::CreateInstance<Surface>();
	particle.surface->transform.translation = { particle.bithPosition.x,particle.bithPosition.y,particle.bithPosition.z };
	particle.surface->transform.scale = { 0.3f, 0.3f, 0.0f };
	particle.surface->texture = Texture;
	particle.surface->material.enableLighting = false;
	particle.surface->isActive = true;

	return particle;
}


// 乱数生成器を受け取る関数
float Fire::generateRandomValue(std::mt19937& generation, float min, float max)
{
	std::uniform_real_distribution<float> dis(min, max);
	return dis(generation);
}

void Fire::ResetParticle(std::vector<Particle>::iterator it,std::mt19937& randomEngine, LWP::Math::Vector3 position)
{
	float velocity = generateRandomValue(randomEngine, 0.1f, 2.0f);
	float lifeTime = generateRandomValue(randomEngine, 1.0f, 3.0f);
	float bithPosition = generateRandomValue(randomEngine, position.x - 0.1f, position.x + 0.1f);

	it->surface->transform.translation = { bithPosition,position_.y,position_.z };
	it->surface->transform.scale = { 0.3f, 0.3f, 0.0f };
	it->velocity = velocity;
	it->surface->isActive = true;
}
