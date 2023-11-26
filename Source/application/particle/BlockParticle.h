#pragma once
#include "ObjParticleEmitter.h"
#include "ObjectAttribute.h"

class BlockParticle : public ObjParticleEmitter
{
public:

	static std::unique_ptr<BlockParticle> Create(std::string modelName, int32_t maxParticleNum);

	void Initialize();

	void Update(Camera* camera) override;

	void Reset();

	void PopUpdate(Camera* camera,const Vector3& popPos,bool isLanded,bool isPlayerDead,Attribute attributeColor);

	void ParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor);

	void LandParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor);
	void DeadParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor);


	/*void Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, const Vector4& colorSpeed, const Vector3& start_scale, const Vector3& end_scale);*/

	bool GetCanReset() { return canReset_; }

	void SetPlayerIsDead(bool isPlayerDead) { isPlayerDead_ = isPlayerDead; }
	void SetCanReset(bool canReset) { canReset_ = canReset; }
	void SetIsStartPoped(bool isStartPoped) { isStartPoped_ = isStartPoped; };
	
private:

	//座標
	Vector3 setPos_;
	//方向
	Vector3 setVel_;		

	//初期スケール
	Vector3 startScale_ = {};	
	//終期スケール
	Vector3 endScale_ = {};		

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };

	int32_t particleCount_;

	//生成開始フラグ
	bool isStartPoped_;

	//bool isStartDeadParticle_;
	//bool isStartLandParticle_;

	//ゲームオーバーかどうか
	bool isPlayerDead_;

	//リセットしてもよいか
	bool canReset_;

	Attribute attributeColor_;

};

