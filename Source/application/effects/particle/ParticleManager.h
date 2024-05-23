#pragma once
#include "Camera.h"
#include "Vector4.h"
#include "ObjectAttribute.h"
#include "ParticleEmitter.h"
#include "TextureManager.h"
#include "GroundParticle.h"
#include "HitParticle2D.h"
#include "SecondJump2DParticle.h"
#include <forward_list>

/// <summary>
/// 2Dパーティクルマネージャー
/// 2Dパーティクル生成器を管理する
/// </summary>

class Player;
class GameSpeed;

class ParticleManager final
{
private:

	//コントラクタ
	ParticleManager();
	//デストラクタ
	~ParticleManager();

private: // エイリアス

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//コピーコンストラクタの無効
	ParticleManager(const ParticleManager& particleManager) = delete;
	//代入演算子の無効
	ParticleManager& operator=(const ParticleManager& particleManager) = delete;

public: // 静的メンバ関数

	static ParticleManager* GetInstance()
	{
		static ParticleManager instance;
		return &instance;
	}

	// 静的初期化
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList*);

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

	/// <summary>
	/// エミッター(生成器)の追加
	/// </summary>
	/// <param name="particleEmitter"></param>
	void AddEmitter(ParticleEmitter* particleEmitter);

	//パーティクルとエミッターを全て削除
	void AllRemove();

	//パーティクルのみ削除
	void ParticleRemove();

	//エミッターにプレイヤーをセット
	void SetEmitterPlayer(Player* player);

	//エミッターにゲームスピードをセット
	void SetEmitterGameSpeed(GameSpeed* gameSpeed);

	void Preparation(GameSpeed* gameSpeed, Player* player);

	//プレイヤーが死んだ場合の処理
	void ProcessPlayerDead(Vector3 transform, bool isDead);

	//ImGuiの更新
	void ImGuiUpdate();

private: // メンバ変数

	static ID3D12GraphicsCommandList* cmdList_;
	static ID3D12Device* device_;

	std::vector<ParticleEmitter*> emitters_;

	std::unique_ptr<GroundParticle> groundParticle_;
	std::unique_ptr<HitParticle2D> hitParticle_;
	std::unique_ptr<SecondJump2DParticle> secondJumpParticle_;
};

