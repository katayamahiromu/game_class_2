#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include<stdio.h>
#include"../ResourceManager.h"
#include"Misc.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	resource = ReosurceManager::Instance().LoadModelResource(filename);
	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

//�A�j���[�V�����X�V����
void Model::UpdateAnimation(float elapsedTime)
{
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;
	//�Đ����~�߂Ă���
	if (anime_stop)return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendSeconds > animationBlendTime)
	{
		animationBlendTime += elapsedTime;
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAimationIndex);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int KeyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0;keyIndex < KeyCount - 1;++keyIndex)
	{
		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& Keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& Keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= Keyframe0.seconds &&
			currentAnimationSeconds < Keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - Keyframe0.seconds) /(Keyframe1.seconds - Keyframe0.seconds);
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0;nodeIndex < nodeCount;++nodeIndex)
			{
				if (blendRate < 1.0f)
				{
					//2�̃L�[�t���[���Ԃ̕⊮�v�Z
					const ModelResource::NodeKeyData& key0 = Keyframe0.nodeKeys.at(nodeIndex);
					const ModelResource::NodeKeyData& key1 = Keyframe1.nodeKeys.at(nodeIndex);
					Node& node = nodes[nodeIndex];
					//��] �g�����X���[�g�̌v�Z

					DirectX::XMVECTOR Key0_Pos = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR Key1_Pos = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Key0_Pos, Key1_Pos, blendRate);
					DirectX::XMStoreFloat3(&node.translate, Position);

					DirectX::XMVECTOR Key0_Rot = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR Key1_Rot = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR Rotation = DirectX::XMQuaternionSlerp(Key0_Rot, Key1_Rot, blendRate);
					DirectX::XMStoreFloat4(&node.rotate, Rotation);

				}
				else//�ʏ�̌v�Z
				{
					//2�̃L�[�t���[���Ԃ̕⊮�v�Z
					const ModelResource::NodeKeyData& key0 = Keyframe0.nodeKeys.at(nodeIndex);
					const ModelResource::NodeKeyData& key1 = Keyframe1.nodeKeys.at(nodeIndex);
					Node& node = nodes[nodeIndex];
					//��] �g�����X���[�g�̌v�Z

					DirectX::XMVECTOR Key0_Pos = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR Key1_Pos = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Key0_Pos, Key1_Pos, rate);
					DirectX::XMStoreFloat3(&node.translate, Position);

					DirectX::XMVECTOR Key0_Rot = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR Key1_Rot = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR Rotation = DirectX::XMQuaternionSlerp(Key0_Rot, Key1_Rot, rate);
					DirectX::XMStoreFloat4(&node.rotate, Rotation);
				}
			}
			break;
		}

	}

	//���Ԍo��
	currentAnimationSeconds += elapsedTime;

	//�Đ����Ԃ��I�[���Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		//���[�v���邩
		if (animationLoopFlag) {
			//�Đ����Ԃ������߂�
			currentAnimationSeconds -= animation.secondsLength;
		}
		else {
			currentAimationIndex = -1;
			animationEndFlag = true;
		}
	}
}
//�A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

//�A�j���[�V�����Đ�����
bool Model::IsPlayAnimation()const
{
	if (currentAimationIndex < 0)return false;
	if (currentAimationIndex >= resource->GetAnimations().size())return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{
	//�S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	for (Node node : nodes) {
		for (Node *node_childe : node.children) {
			if (strcmp(node_childe->name,name)==0)
			{
				return node_childe;
			}
		}
	}
	//������Ȃ�����
	return nullptr;
}