#pragma once
#include "Player.h"
#include "Billboard.h"

class AccelEffect
{
public:

	AccelEffect();
	~AccelEffect();

	//初期化
	void Initialize(Player* player, const std::string& fileName, const Billboard::BillboardType& billBoardtype);
	//更新
	void Update(Camera* camera);
	//描画
	void Draw();

private:

	const float kMaxScale_ = 10.0f;
	const float kMinScale_ = 1.0f;
	const float kMaxAlpha_ = 0.9f;
	const Vector4 kDefaultColor_ = {1.0f,1.0f,1.0f,1.0f};

private:

	Player* player_ = nullptr;

	std::unique_ptr<Billboard> axcellBillboard_ = nullptr;

	Vector4 color_ = { 0.0f,0.0f,0.0f,0.0f };

	const float runDistace = 4.9f;

	float scale_ = 0.0f;

	float distance = 0.0f;
	float scaleRate = 0.0f;
};

