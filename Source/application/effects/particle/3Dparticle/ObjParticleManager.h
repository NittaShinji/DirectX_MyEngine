#pragma once
#include "Object3d.h"
#include "ObjParticleEmitter.h"
#include "LandParticle.h"
#include "DeadParticle.h"
#include "BreakParticle.h"

/// <summary>
/// パーティクルマネージャー
/// パーティクル生成器を管理する
/// </summary>
 
class Stage;

class ObjParticleManager final
{
public:

	//コンストラクタ
	ObjParticleManager();
	//デストラクタ
	~ObjParticleManager();

private:

	//コピーコンストラクタの無効
	ObjParticleManager(const ObjParticleManager& objParticleManager) = delete;
	//代入演算子の無効
	ObjParticleManager& operator=(const ObjParticleManager& objParticleManager) = delete;

public:

	//インスタンスを取得
	static ObjParticleManager* GetInstance()
	{
		static ObjParticleManager instance;
		return &instance;
	}

	// 静的初期化
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

public: // メンバ関数

	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//エミッターを追加する
	void AddEmitter(ObjParticleEmitter* particleEmitter);
	
	//パーティクルをリセット
	void ParticleReset(Camera* camera);

	//パーティクルを削除
	void ParticleRemove();

	//パーティクルとエミッターを全て削除
	void AllRemove();

	//エミッターにプレイヤーをセット
	void SetEmitterPlayer(Player* player_);

	//エミッターにゲームスピードをセット
	void SetEmitterGameSpeed(GameSpeed* gameSpeed);

	//プレイヤーが死んだ場合の処理
	void ProcessPlayerDead(Camera* gameCamera);

	//生成更新
	void PopUpdate(GameSpeed* gameSpeed, Camera* camera, Player* player, Stage* stage);

private: // メンバ変数

	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	//デバイス
	static ID3D12Device* device_;
	//エミッター
	std::vector<ObjParticleEmitter*> emitters_;

	//3Dパーティクル
	std::unique_ptr<LandParticle> landParticle_ = nullptr;
	std::unique_ptr<DeadParticle> deadParticle_ = nullptr;
	std::unique_ptr<BreakParticle> breakParticle_ = nullptr;

	bool canReseted_ = false;

public: //アクセッサ

	bool GetCanReset() { return canReseted_; }
	DeadParticle* GetDeadParticle() { return deadParticle_.get(); }
};

