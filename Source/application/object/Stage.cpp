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

	blurbackGround_ = std::make_unique<BackGround>();
	normalbackGround_ = std::make_unique<BackGround>();
	blurbackGround_->Initialize("backGround.json",stageEdge_);
	normalbackGround_->Initialize("normalOBJ.json", stageEdge_);
	canResetLoadedStage_ = false;
}

void Stage::Update(Camera* camera, Player* player, GameSpeed* gameSpeed)
{
	//背景オブジェクトを更新
 	blurbackGround_->Update(camera);
	normalbackGround_->Update(camera);

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

	//ミラーステージの更新
	for(auto& mirrorStageObject : mirrorStageObjects_)
	{
		mirrorStageObject->Update(camera);
	}

	//ループオブジェクトの更新
	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		resultRoopStages_[i]->Update(camera, player->GetTransform(), player_->GetCollisionArea());
	}

	//ミラーループオブジェクトの更新
	for(auto& mirrorRoopObject : mirrorRoopObjects_)
	{
		mirrorRoopObject->Update(camera);
	}

	float distance = std::fabs(goal_->GetTransform().z - player->GetTransform().z);
	if(distance < player_->GetCollisionArea())
	{
		goal_->AddCollider(goal_->GetModel());
	}
	goal_->Update(camera, player->GetTransform());

	//ゴールに近づいたらスローにする
	goal_->SlowDownNearGoal(gameSpeed, player_->GetIsFinish(),player_->GetIsDead());
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
			roopStage->SetInitPos(objectData.translation);
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
				std::unique_ptr<MirrorOBJ> newMirrorObject = nullptr;

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
				newObject->SetInitPos(objectData.translation);
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
			newGoal->SetInitPos(objectData.translation);
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
			newWall->SetInitPos(objectData.translation);
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

	//ミラーステージを生成
	for(auto& object : objects_)
	{
		std::unique_ptr<MirrorOBJ> mirrorStage = nullptr;
		mirrorStage = MirrorOBJ::Create(object.get());
		mirrorStage->Initialize();
		mirrorStage->SetInitPos(object.get()->GetInitPos());
		mirrorStageObjects_.push_back(std::move(mirrorStage));
	}

	//ミラーループオブジェクトを生成
	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		std::unique_ptr<MirrorOBJ> mirrorObject = nullptr;
		mirrorObject = MirrorOBJ::Create(resultRoopStages_[i].get());
		mirrorObject->Initialize();
		mirrorObject->SetInitPos(resultRoopStages_[i].get()->GetInitPos());
		mirrorRoopObjects_.push_back(std::move(mirrorObject));
	}
}

void Stage::Draw()
{
	normalbackGround_->Draw();

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
	blurbackGround_->Draw();

	for(auto& mirrorStageObject : mirrorStageObjects_)
	{
		mirrorStageObject->Draw();
	}

	for(auto& mirrorRoopObject : mirrorRoopObjects_)
	{
		mirrorRoopObject->Draw();
	}
}

void Stage::Reset()
{
	//壁を削除
	walls_.clear();

	//ループ関係のオブジェクトをクリア
	//ループステージオブジェクトをクリア
	resultRoopStages_.clear();
	//ループ反射オブジェクトをクリア
	mirrorRoopObjects_.clear();

	//進んだステージから始まるようにステージの端を初期化
	//stageEdge_ = 0.0f;
	//新ステージを読み込む場合
	//リセット時の読み込みの際に位置だけずらしたいので一回だけ読み込む
	if(canResetLoadedStage_ == true)
	{
		for(auto& object : objects_)
		{
			//Vector3 initPos = object->GetInitPos();	
			object->SetTransform(Vector3(object->GetInitPos()));
		}

		for(auto& mirrorStageObject : mirrorStageObjects_)
		{
			/*Vector3 initPos = mirrorStageObject->GetInitPos();*/
			mirrorStageObject->SetTransform(Vector3(mirrorStageObject->GetInitPos()));
		}

		goal_->SetTransform(Vector3(goal_->GetInitPos()));
		goal_->Reset();

		blurbackGround_->ResetPos();
		normalbackGround_->ResetPos();

		/*objects_.clear();
		mirrorStageObjects_.clear();
		goal_.reset();*/

		//Load();
		/*for(auto& resultRoopStage : resultRoopStages_)
		{
			resultRoopStage->SetTransform(Vector3(resultRoopStage->GetInitPos()));
		}
		for(auto& mirrorRoopObject : mirrorRoopObjects_)
		{
			mirrorRoopObject->SetTransform(Vector3(mirrorRoopObject->GetInitPos()));
		}*/

		stageEdge_ = 0.0f;
		canResetLoadedStage_ = false;
	}
	//新たにステージを変えない場合
	else 
	{
		//壁とループオブジェクトを再配置する
		ResetLoad();
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
	//前ステージの背景オブジェクトをクリア
	blurbackGround_->Clear();
	normalbackGround_->Clear();

	//ループ終了
	ResultRoopStage::SetIsFinishedRoopObjects(true);

	float maxPosZ = 0.0f;
	//ループしていたものから一番遠くにあるものを求める
	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		if(i == 0)
		{
			maxPosZ = resultRoopStages_[i]->GetTransform().z;
			stageEdge_ = resultRoopStages_[i]->GetTransform().z + ResultRoopStage::GetObjectRadius();
		}
		else
		{
			if(maxPosZ < resultRoopStages_[i]->GetTransform().z)
			{
				maxPosZ = resultRoopStages_[i]->GetTransform().z;
				stageEdge_ = resultRoopStages_[i]->GetTransform().z + ResultRoopStage::GetObjectRadius();
			}
		}
	}

	//新規ステージ読み込み
	Load();
	blurbackGround_->Initialize("backGround.json",stageEdge_);
	normalbackGround_->Initialize("normalOBJ.json", stageEdge_);
}

void Stage::ResetLoad()
{
	//ステージ番号から読み込み
	std::string stageNumString = std::to_string(stageNum_);
	std::string LoadName = kDefaultStageName_ + stageNumString + ".json";

	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(LoadName);

	int32_t roopObjectCount = 0;
	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		std::unordered_map<std::string, Model> models_;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }

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

	roopObjectCount = 8;
	ResultRoopStage::SetRoopObjectNum(roopObjectCount);

	//読み込み後にミラーオブジェクトを生成
	for(size_t i = 0; i < resultRoopStages_.size(); i++)
	{
		std::unique_ptr<MirrorOBJ> mirrorObject = nullptr;
		mirrorObject = MirrorOBJ::Create(resultRoopStages_[i].get());
		mirrorObject->Initialize();
		mirrorRoopObjects_.push_back(std::move(mirrorObject));
	}

	goal_->Reset();
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
