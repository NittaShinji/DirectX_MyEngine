#pragma once
#include "ResultRoopStage.h"

class MirrorOBJ : Object3d
{
public: //メンバ関数

	//コンストラクタ
	MirrorOBJ();
	//デストラクタ
	~MirrorOBJ();

	static std::unique_ptr<MirrorOBJ> Create(Object3d* object);

	//初期化
	void Initialize() override;

	//更新
	void Update(Camera* camera) override;

	//描画
	void Draw();

	//鏡面反射に必要な情報をセット
	void SetMirrorInfo();

private: //定数

	//オブジェクトに対して鏡面反射した場合の距離
	const float kMirrorDistance_ = -6.0f;

private: //メンバ変数

	Object3d* object3d_;
	
public: //アクセッサ
};

