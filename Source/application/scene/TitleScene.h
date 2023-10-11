#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
#include "GamePad.h"
#include "SoundManager.h"

class TitleScene : public BaseScene
{
public:

	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

	//シーン遷移
	void SceneAnimation();

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	std::unique_ptr<Sprite> titleSprite_ = nullptr;
	std::unique_ptr<Sprite> clickSprite_ = nullptr;
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> bButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;

	std::unique_ptr<Object3d> titleSphere_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	//待ち時間
	static const int32_t waitTime = 40;

	Vector3 sphereRotate = { 0,0,0 };
	Vector3 spherPos_;

	//1周回る時間
	const float kRotateTime_ = 90.0f;
	const float kActionTime_ = 20.0f;

	const float kChangeWhiteTime_ = 10.0f;
	const float kChangeColorTime_ = 1080.0f;

	float moveTimer_ = kActionTime_;
	float rotateTimer_ = kRotateTime_;
	float changecolorTimer__ = kChangeColorTime_;
	float changeWhiteTimer_ = kChangeWhiteTime_;

	bool isUp_ = false;
	bool isDown_ = true;
	bool isChangeColor_ = true;
	bool isChangeScene_;
	Vector3 move_ = { 0,0,0 };

	float rotateAcc_;
	float moveRotate_;

	bool isJump_;

	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//シーンアニメーション時間
	const float kSceneAnimeTime_ = 45.0f;
	//アニメーション時間の変化量
	float sceneAnimeTimer_ = 0;
	//シーンアニメーション用変化量
	Vector2 animationMoveVec2;
	Vector3 animationMoveVec3;

	//アニメーションが終わったかどうか
	bool isFinishAnimetion;

	//サウンド
	std::unique_ptr<Sound> titleSound_ = nullptr;
	std::unique_ptr<Sound> touchSound_ = nullptr;
};

