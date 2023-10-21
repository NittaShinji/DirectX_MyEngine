#include "ObjParticleEmitter.h"

void ObjParticleEmitter::Initialize()
{
	isMaxParticle_ = false;
	generationNum_ = 0;
	nowParticleCount_ = 0;
}

void ObjParticleEmitter::Update(Camera* camera)
{
	//寿命が尽きたパーティクルを全削除
	particles_.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	std::size_t size = std::distance(particles_.begin(), particles_.end());
	if(size < kVertexCount)
	{
		isMaxParticle_ = false;
	}
	else
	{
		isMaxParticle_ = true;
	}

	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		it->frame++;
		//進行度を0～1の範囲に換算
		float f = (float)it->frame / it->num_frame;

		//スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;
		it->object3d.SetScale(it->scale);

		it->object3d.Update(camera);
	}
}

void ObjParticleEmitter::Draw()
{
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		it->object3d.Draw();
	}
}

std::unique_ptr<ObjParticleEmitter> ObjParticleEmitter::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<ObjParticleEmitter> instance = nullptr;
	instance = std::make_unique<ObjParticleEmitter>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	return instance;
}

void ObjParticleEmitter::Add(const std::string fileName,int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, const Vector4& colorSpeed, const Vector3& start_scale, const Vector3& end_scale)
{
	int32_t isParticleNum = 0;
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		if(isParticleNum + generationNum_ >= kVertexCount || isParticleNum >= kVertexCount)
		{
			isMaxParticle_ = true;
			break;
		}
		else
		{
			isMaxParticle_ = false;
		}
		isParticleNum++;
	}

	if(isMaxParticle_ == false)
	{
		//リストに要素を追加
		particles_.emplace_front();
		//追加した要素の参照
		Particle& p = particles_.front();
		//値のセット
		p.object3d.Initialize();
		p.object3d.SetModel(fileName);
		p.object3d.SetTransform(position);
		//p.position = position;
		p.velocity = velocity;
		p.accel = accel;
		p.num_frame = life;
		p.object3d.SetScale(start_scale);
		//p.scale = start_scale;
		p.s_scale = start_scale;
		p.e_scale = end_scale;
		p.colorSpeed = colorSpeed;
	}
}

void ObjParticleEmitter::AllRemove()
{
	//パーティクルを全削除
	particles_.clear();
}

