#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene(){}

GameScene::~GameScene(){}

void GameScene::Initialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = directXBasic;
	imGuiManager_ = imGuiManager;
	scene_ = TITLE;

	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//------------���C�g------------
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());

	lightGroup_ = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup_);

	//------------�T�E���h----------
	sound_ = std::make_unique<Sound>();
	sound_->Initialize();
	sound_->LoadSoundWave("Alarm01.wav");
	sound_->PlaySoundWave("Alarm01.wav");
	
	//-----------�ǂݍ���---------------
	 
	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile("test3.json");

	for(auto& objectData : levelData_->objects)
	{
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//���f�����w�肵��3D�I�u�W�F�N�g���쐬
		//���f�������[�h
		Model::Load(objectData.fileName);

		//3D�I�u�W�F�N�g�̐���
		std::unique_ptr<Object3d> newObject = std::make_unique<Object3d>();

		//���W
		DirectX::XMFLOAT3 pos;
		pos = objectData.translation;
		newObject->SetTransform(pos);
		//��]�p
		DirectX::XMFLOAT3 rot;
		rot = objectData.rotation;

		newObject->SetRotation(rot);
		//���W
		DirectX::XMFLOAT3 scale;
		scale = objectData.scaling;
		newObject->SetScale(scale);

		newObject->Initialize(objectData.fileName, pos, scale);

		//�z��ɓo�^
		objects_.push_back(std::move(newObject));
	}

	//------------�摜�ǂݍ���----------
	title_ = std::make_unique<Sprite>();
	test_ = std::make_unique<Sprite>();
	//spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_ = SpriteCommon::GetInstance();

	//�X�v���C�g�֌W������
	spriteCommon_->Initialize(directXBasic_);
	//�摜�ǂݍ���
	//spriteCommon_->LoadTexture("title.png");
	SpriteCommon::LoadTexture("tomas.png");
	SpriteCommon::LoadTexture("title.png");

	Sprite::StaticInitialize();
	
	//�X�̉摜��������(�w�肵���ԍ��̉摜���g�p����)
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	XMFLOAT2 testPosition = { 100,100 };
	XMFLOAT2 testSize = { 266,369 };

	title_->Initialize(titlePosition, titleSize);
	test_->Initialize(testPosition, testSize);

	//�V�F�[�_�[�ǂݍ���
	spriteCommon_->ShaderLoad();
	//�X�v���C�g�p�̃p�C�v���C���X�e�[�g(���߉\)��p��
	spriteCommon_->SemiTransparent();

	//------------���f���ǂݍ���----------
	
	//���f���ǂݍ���
	const string sphere = "sphere";
	const string test = "NoImageModel";

	/*const string ground = "ground";
	const string testTriangle = "triangle_tex";
	const string ray = "blackCube";*/

	Model::Load(test);
	Model::Load(sphere);
	/*Model::Load(ground);
	Model::Load(testTriangle);
	Model::Load(ray);*/

	//3D�I�u�W�F�N�g�̐���
	XMFLOAT3 sphereScale = { 10,10,10 };
	XMFLOAT3 groundScale = { 20,3,20 };
	XMFLOAT3 triangleScale = { 20,1,20 };
	XMFLOAT3 rayScale = { 1,1,1 };

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 spherePosition = { 0,0,0 };
	XMFLOAT3 groundPosition = { 0,0,0 };
	XMFLOAT3 trianglePosition = { 0,0,0 };
	//XMFLOAT3 raySetPosition = { 0,rayScale.y * 2,rayScale.z * 2 };
	//XMFLOAT3 raySetPosition = { 0,rayScale.y,0 };

	sphere_ = std::make_unique<Object3d>();
	sphere_->Initialize(sphere, XMFLOAT3(30, 0, 0), sphereScale);
	testObject_ = std::make_unique<Object3d>();
	testObject_->Initialize(test, XMFLOAT3(-30,0,0), sphereScale);

	//------------�J����----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = std::make_unique<Camera>();
	//camera_ = new Camera;
	testCamera_ = std::make_unique<Camera>();


	XMFLOAT3 cameraEye = { 0,60,-30 };
	XMFLOAT3 testCameraEye = { 50,60,-30 };

	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize(testCameraEye, cameraTarget, cameraUp);

	//------------�����蔻��----------
	//--���̏����l��ݒ�
	//���S���W
	/*sphereCollision.pos = sphere_->GetWorldPos();
	sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };
	sphereCollision.radius = 10.0f;
	sphere_->SetTransform(sphereCollision.pos);*/

	//���ʂ̏����l��ݒ�
	//�@���x�N�g��
	//planeCollision.normal = XMVectorSet(0, 1, 0, 0);
	////���_(0,0,0)����̋���
	//planeCollision.distance = 0.0f;

	//�O�p�`�̏����l��ݒ�
	//���W�A��90�x�@1.5708f�@
	//XMFLOAT3 triangleRotation = { 1.5708f,0.0f,0.0f };
	//triangle_->SetRotation(triangleRotation);
	//trianglePosition2 = triangle_->GetWorldPos();

	//triangleCollison.p0 = XMVectorSet(trianglePosition2.x - triangleScale.x, trianglePosition2.y, trianglePosition2.z - triangleScale.z, 1); //����O
	//triangleCollison.p1 = XMVectorSet(trianglePosition2.x - triangleScale.x, trianglePosition2.y, trianglePosition2.z + triangleScale.z, 1); //��
	//triangleCollison.p2 = XMVectorSet(trianglePosition2.x + triangleScale.x, trianglePosition2.y, trianglePosition2.z - triangleScale.z, 1); //�E��O

	//triangleCollison.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0); //�����

	////���C
	////3D�I�u�W�F�N�g�̍��W���擾
	//rayWorldPositon = ray_->GetWorldPos();
	////���C�̏����l��ݒ�
	//rayCollision.start = XMVectorSet(rayWorldPositon.x, rayWorldPositon.y, rayWorldPositon.z, 1);//���_����
	//rayCollision.dir = XMVectorSet(0, -1, 0, 0);//������
	////�΂ߌ����ɂ���ꍇ�͐��K������B

}

void GameScene::Update()
{
	//�������������l
	static XMVECTOR lightDir = { 0,1,5,0 };
	static XMFLOAT3 color = { 1, 1, 1 };

	if(keys_->HasPushedKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
	else if(keys_->HasPushedKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; }
	if(keys_->HasPushedKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
	else if(keys_->HasPushedKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; }

	if(keys_->HasPushedKey(DIK_2)) { color.x -= 0.01f; }
	else if(keys_->HasPushedKey(DIK_3)) { color.x += 0.01f; }
	else if(keys_->HasPushedKey(DIK_4)) { color.y += 0.01f; }
	else if(keys_->HasPushedKey(DIK_5)) { color.y -= 0.01f; }

	lightGroup_->SetAmbientColor(color);
	lightGroup_->SetDirLightDir(0, lightDir);
	lightGroup_->SetDirLightColor(0, XMFLOAT3(0,0,1));

	lightGroup_->SetDirLightDir(1, lightDir);
	lightGroup_->SetDirLightColor(1, XMFLOAT3(0,1,0));

	lightGroup_->SetDirLightDir(2, lightDir);
	lightGroup_->SetDirLightColor(2, XMFLOAT3(1,0,0));

	{
		//imgui����̃��C�g�p�����[�^�𔽉f
		lightGroup_->SetAmbientColor(XMFLOAT3(ambientColor0_));
		lightGroup_->SetDirLightDir(0, XMVECTOR({ lightDir0_[0], lightDir0_[1], lightDir0_[2], 0 }));
		lightGroup_->SetDirLightColor(0, XMFLOAT3(lightColor0_));
		lightGroup_->SetDirLightDir(1, XMVECTOR({ lightDir1_[0], lightDir1_[1], lightDir1_[2], 0 }));
		lightGroup_->SetDirLightColor(1, XMFLOAT3(lightColor1_));
		lightGroup_->SetDirLightDir(2, XMVECTOR({ lightDir2_[0], lightDir2_[1], lightDir2_[2], 0 }));
		lightGroup_->SetDirLightColor(2, XMFLOAT3(lightColor2_));
	}

	lightGroup_->Update();

	switch(scene_)
	{

	case TITLE:

		//�A���J�[�|�C���g�̐ݒ�
		XMFLOAT2 anchorPoint = { 0.0f,0.0f };
		title_->SetAnchorPoint(anchorPoint);
		title_->matUpdate();
		test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();

		if(keyTimer_ < 0)
		{
			/*if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = GAME;
				keyTimer = waitTime;
			}*/

			if(KeyInput::HasPushedKey(DIK_SPACE))
			{

				scene_ = GAME;
				keyTimer_ = kWaitTime_;
			}

			/*if(input_->GetKeys()->HasPushedKey(DIK_SPACE))
			{
				scene_ = GAME;
				keyTimer = waitTime;
			}*/
		}
		else
		{
			keyTimer_--;
		}

		break;

	case GAME:

		camera_->Updata();
		testCamera_->Updata();

		//�J�����̐؂�ւ�
		if(keys_->HasPushedKey(DIK_0))
		{
			for(auto& object : objects_)
			{
				object->Update(testCamera_.get());
			}

			testObject_->Update(testCamera_.get());
			sphere_->Update(testCamera_.get());
			//ground_->Update(testCamera_);
			//triangle_->Update(testCamera_);
			//ray_->Update(testCamera_);
		}
		else
		{
			for(auto& object : objects_)
			{
				object->Update(testCamera_.get());
			}

			testObject_->Update(camera_.get());
			//���f���̍X�V����
			sphere_->Update(camera_.get());

			//ground_->Update(camera_);
			//triangle_->Update(camera_);
			//ray_->Update(camera_);
		}

		//�摜�̍X�V����

		/*test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();*/

		///*if(moveTimer >= 0)
		//{
		//	moveTimer--;
		//}
		//else if(moveTimer < 0)
		//{
		//	moveTimer = actionTime;
		//}

		//if(moveTimer >= 30)
		//{
		//	isDown = false;
		//	isUp = true;
		//	move.y = 0;
		//}
		//else if(moveTimer <= 29)
		//{
		//	isUp = false;
		//	isDown = true;
		//	move.y = 0;
		//}

		//if(isDown == true && isUp == false)
		//{
		//	move.y -= 0.3f;
		//}
		//else if(isUp == true && isDown == false)
		//{
		//	move.y += 0.3f;
		//}*/

		/*if(keys_->HasPushedKey(DIK_W))
		{
			move.z -= 0.01f;
		}
		else if(keys_->HasReleasedKey(DIK_W) != keys_->HasPushedKey(DIK_S))
		{
			move.z = 0;
		}
		else if(keys_->HasPushedKey(DIK_S))
		{
			move.z += 0.01f;
		}
		else if(keys_->HasReleasedKey(DIK_S)&& keys_->HasPushedKey(DIK_W))
		{
			move.z = 0;
		}

		if(keys_->HasPushedKey(DIK_A))
		{
			move.x -= 0.01f;
		}
		else if(keys_->HasReleasedKey(DIK_A) != keys_->HasPushedKey(DIK_D))
		{
			move.x = 0;
		}
		else if(keys_->HasPushedKey(DIK_D))
		{
			move.x += 0.01f;
		}
		else if(keys_->HasReleasedKey(DIK_D)&& keys_->HasPushedKey(DIK_A))
		{
			move.x = 0;
		}

		if(keys_->HasPushedKey(DIK_Q))
		{
			move.y -= 0.01f;
		}
		else if(keys_->HasReleasedKey(DIK_Q) != keys_->HasPushedKey(DIK_E))
		{
			move.y = 0;
		}
		else if(keys_->HasPushedKey(DIK_E))
		{
			move.y += 0.01f;
		}
		else if(keys_->HasReleasedKey(DIK_E)&& keys_->HasPushedKey(DIK_Q))
		{
			move.y = 0;
		}*/

		

		////���C����

		//rayWorldPositon = ray_->GetWorldPos();

		//{
		//	float moveZ = 0.1f;
		//	if(keys_->HasPushedKey(DIK_W)) { rayWorldPositon.z += moveZ; }
		//	else if(keys_->HasPushedKey(DIK_S)) { rayWorldPositon.z -= moveZ; }

		//	float moveY = 0.1f;
		//	if(keys_->HasPushedKey(DIK_Q)) { rayWorldPositon.y += moveY; }
		//	else if(keys_->HasPushedKey(DIK_E)) { rayWorldPositon.y -= moveY; }

		//	float moveX = 0.1f;
		//	if(keys_->HasPushedKey(DIK_A)) { rayWorldPositon.x -= moveX; }
		//	else if(keys_->HasPushedKey(DIK_D)) { rayWorldPositon.x += moveX; }


		//}

		//���C�̓����蔻����X�V
		//rayCollision.dir = XMVectorSet(0, -1, 0, 0);//������
		//ray_->SetTransform(rayWorldPositon);

		//rayCollision.start = XMVectorSet(rayWorldPositon.x, rayWorldPositon.y, rayWorldPositon.z, 1);//���_����
		//rayCollision.dir = XMVectorSet(0, -1, 0, 0);//������

		//sphereCollision.pos.y += move.x;
		//sphereCollision.pos.y += move.y;
		//sphereCollision.pos.z += move.z;

		/*rotate.y += move.y;


		sphere_->SetTransform(sphereCollision.pos);
		sphere_->SetRotation(rotate);*/


		//sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };*/

		//trianglePosition2 = triangle_->GetWorldPos();

		//

		////���ƎO�p�`�̓����蔻��
		///*XMVECTOR inter;
		//hit = Collision::CheckSphere2Triangle(sphereCollision, triangleCollison, &inter);*/
		////���C�ƕ��ʂ̓����蔻��
		//XMVECTOR inter;
		//float distance;
		////hit = Collision::CheckRay2Plane(rayCollision, planeCollision, &distance, &inter);
		//hit = Collision::CheckRay2Triangle(rayCollision, triangleCollison, &distance, &inter);

		//�X�v���C�g�̕ҏW�E�C���h�E�̕\��
		{

			ImGui::Begin("Light");
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(500, 200));

			ImGui::ColorEdit3("ambientColor", ambientColor0_, ImGuiColorEditFlags_Float);
			ImGui::InputFloat3("lightDir0", lightDir0_);
			ImGui::ColorEdit3("lightColor0", lightColor0_, ImGuiColorEditFlags_Float);
			ImGui::InputFloat3("lightDir1", lightDir1_);
			ImGui::ColorEdit3("lightColor1", lightColor1_, ImGuiColorEditFlags_Float);
			ImGui::InputFloat3("lightDir2", lightDir2_);
			ImGui::ColorEdit3("lightColor2", lightColor2_, ImGuiColorEditFlags_Float);

			ImGui::End();
		}


		if(keyTimer_ < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer_ = kWaitTime_;
			}
		}
		else
		{
			keyTimer_--;
		}

		break;

	case END:

		if(keyTimer_ < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer_ = kWaitTime_;
			}
		}
		else
		{
			keyTimer_--;
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
		spriteCommon_->BeforeDraw();
		spriteCommon_->Update();
		title_->Draw("title.png");
		test_->Draw("tomas.png");

		break;

	case GAME:

		//���f���`��
		//sphere_->BeforeDraw();
		//ground_->BeforeDraw();
		//testObject->BeforeDraw();
		Object3d::BeforeDraw();
		
		sphere_->Draw();
		testObject_->Draw();

		for(auto& object : objects_)
		{
			object->Draw();
		}

		//ground_->Draw();
		//triangle_->Draw();
		//ray_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
