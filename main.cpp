
//#include <d3d12.h>
//#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
using namespace DirectX;
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include "Input.h"
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include <memory>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform
{
	XMMATRIX mat;	// 3D変換行列
};

struct Object3d
{
	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform;
	//定数バッファマップ(行列用)
	ConstBufferDataTransform* constMapTransform;

	//アフィン変換情報
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matworld;
	//親オブジェクトのポインタ
	Object3d* parent = nullptr;

};

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();
	WindowsAPI* winApi = nullptr;
	winApi = new WindowsAPI();
	winApi->Initialize();

	std::unique_ptr<Input> input = std::make_unique<Input>();
	//std::unique_ptr<DirectXBasic> directBasic_ = std::make_unique<DirectXBasic>();
	DirectXBasic* directXBasic = nullptr;
	directXBasic = new DirectXBasic();
	directXBasic->Initialize(winApi);

	SpriteCommon* spriteCommon = nullptr;
	//スプライト共通部分
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(directXBasic);

	//ゲーム全体の初期化
	Sprite* sprite = nullptr;
	sprite = new Sprite;
	sprite->Initialize(spriteCommon);
	
#pragma region キー入力 (P02_03)

	//シングルトンインスタンスを作成
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);

#pragma endregion

#pragma region マテリアル用の定数バッファの初期化(P03_02)
	
#pragma endregion

#pragma region マテリアル用の定数バッファの転送(P03_02)
	
#pragma endregion

#pragma region 三次元用の定数バッファの初期化(P05_02)
	
#pragma endregion

#pragma region 三次元用の定数バッファの転送(P05_02)
	
#pragma endregion

#pragma region 二個目のオブジェクト用の定数バッファ
	
#pragma endregion	

#pragma	region 頂点データ(P02_01)
	
#pragma endregion 

#pragma region 頂点バッファの確保(P02_01)

	
#pragma endregion 

#pragma region インデックスバッファへのデータ転送(P03_04)
	
#pragma endregion

#pragma region インデックスバッファビューの作成(P03_04)
	
#pragma endregion

#pragma region 頂点バッファビューの作成(P02_01)
	
#pragma endregion
#pragma region 頂点シェーダファイルの読み込みとコンパイル(P02_01)

#pragma endregion

#pragma region ピクセルシェーダの読み込みとコンパイル(P02_01)
	
#pragma endregion

#pragma region 頂点レイアウト(P02_01)
	
#pragma endregion

#pragma region 画像読み込み
	
#pragma endregion

#pragma region 画像読み込み二枚目

#pragma endregion

#pragma region 画像読み込み三枚目

#pragma endregion

#pragma region 画像イメージデータの作成(自分で)

#pragma endregion 

#pragma region テクスチャバッファの設定(P04_02)

#pragma endregion

#pragma region 二枚目のテクスチャバッファの設定(P04_02)

#pragma endregion

#pragma region 三枚目のテクスチャバッファの設定(P04_02)
	
#pragma endregion

#pragma region テクスチャバッファの生成(P04_02)
	
#pragma endregion

#pragma region 二枚目のテクスチャバッファの生成(P04_02)
	
#pragma endregion

#pragma region 三枚目のテクスチャバッファの生成(P04_02)
	
#pragma endregion

#pragma region テクスチャバッファにデータ転送(P04_03_27)

#pragma endregion

#pragma region デスクリプタヒープ生成(04_02)

#pragma endregion
#pragma region デスクリプタハンドル
	
#pragma endregion

#pragma region 深度ビュー用のデスクリプタヒープを作成

#pragma endregion

#pragma region 深度ステンシルビューの生成

#pragma endregion

#pragma region シェーダーリソースビュー設定(P04_03)
	
#pragma endregion

#pragma region 二枚目の画像用シェーダーリソースビュー設定(P06_06)

	
#pragma endregion

#pragma region 三枚目の画像用シェーダーリソースビュー設定(P06_06)

	
#pragma endregion

#pragma region グラフィックスパイプライン設定(P02_02_P04)
	
#pragma region ブレンド設定(03_01)
	
#pragma endregion 	
#pragma region デスクリプタレンジの設定

	
#pragma endregion 
#pragma region ルートパラメータの設定(P03_02)(05_02_P16) 
	
#pragma endregion 
#pragma region ルートシグネチャの設定(P02_01_P38)

	
#pragma endregion 

#pragma endregion 

	// ゲームループ
	while (true) {

#pragma region ウインドウメッセージ処理
		//ゲームを実行している間、繰り返し行う処理
		// ブロック内はページ右側を参照
		// メッセージがある?
		//windowsメッセージ処理
		if (winApi->ProcessMessage())
		{
			//終了メッセージが来たらゲームループを抜ける
			break;
		}
#pragma endregion ウインドウメッセージ処理
#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理 ここから
		input->Update();
		
#pragma region 頂点バッファへのデータ転送 (P02_01)
	
#pragma endregion

#pragma endregion

#pragma region ワールド変換行列 

#pragma endregion

		directXBasic->BeforeDraw();

#pragma region パイプラインステートとルートシグネチャの設定コマンド(P02_01)

#pragma endregion

#pragma region SRV設定コマンド

		
#pragma endregion 

		spriteCommon->Update();
		sprite->Update();
		
#pragma region その他の設定コマンド

		directXBasic->AfterDraw();
	}

#pragma endregion

#pragma region WindowsAPI後始末
	//ゲーム全体の終了処理
	
	//input->Delete();
	//KeyInput::destroy();
	//delete keyInput;
	delete winApi;
	delete directXBasic;
	delete spriteCommon;
	//delete playerSprite;
	delete sprite;
#pragma endregion WindowsAPI後始末

	winApi = nullptr;
	directXBasic = nullptr;
	//input = nullptr;
	return 0;
}