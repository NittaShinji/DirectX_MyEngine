#include "MirrorPlayer.h"

Player* MirrorPlayer::player_ = nullptr;

MirrorPlayer::MirrorPlayer() {}
MirrorPlayer::~MirrorPlayer() {}

std::unique_ptr<MirrorPlayer> MirrorPlayer::Create(Player* player)
{
	//3Dオブジェクトのインスタンスを生成
	std::unique_ptr<MirrorPlayer> instance = std::make_unique<MirrorPlayer>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->player_ = player;
	instance->Initialize();
	instance->SetModel(player_->GetModel()->GetName());

	return instance;
}

void MirrorPlayer::Initialize()
{
	Object3d::Initialize();
	transform_ = player_->GetTransform();
}

void MirrorPlayer::Update(Camera* camera)
{
	Object3d::Update(camera);
}

void MirrorPlayer::Draw()
{
	if(player_->GetIsDead() == false)
	{
		Object3d::Draw();
	}
	
}
