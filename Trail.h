#pragma once
#include "MathUtillity.h"
#include <vector>

struct PosBuffer
{
	Vector3 head; //軌跡の先端の位置
	Vector3 tail; //軌跡の末端の位置
	bool isUsed;  //使われているかどうか
};

//頂点バッファ
struct SwordTrailVertex
{
	Vector3 pos = { 0, 0, 0 };
	Vector2 uv = { 0, 0 };
};

//軌跡クラス
class Trail
{
private:
	std::vector<PosBuffer> posArray;             //剣の位置を保存するバッファ
	std::vector<SwordTrailVertex> vertex;        //頂点バッファ
	PosBuffer tempPos;                           //現在フレームでの剣の位置
public:
	Trail(int bufferSize);                   //コンストラクタでposArrayのサイズを決める
	void Update();                                //履歴を更新して、頂点バッファを更新する
	void SetPos(Vector3& head, Vector3& tail);

	std::vector<PosBuffer> GetUsedPosArray() { return posArray; }
	void CreateCurveVertex(std::vector<PosBuffer>& usedPosArray);

};

