#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "GameScene.h"
#include "Input.h"

//ゲーム全体
class MyFramework
{
public:	//メンバ関数

	//初期化
	virtual void Initialize();

	//毎フレーム更新
	virtual void Update();

	//終了
	virtual void Finalize();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequst() { return endRequst_; }

	//実行
	void Run();

public: //メンバ関数

	//仮想デストラクタ
	virtual ~MyFramework() = default;

protected: //メンバ変数

	//WindowsApi
	std::unique_ptr<WindowsAPI> winApi_ = nullptr;

	//DirectX基盤
	std::unique_ptr<DirectXBasic> directXBasic_ = nullptr;

	//入力処理
	std::unique_ptr<Input> input_ = nullptr;

	//ゲーム終了フラグ
	bool endRequst_ = false;

};

