
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
#include "Object3d.h"
#include "Model.h"
#include <memory>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform
{
	XMMATRIX mat;	// 3D変換行列
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

	//DirectX初期化処理
	directXBasic->Initialize(winApi);

	Model::StaticInitialize(directXBasic);

	//シングルトンインスタンスを作成
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);

	//3Dオブジェクト静的初期化
	//Object3d::StaticInitialize(directXBasic->GetDevice(), directXBasic->GetWinWidth(), directXBasic->GetWinHeight());
	//スプライト初期化処理
	Sprite* sprite = nullptr;
	sprite = new Sprite;
	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;

	spriteCommon->Initialize(directXBasic);
	sprite->Initialize(spriteCommon);
	spriteCommon->ShaderLoad();

	sprite->LoadTexture(0,"reimu.png");
	sprite->LoadTexture(1,"tomas.png");


	//3Dオブジェクト生成
	Object3d* object3d = nullptr;
	object3d = new Object3d("Resources/triangle/triangle.obj",directXBasic,sprite);


	//Sprite* testSprite = nullptr;
	//testSprite = new Sprite;

	///*Model* model = nullptr;
	//model = new Model(directXBasic);*/


	/*sprite->TexMapping();
	sprite->TexMappingSRVSet();*/
	/*sprite->ImageDateSet();
	sprite->ImageDateSRVSet();*/
	//sprite->LoadTexture(0,"tomas.png");
	//testSprite->LoadTexture(1, "reimu.png");
	//spriteCommon->SemiTransparent();

#pragma region キー入力 (P02_03)

	
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

		//XMFLOAT2 position = sprite->GetPosition();
		////座標を変更する
		//position.x += 0.1f;
		////変更を反映
		//sprite->SetPosition(position);

		//float rotation = sprite->GetRotation();
		////角度を変更する
		//rotation += 0.001f;
		////変更を反映
		//sprite->SetRotation(rotation);

		//XMFLOAT4 color = sprite->GetColor();
		//color = { 0,0,1,1 };
		//sprite->SetColor(color);

		//XMFLOAT2 anchorPoint = sprite->GetAnchorPoint();
		//anchorPoint = { 1.0f,1.0f };
		//sprite->SetAnchorPoint(anchorPoint);

		sprite->matUpdate();
		//testSprite->matUpdate();

		object3d->Update();

#pragma endregion

		directXBasic->BeforeDraw();

#pragma region パイプラインステートとルートシグネチャの設定コマンド(P02_01)

#pragma endregion

#pragma region SRV設定コマンド

		//object3d->Draw();

#pragma endregion 
		//spriteCommon->SetSRVheap(sprite->GetSRVheap());
		/*spriteCommon->Update();*/

		//スプライト描画開始
		//spriteCommon->BeforeDraw();
		//sprite->Draw();
		////スプライト描画終了
		//spriteCommon->AfterDraw();
		//

		object3d->BeforeDraw();

		object3d->Draw(sprite->GetSRVHeap());

		object3d->AfterDraw();

		//testSprite->Draw();

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
	//delete testSprite;
	delete object3d;
#pragma endregion WindowsAPI後始末

	winApi = nullptr;
	directXBasic = nullptr;
	//input = nullptr;
	return 0;
}