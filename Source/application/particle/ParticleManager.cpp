#include "ParticleManager.h"
#include "ImGuiManager.h"
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

