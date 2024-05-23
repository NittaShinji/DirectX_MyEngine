#pragma once
#include "TouchableObject.h"

class StageBlock : public TouchableObject
{
public: //メンバ関数

	static std::unique_ptr<StageBlock> Create(const std::string& fileName, const unsigned short coliderAttribute);

private: //メンバ変数

	//どのステージに所属しているか
	int32_t stageNum_;

public: //アクセッサ

	//ステージ数をセットする
	void SetStageNum(int32_t stageNum) { stageNum_ = stageNum; };

	//ステージ数を取得する
	int32_t GetStageNum() { return stageNum_; };
};

