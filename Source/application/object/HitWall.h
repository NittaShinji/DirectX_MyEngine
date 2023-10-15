#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class HitWall : public TouchableObject
{
public:

	static std::unique_ptr<HitWall> Create(const std::string& fileName);

	void OnCollision(const CollisionInfo& info) override;

	void Draw() override;

	//void Reset(const std::string& fileName);

	bool GetIsBreak_() { return isBreak_; }
	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }
	//void SetWallCollider(BaseCollider* collider) { collider_ = collider; }
	
private:

	//壁が壊れているか
	bool isBreak_;

	//BaseCollider* wallCollider_ = nullptr;
};

