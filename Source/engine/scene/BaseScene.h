#pragma once
#include "DirectXBasic.h"
#include "Input.h"
#include "ImGuiManager.h"

//前方宣言
class SceneManager;

/// <summary>
/// シーン基底クラス
/// </summary>
class BaseScene
{
public:

	//静的初期化
	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	//初期化
	virtual void Initialize();

	//毎フレーム更新
	virtual void Update();

	//終了
	virtual void Finalize();

	//描画
	virtual void Draw();

public: //メンバ関数

	virtual ~BaseScene() = default;
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:

	enum PadNum
	{
		Player1 = 0,
	};

	static DirectXBasic* directXBasic_;

	static ImGuiManager* imGuiManager_;

	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
};

