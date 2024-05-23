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
	axcellBillboard_->SetColor(color_);
	axcellBillboard_->SetScale(scale_);
}

void AccelEffect::Update(Camera* camera)
{
	Vector3 pos = player_->GetTransform();
	axcellBillboard_->SetPos(pos);

	//加速できる色になったら
	if (player_->GetIsAccelColor())
	{
		//徐々に姿が見えるように
		if (color_.w <= kMaxAlpha_)
		{
			const float increaseAlphaValue = 0.05f;
			color_.w += increaseAlphaValue;
			axcellBillboard_->SetColor(color_);
		}
		//スケールをプレイヤーの距離に合わせて変更
		distance = player_->GetDistanceFloor();
		scaleRate = distance / player_->GetAccelDistance();
		const float increaseScaleRate = 4.5f;
		scaleRate *= increaseScaleRate;
		axcellBillboard_->SetScale(scaleRate);
	}
	else
	{
		//出現していたら徐々に消えるように
		if (color_.w > 0.0f)
		{
			const float decreaseAlphaValue = 0.1f;
			color_.w -= decreaseAlphaValue;
			axcellBillboard_->SetColor(color_);
		}

		if (scaleRate > 0.0f)
		{
			const float decreaseScaleRate = 0.1f;
			scaleRate -= decreaseScaleRate;
			axcellBillboard_->SetScale(scaleRate);
		}
	}

	//更新
	axcellBillboard_->Update(camera);
}

void AccelEffect::Draw()
{
	//ビルボード描画
	axcellBillboard_->PreDraw();
	axcellBillboard_->Draw();
}
