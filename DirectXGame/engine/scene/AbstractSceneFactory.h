#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// �V�[���H��(�T�O)
/// </summary>
class AbstractSceneFactory
{
public:

	//���z�f�X�g���N�^
	virtual ~AbstractSceneFactory() = default;
	//�V�[������
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};
