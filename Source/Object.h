#pragma once

#include <memory>
#include "Graphics/Model.h"
#include "Graphics/Shader.h"

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {}
	void UpdateTransform();

	virtual void Update(float elapsedTime) {};
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) {}

protected:
	DirectX::XMFLOAT3   position = { 0,0,0 };
	DirectX::XMFLOAT3   angle = { 0,0,0 };
	DirectX::XMFLOAT3   scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	std::unique_ptr<Model>model;
};