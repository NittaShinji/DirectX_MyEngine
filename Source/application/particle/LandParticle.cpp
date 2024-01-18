#include "LandParticle.h"
#include "ObjectColor.h"
#include "Player.h"

std::unique_ptr<LandParticle> LandParticle::Create(std::string modelName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<LandParticle> instance = nullptr;
	instance = std::make_unique<LandParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->modelName_ = modelName;
	instance->Initialize();

	return instance;
}

void LandParticle::Initialize()
{
	const Vector3 kInitStartScale = { 0.9f,0.9f,0.9f };
	const Vector3 kInitEndScale = { 0.0f,0.0f,0.0f };
	
	startScale_ = kInitStartScale;
	endScale_ = kInitEndScale;
	generationNum_ = kInitCount;
	nowParticleCount_ = kInitCount;
	particleCount_ = kInitCount;
	canReset_ = false;
	isStartPoped_ = false;
	isMaxParticle_ = false;
	maxParticleNum_ = kMaxParticleNum_;

	for(int32_t i = 0; i < kMaxParticleNum_; i++)
	{
		Preparation();
	}

	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		it->object3d.Initialize();
	}
}

void LandParticle::Update(Camera* camera)
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

			//ゲームスピードに応じてフレームを加算する
			it->frame += freamIncreaseValue_ * gameSpeed_->GetSpeedNum();
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
				if(it->scale.x >= it->e_scale.x)
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

void LandParticle::Reset()
{
	resetCount_ = 0;
	canReset_ = false;
	isStartPoped_ = false;
}

void LandParticle::PopUpdate(Camera* camera, const Vector3& popPos, bool isLanded,Attribute attributeColor)
{
	if(isLanded == true && player_->GetIsDead() == false)
	{
		isStartPoped_ = true;
	}

	if(isStartPoped_ == true)
	{
		//最大数パーティクルを生み出したら出現を止める
		if(particleCount_ < kMaxParticleNum_)
		{
			particleCount_++;
			
			if(particleCount_ != kMaxParticleNum_)
			{
				LandParticlePop(camera, popPos, attributeColor);
			}
		}
		else
		{
			isStartPoped_ = false;
			particleCount_ = 0;
		}
	}
}

void LandParticle::LandParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor)
{
	std::forward_list<Particle>::iterator it = particles_.begin();
	for(int32_t i = 0; i < particleCount_; i++)
	{
		it++;
	}

	//半分にするようの変数
	const float divideForHalf = 2.0f;
	
	const float md_pos = 2.0f;
	setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + popPos.x + imGuiPos_[0];
	const float shiftY = -0.8f;
	setPos_.y = popPos.y + shiftY + imGuiPos_[1];
	const float shiftZ = -0.5f;
	setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + popPos.z + shiftZ + imGuiPos_[2];

	setVel_.x = imGuiVel_[0];

	const float md_velY = 0.5f;
	const float md_velZ = 0.3f;
	setVel_.y = (float)rand() / RAND_MAX * md_velY + imGuiVel_[1];
	setVel_.z = md_velZ;

	//重力に見立ててYのみ{-0.001f,0}でランダムに分布
	Vector3 acc{};
	const float initAcc = 0.0f;
	const float mdAcc = -0.017f;

	acc.x = initAcc;
	acc.y = mdAcc;
	acc.z = initAcc;

	//色を変化させる
	const Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

	if(it->isGenerated == false)
	{
		it->object3d.SetTransform(setPos_);
		it->object3d.SetScale(it->s_scale);
		it->object3d.SetColorFlag(true);
		if(attributeColor == Attribute::pink)
		{
			it->object3d.SetColor(kTitlePinkOBJColor);
		}
		else if(attributeColor == Attribute::yellow)
		{
			it->object3d.SetColor(kYellowOBJColor);
		}
		else
		{
			it->object3d.SetColor(kBlackOBJColor);
		}

		it->object3d.Update(camera);
		it->velocity = setVel_;
		it->accel = acc;
		it->colorSpeed = colorSpeed;
		it->s_scale = startScale_;
		it->scale = it->s_scale;
		it->e_scale = endScale_;
		it->frame = static_cast<float>(kInitCount);
		it->isGenerated = true;
		it->canReset = false;
	}
}

void LandParticle::ResetParticleArea(Vector3 playerPos)
{
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		Vector3 particlePos = it->object3d.GetTransform();

		if(playerPos.z > particlePos.z + kResetDitance_)
		{
			if(it->isGenerated == true)
			{
 				it->canReset = true;
			}
		}
	}
}
