#include "SoundManager.h"

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

void SoundManager::Initialize()
{
	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//�}�X�^�[�{�C�X�̐���
	IXAudio2MasteringVoice* masterVoice_;
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result));
}

void SoundManager::LoadSoundWave(const std::string& fileName)
{
	//�d���ǂݍ��݃`�F�b�N
	if(soundDatas_.find(fileName) != soundDatas_.end())
	{
		return;
	}

	//--�t�@�C���I�[�v��
	//�f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	const std::string directoryPath = "Resources/Sound/";
	std::string fullPath = directoryPath + fileName;
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fullPath, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	//--.wav�f�[�^�ǂݍ���

#pragma region RIFF�`�����N�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	Sound::RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if(strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if(strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}
#pragma endregion

#pragma region FORMAT�`�����N�ǂݍ���
	//FORMAT�`�����N�̓ǂݍ���
	Sound::FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(Sound::ChunkHeader));
	if(strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
#pragma endregion

#pragma region DATA�`�����N�ǂݍ���

	Sound::ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if(strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	if(strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//DATA�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	std::vector<char> pBuffer(data.size);
	file.read(pBuffer.data(), data.size);

#pragma endregion

	//--�t�@�C�������
	file.close();

	//--�ǂݍ��񂾉����f�[�^��return
	Sound::SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = std::vector<BYTE>(pBuffer.begin(), pBuffer.end());
	soundData.bufferSize = data.size;

	//�T�E���h�f�[�^��A�z�z��(map)�Ɋi�[
	soundDatas_.insert(std::make_pair(fileName, soundData));
}

//�L�[�ɑΉ������T�E���h�f�[�^��Ԃ�
Sound::SoundData SoundManager::GetSoundData(std::string fileName)
{
	Sound::SoundData soundData = soundDatas_.at(fileName);

	return soundData;
}

