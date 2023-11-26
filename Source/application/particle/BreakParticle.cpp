#include "BreakParticle.h"
#include "ImGuiManager.h"

std::unique_ptr<BreakParticle> BreakParticle::Create(std::string modelName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<BreakParticle> instance = nullptr;
	instance = std::make_unique<BreakParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->modelName_ = modelName;
	instance->Initialize();

	return instance;
}

void BreakParticle::Initialize()
{
	startScale_ = { 0.9f,0.9f,0.9f };
	endScale_ = { 0.0f,0.0f,0.0f };
	particleCount_ = 0;
	isPlayerDead_ = false;
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

void BreakParticle::Update(Camera* camera)
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
		if(isPlayerDead_ == true)
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

void BreakParticle::Reset()
{
	resetCount_ = 0;
	canReset_ = false;
	isStartPoped_ = false;
}

void BreakParticle::PopUpdate(Camera* camera, std::vector<Vector3> breakWallsPos)
{
	std::vector<Vector3>::iterator it = breakWallsPos.begin();

	//もし壊れた壁があれば
	if(it != breakWallsPos.end())
	{
		isStartPoped_ = true;
	}

	/*if(isBreakOnWall == true)
	{
		isStartPoped_ = true;
	}*/

	if(isStartPoped_ == true)
	{
		//最大数パーティクルを生み出したら出現を止める
		if(particleCount_ < kMaxParticleNum_)
		{
			//最大数に達していなければ生成
			if(particleCount_ != kMaxParticleNum_)
			{
				for(std::vector<Vector3>::iterator itr = breakWallsPos.begin(); itr != breakWallsPos.end(); itr++)
				{
					for(int32_t i = 0; i < kMaxParticleNum_; i++)
					{
						BreakParticlePop(camera, Vector3(itr->x, itr->y, itr->z));
					}
				}
			}
		}
		else
		{
			isStartPoped_ = false;
			particleCount_ = 0;
		}
	}
}

void BreakParticle::Preparation()
{
	//色を変化させる
	Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

	const float kInitLife = 60.0f;

	Add(modelName_, kInitLife, InitPos, InitVel, InitAcc, colorSpeed, InitStartScale, InitEndScale);
}

void BreakParticle::BreakParticlePop(Camera* camera, const Vector3& popPos)
{
	std::forward_list<Particle>::iterator it = particles_.begin();
	for(int32_t i = 0; i < particleCount_; i++)
	{
		it++;
	}

	if(particleCount_ == 0)
	{
		setPos_.x = popPos.x + imGuiPos_[0] + 2.4f;
		setPos_.y = popPos.y + imGuiPos_[1] + 2.4f;
		setPos_.z = popPos.z + imGuiPos_[2];
	}
	else if(particleCount_ == 1)
	{
		setPos_.x = popPos.x + imGuiPos_[0] - 2.4f;
		setPos_.y = popPos.y + imGuiPos_[1] + 2.4f;
		setPos_.z = popPos.z + imGuiPos_[2];
	}
	else if(particleCount_ == 2)
	{
		setPos_.x = popPos.x + imGuiPos_[0] + 2.4f;
		setPos_.y = popPos.y + imGuiPos_[1] - 2.4f;
		setPos_.z = popPos.z + imGuiPos_[2];
	}
	else if(particleCount_ == 3)
	{ 
		setPos_.x = popPos.x + imGuiPos_[0] - 2.4f;
		setPos_.y = popPos.y + imGuiPos_[1] - 2.4f;
		setPos_.z = popPos.z + imGuiPos_[2];
	}

	//const float md_pos = 2.0f;
	//setPos_.x = popPos.x + imGuiPos_[0];
	//const float shiftY = -0.8f;
	//const float shiftZ = -0.8f;
	//setPos_.y = popPos.y + imGuiPos_[1];
	//setPos_.z = popPos.z + imGuiPos_[2];
	//setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + popPos.z + imGuiPos_[2];

	setVel_.x = 0.0f + imGuiVel_[0];

	const float md_velY = 0.274f;
	const float md_velZ = 0.781f;
	//setVel_.y = (float)rand() / RAND_MAX * md_velY + imGuiVel_[1];
	//setVel_.z = (float)rand() / RAND_MAX * md_velZ + imGuiVel_[2];
	setVel_.y =  md_velY + imGuiVel_[1];
	setVel_.z =  md_velZ + imGuiVel_[2];

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
		it->object3d.SetColor(Vector3(1.0f, 1.0f, 1.0f));
		
		it->object3d.Update(camera);
		it->velocity = setVel_;
		it->accel = acc;
		it->colorSpeed = colorSpeed;
		it->s_scale = Vector3(1.0f, 1.0f, 1.0f);
		it->scale = it->s_scale;
		//it->e_scale = Vector3Zero();
		it->e_scale = it->scale;
		it->frame = 0;
		it->isGenerated = true;
	}

	particleCount_++;
}

void BreakParticle::ImGuiUpdate()
{
	ImGui::Begin("BreakParticle");
	ImGui::SetWindowPos(ImVec2(600, 0));
	ImGui::SetWindowSize(ImVec2(300, 300));

	ImGui::SliderFloat("imGuiPos_.x", &imGuiPos_[0], -10.0f, 10.0f);
	ImGui::SliderFloat("imGuiPos_.y", &imGuiPos_[1], -10.0f, 10.0f);
	ImGui::SliderFloat("imGuiPos_.z", &imGuiPos_[2], -10.0f, 10.0f);

	ImGui::SliderFloat("imGuiVel_.x", &imGuiVel_[0], -10.0f, 10.0f);
	ImGui::SliderFloat("imGuiVel_.y", &imGuiVel_[1], -10.0f, 10.0f);
	ImGui::SliderFloat("imGuiVel_.z", &imGuiVel_[2], -10.0f, 10.0f);

	ImGui::SliderFloat("imGuiAcc_x", &imGuiAcc_[0], -10.0f, 10.0f);
	ImGui::SliderFloat("imGuiAcc_y", &imGuiAcc_[1], -10.0f, 10.0f);
	ImGui::SliderFloat("imGuiAcc_z", &imGuiAcc_[2], -10.0f, 10.0f);

	ImGui::End();

}


