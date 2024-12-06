#include"StageObject.h"
#include"StageManager.h"

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