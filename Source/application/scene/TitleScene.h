#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
#include "GamePad.h"
#include "SoundManager.h"
#include "Easing.h"


/// <summary>
/// タイトルシーン
/// </summary>
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

	//スプライト
	std::unique_ptr<Sprite> titleSprite_ = nullptr;
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;
	
	std::unique_ptr<Object3d> titleSphere_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;

	//ライト
	LightGroup* lightGroup_ = nullptr;

	//待ち時間
	static const int32_t waitTime = 40;

	//球の演出用メンバ変数
	//球の回転角
	Vector3 sphereRotate = { 0,0,0 };

	//球の座標
	const Vector3 kSpherInitPos_ = {0.0f,5.0f,5.0f};
	const Vector3 kSpherInitScale_ = {3.0f,3.0f,3.0f};

	Vector3 spherPos_;

	//1周回る時間
	const float kRotateTime_ = 90.0f;
	const float kActionTime_ = 20.0f;

	//1周で色が変わる時間
	const float kChangeWhiteTime_ = 10.0f;
	//選択時に白色に変わる時間
	const float kChangeColorTime_ = 1080.0f;

	//動く時間のタイマー
	float moveTimer_ = kActionTime_;
	//回転時間のタイマー
	float rotateTimer_ = kRotateTime_;
	//色が変わるまでの時間のタイマー
	float changeColorTimer_ = kChangeColorTime_;
	//選択時に白色に変わる時間のタイマー
	float changeWhiteTimer_ = kChangeWhiteTime_;

	//球が上がっているか
	bool isSphereUp_ = false;
	//球が下がっているか
	bool isSphereDown_ = true;
	//色が変わっているか
	bool isChangeColor_ = true;
	//シーンが変わっているか
	bool isChangeScene_;
	//球の移動量
	const float kSphereMoveY_ = 0.3f;
	Vector3 sphereMoveValue_ = { 0,0,0 };
	//球がジャンプしているか
	bool isSphereJump_;

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

	EasingInfo backGroundChangEasing_ = { 1.0f, -1.0f, 0.0f, 30.0f };
	Vector4 backChangeColor = {0.0f,0.0f,0.0f,1.0f};

	//サウンド
	std::unique_ptr<Sound> titleSound_ = nullptr;
	std::unique_ptr<Sound> touchSound_ = nullptr;
};

