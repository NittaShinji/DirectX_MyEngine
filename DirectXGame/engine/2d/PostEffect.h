#pragma once
#include "Sprite.h"

class PostEffect :
    public Sprite
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(const std::string& fileName);

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList"></param>
    void PreDrawScene();

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList"></param>
    void PostDrawScene();



private:

    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;

    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //RTN用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //画面クリアカラー
    static const float clearColor[4];
};

