#include "Stage.h"
#include "Player.h"
#include "CollisionAttribute.h"
#include <string>

void Stage::Initialize(const std::string& fileName, Player* player)
{
	player_ = player;
	goalPos_ = { 0,0,0 };

	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(fileName);

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		std::unordered_map<std::string, Model> models_;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成
		if(objectData.fileName == "sphere" || objectData.fileName == "skydome" || objectData.fileName == "Plane" || objectData.fileName == "testStage0" || objectData.fileName == "Cube" || objectData.fileName == "StageBlock")
		{
			//3Dオブジェクトの生成
			std::unique_ptr<TouchableObject> newObject = nullptr;
			if(objectData.attribute == "Pink")
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_PINK);
			}
			else if(objectData.attribute == "Yellow")
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_YELLOW);
			}
			else if(objectData.attribute == "Goal")
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_GOAL);
			}
			else
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_BLACK);
			}

			//座標
			Vector3 pos;
			pos = objectData.translation;
			newObject->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newObject->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newObject->SetScale(scale);

			//属性指定
			if(objectData.attribute == "Pink")
			{
				newObject->SetAttributeColor(Attribute::pink);
			}
			else if(objectData.attribute == "Yellow")
			{
				newObject->SetAttributeColor(Attribute::yellow);
			}
			else if(objectData.attribute == "Goal")
			{
				goalPos_ = pos;
				newObject->SetAttributeColor(Attribute::Goal);
			}
			else
			{
				newObject->SetAttributeColor(Attribute::black);
			}

			//コライダーの設定
			float distance = std::fabs(objectData.translation.z - player->GetTransform().z);

			if(distance > player_->GetCollisionArea())
			{
				newObject->RemoveCollider();
			}

			//色を指定
			if(objectData.fileName == "sphere" || objectData.fileName == "testStage0" || objectData.fileName == "Cube" || objectData.fileName == "GoalWall" || objectData.fileName == "StageBlock")
			{
				if(newObject->GetColorFlag() == false)
				{
					newObject->SetColorFlag(true);

					if(newObject->GetAttributeColor() == Attribute::yellow)
					{
						newObject->SetColor(Vector3(1.0f, 0.469f, 0.0f));
					}
					else if(newObject->GetAttributeColor() == Attribute::pink)
					{
						newObject->SetColor(Vector3(0.78f, 0.08f, 0.52f));
					}
					else
					{
						newObject->SetColor(Vector3(0.0f, 0.0f, 0.0f));
					}
				}
				else if(newObject->GetColorFlag() == true)
				{
					newObject->SetColorFlag(false);
				}
			}

			//配列に登録
			objects_.push_back(std::move(newObject));
		}

		if(objectData.fileName == "GoalWall")
		{
			//衝突壁オブジェクトの生成
			std::unique_ptr<GoalOBJ> newGoal = nullptr;
			newGoal = GoalOBJ::Create(objectData.fileName, COLLISION_ATTR_GOAL);
			//座標
			Vector3 pos;
			pos = objectData.translation;
			newGoal->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newGoal->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newGoal->SetScale(scale);

			//コライダーの設定
			float distance = std::fabs(objectData.translation.z - player->GetTransform().z);
			if(distance > player_->GetCollisionArea())
			{
				newGoal->RemoveCollider();
			}

			//その他の初期化
			newGoal->Initialize();
			goalPos_ = pos;


			//色
			newGoal->SetAttributeColor(Attribute::Goal);
			if(newGoal->GetColorFlag() == false)
			{
				newGoal->SetColorFlag(true);
			}

			newGoal->SetColor(Vector3(0.78f, 0.78f, 0.78f));

			//登録
			goal_ = std::move(newGoal);
		}

		if(objectData.fileName == "wall")
		{
			//衝突壁オブジェクトの生成
			std::unique_ptr<HitWall> newWall = nullptr;
			newWall = HitWall::Create(objectData.fileName);
			//座標
			Vector3 pos;
			pos = objectData.translation;
			newWall->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newWall->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newWall->SetScale(scale);

			//コライダーの設定
			float distance = std::fabs(objectData.translation.z - player->GetTransform().z);
			if(distance > player_->GetCollisionArea())
			{
				newWall->RemoveCollider();
			}

			//その他の初期化
			newWall->Initialize();

			//配列に登録
			walls_.push_back(std::move(newWall));
		}
	}
}

void Stage::Update(Camera* camera, Player* player)
{
	for(auto& object : objects_)
	{
		float distance = std::fabs(object->GetTransform().z - player->GetTransform().z);
		if(distance <= player_->GetCollisionArea())
		{
			object->AddCollider(object->GetModel());
		}
		
		object->Update(camera);
	}

	for(size_t i = 0; i < walls_.size(); i++)
	{
		float distance = std::fabs(walls_[i]->GetTransform().z - player->GetTransform().z);
		if(distance < player_->GetCollisionArea())
		{
			walls_[i]->AddCollider(walls_[i]->GetModel());
		}
		walls_[i]->Update(camera, player->GetRightAxcell());

		//壁が壊れていたら削除
		if(walls_[i]->GetIsBreak_())
		{
			walls_.erase(walls_.begin() + i);
			i = (size_t)-1;
		}
	}

	float distance = std::fabs(goal_->GetTransform().z - player->GetTransform().z);
	if(distance < player_->GetCollisionArea())
	{
		goal_->AddCollider(goal_->GetModel());
	}
	goal_->Update(camera, player->GetTransform());
}

void Stage::Draw()
{
	for(auto& object : objects_)
	{
		object->Draw();
	}

	for(auto& wall : walls_)
	{
		wall->Draw();
	}

	goal_->Draw();
}

void Stage::Reset(const std::string& fileName)
{
	//壁を削除
	walls_.clear();

	//壁を再配置
	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(fileName);

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		std::unordered_map<std::string, Model> models_;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }

		if(objectData.fileName == "wall")
		{
			//衝突壁オブジェクトの生成
			std::unique_ptr<HitWall> newWall = nullptr;
			newWall = HitWall::Create(objectData.fileName);

			//座標
			Vector3 pos;
			pos = objectData.translation;
			newWall->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newWall->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newWall->SetScale(scale);

			//コライダーの設定
			float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);
			if(distance > player_->GetCollisionArea())
			{
				newWall->RemoveCollider();
			}
			
			//その他の初期化
			newWall->Initialize();

			//配列に登録
			walls_.push_back(std::move(newWall));
		}
	}
}

void Stage::ColliderUpdate()
{
	
}

std::vector<Vector3> Stage::GetBreakWallsPos()
{
	std::vector<Vector3> BreakWallsPos_;

	for(std::vector<std::unique_ptr<HitWall>>::iterator it = walls_.begin(); it != walls_.end(); it++)
	{
		if(it->get()->GetIsBreak_() == true)
		{
			BreakWallsPos_.push_back(it->get()->GetWallPos());
		}
	}

	return BreakWallsPos_;
}
