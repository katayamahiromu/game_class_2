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


// 初期化
void SceneGame::Initialize()
{
#if true
	select = SceneManager::instance().select;
	StageManager& stageManager = StageManager::Instance();
	
	// プレイヤー初期化
	player = new Player;

	// スイッチ初期化
	Switch* swi = new Switch;
	stageManager.Register(swi);

	//ステージ初期化
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	//StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	//stageManager.Register(stageMoveFloor);

	switch (select)
	{
		// ステージ1
	case 0:
		switchPos = { 10.0f,1.232f,1.502f };
		swi->SetPositon(switchPos);
		
		
		//stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
		//stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
		//stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
		
		player->SetPositon(DirectX::XMFLOAT3(16.035f, 5.233f, 1.502f));

		//エネミー初期化
		for (int i = 0; i < 1; ++i) {
			EnemySlime* slime = new EnemySlime;
			slime->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 10, 5));
			slime->SetTerritory(slime->GetPosition(), 10.0f);
			EnemeyManager::Instance().Register(slime);
		}

		break;

		// ステージ２
	case 1:
		switchPos = { 10.0f,1.232f,1.502f };
		swi->SetPositon(switchPos);

		//StageMoveFloor* stageMoveFloor = new StageMoveFloor();
		//stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
		//stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
		//stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
		//stageManager.Register(stageMoveFloor);
		player->SetPositon(DirectX::XMFLOAT3(16.035f, 5.233f, 1.502f));

		//エネミー初期化
		for (int i = 0; i < 1; ++i) {
			EnemySlime* slime = new EnemySlime;
			slime->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 10, 5));
			slime->SetTerritory(slime->GetPosition(), 10.0f);
			EnemeyManager::Instance().Register(slime);
		}

	player->SetPosition(DirectX::XMFLOAT3(16.035f, 5.233f, 1.502f));
	//カメラコントローラー初期化
	cameraController = new CameraController;

	//カメラ初期設定
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
	//エネミー初期化
#if 1
	for (int i = 0;i < 1;++i) {
		EnemySlime* slime = new EnemySlime;
		slime->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 10, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		EnemeyManager::Instance().Register(slime);
	}
#else
	for(int i= 0;i<50;++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		EnemeyManager::Instance().Register(slime);
	}
#endif
	//ゲージスプライト
	gauge = new Sprite();
	back = new Sprite("Data/Sprite/back.png");

	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = { 0.256f,9.133f,1.502f };
	cameraController->SetTarget(target);
#endif
}

// 終了化
void SceneGame::Finalize()
{
#if true
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	//ステージ終了化
	StageManager::Instance().Clear();

	if(player !=nullptr)
	{
		delete player;
		player = nullptr;
	}

	if (back != nullptr)
	{
		delete back;
		back = nullptr;
	}

	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
	EnemeyManager::Instance().Clear();
#endif
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
#if true
	StageManager::Instance().Update(elapsedTime);
	player->Update(elapsedTime);
	EnemeyManager::Instance().Update(elapsedTime);
	cameraController->Update(elapsedTime);
	//エフェクト更新処理
	EffectManager::Instace().Update(elapsedTime);
#endif
}

// 描画処理
void SceneGame::Render()
{
#if true
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11RasterizerState* rs = graphics.GetRasterizerState();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0〜1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	dc->RSSetState(rs);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）
#endif

#if true
	//カメラのパラメーター設定
	Camera& camera = Camera::Instance();
	rc.view		   = camera.GetView();
	rc.projection  = camera.GetProjection();

	back->Render(dc,
		0, 0, 1280, 720,
		0, 0, 900, 675, 0,
		1, 1, 1, 1);

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);
		player->Render(dc, shader);
		EnemeyManager::Instance().Render(dc, shader);
		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instace().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();
		//エネミーデバッグプリミティブ
		EnemeyManager::Instance().DrawDebugPrimitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2DデバッグGUI描画
	{
		player->DrawDebugGui();
		//EnemeyManager::Instance().GetEnemy(0)->DebugGui();
	}
#endif
}


//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext*dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemeyManager& enemyManager = EnemeyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0;i < enemyCount;++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR ScreenPosition;
		DirectX::XMFLOAT3 Position;
		Position =  enemy->GetPosition();
		Position.y += enemy->GetHeight();
		ScreenPosition = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&Position),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, ScreenPosition);
		gauge->Render(dc,
			pos.x - 50.0f * 0.5f, pos.y - 10.0f,
			10.0f * enemy->GetHealth(), enemy->GetMaxhealth(),
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f,
			1.0f, 0.0f, 0.0f, 0.0f);
	}

	//エネミー配置処理
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//マウスカーソル座標を取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		screenPosition.z = 0.0f;
		DirectX::XMFLOAT3  worldPosition = screenPosition;
		DirectX::XMFLOAT3 WorldEnd = { screenPosition.x, screenPosition.y, 1.0f };

		DirectX::XMVECTOR WorldPosition;
		WorldPosition = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPosition),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		DirectX::XMVECTOR End;
		End = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&WorldEnd),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		//レイを作成してステージに対してレイキャストする
		DirectX::XMStoreFloat3(&worldPosition, WorldPosition);
		DirectX::XMStoreFloat3(&WorldEnd, End);
		HitResult hit;

		if (StageManager::Instance().RayCast(worldPosition, WorldEnd, hit))
		{
			EnemySlime* slime = new EnemySlime;
			slime->SetPositon(hit.position);
			EnemeyManager::Instance().Register(slime);
		}
		
	}

}