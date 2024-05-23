#include "HitParticle2D.h"
#include "ParticleEmitter.h"
#include "ImGuiManager.h"
#include "Player.h"

std::unique_ptr<HitParticle2D> HitParticle2D::Create(std::string fileName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<HitParticle2D> instance = nullptr;
	instance = std::make_unique<HitParticle2D>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->particleFileName_ = fileName;
	instance->waitingTimeforGeneration_ = kWaitingTime;
	instance->isGenerated_ = false;

	return instance;
}

void HitParticle2D::Preparation(Vector3 playerPos,bool isPlayerDead)
{	
	if(isGenerated_ == false)
	{
		isGenerated_ = true;

		for(int32_t i = 0; i < kOneTimeGenerationNum; i++)
		{
			//重力に見立ててYのみ{-0.001f,0}でランダムに分布
			Vector3 acc{};
			const float md_acc = 0.0f;
			acc.x = imGuiAcc_[0];
			acc.y = md_acc + imGuiAcc_[1];

			//回転
			const float md_rotate = 1.0f;
			float rotation = (float)rand() / RAND_MAX * md_rotate;
			float rotationSpeed = 0.005f;

			//色
			const Vector4 startColor = { 1.0f,1.0f,1.0f,1.0f };
			Vector4 endColor;

			//生成する座標
			const float md_pos = 2.0f;
			//半分にするようの変数
			const float divideForHalf = 2.0f;
			
			if(isPlayerDead == true)
			{
				//生成時にずれる座標の量
				const float shiftQuantity = 0.4f;
				//パーティクルのY座標を生成時にずらす値
				const float shiftY = -0.8f;
				const float shiftZ = 1.5f;

				
				const Vector4 setEndColor = { 1.0f,1.0f,1.0f,0.5f };
				endColor = setEndColor;
				setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + playerPos.x + imGuiPos_[0] + i * shiftQuantity;
				setPos_.y = playerPos.y + shiftY + imGuiPos_[1] - i * shiftQuantity;
				setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + playerPos.z + imGuiPos_[2] + i * shiftQuantity + shiftZ;

				const float velX = 0.1f;
				const float velZ = 0.1f;

				setVel_.x = (float)rand() / RAND_MAX * velX - velX / divideForHalf + imGuiVel_[0];
				setVel_.z = (float)rand() / RAND_MAX * velZ - velZ / divideForHalf + imGuiVel_[2];
			}
			else
			{
				//生成時にずれるパーティクル一つあたりの座標値
				const float shiftQuantity = 0.2f;
				//生成時にずれるパーティクル全体の座標値
				const float shiftY = -0.4f;
				const float shiftZ = 1.5f;
		
				setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + playerPos.x + imGuiPos_[0] + i * shiftQuantity;
				setPos_.y = playerPos.y + shiftY + imGuiPos_[1] - i * shiftQuantity;
				setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + playerPos.z + imGuiPos_[2] - i * shiftQuantity + shiftZ;

				const float md_velX = 0.0f;
				setVel_.x = (float)rand() / RAND_MAX * md_velX - md_velX / divideForHalf + imGuiVel_[0];

				const float md_velY = 0.0f;
				setVel_.y = md_velY + imGuiVel_[1];

				const float md_velZ = 0.0f;
				setVel_.z = (float)rand() / RAND_MAX * md_velZ - md_velZ / divideForHalf + imGuiVel_[2];


				const Vector4 setEndColor = { 1.0f,1.0f,1.0f,0.0f };
				endColor = setEndColor;
			}
			
			
			//色を変化させる
			const Vector4 colorSpeed{ 0.02f,0.02f,0.02f,0.03f };

			//初期ライフ
			const float InitLife = 90.0f;

			//追加
			if(GetIsMaxParticle() == false)
			{
				Add(InitLife, setPos_, setVel_, acc, startColor, endColor, colorSpeed, startScale_, endScale_, rotation, rotationSpeed);
			}
		}
	}

}

void HitParticle2D::ImguiUpdate()
{
	ImGui::Begin("groundParticle");
	ImGui::SetWindowPos(ImVec2(900, 0));
	ImGui::SetWindowSize(ImVec2(300, 100));

	ImGui::SliderFloat3("pos", imGuiPos_, -10.0f, 30.0f);
	ImGui::SliderFloat3("vel", imGuiVel_, -5.0f, 5.0f);
	ImGui::SliderFloat3("acc", imGuiAcc_, -0.05f, 0.05f);
	ImGui::SliderFloat4("color", imGuiColor_, -0.5f, 0.5f);

	ImGui::End();
}

void HitParticle2D::Update(Camera* camera)
{
	HRESULT result;

	//寿命が尽きたパーティクルを全削除
	particles_.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//ビュー変換行列
	matView_ = camera->GetMatView();
	//射影変換行列
	matProjection_ = camera->GetMatProjection();

	//ビルボード行列
	Matrix4 matBillboard = MatrixIdentity();
	matBillboard = camera->GetMatBillboard();

	//頂点バッファへデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);

	nowParticleCount_ = 0;

	if(isGenerated_ == true)
	{
		waitingTimeforGeneration_--;
		if(waitingTimeforGeneration_ < 0)
		{
			isGenerated_ = false;
			waitingTimeforGeneration_ = kWaitingTime;
		}
	}

	if(SUCCEEDED(result))
	{
		for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
		{
			if(nowParticleCount_ + generationNum_ >= kVertexCount || nowParticleCount_ >= kVertexCount)
			{
				isMaxParticle_ = true;
				break;
			}
			else
			{
				isMaxParticle_ = false;
				nowParticleCount_++;
			}

			//速度に加速度を加算
			it->velocity = (it->velocity + it->accel) * gameSpeed_->GetSpeedNum();

			//速度による移動
			it->position = it->position + it->velocity;

			//ゲームスピードに応じてフレームを加算する
			it->frame += freamIncreaseValue_ * gameSpeed_->GetSpeedNum();

			//進行度を0～1の範囲に換算
			float f = (float)it->frame / it->num_frame;

			//スケールの線形補間
			it->scale = (it->e_scale - it->s_scale) * f;
			it->scale += it->s_scale;

			it->rotation += it->rotationSpeed;

			//座標
			vertMap->pos.x = it->position.x;
			vertMap->pos.y = it->position.y;
			vertMap->pos.z = it->position.z;

			//スケール
			vertMap->scale = it->scale;

			//色
			if(it->color.x <= it->endColor.x)
			{
				it->color.x += it->colorSpeed.x * gameSpeed_->GetSpeedNum();
			}
			else if(it->color.x > it->endColor.x)
			{
				it->color.x -= it->colorSpeed.x * gameSpeed_->GetSpeedNum();
			}

			if(it->color.y <= it->endColor.y)
			{
				it->color.y += it->colorSpeed.y * gameSpeed_->GetSpeedNum();
			}
			else if(it->color.y > it->endColor.y)
			{
				it->color.y -= it->colorSpeed.y * gameSpeed_->GetSpeedNum();
			}

			if(it->color.z <= it->endColor.z)
			{
				it->color.z += it->colorSpeed.z * gameSpeed_->GetSpeedNum();
			}
			else if(it->color.z > it->endColor.z)
			{
				it->color.z -= it->colorSpeed.z * gameSpeed_->GetSpeedNum();
			}

			if(it->color.w <= it->endColor.w)
			{
				it->color.w += it->colorSpeed.w * gameSpeed_->GetSpeedNum();
			}
			else if(it->color.w > it->endColor.w)
			{
				it->color.w -= it->colorSpeed.w * gameSpeed_->GetSpeedNum();
			}

			vertMap->color.x = it->color.x;
			vertMap->color.y = it->color.y;
			vertMap->color.z = it->color.z;
			vertMap->color.w = it->color.w;

			vertMap->rotate = it->rotation;

			//次の頂点へ
			vertMap++;
		}

		vertBuff_->Unmap(0, nullptr);
	}

	Matrix4 matRot = MatrixIdentity();
	matRot *= MatrixRotateZ(rotation_);
	rotation_ += kMoveRotateValue_;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->viewProjection = matView_ * matProjection_;	// 行列の合成
	constMap->matBillboard = matBillboard;
	constBuff_->Unmap(0, nullptr);

}

