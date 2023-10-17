#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class HitWall : public TouchableObject
{
public:

	static std::unique_ptr<HitWall> Create(const std::string& fileName);

	void OnCollision(const CollisionInfo& info) override;

	void Update(Camera* camera) override;

	void Draw() override;

	bool GetIsBreak_() { return isBreak_; }
	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }
	
private:

	//壁が壊れているか
	bool isBreak_;

	////リセット用の座標
	//Vector3 resetPos_;

	////破壊された際の退避用座標
	//Vector3 kEscapePos_;
};

