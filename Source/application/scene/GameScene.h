#pragma once

#include "Sprite.h"
#include "Object3d.h"
#include "Sound.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
#include "LevelManager.h"
#include "GameCamera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "BaseScene.h"
#include "Stage.h"
#include "ParticleManager.h"
#include "BackGround.h"
#include "PostEffect.h"
#include "SoundManager.h"
#include "GroundParticle.h"
#include "BlockParticle.h"
#include "LandParticle.h"
#include "DeadParticle.h"
#include "BreakParticle.h"
#include "SecondJump2DParticle.h"
#include "GameSpeed.h"
#include "HitParticle2D.h"
#include "MirrorPlayer.h"
#include "TutorialEvent.h"
#include "GameSprite.h"
#include "ResultSprite.h"
#include "Billboard.h"

namespace NsEngine
{
	class CollisionManager;
}
class TouchableObject;
class Player;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

	//ライト
	LightGroup* lightGroup_ = nullptr;

	//カメラ
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	//3Dオブジェクト
	//ステージ
	std::unique_ptr<Stage> stage_ = nullptr;
	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<MirrorPlayer> mirrorPlayer_ = nullptr;

	//背景
	std::unique_ptr<Object3d> skydome_ = nullptr;
	std::unique_ptr<TouchableObject> plane_ = nullptr;
	std::unique_ptr<BackGround> backGround_ = nullptr;
	std::unique_ptr<BackGround> normalBackGround_ = nullptr;
	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//コライダー
	std::unique_ptr<SphereCollider> sphereCollider_ = nullptr;
	std::unique_ptr<TouchableObject> objGround_ = nullptr;

	//衝突マネージャー
	NsEngine::CollisionManager* collisionManager_ = nullptr;

	//待ち時間
	static const int32_t kWaitTime_ = 40;

	const Vector3 ambientLightColor_ = { 1,1,1 };
	//光線方向初期値
	const Vector3 lightDir_ = { 1,-5,-5 };
	const Vector3 lightColor_ = { 1,1,1 };
	const Vector3 whiteColor_ = { 1,1,1 };

	//スプライト
	std::unique_ptr<GameSprite> gameSprite_ = nullptr;

	const float initImguiValue_ = 0.0f;
	float imGuiPos[3]{ initImguiValue_,initImguiValue_,initImguiValue_ };
	float imGuiVel[3]{ initImguiValue_,initImguiValue_,initImguiValue_ };
	float imGuiAcc[3]{ initImguiValue_,initImguiValue_,initImguiValue_ };
	float imGuiDir_[3]{ initImguiValue_,initImguiValue_,initImguiValue_ };

	//サウンド
	std::unique_ptr<Sound> gameSound_ = nullptr;

	//アニメーションが終わったかどうか
	bool isFinishAnimetion_;

	//シーンアニメーション時間
	const float kSceneAnimeTime_ = 45.0f;
	//アニメーション時間の変化量
	float sceneAnimeTimer_ = 0;
	//シーンアニメーション用変化量
	Vector2 sceneAnimationVec_;

	//ゲームスピード
	std::unique_ptr<GameSpeed> gameSpeed_ = nullptr;

	//チュートリアルイベント
	std::unique_ptr<TutorialEvent> tutorialEvent_ = nullptr;

	//リザルト画像
	std::unique_ptr<ResultSprite> resultSprite_ = nullptr;

	int32_t nextStageNum_ = 0;
};

