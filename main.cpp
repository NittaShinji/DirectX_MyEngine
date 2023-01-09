
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
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include "Input.h"
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "GameScene.h"
#include "xaudio2.h"
#include <memory>
#include <fstream>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"xaudio2.lib")

////�`�����N�w�b�_
//struct ChunkHeader
//{
//	char id[4];			//�`�����N����ID
//	int32_t size;		//�`�����N�T�C�Y
//};
//
////RIFF�w�b�_�`�����N
//struct RiffHeader
//{
//	ChunkHeader chunk;	//"RIFF"
//	char type[4];		//"WAVE"
//};
//
////FMT�`�����N
//struct FormatChunk
//{
//	ChunkHeader chunk;	//"FMT"
//	WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
//};
//
////���y�f�[�^
//struct SoundData
//{
//	//�g�`�t�H�[�}�b�g
//	WAVEFORMATEX wfex;
//	//�o�b�t�@�̐擪�A�h���X
//	BYTE* pBuffer;
//	//�o�b�t�@�̃T�C�Y
//	unsigned int bufferSize;
//};
//
//SoundData SoundLoadWave(const char* fileName)
//{
//	HRESULT result;
//
//	//--�t�@�C���I�[�v��
//
//	//�t�@�C�����̓X�g���[���̃C���X�^���X
//	std::ifstream file;
//	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
//	file.open(fileName, std::ios_base::binary);
//	//�t�@�C���I�[�v�����s�����o����
//	assert(file.is_open());
//
//	//--.wav�f�[�^�ǂݍ���
//	 
//#pragma region RIFF�`�����N�ǂݍ���
//	//RIFF�w�b�_�[�̓ǂݍ���
//	RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	//�t�@�C����RIFF���`�F�b�N
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
//	{
//		assert(0);
//	}
//	//�^�C�v��WAVE���`�F�b�N
//	if (strncmp(riff.type, "WAVE", 4) != 0)
//	{
//		assert(0);
//	}
//#pragma endregion
//
//#pragma region FORMAT�`�����N�ǂݍ���
//	//FORMAT�`�����N�̓ǂݍ���
//	FormatChunk format = {};
//	//�`�����N�w�b�_�[�̊m�F
//	file.read((char*)&format, sizeof(ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//�`�����N�{�̂̓ǂݍ���
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char*)&format.fmt, format.chunk.size);
//#pragma endregion
//
//#pragma region DATA�`�����N�ǂݍ���
//
//	ChunkHeader data;
//	file.read((char*)&data, sizeof(data));
//	//JUNK�`�����N�����o�����ꍇ
//	if (strncmp(data.id, "JUNK", 4) == 0)
//	{
//		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
//		file.seekg(data.size, std::ios_base::cur);
//		//�ēǂݍ���
//		file.read((char*)&data, sizeof(data));
//	}
//	if (strncmp(data.id, "data", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//DATA�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
//	char* pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//
//#pragma endregion
//
//	//--�t�@�C�������
//	file.close();
//
//	//--�ǂݍ��񂾉����f�[�^��return
//	
//	//return����ׂ̉����f�[�^
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE*> (pBuffer);
//	soundData.bufferSize = data.size;
//
//	return soundData;
//}
//
////�����f�[�^���
//void SoundUnload(SoundData* soundData)
//{
//	//�o�b�t�@�̃����������
//	delete[] soundData->pBuffer;
//
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
////�����Đ�
//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData)
//{
//	HRESULT result;
//
//	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	//�Đ�����g�`�f�[�^�̐ݒ�
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	//�g�`�f�[�^�̍Đ�
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//
//}

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();
	
	WindowsAPI* winApi = nullptr;
	winApi = new WindowsAPI();
	winApi->Initialize();

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	std::unique_ptr<Input> input = std::make_unique<Input>();
	//std::unique_ptr<DirectXBasic> directBasic_ = std::make_unique<DirectXBasic>();
	DirectXBasic* directXBasic = nullptr;
	directXBasic = new DirectXBasic();

	//DirectX����������
	directXBasic->Initialize(winApi);

	//------�T�E���h

	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//�����ǂݍ���
	//SoundData testSound = SoundLoadWave("Resources/Alarm01.wav");

	//�V���O���g���C���X�^���X���쐬
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);
	
	GameScene* gameScene = nullptr;
	gameScene = new GameScene;
	gameScene->Initialize(directXBasic);

	//SoundPlayWave(xAudio2.Get(), testSound);

	// �Q�[�����[�v
	while (true) {
		//windows���b�Z�[�W����
		if (winApi->ProcessMessage())
		{
			//�I�����b�Z�[�W��������Q�[�����[�v�𔲂���
			break;
		}

		// DirectX���t���[������ ��������
		input->Update();

		gameScene->Update();

		//�`��
		directXBasic->BeforeDraw();
		
		gameScene->Draw();
		
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI��n��
	//�Q�[���S�̂̏I������

	delete winApi;
	delete directXBasic;
	delete gameScene;
	
#pragma endregion WindowsAPI��n��

	winApi = nullptr;
	directXBasic = nullptr;
	
	return 0;
}