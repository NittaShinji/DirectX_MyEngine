#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace DirectX;

DirectXBasic* Model::directXBasic_ = nullptr;
std::map<Model::MODELKEY, Model::MODELVALUE> Model::models_;
std::string Model::kDefaultTextureDirectoryPath_ = "Resources/";

void Model::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Model::Load(const std::string& path, DirectXBasic* directXBasic)
{
	Model model;
	model.name_ = path;

	//ファイルストリーム
	std::ifstream file;
	// .OBJファイルを開く
	//file.open("Resources/triangle_tex/triangle_tex.obj");

	const string modelName = "triangle_tex";
	const string fileName = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";

	file.open(directoryPath + fileName);
	
	//ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//頂点座標
	vector<XMFLOAT3>normals;	//法線ベクトル
	vector<XMFLOAT2>texcoords;	//テクスチャUV

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列が"v"なら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}
		
		//先頭文字列が"vt"ならテクスチャ
		if (key == "vt")
		{
			////UV成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列が"vf"なら法線ベクトル
		if (key == "vn")
		{
			////X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列が"f"ならポリゴン(三角形)
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexNormal;
				//頂点データ追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model.infomation_.vertices_.emplace_back(vertex);
				//インデックスデータの追加
				model.infomation_.indices_.emplace_back((unsigned short)model.infomation_.indices_.size());
			}
		}
	}

	file.close();

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * model.infomation_.vertices_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	HRESULT result;
	result = directXBasic_->GetResult();
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&model.infomation_.vertBuff_)); 
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = model.infomation_.vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(model.infomation_.vertices_.begin(), model.infomation_.vertices_.end(), vertMap);
	
	// 繋がりを解除
	model.infomation_.vertBuff_->Unmap(0, nullptr);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * model.infomation_.indices_.size());
	
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//　インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの設定
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&model.infomation_.indexBuff)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = model.infomation_.indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	std::copy(model.infomation_.indices_.begin(), model.infomation_.indices_.end(), indexMap);

	//マッピング解除
	model.infomation_.indexBuff->Unmap(0, nullptr);

#pragma region 頂点バッファビューの作成

	// GPU仮想アドレス
	model.infomation_.vbView.BufferLocation = model.infomation_.vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	model.infomation_.vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	model.infomation_.vbView.StrideInBytes = sizeof(infomation_.vertices_[0]);

#pragma endregion

#pragma region インデックスバッファビューの作成

	//インデックスバッファビューの作成
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	model.infomation_.ibView.BufferLocation = model.infomation_.indexBuff->GetGPUVirtualAddress();
	model.infomation_.ibView.Format = DXGI_FORMAT_R16_UINT;
	model.infomation_.ibView.SizeInBytes = sizeIB;

#pragma endregion

	models_.insert_or_assign(model.name_, model.infomation_);
}

void Model::Update(){}

void Model::Draw(ID3D12DescriptorHeap* srvHeapHandle){}


const Model::MODELVALUE* Model::GetMODELVALUE(const MODELKEY path)
{
	return &models_.at(path);
}
