#pragma once

#include<memory>
#include<string>
#include<map>
#include"Graphics/ModelResource.h"

//���\�[�X�}�l�[�W���[
class ReosurceManager
{
private:
	ReosurceManager() {};
	~ReosurceManager() {};
public:
	//�B��̃C���X�^���X�擾
	static ReosurceManager& Instance()
	{
		static ReosurceManager instance;
		return instance;
	}

	//���f�����\�[�X�ǂݍ���
	std::shared_ptr<ModelResource>LoadModelResource(const char* filename);
private:
	using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;
	ModelMap models;
};
