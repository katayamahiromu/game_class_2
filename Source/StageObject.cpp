#include"StageObject.h"
#include"StageManager.h"
#include"Graphics/Graphics.h"

AppearStage::AppearStage(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale)
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	SetPosition(pos);
	SetScale(scale);
	UpdateTranceform();
	model->UpdateTransform(transform);
}

void AppearStage::Update(float elapsedTime)
{
	StageManager& manger = StageManager::Instance();
	if (manger.GetGoalCount() == manger.GetPushCount())
	{
		isAppear = false;
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


Cube::Cube(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale)
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	SetPosition(pos);
	SetScale(scale);
}

Cube::~Cube()
{
}

void Cube::Update(float elapsedTime)
{
	UpdateTranceform();
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
	ImGui::SliderFloat3("position", &position.x, -16.0f, 16.0f);
	ImGui::SliderFloat3("Scale", &scale.x, 1.0f, 20.0f);
}