#pragma once
#include "Camera.h"
#include "Vector4.h"
#include "ObjectAttribute.h"
#include "ParticleEmitter.h"
#include "TextureManager.h"
#include <forward_list>

/// <summary>
/// 3Dオブジェクト
/// </summary>

class ParticleManager final
{
private:

	ParticleManager();
	~ParticleManager();

private: // エイリアス

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//コピーコンストラクタの無効
	ParticleManager(const ParticleManager& soundManager) = delete;
	//代入演算子の無効
	ParticleManager& operator=(const ParticleManager& soundManager) = delete;

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

	//static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//static void PostDraw();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(Camera* camera, Attribute attribute);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//static std::unique_ptr<ParticleManager> Create();

	void AddEmitter(ParticleEmitter* particleEmitter);
	//void AddEmitter(std::unique_ptr<ParticleEmitter> particleEmitter);


	//static void LoadTexture();
	//static void LoadTexture(const std::string& fileName);

	//パーティクルとエミッターを全て削除
	void AllRemove();

	void ParticleRemove();

	void EmitterRemove();



private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int kVertexCount = 1024;		// 頂点数

private: // メンバ変数

	static ID3D12GraphicsCommandList* cmdList_;
	static ID3D12Device* device_;

	std::vector<ParticleEmitter*> emitters_;
	//std::vector<std::unique_ptr<ParticleEmitter>> emitters_;


};

