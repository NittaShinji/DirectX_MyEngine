#pragma once
#include "Sprite.h"

class GameSprite
{
public:

	//読み込み処理
	void Load();

	//初期化
	void Initialize();

	//更新
	void Update();

	//背景描画
	void BackGroundDraw();

	//UI描画
	void UIDraw();

	//トランジション描画
	void TransitionDraw();

	//シーンアニメーション
	void SceneAnimation();

	//シーン遷移アニメーションをリセット
	void ResetSceneAnimation();

private:

	const Vector2 kUiSize_ = { 64.0f,64.0f };

private:

	//シーンアニメーションが終わったかどうか
	bool isFinishAnimetion_;
	//シーンアニメーションを開始しているかどうか
	bool isStartSceneAnimation_ = false;
	//シーンアニメーション時間
	const float kSceneAnimeTime_ = 45.0f;
	//アニメーション時間の変化量
	float sceneAnimeTimer_ = 0;
	//シーンアニメーション用変化量
	Vector2 sceneAnimationVec_;

	//UIスプライト
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> jumpSprite_ = nullptr;

	std::unique_ptr<Sprite> bButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> arrowSprite_ = nullptr;

	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;
	std::unique_ptr<Sprite> sceneTransitionUp_ = nullptr;
	std::unique_ptr<Sprite> sceneTransitionDown_ = nullptr;
	std::unique_ptr<Sprite> nowLoadingSprite_ = nullptr;

};

