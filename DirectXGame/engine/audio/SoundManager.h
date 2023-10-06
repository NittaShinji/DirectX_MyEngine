#pragma once
#include "Sound.h"
#include<map>

class SoundManager final
{
private:

	SoundManager();
	~SoundManager();

public:

	//�R�s�[�R���X�g���N�^�̖���
	SoundManager(const SoundManager& soundManager) = delete;
	//������Z�q�̖���
	SoundManager& operator=(const SoundManager& soundManager) = delete;

public:

	// �Đ��f�[�^
	struct Voice
	{
		std::string fileName;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

public:

	static SoundManager* GetInstance()
	{
		static SoundManager instance;
		return &instance;
	}

	void Initialize();

	/// <sumary>
	/// WAV�����ǂݍ���
	/// </sumary>
	/// <param name="filename">WAV�t�@�C����</param>
	void LoadSoundWave(const std::string& fileName);

	//�����f�[�^���
	void UnloadSound(Sound::SoundData* soundData);

	//�I������
	void Finalize();

private:

	//XAudio2�̃C���X�^���X
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	//�T�E���h�f�[�^�R���e�i
	std::map<std::string, Sound::SoundData> soundDatas_;

	////�T�E���h
	//std::unique_ptr<SoundData> sound_;
	//�Đ����̃T�E���h�f�[�^�R���e�i
	std::map<char, Voice> voices_;

public:

	Sound::SoundData GetSoundData(std::string fileName);

};

