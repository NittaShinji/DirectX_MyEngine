#include "Sound.h"
#include <mutex>

#pragma comment(lib,"xaudio2.lib")

Sound::Sound(){}

Sound::~Sound(){}

void Sound::Initialize()
{
	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//�}�X�^�[�{�C�X�̐���
	IXAudio2MasteringVoice* masterVoice_;
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result));

	indexVoice_ = 0u;
}

void Sound::LoadSoundWave(const std::string& fileName)
{
	//�d���ǂݍ��݃`�F�b�N
	if (soundDates_.find(fileName) != soundDates_.end())
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
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}
#pragma endregion

#pragma region FORMAT�`�����N�ǂݍ���
	//FORMAT�`�����N�̓ǂݍ���
	Sound::FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(Sound::ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
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
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0)
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
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = std::vector<BYTE>(pBuffer.begin(),pBuffer.end());
	soundData.bufferSize = data.size;

	//�T�E���h�f�[�^��A�z�z��(map)�Ɋi�[
	soundDates_.insert(std::make_pair(fileName, soundData));
}

//�����f�[�^���
void Sound::UnloadSound(SoundData* soundData)
{
	//�o�b�t�@�̃����������
	soundData->pBuffer = { 0 };
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

//�����Đ�
void Sound::PlaySoundWave(const std::string& fileName, bool isLoop)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDates_.find(fileName);
	//���ǂݍ��݂̌��o
	assert(it != soundDates_.end());
	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;

	uint32_t handle = indexVoice_;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// �Đ����f�[�^
	std::unique_ptr<Voice> voice = std::make_unique<Voice>();
	voice->fileName = fileName;
	voice->sourceVoice = pSourceVoice;
	
	//// �Đ����f�[�^�R���e�i�ɓo�^
	//voices_.insert(voice.get());

	
	//std::unique_lock<std::mutex> guard(voiceMutex_);
	// �Đ����f�[�^�R���e�i�ɓo�^
	voices_.insert(voice.get());
	

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer.data();
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	
	//���[�v����
	if(isLoop)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void Sound::StopSound(const std::string& fileName)
{
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
	// ����
	if(it != voices_.end())
	{
		(*it)->sourceVoice->DestroyVoice();

		voices_.erase(it);
	}
}

void Sound::PauseSound(const std::string& fileName)
{
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
	// ����
	if(it != voices_.end())
	{
		// �Đ���~
		(*it)->sourceVoice->Stop();
	}
}

void Sound::ResumeWave(const std::string& fileName)
{
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
	// ����
	if(it != voices_.end())
	{
		// �Đ��ĊJ
		(*it)->sourceVoice->Start();
	}
}

void Sound::SetVolume(const std::string& fileName, float volume)
{
	// �Đ������X�g���猟��
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
	// ����
	if(it != voices_.end())
	{
		(*it)->sourceVoice->SetVolume(volume);
	}
}

void Sound::Finalize()
{
	//XAudio2���
	xAudio2_.Reset();
	
	//�����f�[�^���
	//�C�e���[�^�[����
	std::map<std::string, SoundData>::iterator it = soundDates_.begin();
	for (; it != soundDates_.end(); it++)
	{
		UnloadSound(&it->second);
	}

	soundDates_.clear();
}