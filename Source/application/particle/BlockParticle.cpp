#include "BlockParticle.h"
#include "ObjectColor.h"

std::unique_ptr<BlockParticle> BlockParticle::Create(std::string modelName,int32_t maxParticleNum)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<BlockParticle> instance = nullptr;
	instance = std::make_unique<BlockParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->modelName_ = modelName;
	instance->maxParticleNum_ = maxParticleNum;
	instance->Initialize();

	return instance;
}

void BlockParticle::Initialize()
{
	startScale_ = kInitStartScale_;
	endScale_ = kInitEndScale_;
	particleCount_ = kInitCount_;
	isPlayerDead_ = false;
	canReset_ = false;
	
	//事前に最大値まで生成
	for(int32_t i = 0; i < maxParticleNum_; i++)
	{
		Preparation();
	}
	//初期化
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		it->object3d.Initialize();
	}
}

void BlockParticle::Update(Camera* camera)
{
	//寿命が尽きたパーティクルを初期化
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		if(it->frame >= it->num_frame)
		{
			it->scale = InitStartScale;
			it->accel = InitAcc;
 			it->velocity = InitVel;
			it->frame = static_cast<float>(kInitCount_);
			it->object3d.SetTransform(InitPos);
			it->object3d.SetScale(it->scale);
			it->object3d.Update(camera);
			it->isGenerated = false;
		}
	}

	//最大数生成しているかどうか
	std::size_t size = std::distance(particles_.begin(), particles_.end());
	if(size < maxParticleNum_)
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
			it->velocity = it->velocity + it->accel;
			//速度による移動
			Vector3 particlePos = it->object3d.GetTransform();
			particlePos = particlePos + it->velocity;
			it->object3d.SetTransform(particlePos);

			it->frame++;
			//進行度を0～1の範囲に換算
			float f = (float)it->frame / it->num_frame;

			//スケールの線形補間
			it->scale = (it->e_scale - it->s_scale) * f;
			it->scale += it->s_scale;

			
			it->object3d.SetScale(it->scale);
			it->object3d.Update(camera);
		}

		//プレイヤーが死んだ際に生成を止めてリセットできるように
		if(isPlayerDead_ == true)
		{
			if(it->isGenerated == true)
			{
				if(it->scale.x <= it->e_scale.x)
				{
					resetCount_++;
					if(resetCount_ == maxParticleNum_)
					{
						canReset_ = true;
					}
				}
			}
		}
	}
}

void BlockParticle::Reset()
{
	resetCount_ = 0;
	canReset_ = false;
}

void BlockParticle::PopUpdate(Camera* camera, const Vector3& popPos, bool isLanded,bool isPlayerDead, Attribute attributeColor)
{
	if(isPlayerDead) {}
	if(isLanded) {}
	attributeColor_ = attributeColor;

	if(isStartPoped_ == true)
	{
		//最大数パーティクルを生み出したら出現を止める
		if(particleCount_ < maxParticleNum_)
		{
			//最大数に達していなければ生成
			if(particleCount_ != maxParticleNum_)
			{
				ParticlePop(camera, popPos, attributeColor_);
			}
		}
		else
		{
			isStartPoped_ = false;
			particleCount_ = 0;
		}
	}
}

void BlockParticle::ParticlePop(Camera* camera, const Vector3& popPos,Attribute attributeColor)
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

	setVel_.x = imGuiVel_[0];

	const float md_velY = 0.174f;
	const float md_velZ = 0.8f;

	setVel_.y = (float)rand() / RAND_MAX * md_velY + imGuiVel_[1];
	setVel_.z = md_velZ;

	//重力に見立ててYのみ{-0.001f,0}でランダムに分布
	Vector3 acc{};
	const float md_acc = -0.017f;
	acc.x = 0.0f;
	acc.y = md_acc;
	acc.z = 0.0f;

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
		it->s_scale = kInitStartScale_;
		it->scale = it->s_scale;
		it->e_scale = Vector3Zero();
		it->frame = 0;
		it->isGenerated = true;
	}
}
