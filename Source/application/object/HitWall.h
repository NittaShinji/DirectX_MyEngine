#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class HitWall : public TouchableObject
{
public:

	static std::unique_ptr<HitWall> Create(const std::string& fileName);

	void OnCollision(const CollisionInfo& info) override;

	void Initialize();

	void Update(Camera* camera, bool isPlayerAccelerating);

	void Draw() override;

	bool GetIsBreak_() { return isBreak_; }
	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }

	Vector3 GetWallPos() { return transform_; }
	bool GetIsCheckChangeColor() { return isCheckChangeColor_; }
	
private:

	//壁が壊れているか
	bool isBreak_;

	//プレイヤーが加速しているか
	bool isPlayerAccelerating_;

	bool isCheckChangeColor_;
};

