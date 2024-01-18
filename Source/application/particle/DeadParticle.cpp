#include "DeadParticle.h"
#include "Player.h"

std::unique_ptr<DeadParticle> DeadParticle::Create(std::string modelName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<DeadParticle> instance = nullptr;
	instance = std::make_unique<DeadParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->modelName_ = modelName;
	instance->Initialize();

	return instance;
}

void DeadParticle::Initialize()
{
	startScale_ = { 0.9f,0.9f,0.9f };
	endScale_ = { 0.0f,0.0f,0.0f };
	particleCount_ = 0;
	canReset_ = false;
	isStartPoped_ = false;
	isMaxParticle_ = false;
	generationNum_ = 0;
	nowParticleCount_ = 0;
	maxParticleNum_ = kMaxParticleNum_;
	resetCount_ = 0;

	for(int32_t i = 0; i < kMaxParticleNum_; i++)
	{
		Preparation();
	}

	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		it->object3d.Initialize();
	}
}

void DeadParticle::Update(Camera* camera)
{
	//寿命が尽きたパーティクルを初期化
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		if(it->frame >= it->num_frame)
		{
			it->scale = InitStartScale;
			it->accel = InitAcc;
			it->velocity = InitVel;
			it->frame = 0;
			it->object3d.SetTransform(InitPos);
			it->object3d.SetScale(it->scale);
			it->object3d.Update(camera);
			it->isGenerated = false;
		}
	}

	//最大数生成しているかどうか
	std::size_t size = std::distance(particles_.begin(), particles_.end());
	if(size < kMaxParticleNum_)
	{
		isMaxParticle_ = false;
	}
	else
	{
		isMaxParticle_ = true;
	}

 	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		if(it->isGenerated == true)
		{
			//速度に加速度を加算
			if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::STOP)
			{
				it->velocity = (it->velocity + it->accel) * gameSpeed_->GetSpeedNum();
			}
			else
			{
				it->velocity = (it->velocity + it->accel);
			}
			//速度による移動
			Vector3 particlePos = it->object3d.GetTransform();
			particlePos = particlePos + it->velocity;
			it->object3d.SetTransform(particlePos);

			it->frame += freamIncreaseValue_* gameSpeed_->GetSpeedNum();
			//進行度を0～1の範囲に換算
			float f = (float)it->frame / it->num_frame;

			//スケールの線形補間
			it->scale = (it->e_scale - it->s_scale) * f;
			it->scale += it->s_scale;

			it->object3d.SetScale(it->scale);
			it->object3d.Update(camera);
		}

		//プレイヤーが死んだ際に生成を止めてリセットできるように
		if(player_->GetIsDead() == true)
		{
			if(it->isGenerated == true)
			{
				if(it->scale.x <= it->e_scale.x)
				{
					resetCount_++;
					if(resetCount_ == kMaxParticleNum_)
					{
						canReset_ = true;
					}
				}
			}
		}
	}
}

void DeadParticle::Reset()
{
	resetCount_ = 0;
	canReset_ = false;
	isStartPoped_ = false;
}

void DeadParticle::PopUpdate(Camera* camera, const Vector3& popPos,bool isPlayerDead, Attribute attributeColor)
{
	//死亡時にパーティクル生成開始
	if(isPlayerDead == true)
	{
		isStartPoped_ = true;
	}

	if(isStartPoped_ == true)
	{
		//最大数パーティクルを生み出したら出現を止める
		if(particleCount_ < kMaxParticleNum_)
		{
			//最大数に達していなければ生成
			if(particleCount_ != kMaxParticleNum_)
			{
				DeadParticlePop(camera, popPos, attributeColor);
			}
		}
		else
		{
			isStartPoped_ = false;
			particleCount_ = 0;
		}
	}
}

void DeadParticle::DeadParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor)
{
	std::forward_list<Particle>::iterator it = particles_.begin();
	for(int32_t i = 0; i < particleCount_; i++)
	{
		it++;
	}

	const float md_pos = 2.0f;
	setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + popPos.x + imGuiPos_[0];
	const float shiftY = -0.8f;
	setPos_.y = popPos.y + shiftY + imGuiPos_[1];
	setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + popPos.z + imGuiPos_[2];

	setVel_.x = 0.0f + imGuiVel_[0];

	const float md_velY = 0.274f;
	setVel_.y = (float)rand() / RAND_MAX * md_velY + imGuiVel_[1];
	setVel_.z = -0.1f;

	//重力に見立ててYのみ{-0.001f,0}でランダムに分布
	Vector3 acc{};
	const float md_acc = -0.017f;
	acc.x = 0.0f;
	acc.y = md_acc;
	acc.z = 0.0f;

	//色を変化させる
	Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

	if(it->isGenerated == false)
	{
		it->object3d.SetTransform(setPos_);
		it->object3d.SetScale(it->s_scale);
		it->object3d.SetColorFlag(true);
		if(attributeColor == Attribute::pink)
		{
			it->object3d.SetColor(Vector3(1.0f, 0.4f, 0.7f));
		}
		else if(attributeColor == Attribute::yellow)
		{
			it->object3d.SetColor(Vector3(1.0f, 0.469f, 0.0f));
		}
		else
		{
			it->object3d.SetColor(Vector3(0.0f, 0.0f, 0.0f));
		}
		it->object3d.Update(camera);
		it->velocity = setVel_;
		it->accel = acc;
		it->colorSpeed = colorSpeed;
		it->s_scale = Vector3(1.0f, 1.0f, 1.0f);
		it->scale = it->s_scale;
		it->e_scale = Vector3Zero();
		it->frame = 0;
		it->isGenerated = true;
	}

	particleCount_++;
}


