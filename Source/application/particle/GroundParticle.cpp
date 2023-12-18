#include "GroundParticle.h"
#include "ImGuiManager.h"

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
	instance->changeColorTimer_ = kChangeColorTime;
	instance->blackTime_ = kBlackTime_;

	return instance;
}

void GroundParticle::Preparation(Vector3 playerPos, Attribute playerColor, bool isPlayerAxcelled)
{
	for(int32_t i = 0; i < 3; i++)
	{
		const float md_pos = 2.0f;
		setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.x + imGuiPos_[0] + i * 0.2f;
		const float shiftY = -0.8f;
		setPos_.y = playerPos.y + shiftY + imGuiPos_[1];
		setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.z + imGuiPos_[2];

		setVel_.x = 0.0f + imGuiVel_[0];

		const float md_velY = 0.174f;
		setVel_.y = md_velY + imGuiVel_[1];

		const float md_velZ = -0.68f;
		setVel_.z = md_velZ + imGuiVel_[2];

		//重力に見立ててYのみ{-0.001f,0}でランダムに分布
		Vector3 acc{};
		const float md_acc = -0.017f;
		acc.x = imGuiAcc_[0];
		acc.y = md_acc + imGuiAcc_[1];
		//acc.z = md_acc + imGuiAcc_[2];

		//Vector4 endColor;

		//Vector4 endColor = { 0.0f,0.0f,0.0f,1.0f };
		
		//色設定

		if(isPlayerAxcelled == false)
		{
			if(playerColor == Attribute::pink)
			{
				//color = { kColorPinkR + imGuiColor_[0],kColorPinkG + imGuiColor_[1],kColorPinkB + imGuiColor_[2],0.0f + imGuiColor_[3] };
				endColor_ = { 0.0f,0.0f,0.0f,1.0f };
				startColor_ = { kColorPinkR + 0.500f,kColorPinkG + 0.300f,kColorPinkB + 0.500f,0.0f + imGuiColor_[3] };

			}
			else if(playerColor == Attribute::yellow)
			{
				//color = { kColorYellowR + imGuiColor_[0],kColorYellowG + imGuiColor_[1],kColorYellowB + imGuiColor_[2],0.0f + imGuiColor_[3] };
				endColor_ = { 0.0f,0.0f,0.0f,1.0f };
				startColor_ = { kColorYellowR + 0.500f,kColorYellowG + 0.300f,kColorYellowB + 0.500f,0.0f + imGuiColor_[3] };
			}
		}
		

		if(isPlayerAxcelled == true) 
		{
			//startColor_ = { 1.0f,1.0f,1.0f,1.0f };
		}

		//if(isAxcelled == false)
		//{
		//	endColor = { 0.0f,0.0f,0.0f,1.0f };
		//	if(playerColor == Attribute::pink)
		//	{
		//		//color = { kColorPinkR + imGuiColor_[0],kColorPinkG + imGuiColor_[1],kColorPinkB + imGuiColor_[2],0.0f + imGuiColor_[3] };
		//		startColor = { kColorPinkR + 0.500f,kColorPinkG + 0.300f,kColorPinkB + 0.500f,0.0f + imGuiColor_[3] };

		//	}
		//	else if(playerColor == Attribute::yellow)
		//	{
		//		//color = { kColorYellowR + imGuiColor_[0],kColorYellowG + imGuiColor_[1],kColorYellowB + imGuiColor_[2],0.0f + imGuiColor_[3] };
		//		startColor = { kColorYellowR + 0.500f,kColorYellowG + 0.300f,kColorYellowB + 0.500f,0.0f + imGuiColor_[3] };
		//	}
		//}
		//else
		//{
		//	startColor = { 1.0f,1.0f,1.0f,0.0f + imGuiColor_[3] };
		//	endColor = { 0.0f,0.0f,0.0f,1.0f };
		//}
		//


		//if(playerColor == Attribute::pink)
		//{
		//	/*color = { 0.0f,0.0f,0.0f,0.0f };
		//	endColor = { kColorPinkR,kColorPinkG,kColorPinkB,kColorPinkAlpha };*/
		//}
		//else if(playerColor == Attribute::yellow)
		//{
		//	/*color = { 0.0f,0.0f,0.0f,0.0f };
		//	endColor = { kColorYellowR,kColorYellowG,kColorYellowB,kColorYellowAlpha };*/
		//}

		//color = { 1.0f,1.0f,1.0f,1.0f };

		const float md_rotate = 1.0f;
		float rotation = (float)rand() / RAND_MAX * md_rotate - 0.0f;
		float rotationSpeed = 0.005f;

		//初期ライフ
		const float InitLife = 30.0f;

		//追加
		if(GetIsMaxParticle() == false)
		{
			Add(InitLife, setPos_, setVel_, acc, startColor_, endColor_,  colorSpeed_, startScale_, endScale_,rotation,rotationSpeed);
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

	if(isPlayerAxcelled_ == true)
	{
		if(blackTime_ > 0)
		{
			blackTime_--;

			redEasing_.endDistance = 0.0f;
			greenEasing_.endDistance = 0.0f;
			blueEasing_.endDistance = 0.0f;

			endColor_.x = 0.9f;
			endColor_.y = 0.9f;
			endColor_.z = 0.9f;
			endColor_.w = 1.0f;

			//startColor_ = { 1.0f,1.0f,1.0f,1.0f };

			colorChangeValue_ += colorSpeed_;


			startColor_.x = 1.0f - colorChangeValue_.x;
			startColor_.y = 1.0f - colorChangeValue_.y;
			startColor_.z = 1.0f - colorChangeValue_.z;
			startColor_.w = 1.0f;

			
		}
		else
		{
			blackTime_ = 0;

			
			//初期値
			if(redEasing_.time == 0.0f)
			{	
				startColor_.x = std::abs(1.0f - colorChangeValue_.x);
				startColor_.y = std::abs(1.0f - colorChangeValue_.y);
				startColor_.z = std::abs(1.0f - colorChangeValue_.z);

				colorChangeValue_ = { 0.0f,0.0f,0.0f,0.0f };
				/*endColor_.x = 0.0f;
				endColor_.y = 0.0f;
				endColor_.z = 0.0f;
				endColor_.w = 1.0f;*/

				redEasing_.startPos = startColor_.x;
				greenEasing_.startPos = startColor_.y;
				blueEasing_.startPos = startColor_.z;

				if(playerColor_ == pink)
				{
					colorChangeValue_ += colorSpeed_;

					//redEasing_.startPos = startColor_.x + colorChangeValue_.x;
					//greenEasing_.startPos = startColor_.y + colorChangeValue_.y;
					//blueEasing_.startPos = startColor_.z + colorChangeValue_.z;

					//startColor_.x = - colorChangeValue_.x;
					//startColor_.y = - colorChangeValue_.y;
					//startColor_.z = - colorChangeValue_.z;




					//kColorPinkR + 0.500f, kColorPinkG + 0.300f, kColorPinkB + 0.500f, 0.0f + imGuiColor_[3]
				/*	redEasing_.endDistance = (kColorPinkR + 0.500f);
					greenEasing_.endDistance = (kColorPinkG + 0.300f);
					blueEasing_.endDistance = (kColorPinkB + 0.500f);*/

					//endColor_.x = kColorPinkR + 0.500f;
					//endColor_.y = kColorPinkG + 0.300f;
					//endColor_.z = kColorPinkB + 0.500f;
					//endColor_.w = 1.0f;

					
					redEasing_.endDistance = -(kColorPinkR + 0.500f);
					greenEasing_.endDistance = -(kColorPinkG + 0.300f);
					blueEasing_.endDistance = -(kColorPinkB + 0.500f);
				}
				else if(playerColor_ == yellow)
				{
					colorChangeValue_ += colorSpeed_;

					//kColorYellowR + 0.500f, kColorYellowG + 0.300f, kColorYellowB + 0.500f, 0.0f + imGuiColor_[3]

					/*redEasing_.endDistance = (kColorYellowR + 0.500f);
					greenEasing_.endDistance = (kColorYellowG + 0.300f);
					blueEasing_.endDistance = (kColorYellowB + 0.500f);
					*/

					//endColor_.x = kColorYellowR + 0.500f;
					//endColor_.y = kColorYellowG + 0.300f;
					//endColor_.z = kColorYellowB + 0.500f;
					//endColor_.w = 1.0f;

					//startColor_.x = -colorChangeValue_.x;
					//startColor_.y = -colorChangeValue_.y;
					//startColor_.z = -colorChangeValue_.z;

					//redEasing_.startPos = startColor_.x + colorChangeValue_.x;
					//greenEasing_.startPos = startColor_.y + colorChangeValue_.y;
					//blueEasing_.startPos = startColor_.z + colorChangeValue_.z;

					redEasing_.endDistance = -(kColorYellowR + 0.500f);
					greenEasing_.endDistance = -(kColorYellowG + 0.300f);
					blueEasing_.endDistance = -(kColorYellowB + 0.500f);
				}

				
			}

			/*whiteEasing_.time++;*/
			redEasing_.time++;
			greenEasing_.time++;
			blueEasing_.time++;



			if(redEasing_.time > 0.0f)
			{
				endColor_.x = PlayEaseIn(redEasing_);
			}
			if(greenEasing_.time > 0.0f)
			{
				endColor_.y = PlayEaseIn(greenEasing_);
			}
			if(blueEasing_.time > 0.0f)
			{
				endColor_.z = PlayEaseIn(blueEasing_);
			}
		}
		
		/*changeColorTimer_--;
		if(changeColorTimer_ > 0.0f)
		{
			endColor_.x -= endColorSpeed_;
			endColor_.y -= endColorSpeed_;
			endColor_.z -= endColorSpeed_;
		}*/
	}
	else
	{
		colorChangeValue_ = {0.0f,0.0f,0.0f,0.0f};
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

			//it->velocity = it->velocity + it->accel;
			//速度による移動
			it->position = it->position + it->velocity;

			//it->frame++;
			it->frame += freamIncreaseValue_ * gameSpeed_->GetSpeedNum();

			//進行度を0～1の範囲に換算
			float f = (float)it->frame / it->num_frame;

			//スケールの線形補間
			it->scale = (it->e_scale - it->s_scale) * f;
			it->scale += it->s_scale;

			/*it->color.x = (it->endColor.x - it->startColor.x) * f;
			it->color.y = (it->endColor.y - it->startColor.y) * f;
			it->color.z = (it->endColor.z - it->startColor.z) * f;
			it->color.w = (it->endColor.w - it->startColor.w) * f;

			it->color.x += it->startColor.x;
			it->color.y += it->startColor.y;
			it->color.z += it->startColor.z;
			it->color.w += it->startColor.w;*/

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
	rotation_ += 0.01f;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->viewProjection = matView_ * matProjection_;	// 行列の合成
	constMap->matBillboard = matBillboard;
	constBuff_->Unmap(0, nullptr);
}

void GroundParticle::SetIsPlayerColor(Attribute playerColor)
{
	playerColor_ = playerColor;
}
