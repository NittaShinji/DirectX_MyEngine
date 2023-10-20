#include "ObjParticleManager.h"

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

void ObjParticleManager::AllRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->AllRemove();
	}

	//エミッターの削除
	emitters_.clear();
}

void ObjParticleManager::ParticleRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->AllRemove();
		emitter->SetIsMaxParticle(false);
	}
}

void ObjParticleManager::EmitterRemove()
{
	//エミッターの削除
	emitters_.clear();
}
