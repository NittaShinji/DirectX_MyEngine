#include "MirrorPlayer.h"
#include "ObjectColor.h"

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
	scale_ = player_->GetScale();
	rotation_ = player_->GetRotation();
	transform_ = player_->GetTransform();
	Object3d::Initialize();
	Object3d::SetColorFlag(true);
}

void MirrorPlayer::Update(Camera* camera)
{
	//オブジェクトの更新
	Object3d::Update(camera);
	//プレイヤーの位置を代入
	scale_ = player_->GetScale();
	rotation_ = player_->GetRotation();
	transform_ = player_->GetTransform();
	//鏡面反射用の座標に設定
	transform_.y = -transform_.y - kMirrorDistance_;

	//プレイヤーの色に応じて、色を変更
	Vector3 color = player_->GetColor();
	SetColor(color);

	//行列の更新
	Object3d::SetTransform(transform_);
}

void MirrorPlayer::Draw()
{
	if(player_->GetIsDead() == false)
	{
		Object3d::Draw();
	}
}
