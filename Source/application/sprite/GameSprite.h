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

	///背景スプライト
	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;

	//UIスプライト
	//Aボタン
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	//Bボタン
	std::unique_ptr<Sprite> bButtonSprite_ = nullptr;
	//ジャンプ
	std::unique_ptr<Sprite> jumpSprite_ = nullptr;
	//色変え矢印
	std::unique_ptr<Sprite> arrowSprite_ = nullptr;

	///トランジション用スプライト
	//上半分用の黒いスプライト
	std::unique_ptr<Sprite> sceneTransitionUp_ = nullptr;
	//下半分用の黒いスプライト
	std::unique_ptr<Sprite> sceneTransitionDown_ = nullptr;
	//読み込み中
	std::unique_ptr<Sprite> nowLoadingSprite_ = nullptr;
};

