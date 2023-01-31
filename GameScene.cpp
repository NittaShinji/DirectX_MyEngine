#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

KeyInput* GameScene::keys_ = nullptr;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//�T�E���h
	sound->Finalize();
	delete sound;
	//�摜
	delete spriteCommon_;
	delete title_;
	delete test_;
	//���f��
	delete object3d_;
	delete nObject3d_;
	delete sObject3d_;
	delete camera_;
	delete testCamera_;
	//delete directXBasic_;

	sound = nullptr;
	title_ = nullptr;
	test_ = nullptr;
	//���f��
	object3d_ = nullptr;
	nObject3d_ = nullptr;
	sObject3d_ = nullptr;
	camera_ = nullptr;
	testCamera_ = nullptr;
}

void GameScene::Initialize(DirectXBasic* directXBasic,ImGuiManager* imGuiManager)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
	imGuiManager_ = imGuiManager;
	scene_ = GAME;

	//------------�T�E���h----------

	//Sound::StaticInitialize();
	sound = new Sound;
	sound->Initialize();
	sound->LoadSoundWave("Alarm01.wav");
	//sound->PlaySoundWave("Alarm01.wav");

	//------------�摜�ǂݍ���----------
	title_ = new Sprite;
	test_ = new Sprite;

	spriteCommon_ = new SpriteCommon;
	//�X�v���C�g�֌W������
	spriteCommon_->Initialize(directXBasic_);
	Sprite::StaticInitialize(spriteCommon_);

	//�摜�ǂݍ���
	Sprite::LoadTexture("tomas.png");
	Sprite::LoadTexture("black.png");

	//�X�̉摜��������(�w�肵���ԍ��̉摜���g�p����)
	XMFLOAT2 titlePosition = { 400,400 };
	XMFLOAT2 titleSize = { 400,400 };
	title_->Initialize(1,titlePosition, titleSize);

	XMFLOAT2 testPosition = { 400,400 };
	XMFLOAT2 testSize = { 100,100 };
	test_->Initialize(2, testPosition, testSize);

	//�V�F�[�_�[�ǂݍ���
	spriteCommon_->ShaderLoad();
	//�X�v���C�g�p�̃p�C�v���C���X�e�[�g(���߉\)��p��
	spriteCommon_->SemiTransparent();

	//------------���f���ǂݍ���----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//���f���ǂݍ���
	const string testModelName = "triangle_tex";
	const string testModelName2 = "block2";
	const string testModelName3 = "pillar";

	Model::Load(testModelName);
	Model::Load(testModelName2);
	Model::Load(testModelName3);

	//3D�I�u�W�F�N�g�̐���
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 position2 = { 30,0,0 };
	XMFLOAT3 position3 = { -30,0,0 };

	object3d_ = new Object3d(testModelName, position);
	nObject3d_ = new Object3d(testModelName2, position2);
	sObject3d_ = new Object3d(testModelName3, position3);

	//------------�J����----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = new Camera;
	testCamera_ = new Camera;
	XMFLOAT3 cameraEye = { 0,0,-100 };
	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize({30,0,-50}, cameraTarget, cameraUp);

	//--���̏����l��ݒ�
	//���S���W
	sphere_.center = XMVectorSet(0, 2, 0, 1);
	sphere_.radius = 1.0f;

	//���ʂ̏����l��ݒ�
	//�@���x�N�g��
	plane_.normal = XMVectorSet(0, 1, 0, 0);
	//���_(0,0,0)����̋���
	plane_.distance = 0.0f;
}

void GameScene::Update()
{
	switch (scene_)
	{

	case TITLE:

		break;

	case GAME:
		camera_->Updata();
		testCamera_->Updata();

		
		//�J�����̐؂�ւ�
		if (keys_->HasPushedKey(DIK_0))
		{
			object3d_->Update(testCamera_);
			nObject3d_->Update(testCamera_);
			sObject3d_->Update(testCamera_);
		}
		else
		{
			//���f���̍X�V����
			object3d_->Update(camera_);
			nObject3d_->Update(camera_);
			sObject3d_->Update(camera_);
		}
		
		//�摜�̍X�V����
		//�A���J�[�|�C���g�̐ݒ�
		XMFLOAT2 anchorPoint = { 0.0f,0.0f };
		title_->SetAnchorPoint(anchorPoint);
		title_->matUpdate();

		/*test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();*/
		
		//�X�v���C�g�̕ҏW�E�C���h�E�̕\��
		{
			static float pos[2] = { title_->GetPosition().x ,title_->GetPosition().x };
			
			ImGui::Begin("Edit");
			ImGui::SetWindowPos("Edit", ImVec2(100, 100));
			ImGui::SetWindowSize("Edit", ImVec2(500, 100));
			ImGui::SliderFloat2("position", pos, 0.0f, 1000.0f);
			title_->SetPosition(XMFLOAT2(pos));

			if(ImGui::Button("Close Me"))
				showEditWindow = false;

			ImGui::End();
		}

		break;

	case END:

		break;

	default:
		break;
	}
}

void GameScene::Draw()
{
	switch (scene_)
	{

	case TITLE:



		break;

	case GAME:

		//���f���`��
		object3d_->BeforeDraw();
		//object3d_->Draw();
		nObject3d_->Draw();
		sObject3d_->Draw();

		//�摜�`��
		spriteCommon_->BeforeDraw();
		spriteCommon_->Update();
		title_->Draw();
		test_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
