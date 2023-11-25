#pragma once
#include "Object3d.h"
#include "ObjParticleEmitter.h"

class ObjParticleManager final
{
public:

	ObjParticleManager();
	~ObjParticleManager();

private:

	//コピーコンストラクタの無効
	ObjParticleManager(const ObjParticleManager& objParticleManager) = delete;
	//代入演算子の無効
	ObjParticleManager& operator=(const ObjParticleManager& objParticleManager) = delete;

public:

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

	void AddEmitter(ObjParticleEmitter* particleEmitter);
	
	void ParticleReset(Camera* camera);

	//パーティクルとエミッターを全て削除
	void AllRemove();

	void ParticleRemove();

private: // 定数
	//static const int division = 50;					// 分割数
	//static const float radius;				// 底面の半径
	//static const float prizmHeight;			// 柱の高さ
	//static const int planeCount = division * 2 + division * 2;		// 面の数
	//static const int kVertexCount = 1024;		// 頂点数

private: // メンバ変数

	static ID3D12GraphicsCommandList* cmdList_;
	static ID3D12Device* device_;

	std::vector<ObjParticleEmitter*> emitters_;
};

