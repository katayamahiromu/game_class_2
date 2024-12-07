#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include"Camera.h"
#include"EnemyManeger.h"
#include"EnemySlime.h"
#include"EffectManager.h"
#include"Input/Input.h"
#include"StageManager.h"
#include"StageMain.h"
#include"StageMoveFloor.h"
#include"Goal.h"
#include"switch.h"
#include"I_Love_taisei.h"
#include "Misc.h"
#include"ScenePause.h"
#include"Input/Input.h"


// ������
void SceneGame::Initialize()
{
	StageManager& stageManager = StageManager::Instance();
	
	// �v���C���[������
	player = std::make_unique<Player>(script[select].PlayerPos);
	//�X�e�[�W������
	//�S�[���̃J�E���g���X�C�b�`�̐��Őݒ�
	stageManager.SetGoalCount(script[select].SwitchPosArray.size());
	stageManager.SetGoalPosition(script[select].GoalPos);
	stageManager.Register(new StageMain(script[select].path));
	for (auto pos : script[select].SwitchPosArray)
	{
		stageManager.Register(new Switch(pos));
	}
	stageManager.Register(new Goal(script[select].GoalPos));
	
	//�����I�u�W�F�N�g�̐ݒ�
	for(auto pos: script[select].ObjectPosArray)
	{
		EnemySlime* slime = new EnemySlime;
		slime->SetPosition(pos);
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		EnemeyManager::Instance().Register(slime);
	}

	//�J�����R���g���[���[������
	cameraController = std::make_unique<CameraController>();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 1, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	//�Q�[�W�X�v���C�g
	gauge = std::make_unique<Sprite>();
	back = std::make_unique<Sprite>("Data/Sprite/back.jpg");

	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = { 0.256f,9.133f,1.502f };
	cameraController->SetTarget(target);


	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	ID3D11Device* device = graphics.GetDevice();
	HRESULT hr = { S_OK };
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer{};
	D3D11_TEXTURE2D_DESC texture2d_desc{};
	texture2d_desc.Width = graphics.GetScreenWidth();
	texture2d_desc.Height = graphics.GetScreenHeight();
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2d_desc, NULL, buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	�����_�[�^�[�Q�b�g�r���[����
	hr = device->CreateRenderTargetView(buffer.Get(), NULL, scene_render.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	�V�F�[�_�[���\�[�X�r���[����
	hr = device->CreateShaderResourceView(buffer.Get(), NULL, scene_shader_resource_view.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�|�[�Y�p�̃X�v���C�g�̍쐬
	gameScene = std::make_unique<Sprite>(scene_shader_resource_view);

	//�|�[�Y�̃V�[���̍쐬
	pause = std::make_unique<ScenePause>();
	pause->Initialize();
}

// �I����
void SceneGame::Finalize()
{
	//�X�e�[�W�I����
	StageManager::Instance().Clear();
	EnemeyManager::Instance().Clear();

	//�|�[�Y�̏I��
	pause->Finalize();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	if (PauseFlag)
	{
		pause->Update(elapsedTime);
		ClosePauseCheck();
		return;
	}
	StageManager::Instance().Update(elapsedTime);
	player->Update(elapsedTime);
	EnemeyManager::Instance().Update(elapsedTime);
	cameraController->Update(elapsedTime);
	//�G�t�F�N�g�X�V����
	EffectManager::Instace().Update(elapsedTime);

	Pause();
}

// �`�揈��
void SceneGame::Render()
{
	if (PauseFlag)
	{
		pause->Render();
		return;
	}
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11RasterizerState* rs = graphics.GetRasterizerState();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	dc->RSSetState(rs);

	ObjectRender();

	DebugGui();
}

void SceneGame::ObjectRender()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = lightDirection;	// ���C�g�����i�������j
	rc.ambientColor = ambientLightColor;

	//�J�����̃p�����[�^�[�ݒ�
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();



	back->Render(dc,
		0, 0, 1280, 720,
		0, 0, 4032, 3024, 0,
		1, 1, 1, 1);

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);
		player->Render(dc, shader);
		EnemeyManager::Instance().Render(dc, shader);
		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instace().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();
		//�G�l�~�[�f�o�b�O�v���~�e�B�u
		EnemeyManager::Instance().DrawDebugPrimitive();
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);
	}
}

void SceneGame::Pause()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_START)
	{
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
		ID3D11RenderTargetView* rtv = scene_render.Get();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
		ID3D11RasterizerState* rs = graphics.GetRasterizerState();

		// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);
		dc->RSSetState(rs);

		ObjectRender();

		pause->SetShaderResourceView(scene_shader_resource_view);
		PauseFlag = true;
	}
}

void SceneGame::ClosePauseCheck()
{
	if (pause->GetCloseFlag())
	{
		PauseFlag = false;
		pause->ResetCloseFlag();

	}
}

void SceneGame::DebugGui()
{
	ImGui::Begin("Texture");
	ImGui::Text("scene_texture");
	ImGui::Image(scene_shader_resource_view.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
	ImGui::SliderFloat4("direction", &lightDirection.x, -1.0f, 1.0f);
	ImGui::ColorEdit4("ambient color", &ambientLightColor.x);
	ImGui::End();

	// 2D�f�o�b�OGUI�`��
	{
		player->DrawDebugGui();
		//EnemeyManager::Instance().GetEnemy(0)->DebugGui();
	}
}

void SceneGame::GameSetting()
{
	player->SetPosition(script[select].PlayerPos);

}

//�G�l�~�[HP�Q�[�W�`��
//void SceneGame::RenderEnemyGauge(
//	ID3D11DeviceContext*dc,
//	const DirectX::XMFLOAT4X4& view,
//	const DirectX::XMFLOAT4X4& projection
//)
//{
//	//�r���[�|�[�g
//	D3D11_VIEWPORT viewport;
//	UINT numViewports = 1;
//	dc->RSGetViewports(&numViewports, &viewport);
//
//	//�ϊ��s��
//	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
//	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
//	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
//
//	//�S�Ă̓G�̓����HP�Q�[�W��\��
//	EnemeyManager& enemyManager = EnemeyManager::Instance();
//	int enemyCount = enemyManager.GetEnemyCount();
//
//	for (int i = 0;i < enemyCount;++i)
//	{
//		Enemy* enemy = enemyManager.GetEnemy(i);
//		DirectX::XMVECTOR ScreenPosition;
//		DirectX::XMFLOAT3 Position;
//		Position =  enemy->GetPosition();
//		Position.y += enemy->GetHeight();
//		ScreenPosition = DirectX::XMVector3Project(
//			DirectX::XMLoadFloat3(&Position),
//			viewport.TopLeftX,
//			viewport.TopLeftY,
//			viewport.Width,
//			viewport.Height,
//			0.0f,
//			1.0f,
//			Projection,
//			View,
//			World
//		);
//		DirectX::XMFLOAT3 pos;
//		DirectX::XMStoreFloat3(&pos, ScreenPosition);
//		gauge->Render(dc,
//			pos.x - 50.0f * 0.5f, pos.y - 10.0f,
//			10.0f * enemy->GetHealth(), enemy->GetMaxhealth(),
//			0.0f, 0.0f,
//			0.0f, 0.0f,
//			0.0f,
//			1.0f, 0.0f, 0.0f, 0.0f);
//	}
//
//	//�G�l�~�[�z�u����
//	Mouse& mouse = Input::Instance().GetMouse();
//	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
//	{
//		//�}�E�X�J�[�\�����W���擾
//		DirectX::XMFLOAT3 screenPosition;
//		screenPosition.x = static_cast<float>(mouse.GetPositionX());
//		screenPosition.y = static_cast<float>(mouse.GetPositionY());
//		screenPosition.z = 0.0f;
//		DirectX::XMFLOAT3  worldPosition = screenPosition;
//		DirectX::XMFLOAT3 WorldEnd = { screenPosition.x, screenPosition.y, 1.0f };
//
//		DirectX::XMVECTOR WorldPosition;
//		WorldPosition = DirectX::XMVector3Unproject(
//			DirectX::XMLoadFloat3(&screenPosition),
//			viewport.TopLeftX,
//			viewport.TopLeftY,
//			viewport.Width,
//			viewport.Height,
//			viewport.MinDepth,
//			viewport.MaxDepth,
//			Projection,
//			View,
//			World
//		);
//
//		DirectX::XMVECTOR End;
//		End = DirectX::XMVector3Unproject(
//			DirectX::XMLoadFloat3(&WorldEnd),
//			viewport.TopLeftX,
//			viewport.TopLeftY,
//			viewport.Width,
//			viewport.Height,
//			viewport.MinDepth,
//			viewport.MaxDepth,
//			Projection,
//			View,
//			World
//		);
//		//���C���쐬���ăX�e�[�W�ɑ΂��ă��C�L���X�g����
//		DirectX::XMStoreFloat3(&worldPosition, WorldPosition);
//		DirectX::XMStoreFloat3(&WorldEnd, End);
//		HitResult hit;
//
//		if (StageManager::Instance().RayCast(worldPosition, WorldEnd, hit))
//		{
//			EnemySlime* slime = new EnemySlime;
//			slime->SetPositon(hit.position);
//			EnemeyManager::Instance().Register(slime);
//		}
//		
//	}
//
//}