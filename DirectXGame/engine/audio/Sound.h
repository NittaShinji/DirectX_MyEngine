#pragma once
#include "xaudio2.h"
#include <fstream>
#include <wrl.h>
#include <cassert>
#include <map>
#include <string>
#include <stdint.h>
#include <vector>
#include <set>

class Sound 
{

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
		std::vector<BYTE> pBuffer;
		//�o�b�t�@�̃T�C�Y
		uint32_t bufferSize;
	};

	// �Đ��f�[�^
	struct Voice
	{
		std::string fileName;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

public: //�����o�֐�

	Sound();
	~Sound();

	static Sound* GetInstance()
	{
		static Sound instance;
		return &instance;
	}

	/// <sumary>
	/// WAV�����ǂݍ���
	/// </sumary>
	/// <param name="filename">WAV�t�@�C����</param>
	//void LoadSoundWave(const std::string& fileName);

	//�����f�[�^���
	//void UnloadSound(SoundData* soundData);

	//�I������
	//void Finalize();

	void Initialize(const std::string& fileName);

	//�����Đ�
	void PlaySoundWave(const std::string& fileName, bool isLoop);

	//������~
	void StopSound(const std::string& fileName);

	//�����ꎞ��~
	void PauseSound(const std::string& fileName);

	//�ꎞ��~����̍ĊJ
	void ResumeWave(const std::string& fileName);

	//���ʒ���
	void SetVolume(const std::string& fileName, float volume);

private:

	//�T�E���h�f�[�^
	SoundData soundData_;
};

