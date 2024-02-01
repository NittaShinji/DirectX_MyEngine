#include "Stage.h"
#include "Player.h"
#include "CollisionAttribute.h"
#include "ObjectColor.h"
#include "ImGuiManager.h"
#include "GameSpeed.h"
#include <string>

void Stage::Initialize(Player* player)
{
	kDebugPinkOBJColor_ = kPinkOBJColor;
	kDebugYellowOBJColor_ = kYellowOBJColor;
	player_ = player;
	goalPos_ = { 0,0,0 };
	stageNum_ = 0;
	isClearedAllStage_ = false;
	stageEdge_ = 0.0f;
	//レベルデータからオブジェクトを生成、配置
	Load();
	canResetLoadedStage_ = false;
}

void Stage::Update(Camera* camera, Player* player, GameSpeed* gameSpeed)
{
	for(auto& object : objects_)
	{
		float distance = std::fabs(object->GetTransform().z - player->GetTransform().z);
		if(distance <= player_->GetCollisionArea())
		{
			object->AddCollider(object->GetModel());
		}

		if(object->GetModel()->GetName() == "StageBlock")
		{
			if(object->GetAttributeColor() == Attribute::pink)
			{
				object->SetColor(kDebugPinkOBJColor_);
			}
			else if(object->GetAttributeColor() == Attribute::yellow)
			{
				object->SetColor(kDebugYellowOBJColor_);
			}
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

	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		resultRoopStages_[i]->Update(camera, player->GetTransform(), player_->GetCollisionArea());
	}

	for(auto& mirrorObject : mirrorObjects_)
	{
		mirrorObject->Update(camera);
	}

	float distance = std::fabs(goal_->GetTransform().z - player->GetTransform().z);
	if(distance < player_->GetCollisionArea())
	{
		goal_->AddCollider(goal_->GetModel());
	}
	goal_->Update(camera, player->GetTransform());

	//ゴールに近づいたらスローにする
	goal_->SlowDownNearGoal(gameSpeed, player_->GetIsFinish());
}

void Stage::Load()
{
	std::string stageNumString = std::to_string(stageNum_);
	std::string LoadName = kDefaultStageName_ + stageNumString + ".json";

	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(LoadName);

	int32_t roopObjectCount = 0;

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		std::unordered_map<std::string, Model> models_;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }

		//ループオブジェクト
		if(objectData.attribute == "RoopPink" || objectData.attribute == "RoopYellow")
		{
			std::unique_ptr<ResultRoopStage> roopStage = nullptr;

			if(objectData.fileName == "StageBlock")
			{
				roopStage = ResultRoopStage::Create(objectData.fileName, COLLISION_ATTR_PINK);
				roopObjectCount++;
			}

			//座標
			Vector3 pos;
			pos = objectData.translation;
			pos.z += stageEdge_;
			roopStage->SetTransform(pos);
			roopStage->SetInitTransFormZ(pos.z);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			roopStage->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			roopStage->SetScale(scale);

			//属性指定
			if(objectData.attribute == "RoopPink")
			{
				roopStage->SetAttributeColor(Attribute::pink);
			}
			else if(objectData.attribute == "RoopYellow")
			{
				roopStage->SetAttributeColor(Attribute::yellow);
			}
			else
			{
				roopStage->SetAttributeColor(Attribute::black);
			}

			//コライダーの設定
			float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);

			if(distance > player_->GetCollisionArea())
			{
				roopStage->RemoveCollider();
			}

			//色を指定
			if(roopStage->GetColorFlag() == false)
			{
				roopStage->SetColorFlag(true);

				if(roopStage->GetAttributeColor() == Attribute::yellow)
				{
					roopStage->SetColor(Vector3(kDebugYellowOBJColor_));
				}
				else if(roopStage->GetAttributeColor() == Attribute::pink)
				{
					roopStage->SetColor(Vector3(kDebugPinkOBJColor_));
				}
				else
				{
					roopStage->SetColor(Vector3(kBlackOBJColor));
				}
			}
			else if(roopStage->GetColorFlag() == true)
			{
				roopStage->SetColorFlag(false);
			}

			//配列に登録
			resultRoopStages_.push_back(std::move(roopStage));
		}
		//ループしないオブジェクト
		else
		{
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
				pos.z += stageEdge_;
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
				float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);

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
							newObject->SetColor(Vector3(kDebugYellowOBJColor_));
						}
						else if(newObject->GetAttributeColor() == Attribute::pink)
						{
							newObject->SetColor(Vector3(kDebugPinkOBJColor_));
						}
						else
						{
							newObject->SetColor(Vector3(kBlackOBJColor));
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
		}

		if(objectData.fileName == "GoalWall")
		{
			//衝突壁オブジェクトの生成
			std::unique_ptr<GoalOBJ> newGoal = nullptr;
			newGoal = GoalOBJ::Create(objectData.fileName, COLLISION_ATTR_GOAL);
			//座標
			Vector3 pos;
			pos = objectData.translation;
			pos.z += stageEdge_;
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
			float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);
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
			pos.z += stageEdge_;
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

	roopObjectCount = 8;
	ResultRoopStage::SetRoopObjectNum(roopObjectCount);

	//読み込み後にミラーオブジェクトを生成
	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		std::unique_ptr<MirrorOBJ> mirrorObject = nullptr;
		mirrorObject = MirrorOBJ::Create(resultRoopStages_[i].get());
		mirrorObject->Initialize();
		mirrorObjects_.push_back(std::move(mirrorObject));
	}
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

	for(auto& roopObject : resultRoopStages_)
	{
		roopObject->Draw();
	}

	goal_->Draw();
}

void Stage::MirrorDraw()
{
	for(auto& mirrorObject : mirrorObjects_)
	{
		mirrorObject->Draw();
	}
}

void Stage::Reset()
{
	//壁を削除
	walls_.clear();

	//ステージをクリア
	mirrorObjects_.clear();

	//ループ
	resultRoopStages_.clear();

	//壁を再配置
	std::string stageNumString = std::to_string(stageNum_);
	std::string LoadName = kDefaultStageName_ + stageNumString + ".json";

	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(LoadName);

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

	for(auto& roopObject : resultRoopStages_)
	{
		roopObject->Reset();
	}

	goal_->Reset();

	//進んだステージから始まるようにステージの端を初期化
	stageEdge_ = 0.0f;
	//ステージ読み込み
	//リセット時の読み込みの際に位置だけずらしたいので一回だけ読み込む
	if(canResetLoadedStage_ == true)
	{
		Load();
		canResetLoadedStage_ = false;
	}
}

void Stage::ImguiUpdate()
{
	ImGui::Begin("STAGE");

	const Vector2 kImGuiPos = { 0.0f,50.0f };
	const Vector2 kImGuiSize = { 300.0f,300.0f };

	ImGui::SetWindowPos(ImVec2(kImGuiPos.x, kImGuiPos.y));
	ImGui::SetWindowSize(ImVec2(kImGuiSize.x, kImGuiSize.y));

	const Vector2 kImGuiPosRate = { 0.0f,1.0f };

	ImGui::SliderFloat("PinkColorX", &kDebugPinkOBJColor_.x, kImGuiPosRate.x, kImGuiPosRate.y);
	ImGui::SliderFloat("PinkColorY", &kDebugPinkOBJColor_.y, kImGuiPosRate.x, kImGuiPosRate.y);
	ImGui::SliderFloat("PinkColorZ", &kDebugPinkOBJColor_.z, kImGuiPosRate.x, kImGuiPosRate.y);

	ImGui::SliderFloat("YellowColorX", &kDebugYellowOBJColor_.x, kImGuiPosRate.x, kImGuiPosRate.y);
	ImGui::SliderFloat("YellowColorY", &kDebugYellowOBJColor_.y, kImGuiPosRate.x, kImGuiPosRate.y);
	ImGui::SliderFloat("YellowColorZ", &kDebugYellowOBJColor_.z, kImGuiPosRate.x, kImGuiPosRate.y);

	ImGui::End();
}

void Stage::NextStageUpdate()
{
	//ステージの数を増やして良い場合
	if(isAllowedToCountStageNum_ == true)
	{
		if(stageNum_ < kEndStageNum_)
		{
			stageNum_++;
			canResetLoadedStage_ = true;
		}
	}

	//全てのステージをクリアしたら
	if(stageNum_ >= kEndStageNum_)
	{
		isClearedAllStage_ = true;
	}
}

void Stage::NextStageLoad()
{
	//ループ終了
	ResultRoopStage::SetIsFinishedRoopObjects(true);

	//ボタンを押された際に、プレイヤーの位置からループステージの端を計算する
	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		float distance = std::fabs(resultRoopStages_[i]->GetTransform().z - player_->GetTransform().z);
		//オブジェクトの半径
		if(distance < ResultRoopStage::GetObjectRadius() && i > 0)
		{
			stageEdge_ = resultRoopStages_[i - 1]->GetTransform().z - ResultRoopStage::GetObjectRadius();
		}
		else if(distance < ResultRoopStage::GetObjectRadius() && i == 0)
		{
			int32_t stageMaxNum = ResultRoopStage::GetRoopObjectNum();
			stageEdge_ = resultRoopStages_[stageMaxNum]->GetTransform().z;
		}
	}

	//新規ステージ読み込み
	Load();
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
