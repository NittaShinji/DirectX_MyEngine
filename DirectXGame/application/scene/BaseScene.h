#pragma once
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "Input.h"

//前方宣言
class SceneManager;

//シーン基底クラス
class BaseScene
{
public:

	//static void StaticInitialize(DirectXBasic* directXBasic);
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

	static DirectXBasic* directXBasic_;

	static ImGuiManager* imGuiManager_;

	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
};

