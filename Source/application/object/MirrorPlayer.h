#pragma once
#include "Object3d.h"
#include "Player.h"

//反射用のプレイヤー
class MirrorPlayer : public Object3d
{
public:

	//インスタンスを生成
	static std::unique_ptr<MirrorPlayer> Create(Player* player);


public:
	//コンストラクタ
	MirrorPlayer();
	//デストラクタ
	~MirrorPlayer();

	//初期化
	void Initialize() override;

	//更新
	void Update(Camera* camera) override;

	//描画
	void Draw();

private: //メンバ変数

	static Player* player_;

public: //アクセッサ

	void SetPlayer(Player* player) { player_ = player; };

};

