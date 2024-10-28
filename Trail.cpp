#include "Trail.h"

Trail::Trail(int bufferSize)
{
    if (bufferSize) {}
}

void Trail::Update()
{
    //データを更新
    for (size_t i = posArray.size() - 1; i > 0; --i)
    {
        posArray[i] = posArray[i - 1];
    }
    posArray.front() = tempPos;
    tempPos = PosBuffer();

    //曲線を作る
    std::vector<PosBuffer> usedPosArray = GetUsedPosArray();
    if (usedPosArray.empty())return;
    CreateCurveVertex(usedPosArray);

    //頂点データを更新する
    float amount = 1.0f / (usedPosArray.size() - 1);
    float v = 0;
    vertex.clear();
    vertex.resize(usedPosArray.size() * 2);
    for (size_t i = 0, j = 0; i < vertex.size() && j < usedPosArray.size(); i += 2, ++j)
    {
        vertex[i].pos = usedPosArray[j].head;
        vertex[i].uv = Vector2(1.0f, v);
        vertex[i + 1].pos = usedPosArray[j].tail;
        vertex[i + 1].uv = Vector2(0.0f, v);
        v += amount;
    }
}

void Trail::SetPos(Vector3& head, Vector3& tail)
{
    tempPos.head = head;
    tempPos.tail = tail;
    tempPos.isUsed = true;
}

void Trail::CreateCurveVertex(std::vector<PosBuffer>& usedPosArray)
{
    if (usedPosArray.size() < 3 )
    {
        return;
    }

    /*if (usedPosArray.size() < 3 || split < 1) 
    {
        return; 
    }*/
    //std::vector<PosBuffer> newPosArray;
    //newPosArray.reserve(usedPosArray.size() + (usedPosArray.size() - 1) * split);
    //const float amount = 1.0f / (split + 1);

    //PosBuffer newPos;
    //newPosArray.push_back(usedPosArray.front());
    //for (size_t i = 0; i < usedPosArray.size() - 1; ++i)
    //{
    //    float ratio = amount;
    //    //  CatMulに使う4つの点を作る（p0, p3がない時の処理も書く）
    //    XMVECTOR p0Head = i == 0 ? (XMLoadFloat3(&usedPosArray[1].headPos) + XMLoadFloat3(&usedPosArray[2].headPos)) * 0.5f : XMLoadFloat3(&usedPosArray[i - 1].headPos);
    //    XMVECTOR p1Head = XMLoadFloat3(&usedPosArray[i].headPos);
    //    XMVECTOR p2Head = XMLoadFloat3(&usedPosArray[i + 1].headPos);
    //    XMVECTOR p3Head = i == usedPosArray.size() - 2 ? (p0Head + p2Head) * 0.5f : XMLoadFloat3(&usedPosArray[i + 2].headPos);

    //    XMVECTOR p0Tail = i == 0 ? (XMLoadFloat3(&usedPosArray[1].tailPos) + XMLoadFloat3(&usedPosArray[2].tailPos)) * 0.5f : XMLoadFloat3(&usedPosArray[i - 1].tailPos);
    //    XMVECTOR p1Tail = XMLoadFloat3(&usedPosArray[i].tailPos);
    //    XMVECTOR p2Tail = XMLoadFloat3(&usedPosArray[i + 1].tailPos);
    //    XMVECTOR p3Tail = i == usedPosArray.size() - 2 ? (p0Tail + p2Tail) * 0.5f : XMLoadFloat3(&usedPosArray[i + 2].tailPos);

    //    for (size_t j = 0; j < static_cast<size_t>(split - 1); ++j)
    //    {
    //        newPos = PosBuffer();

    //        newPos.isUsed = true;
    //        XMStoreFloat3(&newPos.headPos, XMVectorCatmullRom(p0Head, p1Head, p2Head, p3Head, ratio));
    //        XMStoreFloat3(&newPos.tailPos, XMVectorCatmullRom(p0Tail, p1Tail, p2Tail, p3Tail, ratio));

    //        newPosArray.push_back(newPos);
    //        ratio += amount;
    //    }
    //    newPosArray.push_back(usedPosArray[i + 1]);
    //}
    //usedPosArray = newPosArray;

}
