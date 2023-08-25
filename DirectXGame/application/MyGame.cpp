#include "MyGame.h"
#include "SceneFactory.h"
#include "Vector4.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

MyGame::MyGame(){}

void MyGame::Initialize()
{
	//���N���X�̏���������
	MyFramework::Initialize();
	
	//�Q�[���V�[������������
	//�V�[���t�@�N�g���[�𐶐����A�}�l�[�W���ɃZ�b�g
	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(std::move(sceneFactory_));
	//�V�[���}�l�[�W���ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->ChangeScene("TITLE");
	//��x�̂ݏ�����
	TitleScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());

	//�|�X�g�G�t�F�N�g����������
	postEffect_ = std::make_unique<PostEffect>();
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���̓ǂݍ���
	//SpriteCommon::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), "RedTex");
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), "RedTex");

	//SpriteCommon::GetInstance()->LoadTexture("postEffect.png");
	postEffect_->Initialize(directXBasic_.get());

	Vector3 cameraEye = { 30,7.5,-20 };

	Vector3 testCameraEye = { 30,7.5,-20 };
	Vector3 cameraTarget = { 0,5,5 };

	Vector3 testcameraTarget = { 0,5,5 };

	Vector3 cameraUp = { 0,1,0 };

	lightGroup_ = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup_);

	testCamera_ = std::make_unique<Camera>();
	testCamera_->Initialize(cameraEye, cameraTarget, cameraUp);

	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	/*backGroundSprite_ = std::make_unique<Sprite>();
	TextureManager::GetInstance()->LoadTexture("TitleFont.png");

	Vector2 backGroundPosition = { 0.0f,0.0f };
	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;
	const Vector2 backGroundSize = { backGroundWidth,backGroundHeight };

	backGroundSprite_->Initialize(backGroundPosition, backGroundSize);
	*/
}

void MyGame::Update()
{
	// DirectX���t���[������ ��������
	imGuiManager_->Begin();

	//���N���X�̍X�V����
	MyFramework::Update();

	postEffect_->SetAnchorPoint(Vector2(0.0f, 0.0f));

	//backGroundSprite_->matUpdate();
	testCamera_->Update();
	backGround_->Update(testCamera_.get());

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	/*SpriteCommon::GetInstance()->BeforeDraw();
	backGroundSprite_->Update();
	backGroundSprite_->Draw("TitleFont.png");*/

	//�����_�[�e�N�X�`���̕`��
	postEffect_->PreDrawScene();
	//���f���`��
	/*Object3d::BeforeDraw();
	backGround_->Draw();*/
	SceneManager::GetInstance()->Draw();

	postEffect_->PostDrawScene();

	//�`��J�n
	directXBasic_->BeforeDraw();

	//�|�X�g�G�t�F�N�g�̕`��
	postEffect_->Draw("RedTex");
	
	imGuiManager_->Draw();
	
	//�`��I��
	directXBasic_->AfterDraw();
}

void MyGame::Finalize()
{
	//�Q�[���S�̂̏I������
	imGuiManager_->Finalize();

	//���N���X�̏I������
	MyFramework::Finalize();
}