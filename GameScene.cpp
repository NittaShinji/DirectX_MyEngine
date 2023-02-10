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
	//���f��
	delete camera_;
	delete testCamera_;
	delete sphere_;
	delete ground_;

	//delete directXBasic_;

	sound = nullptr;
	title_ = nullptr;
	test_ = nullptr;
	//���f��
	object3d_ = nullptr;
	camera_ = nullptr;
	testCamera_ = nullptr;
	sphere_ = nullptr;
	ground_ = nullptr;
}

void GameScene::Initialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
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
	Sprite::LoadTexture("title.png");
	
	//�X�̉摜��������(�w�肵���ԍ��̉摜���g�p����)
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	title_->Initialize(1, titlePosition, titleSize);

	//�V�F�[�_�[�ǂݍ���
	spriteCommon_->ShaderLoad();
	//�X�v���C�g�p�̃p�C�v���C���X�e�[�g(���߉\)��p��
	spriteCommon_->SemiTransparent();

	//------------���f���ǂݍ���----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//���f���ǂݍ���
	
	const string sphere = "sphere";
	const string ground = "ground";

	Model::Load(sphere);
	Model::Load(ground);


	//3D�I�u�W�F�N�g�̐���
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 spherePosition = { 0,0,0 };
	XMFLOAT3 groundPosition = { 0,-5,0 };

	XMFLOAT3 sphereScale = { 10,10,10 };
	XMFLOAT3 groundScale = { 20,5,20 };

	sphere_ = new Object3d(sphere, spherePosition, sphereScale);
	ground_ = new Object3d(ground, groundPosition, groundScale);

	//------------�J����----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = new Camera;
	testCamera_ = new Camera;
	XMFLOAT3 cameraEye = { 0,0,-100 };
	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize({ 30,0,-50 }, cameraTarget, cameraUp);

	//--���̏����l��ݒ�
	//���S���W
	sphereCollision.pos = sphere_->GetWorldPos();
	sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };
	sphereCollision.radius = 10.0f;
	sphere_->SetTransform(sphereCollision.pos);

	//���ʂ̏����l��ݒ�
	//�@���x�N�g��
	planeCollision.normal = XMVectorSet(0, 1, 0, 0);
	//���_(0,0,0)����̋���
	planeCollision.distance = -5.0f;
}

void GameScene::Update()
{
	switch(scene_)
	{

	case TITLE:

		//�A���J�[�|�C���g�̐ݒ�
		XMFLOAT2 anchorPoint = { 0.0f,0.0f };
		title_->SetAnchorPoint(anchorPoint);
		title_->matUpdate();

		if(keyTimer < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = GAME;
				keyTimer = waitTime;
			}
		}
		else
		{
			keyTimer--;
		}

		break;

	case GAME:
		camera_->Updata();
		testCamera_->Updata();

		//�J�����̐؂�ւ�
		if(keys_->HasPushedKey(DIK_0))
		{
			sphere_->Update(testCamera_);
			ground_->Update(testCamera_);
		}
		else
		{
			//���f���̍X�V����
			sphere_->Update(camera_);
			ground_->Update(camera_);

		}

		//�摜�̍X�V����

		/*test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();*/

		if(moveTimer >= 0)
		{
			moveTimer--;
		}
		else if(moveTimer < 0)
		{
			moveTimer = actionTime;
		}

		if(moveTimer >= 30)
		{
			isDown = false;
			isUp = true;
			move.y = 0;
		}
		else if(moveTimer <= 29)
		{
			isUp = false;
			isDown = true;
			move.y = 0;
		}

		if(isDown == true && isUp == false)
		{
			move.y -= 0.3f;
		}
		else if(isUp == true && isDown == false)
		{
			move.y += 0.3f;
		}

		sphereCollision.pos.y += move.y;
		sphere_->SetTransform(sphereCollision.pos);
		sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };

		hit = Collision::CheckSphere2Plane(sphereCollision, planeCollision);
		sphere_->SetColorFlag(hit);


		//�X�v���C�g�̕ҏW�E�C���h�E�̕\��
		{

			ImGui::Begin("Collision");
			ImGui::SetWindowSize("Collision", ImVec2(500, 100));
			ImGui::InputInt("hit", &hit, 0.0f, 1000.0f);
			ImGui::End();
		}


		if(keyTimer < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer = waitTime;
			}
		}
		else
		{
			keyTimer--;
		}

		break;

	case END:

		if(keyTimer < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer = waitTime;
			}
		}
		else
		{
			keyTimer--;
		}

		break;

	default:
		break;
	}
}

void GameScene::Draw()
{
	switch(scene_)
	{

	case TITLE:

		//�摜�`��
		/*spriteCommon_->BeforeDraw();
		spriteCommon_->Update();
		title_->Draw();*/

		break;

	case GAME:

		//���f���`��
		sphere_->BeforeDraw();
		ground_->BeforeDraw();

		sphere_->Draw();
		ground_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
