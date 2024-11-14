#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"
#include"RenderContext.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }

	//�A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTime);

	//�A�j���[�V�����Đ�
	void PlayAnimation(int index,bool loop,float blendSeconds);

	//�A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;

	//�m�[�h����
	Node* FindNode(const char* name);

	//���݂̃A�j���[�V�����Đ�
	float GetCurrentAnimationSeconds() const { return currentAnimationSeconds; }

	void Stop_Anime() { anime_stop = true; }
	void Restart_Anime() { anime_stop = false; }

	//�J���[�O�����f�B���O
	void SetColorGrading(ColorGradingData cgd) { colorGradingData = cgd; }
	const ColorGradingData& GetColorGrading() const { return colorGradingData; }
private:
	ColorGradingData colorGradingData;
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;
	int currentAimationIndex = -1;
	float currentAnimationSeconds = 0.0f;
	bool animationLoopFlag = false;
	bool animationEndFlag = false;
	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;
	bool anime_stop = false;
	bool is_rende = true;
};