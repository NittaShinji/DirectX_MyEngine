#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
#include "GamePad.h"

class TitleScene : public BaseScene
{
public:

	//静的初期化
	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	std::unique_ptr<Sprite> titleSprite_ = nullptr;
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;

	std::unique_ptr<Object3d> titleSphere_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	//待ち時間
	static const int32_t waitTime = 40;

	XMFLOAT3 sphereRotate = { 0,0,0 };
	const int32_t kRoateTime_ = 240;
	const int32_t kActionTime_ = 60;

	const int32_t kChangeTime_ = 10;


	int32_t moveTimer_ = kActionTime_;
	int32_t rotateTimer_ = kRoateTime_;


	bool isChangeScene_;
	int32_t changeTimer_ = kChangeTime_;
	bool isUp_ = false;
	bool isDown_ = true;
	XMFLOAT3 move_ = { 0,0,0 };

	XMFLOAT3 spherPos_;

	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;
};

