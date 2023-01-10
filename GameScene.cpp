#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	sound->Finalize();
	delete sound;
	//�摜
	delete spriteCommon_;
	delete title_;
	//���f��
	delete object3d_;
	delete nObject3d_;
	delete sObject3d_;
	delete camera_;
	delete testCamera_;
}

void GameScene::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
	scene_ = GAME;

	//------------�T�E���h----------

	//Sound::StaticInitialize();
	sound = new Sound;
	sound->Initialize();
	sound->LoadSoundWave("Alarm01.wav");
	sound->PlaySoundWave("Alarm01.wav");

	//------------�摜�ǂݍ���----------
	title_ = new Sprite;
	spriteCommon_ = new SpriteCommon;
	//�X�v���C�g�֌W������
	spriteCommon_->Initialize(directXBasic_);
	Sprite::StaticInitialize(spriteCommon_);

	//�摜�ǂݍ���
	Sprite::LoadTexture(0, "reimu.png");
	Sprite::LoadTexture(1, "tomas.png");

	//�X�̉摜��������(�w�肵���ԍ��̉摜���g�p����)
	XMFLOAT2 titlePosition = { 400,400 };
	XMFLOAT2 titleSize = { 100,100 };
	title_->Initialize(1,titlePosition, titleSize);

	//�V�F�[�_�[�ǂݍ���
	spriteCommon_->ShaderLoad();
	//�X�v���C�g�p�̃p�C�v���C���X�e�[�g(���߉\)��p��
	spriteCommon_->SemiTransparent();

	//------------���f���ǂݍ���----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//���f���ǂݍ���
	const string testModelName = "triangle_tex";
	const string testModelName2 = "triangle_tex2";
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

		//���ړ�
		
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (keys_->HasPushedKey(DIK_3)) { sphere_.center += moveY; }
		else if (keys_->HasPushedKey(DIK_4)) { sphere_.center -= moveY; }

		XMVECTOR moveX = XMVectorSet(0.01f,0, 0, 0);
		if (keys_->HasPushedKey(DIK_1)) { sphere_.center += moveY; }
		else if (keys_->HasPushedKey(DIK_2)) { sphere_.center -= moveY; }
		

		//stringstream�ŕϐ��̒l�𖄂ߍ���Ő��`����
		std::ostringstream sphertestr;
		sphertestr << "Sphere:("
			<< std::fixed << std::setprecision(2)
			<< sphere_.center.m128_f32[0] << ","
			<< sphere_.center.m128_f32[1] << ","
			<< sphere_.center.m128_f32[2] << ")";

		//���ƕ��ʓ����蔻��
		bool hit = Collision::CheckSphere2Plane(sphere_, plane_);
		/*if (hit)
		{
			sphertestr.str("");
			sphertestr.clear();
			sphertestr << "("
				<< std::fixed << std::setprecision(2);
		}*/

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
		object3d_->Draw();
		nObject3d_->Draw();
		sObject3d_->Draw();

		//�摜�`��
		spriteCommon_->BeforeDraw();
		spriteCommon_->Update();
		title_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
