#include "GroundParticle.h"
#include "ImGuiManager.h"
#include "Player.h"

std::unique_ptr<GroundParticle> GroundParticle::Create(std::string fileName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<GroundParticle> instance = nullptr;
	instance = std::make_unique<GroundParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->particleFileName_ = fileName;
	instance->changeColorTimer_ = kChangeColorTime_;
	instance->blackTime_ = kBlackTime_;

	return instance;
}

void GroundParticle::Preparation(Vector3 playerPos, Attribute playerColor, bool isPlayerAxcelled, bool isSlow)
{
	for(int32_t i = 0; i < kOneTimeGenerationNum; i++)
	{
		//半分にするようの変数
		const float divideForHalf = 2.0f;
		//生成時にずれる座標の量
		const float shiftQuantity = 0.2f;

		const float md_pos = 2.0f;
		setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + playerPos.x + imGuiPos_[0] + i * shiftQuantity;
		const float shiftY = -0.8f;
		setPos_.y = playerPos.y + shiftY + imGuiPos_[1];
		const float shiftZ = -1.5f;
		setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / divideForHalf + playerPos.z + imGuiPos_[2] + shiftZ;

		Vector3 acc{};
		if(isSlow == true)
		{
			setVel_.x = imGuiVel_[0];

			const float md_velY = 0.0f;
			setVel_.y = md_velY + imGuiVel_[1];

			const float md_velZ = -0.68f;
			setVel_.z = md_velZ + imGuiVel_[2];

			//重力に見立ててYのみ{-0.001f,0}でランダムに分布
			const float md_acc = 0.0f;
			acc.x = imGuiAcc_[0];
			acc.y = md_acc + imGuiAcc_[1];
		}
		else
		{
			setVel_.x = imGuiVel_[0];

			const float md_velY = 0.174f;
			setVel_.y = md_velY + imGuiVel_[1];

			const float md_velZ = -0.68f;
			setVel_.z = md_velZ + imGuiVel_[2];

			//重力に見立ててYのみ{-0.001f,0}でランダムに分布
			const float md_acc = -0.017f;
			acc.x = imGuiAcc_[0];
			acc.y = md_acc + imGuiAcc_[1];
		}

		//色設定
		if(isPlayerAxcelled == false)
		{
			if(playerColor == Attribute::pink)
			{
				const Vector4 kStartColor = { kColorPinkR,kColorPinkG,kColorPinkB,0.0f + imGuiColor_[3]};
				const Vector4 kEndColor = { 0.0f,0.0f,0.0f,1.0f};
				startColor_ = kStartColor;
				endColor_ = kEndColor;
			}
			else if(playerColor == Attribute::yellow)
			{
				const Vector4 kStartColor = { kColorYellowR,kColorYellowG,kColorYellowB,imGuiColor_[3] };
				const Vector4 kEndColor = { 0.0f,0.0f,0.0f,1.0f };
				startColor_ = kStartColor;
				endColor_ = kEndColor;
			}
		}

		const float md_rotate = 1.0f;
		float rotation = (float)rand() / RAND_MAX * md_rotate;
		const float rotationSpeed = 0.005f;

		//初期ライフ
		const float InitLife = 30.0f;

		//追加
		if(GetIsMaxParticle() == false)
		{
			Add(InitLife, setPos_, setVel_, acc, startColor_, endColor_, colorSpeed_, startScale_, endScale_, rotation, rotationSpeed);
		}
	}
}

void GroundParticle::ImguiUpdate()
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

void GroundParticle::Update(Camera* camera)
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

	if(player_->GetRightAxcell() == true)
	{
		if(blackTime_ > 0)
		{
			blackTime_--;

			redEasing_.endDistance = 0.0f;
			greenEasing_.endDistance = 0.0f;
			blueEasing_.endDistance = 0.0f;

			//白→灰色への最終地点を設定
			endColor_.x = 0.9f;
			endColor_.y = 0.9f;
			endColor_.z = 0.9f;
			endColor_.w = 1.0f;

			//色の変化量を保存
			colorChangeValue_ += colorSpeed_;

			//パーティクルの初期色に変化量を加えグラデーションのように
			startColor_.x = 1.0f - colorChangeValue_.x;
			startColor_.y = 1.0f - colorChangeValue_.y;
			startColor_.z = 1.0f - colorChangeValue_.z;
			startColor_.w = 1.0f;
		}
		else
		{
			blackTime_ = 0;

			//灰色→プレイヤー色への初期化
			if(redEasing_.time == 0.0f)
			{
				//白→灰色への変化途中の色を受け継ぐ
				startColor_.x = std::abs(1.0f - colorChangeValue_.x);
				startColor_.y = std::abs(1.0f - colorChangeValue_.y);
				startColor_.z = std::abs(1.0f - colorChangeValue_.z);

				//色の変化量をリセット
				colorChangeValue_ = { 0.0f,0.0f,0.0f,0.0f };
			}

			//色の変化量を保存
			colorChangeValue_ += colorSpeed_;

			redEasing_.time++;
			greenEasing_.time++;
			blueEasing_.time++;

			if(playerColor_ == pink)
			{
				if(startColor_.x < kColorPinkR)
				{
					startColor_.x += colorSpeed_.x;
				}
				else if(startColor_.x > kColorPinkR)
				{
					startColor_.x -= colorSpeed_.x;
				}
				else {}

				if(startColor_.y < kColorPinkG)
				{
					startColor_.y += colorSpeed_.y;
				}
				else if(startColor_.y >= kColorPinkG)
				{
					startColor_.y -= colorSpeed_.y;
				}
				else {}

				if(startColor_.z < kColorPinkB)
				{
					startColor_.z += colorSpeed_.z;
				}
				else if(startColor_.z > kColorPinkB)
				{
					startColor_.z -= colorSpeed_.z;
				}
				else {}
			}
			else if(playerColor_ == yellow)
			{
				if(startColor_.x < kColorYellowR)
				{
					startColor_.x += colorSpeed_.x;
				}
				else if(startColor_.x > kColorYellowR)
				{
					startColor_.x -= colorSpeed_.x;
				}
				else {}

				if(startColor_.y < kColorYellowG)
				{
					startColor_.y += colorSpeed_.y;
				}
				else if(startColor_.y >= kColorYellowG)
				{
					startColor_.y -= colorSpeed_.y;
				}
				else {}

				if(startColor_.z < kColorYellowB)
				{
					startColor_.z += colorSpeed_.z;
				}
				else if(startColor_.z > kColorYellowB)
				{
					startColor_.z -= colorSpeed_.z;
				}
				else {}
			}

			//パーティクルの初期色に変化量を加えグラデーションのように
			redEasing_.startPos = startColor_.x + colorChangeValue_.x;
			greenEasing_.startPos = startColor_.y + colorChangeValue_.y;
			blueEasing_.startPos = startColor_.z + colorChangeValue_.z;

			//イージング用の距離を計算
			if(playerColor_ == pink)
			{
				redEasing_.endDistance = kColorPinkR + 0.500f - redEasing_.startPos;
				greenEasing_.endDistance = kColorPinkG + 0.300f - greenEasing_.startPos;
				blueEasing_.endDistance = kColorPinkB + 0.500f - blueEasing_.startPos;
			}
			else if(playerColor_ == yellow)
			{
				redEasing_.endDistance = kColorYellowR + 0.500f - redEasing_.startPos;
				greenEasing_.endDistance = kColorYellowG + 0.300f - greenEasing_.startPos;
				blueEasing_.endDistance = kColorYellowB + 0.500f - blueEasing_.startPos;
			}

			//実際に数値を代入
			if(redEasing_.time > 0.0f)
			{
				endColor_.x = PlayEaseInCubic(redEasing_);
			}
			if(greenEasing_.time > 0.0f)
			{
				endColor_.y = PlayEaseInCubic(greenEasing_);
			}
			if(blueEasing_.time > 0.0f)
			{
				endColor_.z = PlayEaseInCubic(blueEasing_);
			}
		}
	}
	else
	{
		colorChangeValue_ = { 0.0f,0.0f,0.0f,0.0f };
		blackTime_ = kBlackTime_;
		redEasing_.time = 0.0f;
		greenEasing_.time = 0.0f;
		blueEasing_.time = 0.0f;
		startColor_ = { 1.0f,1.0f,1.0f,1.0f };
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
			//it->frame++;
			it->frame += freamIncreaseValue_ * gameSpeed_->GetSpeedNum();

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

			//進行度を0～1の範囲に換算
			float f = (float)it->frame / it->num_frame;

			//スケールの線形補間
			it->scale = (it->e_scale - it->s_scale) * f;
			it->scale += it->s_scale;
			it->rotation += it->rotationSpeed;

			vertMap->pos = it->position;
			vertMap->scale = it->scale;
			vertMap->color = it->color;
			vertMap->rotate = it->rotation;

			//次の頂点へ
			vertMap++;
		}

		vertBuff_->Unmap(0, nullptr);
	}

	Matrix4 matRot = MatrixIdentity();
	matRot *= MatrixRotateZ(rotation_);
	rotation_ += 0.01f;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->viewProjection = matView_ * matProjection_;	// 行列の合成
	constMap->matBillboard = matBillboard;
	constBuff_->Unmap(0, nullptr);
}
