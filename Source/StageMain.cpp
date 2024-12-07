#include"StageMain.h"
#include "SceneManager.h"
#include"Scene.h"

StageMain::StageMain(char* filename)
{
	model = std::make_unique<Model>(filename);
	scale.x = scale.y = scale.z = 0.01f;
	UpdateTranceform();
	model->UpdateTransform(transform);
}

StageMain::~StageMain() {
	
}

void StageMain::Update(float elapsedTime) {

}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader) {
	shader->Draw(dc, model.get());
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::InterserctRayVsModel(start, end, model.get(), hit);
}