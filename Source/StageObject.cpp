#include"StageObject.h"
#include"StageManager.h"

ObjectStage::ObjectStage(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale)
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");
	SetPosition(pos);
	SetScale(scale);
	UpdateTranceform();
	model->UpdateTransform(transform);
}

void ObjectStage::Update(float elapsedTime)
{
	StageManager& manger = StageManager::Instance();
	if (manger.GetGoalCount() == manger.GetPushCount())
	{
		isAppear = false;
	}
}

void ObjectStage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	if (isAppear)
	{
		shader->Draw(dc, model.get());
	}
}

bool ObjectStage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
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