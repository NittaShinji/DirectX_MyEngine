#include "ObjParticleManager.h"
#include "Player.h"
#include "Stage.h"

ObjParticleManager::ObjParticleManager(){}
ObjParticleManager::~ObjParticleManager(){}

//静的メンバ変数の実態
ID3D12GraphicsCommandList* ObjParticleManager::cmdList_;
ID3D12Device* ObjParticleManager::device_ = nullptr;

void ObjParticleManager::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdlist)
{
	device_ = device;
	cmdList_ = cmdlist;
}

void ObjParticleManager::Initialize()
{
	landParticle_ = LandParticle::Create("Cube");
	deadParticle_ = DeadParticle::Create("Cube");
	breakParticle_ = BreakParticle::Create("Cube");
	ObjParticleManager::GetInstance()->AddEmitter(landParticle_.get());
	ObjParticleManager::GetInstance()->AddEmitter(deadParticle_.get());
	ObjParticleManager::GetInstance()->AddEmitter(breakParticle_.get());

	//エミッターの初期化
	for(auto& emitter : emitters_)
	{
 		emitter->Initialize();
	}
}

void ObjParticleManager::Update(Camera* camera)
{
	for(auto& emitter : emitters_)
	{
		emitter->Update(camera);
	}

	if(deadParticle_->GetCanReset() == true)
	{
		canReseted_ = true;
	}
}

void ObjParticleManager::Draw()
{
	for(auto& emitter : emitters_)
	{
		emitter->Draw();
	}
}

void ObjParticleManager::AddEmitter(ObjParticleEmitter* particleEmitter)
{
	emitters_.push_back(particleEmitter);
}

void ObjParticleManager::ParticleReset(Camera* camera)
{
	for(auto& emitter : emitters_)
	{
		emitter->ParticleReset(camera);
	}
}

void ObjParticleManager::AllRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->ParticleRemove();
	}

	//エミッターの削除
	emitters_.clear();
}

void ObjParticleManager::SetEmitterPlayer(Player* player_)
{
	for(auto& emitter : emitters_)
	{
		emitter->SetPlayer(player_);
	}
}

void ObjParticleManager::SetEmitterGameSpeed(GameSpeed* gameSpeed)
{
	for(auto& emitter : emitters_)
	{
		emitter->SetGameSpeed(gameSpeed);
	}
}

void ObjParticleManager::ProcessPlayerDead(Camera* gameCamera)
{
	ParticleReset(gameCamera);
	deadParticle_->SetCanReset(false);
	deadParticle_->Reset();
}

void ObjParticleManager::PopUpdate(GameSpeed* gameSpeed, Camera* camera, Player* player, Stage* stage)
{
	if(gameSpeed->GetSpeedMode() != GameSpeed::SpeedMode::STOP)
	{
		if(gameSpeed->GetSpeedMode() == GameSpeed::SpeedMode::SLOW)
		{
			if(gameSpeed->GetCanMoveInSlow() == true)
			{
				landParticle_->PopUpdate(camera, player->GetTransform(), player->GetIsLanded(), player->GetAttributeColor());
				deadParticle_->PopUpdate(camera, player->GetTransform(), player->GetIsDead(), player->GetAttributeColor());
				breakParticle_->PopUpdate(camera, stage->GetBreakWallsPos());
			}
		}
		else
		{
			landParticle_->PopUpdate(camera, player->GetTransform(), player->GetIsLanded(), player->GetAttributeColor());
			deadParticle_->PopUpdate(camera, player->GetTransform(), player->GetIsDead(), player->GetAttributeColor());
			breakParticle_->PopUpdate(camera, stage->GetBreakWallsPos());
		}
	}
}

void ObjParticleManager::ParticleRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->ParticleRemove();
	}
}
