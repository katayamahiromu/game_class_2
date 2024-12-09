#include"StageObject.h"
#include"StageManager.h"
#include"Graphics/Graphics.h"

AppearStage::AppearStage(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale)
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	position = pos;
	this->scale = scale;
	UpdateTransform();
	model->UpdateTransform(transform);
	position.z = 1.70f;
}

void AppearStage::Update(float elapsedTime)
{
	StageManager& manger = StageManager::Instance();
	if (manger.GetGoalCount() == manger.GetPushCount())
	{
		isAppear = false;
	}
	else
	{
		isAppear = true;
	}
}

void AppearStage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (isAppear)
	{
		shader->Draw(dc, model.get());
	}
}

bool AppearStage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	if (isAppear)
	{
		return Collision::InterserctRayVsModel(start, end, model.get(), hit);
	}
	else
	{
		return false;
	}
}

ArrivalStage::ArrivalStage(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	position = pos;
	this->scale = scale;
	UpdateTransform();
	model->UpdateTransform(transform);
	position.z = 1.70f;
}

void ArrivalStage::Update(float elapsedTime)
{
	StageManager& manger = StageManager::Instance();
	if (manger.GetGoalCount() == manger.GetPushCount())
	{
		isAppear = true;
	}
	else
	{
		isAppear = false;
	}
}

void ArrivalStage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (isAppear)
	{
		shader->Draw(dc, model.get());
	}
}

bool ArrivalStage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	if (isAppear)
	{
		return Collision::InterserctRayVsModel(start, end, model.get(), hit);
	}
	else
	{
		return false;
	}
}

Cube::Cube(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale)
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	position = pos;
	this->scale = scale;
}

Cube::~Cube()
{
}

void Cube::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform(transform);
}

void Cube::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model.get());
}

bool Cube::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::InterserctRayVsModel(start, end, model.get(), hit);
}

void Cube::Gui()
{
	ImGui::SliderFloat3("position", &position.x, -16.0f,16.0f);
	ImGui::SliderFloat3("Scale", &scale.x, 1.0f, 40.0f);
}