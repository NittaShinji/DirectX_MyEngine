#pragma once
#include "Object3d.h"

class MeshCollider;

class TouchableObject : public Object3d
{
public:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	//static TouchableObject* Create(const std::string& path);
	static std::unique_ptr<TouchableObject> Create(const std::string& path);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	void Initialize();

	//�R���C�_�[�̒ǉ�
	void AddCollider(Model* model);

	void ColliderUpdate();

	void Draw() override;

private:

	std::unique_ptr<MeshCollider> objMeshCollider_;

protected:

	//����
	std::string attribute_;

};

