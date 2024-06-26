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
	isPlayerReachedStageEdge_ = false;
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

	//ステージオブジェクトを更新
 	for(auto& stageBlock : stageBlocks_)
	{
		float distance = std::fabs(stageBlock->GetTransform().z - player->GetTransform().z);
		if(distance <= player_->GetCollisionArea())
		{
			stageBlock->AddCollider(stageBlock->GetModel());
		}

		if(stageBlock->GetModel()->GetName() == "StageBlock")
		{
			if(stageBlock->GetAttributeColor() == Attribute::pink)
			{
				stageBlock->SetColor(kDebugPinkOBJColor_);
			}
			else if(stageBlock->GetAttributeColor() == Attribute::yellow)
			{
				stageBlock->SetColor(kDebugYellowOBJColor_);
			}
		}

		stageBlock->Update(camera);
	}

	//壁を更新
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
		//resultRoopStages_[i]->Initialize();
 		resultRoopStages_[i]->Update(camera, player->GetTransform(), player_->GetCollisionArea());
	}

	//ミラーループオブジェクトの更新
	for(auto& mirrorRoopObject : mirrorRoopObjects_)
	{
		mirrorRoopObject->Update(camera);
	}

	//ゴールを更新
	float distance = std::fabs(goal_->GetTransform().z - player->GetTransform().z);
	if(distance < player_->GetCollisionArea())
	{
		goal_->AddCollider(goal_->GetModel());
	}
	goal_->Update(camera, player->GetTransform());

	//ゴールに近づいたらスローにする
	goal_->SlowDownNearGoal(gameSpeed, player_->GetIsFinish(),player_->GetIsDead());

	if(player->GetTransform().z > stageEdge_)
	{
		isPlayerReachedStageEdge_ = true;
	}
}

void Stage::Load()
{
	//ステージがクリア数に到達していなければ読み込む
	if (stageNum_ < kClearStageNum_)
	{
		std::string stageNumString = std::to_string(stageNum_);
		std::string LoadName = kDefaultStageName_ + stageNumString + ".json";
		levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(LoadName);

		int32_t roopObjectCount = 0;

		for (auto& objectData : levelData_->objects)
		{
			//ファイル名から登録済みモデルを検索
			Model* model = nullptr;
			std::unordered_map<std::string, Model> models_;
			decltype(models_)::iterator it = models_.find(objectData.fileName);
			if (it != models_.end()) { model = &it->second; }

			//ループオブジェクト
			if (objectData.attribute == "RoopPink" || objectData.attribute == "RoopYellow")
			{
				std::unique_ptr<ResultRoopStage> roopStage = nullptr;

				if (objectData.fileName == "StageBlock")
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

				//ステージ数をセット
				roopStage->SetStageNum(stageNum_);

				//属性指定
				if (objectData.attribute == "RoopPink")
				{
					roopStage->SetAttributeColor(Attribute::pink);
				}
				else if (objectData.attribute == "RoopYellow")
				{
					roopStage->SetAttributeColor(Attribute::yellow);
				}
				else
				{
					roopStage->SetAttributeColor(Attribute::black);
				}

				//コライダーの設定
				float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);

				if (distance > player_->GetCollisionArea())
				{
					roopStage->RemoveCollider();
				}

				//色を指定
				if (roopStage->GetColorFlag() == false)
				{
					roopStage->SetColorFlag(true);

					if (roopStage->GetAttributeColor() == Attribute::yellow)
					{
						roopStage->SetColor(Vector3(kDebugYellowOBJColor_));
					}
					else if (roopStage->GetAttributeColor() == Attribute::pink)
					{
						roopStage->SetColor(Vector3(kDebugPinkOBJColor_));
					}
					else
					{
						roopStage->SetColor(Vector3(kBlackOBJColor));
					}
				}
				else if (roopStage->GetColorFlag() == true)
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
				if (objectData.fileName == "StageBlock")
				{
					//3Dオブジェクトの生成
					std::unique_ptr<StageBlock> newStageBlock = nullptr;

					std::unique_ptr<MirrorOBJ> newMirrorObject = nullptr;

					if (objectData.attribute == "Pink")
					{
						newStageBlock = StageBlock::Create(objectData.fileName, COLLISION_ATTR_PINK);
					}
					else if (objectData.attribute == "Yellow")
					{
						newStageBlock = StageBlock::Create(objectData.fileName, COLLISION_ATTR_YELLOW);
					}
					else if (objectData.attribute == "Goal")
					{
						newStageBlock = StageBlock::Create(objectData.fileName, COLLISION_ATTR_GOAL);
					}
					else
					{
						newStageBlock = StageBlock::Create(objectData.fileName, COLLISION_ATTR_BLACK);
					}

					//座標
					newStageBlock->SetInitPos(objectData.translation);
					Vector3 pos;
					pos = objectData.translation;
					pos.z += stageEdge_;
					newStageBlock->SetTransform(pos);

					//回転角
					Vector3 rot;
					rot = objectData.rotation;
					newStageBlock->SetRotation(rot);

					//大きさ
					Vector3 scale;
					scale = objectData.scaling;
					newStageBlock->SetScale(scale);

					//読み込んだステージのNoをセット
					newStageBlock->SetStageNum(stageNum_);

					//属性指定
					if (objectData.attribute == "Pink")
					{
						newStageBlock->SetAttributeColor(Attribute::pink);
					}
					else if (objectData.attribute == "Yellow")
					{
						newStageBlock->SetAttributeColor(Attribute::yellow);
					}
					else if (objectData.attribute == "Goal")
					{
						goalPos_ = pos;
						newStageBlock->SetAttributeColor(Attribute::Goal);
					}
					else
					{
						newStageBlock->SetAttributeColor(Attribute::black);
					}

					//コライダーの設定
					float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);

					if (distance > player_->GetCollisionArea())
					{
						newStageBlock->RemoveCollider();
					}

					//色を指定
					if (objectData.fileName == "StageBlock")
					{
						if (newStageBlock->GetColorFlag() == false)
						{
							newStageBlock->SetColorFlag(true);

							if (newStageBlock->GetAttributeColor() == Attribute::yellow)
							{
								newStageBlock->SetColor(Vector3(kDebugYellowOBJColor_));
							}
							else if (newStageBlock->GetAttributeColor() == Attribute::pink)
							{
								newStageBlock->SetColor(Vector3(kDebugPinkOBJColor_));
							}
							else
							{
								newStageBlock->SetColor(Vector3(kBlackOBJColor));
							}
						}
						else if (newStageBlock->GetColorFlag() == true)
						{
							newStageBlock->SetColorFlag(false);
						}
					}

					//配列に登録
					stageBlocks_.push_back(std::move(newStageBlock));
				}
			}

			if (objectData.fileName == "GoalWall")
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
				if (distance > player_->GetCollisionArea())
				{
					newGoal->RemoveCollider();
				}

				//その他の初期化
				newGoal->Initialize();
				goalPos_ = pos;


				//色
				newGoal->SetAttributeColor(Attribute::Goal);
				if (newGoal->GetColorFlag() == false)
				{
					newGoal->SetColorFlag(true);
				}

				newGoal->SetColor(Vector3(0.78f, 0.78f, 0.78f));

				//登録
				goal_ = std::move(newGoal);
			}

			if (objectData.fileName == "wall")
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
				if (distance > player_->GetCollisionArea())
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
		for (auto& stageBlock : stageBlocks_)
		{
			std::unique_ptr<MirrorOBJ> mirrorStage = nullptr;
			mirrorStage = MirrorOBJ::Create(stageBlock.get());
			mirrorStage->Initialize();
			mirrorStage->SetInitPos(stageBlock.get()->GetInitPos());
			mirrorStage->SetStageNum(stageNum_);
			mirrorStageObjects_.push_back(std::move(mirrorStage));
		}

		//ミラーループオブジェクトを生成
		for (size_t i = 0; i < resultRoopStages_.size(); i++)
		{
			std::unique_ptr<MirrorOBJ> mirrorObject = nullptr;
			mirrorObject = MirrorOBJ::Create(resultRoopStages_[i].get());
			mirrorObject->Initialize();
			mirrorObject->SetInitPos(resultRoopStages_[i].get()->GetInitPos());
			mirrorObject->SetStageNum(stageNum_);
			mirrorRoopObjects_.push_back(std::move(mirrorObject));
		}
	}
}

void Stage::Draw()
{
	normalbackGround_->Draw();

	for(auto& stageBlock : stageBlocks_)
	{
		stageBlock->Draw();
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

	//リセット時の読み込みの際に位置だけずらしたいので一回だけ読み込む
	if(canResetLoadedStage_ == true)
	{
		//ステージの配置を移動
		for(auto& stageBlock : stageBlocks_)
		{	
			stageBlock->SetTransform(Vector3(stageBlock->GetInitPos()));
		}

		for(auto& mirrorStageObject : mirrorStageObjects_)
		{
			mirrorStageObject->SetTransform(Vector3(mirrorStageObject->GetInitPos()));
		}

		goal_->SetTransform(Vector3(goal_->GetInitPos()));
		goal_->Reset();

		blurbackGround_->ResetPos();
		normalbackGround_->ResetPos();

		stageEdge_ = 0.0f;
		canResetLoadedStage_ = false;

		ResetLoad();
	}
	//新ステージをずらし終えた場合
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
		if(stageNum_ < kClearStageNum_)
		{
			stageNum_++;
			canResetLoadedStage_ = true;
		}
	}
	//全てのステージをクリアしたら
	if(stageNum_ >= kClearStageNum_)
	{
		isClearedAllStage_ = true;
	}
}

void Stage::NextStageLoad()
{
	//前ステージの背景オブジェクトを削除
	blurbackGround_->Clear();
	normalbackGround_->Clear();

	//前ステージのステージオブジェクトを削除
	int32_t stageBeforeBlockCount_ = 0;
	for(auto& stageBlock : stageBlocks_)
	{
		//現在のステージ以外のステージブロックを数える
		if(stageBlock->GetStageNum() != stageNum_)
		{
			stageBeforeBlockCount_++;
		}
	}
	//現在のステージで使用しないステージブロックを削除
	stageBlocks_.erase(stageBlocks_.begin(), stageBlocks_.begin() + stageBeforeBlockCount_);

	int32_t mirrorStageBeforeBlockCount_ = 0;
	for(auto& mirrorStageObject : mirrorStageObjects_)
	{
		//現在のステージ以外のステージブロックを削除
		if(mirrorStageObject->GetStageNum() != stageNum_)
		{
			mirrorStageBeforeBlockCount_++;
		}
		else
		{
			mirrorStageObject->SetTransform(Vector3(mirrorStageObject->GetInitPos()));
		}
	}

	//現在のステージで使用しないステージブロックを削除
	mirrorStageObjects_.erase(mirrorStageObjects_.begin(), mirrorStageObjects_.begin() + mirrorStageBeforeBlockCount_);

	int32_t roopStageBeforeBlockOBJ = 0;
	const int32_t deleteStageNum = 2;
	for(auto& resultRoopStage_ : resultRoopStages_)
	{
		//現在のステージ以外のステージブロックを削除
		if(stageNum_ - resultRoopStage_->GetStageNum() >= deleteStageNum)
		{
			roopStageBeforeBlockOBJ++;
		}
	}

	if(roopStageBeforeBlockOBJ > 0)
	{
		resultRoopStages_.erase(resultRoopStages_.begin(), resultRoopStages_.begin() + roopStageBeforeBlockOBJ);
	}

	int32_t mirrorRoopStageOBJCount = 0;
	for(auto& mirrorRoopObject : mirrorRoopObjects_)
	{
		//現在のステージ以外のステージブロックを削除
		if(stageNum_ - mirrorRoopObject->GetStageNum() >= deleteStageNum)
		{
			mirrorRoopStageOBJCount++;
		}
	}

	if(mirrorRoopStageOBJCount > 0)
	{
		mirrorRoopObjects_.erase(mirrorRoopObjects_.begin(), mirrorRoopObjects_.begin() + mirrorRoopStageOBJCount);
	}
	
	//ループ終了
	ResultRoopStage::SetIsFinishedRoopObjects(true);

	float maxPosZ = 0.0f;
	isPlayerReachedStageEdge_ = false;
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
	//ステージがクリア数に到達していなければ読み込む
	if (stageNum_ < kClearStageNum_)
	{
		//ステージ番号から読み込み
		std::string stageNumString = std::to_string(stageNum_);
		std::string LoadName = kDefaultStageName_ + stageNumString + ".json";

		//レベルデータからオブジェクトを生成、配置
		levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(LoadName);

		int32_t roopObjectCount = 0;
		for (auto& objectData : levelData_->objects)
		{
			//ファイル名から登録済みモデルを検索
			Model* model = nullptr;
			std::unordered_map<std::string, Model> models_;
			decltype(models_)::iterator it = models_.find(objectData.fileName);
			if (it != models_.end()) { model = &it->second; }

			if (objectData.attribute == "RoopPink" || objectData.attribute == "RoopYellow")
			{
				std::unique_ptr<ResultRoopStage> roopStage = nullptr;

				if (objectData.fileName == "StageBlock")
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
				if (objectData.attribute == "RoopPink")
				{
					roopStage->SetAttributeColor(Attribute::pink);
				}
				else if (objectData.attribute == "RoopYellow")
				{
					roopStage->SetAttributeColor(Attribute::yellow);
				}
				else
				{
					roopStage->SetAttributeColor(Attribute::black);
				}

				//コライダーの設定
				float distance = std::fabs(objectData.translation.z - player_->GetTransform().z);

				if (distance > player_->GetCollisionArea())
				{
					roopStage->RemoveCollider();
				}

				//色を指定
				if (roopStage->GetColorFlag() == false)
				{
					roopStage->SetColorFlag(true);

					if (roopStage->GetAttributeColor() == Attribute::yellow)
					{
						roopStage->SetColor(Vector3(kDebugYellowOBJColor_));
					}
					else if (roopStage->GetAttributeColor() == Attribute::pink)
					{
						roopStage->SetColor(Vector3(kDebugPinkOBJColor_));
					}
					else
					{
						roopStage->SetColor(Vector3(kBlackOBJColor));
					}
				}
				else if (roopStage->GetColorFlag() == true)
				{
					roopStage->SetColorFlag(false);
				}

				//配列に登録
				resultRoopStages_.push_back(std::move(roopStage));
			}

			if (objectData.fileName == "wall")
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
				if (distance > player_->GetCollisionArea())
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
		for (size_t i = 0; i < resultRoopStages_.size(); i++)
		{
			std::unique_ptr<MirrorOBJ> mirrorObject = nullptr;
			mirrorObject = MirrorOBJ::Create(resultRoopStages_[i].get());
			mirrorObject->Initialize();
			mirrorRoopObjects_.push_back(std::move(mirrorObject));
		}

		goal_->Reset();
	}
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
