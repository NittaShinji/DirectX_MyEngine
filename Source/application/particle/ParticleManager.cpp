#include "ParticleManager.h"
#include "GroundParticle.h"
#include "ImGuiManager.h"
#include "Player.h"
#include <d3dcompiler.h>
#include <string.h>
#include <cassert>
#pragma warning(push)
#pragma warning(disable:4061)
#pragma warning(disable:4820)
#include <DirectXTex.h>
#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//静的メンバ変数の実態
ID3D12GraphicsCommandList* ParticleManager::cmdList_;
ID3D12Device* ParticleManager::device_ = nullptr;

ParticleManager::ParticleManager() {}
ParticleManager::~ParticleManager() {}

void ParticleManager::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdlist)
{
	device_ = device;
	cmdList_ = cmdlist;
}

void ParticleManager::Initialize()
{
	groundParticle_ = GroundParticle::Create("cloud.png");
	hitParticle_ = HitParticle2D::Create("cloud.png");
	secondJumpParticle_ = SecondJump2DParticle::Create("jumpEffect6.png");
	AddEmitter(groundParticle_.get());
	AddEmitter(secondJumpParticle_.get());
	AddEmitter(hitParticle_.get());

	//エミッターの初期化
	for(auto& emitter : emitters_)
	{
		emitter->Initialize(device_);
	}
}

void ParticleManager::AllRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->ParticleRemove();
	}

	//エミッターの削除
	emitters_.clear();
}

void ParticleManager::ParticleRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->ParticleRemove();
	}
}

void ParticleManager::SetEmitterPlayer(Player* player_)
{
	for(auto& emitter : emitters_)
	{
		emitter->SetPlayer(player_);
	}
}

void ParticleManager::SetEmitterGameSpeed(GameSpeed* gameSpeed)
{
	for(auto& emitter : emitters_)
	{
		emitter->SetGameSpeed(gameSpeed);
	}
}

void ParticleManager::Preparation(GameSpeed* gameSpeed,Player* player)
{
	if(player->GetIsLanded() == true)
	{
		groundParticle_->SetIsPlayerColor(player->GetAttributeColor());
	}

	if(gameSpeed->GetSpeedMode() != GameSpeed::SpeedMode::STOP)
	{
		if(gameSpeed->GetSpeedMode() == GameSpeed::SpeedMode::SLOW)
		{
			if(player->GetOnGround() == false && player->GetIsMoving() == true && player->GetIsDead() == false)
			{
				if(player->GetIsSecondJumpMoment() == true)
				{
					secondJumpParticle_->Preparation(player->GetTransform());
				}
			}

			if(gameSpeed->GetCanMoveInSlow() == true)
			{
				if(player->GetOnGround() == true && player->GetIsMoving() == true && player->GetIsDead() == false)
				{
					if(player->GetIsSecondJumpMoment() == false)
					{
						groundParticle_->Preparation(player->GetTransform(), player->GetAttributeColor(), player->GetRightAxcell(), gameSpeed->GetCanMoveInSlow());
					}
				}

				if(player->GetIsMoving() == true && player->GetIsDead() == false)
				{
					bool isTouchObject = player->GetIsTouchObject();

					if(player->GetIsLanded() == true || isTouchObject == true)
					{
						hitParticle_->Preparation(player->GetTransform(), player->GetIsDead());
						if(isTouchObject == true)
						{
							player->SetIsTouchObject(false);
						}
					}
				}
			}
		}
		else
		{
			if(player->GetOnGround() == false && player->GetIsMoving() == true && player->GetIsDead() == false)
			{
				if(player->GetIsSecondJumpMoment() == true)
				{
					secondJumpParticle_->Preparation(player->GetTransform());
				}
			}

 			if(player->GetOnGround() == true && player->GetIsMoving() == true && player->GetIsDead() == false)
			{
				if(player->GetIsSecondJumpMoment() == false)
				{
					groundParticle_->Preparation(player->GetTransform(), player->GetAttributeColor(), player->GetRightAxcell(), gameSpeed->GetCanMoveInSlow());
				}
			}

			if(player->GetIsMoving() == true)
			{
				//一度だけタッチフラグをオンにする
				bool isTouchObject = player->GetIsTouchObject();

				if(player->GetIsLanded() == true || isTouchObject == true)
				{
   					hitParticle_->Preparation(player->GetTransform(), player->GetIsDead());

					if(isTouchObject == true)
					{
						player->SetIsTouchObject(false);
					}
				}
			}
		}

		if(player->GetIsDead() == true)
		{
			hitParticle_->Preparation(player->GetTransform(), player->GetIsDead());
		}
	}
}

void ParticleManager::ProcessPlayerDead(Vector3 transform,bool isDead)
{
	secondJumpParticle_->ParticleRemove();
	groundParticle_->ParticleRemove();
	hitParticle_->Preparation(transform, isDead);
}

void ParticleManager::ImGuiUpdate()
{
	groundParticle_->ImguiUpdate();
}

void ParticleManager::Update(Camera* camera)
{
	for(auto& emitter : emitters_)
	{
		emitter->Update(camera);
	}
}

void ParticleManager::Draw()
{
	for(auto& emitter : emitters_)
	{
		emitter->PreDraw(cmdList_);
		emitter->Draw();
	}
}

void ParticleManager::AddEmitter(ParticleEmitter* particleEmitter)
{
	emitters_.push_back(particleEmitter);
}

