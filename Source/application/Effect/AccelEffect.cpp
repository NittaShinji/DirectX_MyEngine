#include "AccelEffect.h"

AccelEffect::AccelEffect()
{
}

AccelEffect::~AccelEffect()
{
}

void AccelEffect::Initialize(Player* player, const std::string& fileName, const Billboard::BillboardType& billBoardtype)
{
	player_ = player;
	color_ = kDefaultColor_;
	scale_ = kMaxScale_;
	axcellBillboard_ = Billboard::Create(fileName, billBoardtype);
	axcellBillboard_->SetPos(player_->GetTransform());
	axcellBillboard_->SetScale(scale_);
}

void AccelEffect::Update(Camera* camera)
{
	Vector3 pos = player_->GetTransform();
	axcellBillboard_->SetPos(pos);

	if (player_->GetIsAccelColor())
	{
		color_.w += 0.05f;
		distance = player_->GetDistanceFloor();
		scaleRate = distance / player_->GetAccelDistance();
		scaleRate *= 4.5f;
		axcellBillboard_->SetScale(scaleRate);
		axcellBillboard_->SetColor(color_);
	}
	else
	{
		scaleRate = 0.0f;
		color_.w = 0.0f;
	}

	axcellBillboard_->Update(camera);
}

void AccelEffect::Draw()
{
	if (player_->GetIsAccelColor())
	{
		axcellBillboard_->PreDraw();
		axcellBillboard_->Draw();
	}
}
