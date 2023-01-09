#pragma once
#include "xaudio2.h"
#include <fstream>
#include <wrl.h>
#include <cassert>
#include <map>
#include <string>

class Sound
{

public:
	Sound();
	~Sound();

	//static void StaticInitialize();
	void Initialize();

public:

	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];			//�`�����N����ID
		int32_t size;		//�`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];		//"WAVE"
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;	//"FMT"
		WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
	};

	//���y�f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
	};
	
	/// <sumary>
	/// WAV�����ǂݍ���
	/// </sumary>
	/// <param name="filename">WAV�t�@�C����</param>
	//void LoadSoundWave(const char* fileName);
	void LoadSoundWave(const std::string& fileName);

	//�����f�[�^���
	void UnloadSound(SoundData* soundData);

	//�I������
	void Finalize();

	//�����Đ�
	void PlaySoundWave(const std::string& fileName);

private:

	//XAudio2�̃C���X�^���X
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	
	//�T�E���h�f�[�^
	std::map<std::string, SoundData> soundDates_;
	
};

