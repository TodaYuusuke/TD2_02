#include "Fire.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;

void Fire::Initialize(LWP::Math::Vector3 Position)
{
	// テクスチャ読み込み
	Texture = LWP::Resource::LoadTexture("Fire.png");

	//乱数生成器の初期化
	std::mt19937 randomEngine(seedGenerator());

	light_ = LWP::Object::CreateInstance<LWP::Object::PointLight>();
	light_->transform.translation = { -0.1f,4.0f,-0.2f };
	light_->color = LWP::Utility::Color::KelvinToRGB(1700);
	light_->radius = 1.9f;
	light_->decay = 1.5f;
	light_->isActive = true;
	light_->name = "Fire";
	
	position_ = Position;
	position_.y += 0.01;
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
		if (it->fire->isActive == true)
		{
			const float kDeltaTime = 0.1f / 60.0f;
			it->fire->transform.translation.y += it->velocity * kDeltaTime;
			it->fire->transform.scale.x -= 0.0005f;
			it->fire->transform.scale.y -= 0.0005f;
			it->fire->transform.scale.z -= 0.0005f;


			// スケールが0なったら消す
			if (it->fire->transform.scale.x <= 0.0f)
			{
				it->fire->isActive = false;
				ResetParticle(it, randomEngine, position_);
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


Particle Fire::CreateParticle(std::mt19937& randomEngine, LWP::Math::Vector3 position)
{
	Particle particle;
	particle.velocity = generateRandomValue(randomEngine, 0.1f, 2.0f);
	particle.bithPosition.x = generateRandomValue(randomEngine, position.x - 0.04f, position.x + 0.04f);
	particle.bithPosition.y = position.y;
	particle.bithPosition.z = position.z;

	
	particle.fire = LWP::Primitive::CreateInstance<Mesh>();
	particle.fire = LWP::Resource::LoadModel("Bonfire/fire.obj");
	particle.fire->transform.translation = { particle.bithPosition.x,particle.bithPosition.y,particle.bithPosition.z };
	particle.fire->transform.scale = { 0.03f, 0.03f, 0.02f };
	particle.fire->texture = Texture;
	particle.fire->material.enableLighting = false;
	particle.fire->isActive = true;

	return particle;
}


// 乱数生成器を受け取る関数
float Fire::generateRandomValue(std::mt19937& generation, float min, float max)
{
	std::uniform_real_distribution<float> dis(min, max);
	return dis(generation);
}

void Fire::ResetParticle(std::vector<Particle>::iterator it, std::mt19937& randomEngine, LWP::Math::Vector3 position)
{
	float velocity = generateRandomValue(randomEngine, 0.1f, 2.0f);
	float lifeTime = generateRandomValue(randomEngine, 1.0f, 3.0f);
	float bithPosition = generateRandomValue(randomEngine, position.x - 0.04f, position.x + 0.04f);

	it->fire->transform.translation = { bithPosition,position_.y,position_.z };
	it->fire->transform.scale = { 0.03f, 0.03f, 0.02f };
	it->velocity = velocity;
	it->fire->isActive = true;
}
