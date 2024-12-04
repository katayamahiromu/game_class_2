#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneStageSelect.h"
#include"SceneManager.h"
#include"Input/Input.h"
#include"SceneLoading.h"

#include"Camera.h"


//初期化
void SceneTitle::Initialize()
{
	//スプライト初期化
	rogo = std::make_unique<Sprite>("Data/Sprite/Rogo.png");
	press_enter = std::make_unique<Sprite>("Data/Sprite/Press.png");
	press_bar = std::make_unique<Sprite>("Data/Sprite/pressBar.png");
	back = std::make_unique<Sprite>("Data/Sprite/R.jpg");

	test = Audio::Instance().MakeSubMix();
	Cdur = Audio::Instance().LoadAudioSource("Data/Audio/SE.wav");
	Cdur->Set_Submix_voice(test->Get_Submix_Voice());
	test->SetVolum(1.0f);
	test->equalizer();

	// プレイヤー初期化
	player = new Player();

	// カメラ初期化
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
}

//終了化
void SceneTitle::Finalize()
{

}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
	player->TitleUpdate(elapsedTime);

	GamePad& gamePad = Input::Instance().GetGamePad();

	//エンターキーを押したらゲームシーンへの切り替え
	if (gamePad.GetButtonDown() & GamePad::BTN_A) {
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneStageSelect));
		//SceneManager::instance().ChengeScene(new SceneGame);
	}
}

//描画処理
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆レンダーターゲット
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラのパラメーター設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	back->Render(dc,
		0, 0, 1280, 720,
		0, 0, 1024, 768, 0,
		1, 1, 1, 1);

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		// プレイヤー描画
		player->Render(dc, shader);

		shader->End(dc);
	}

	//2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		float textureWidth = static_cast<float>(rogo->GetTextureWidth());
		float textureHeight = static_cast<float>(rogo->GetTextureHeight());
		//　ロゴスプライト描画
		rogo->Render(dc,
			20, 20, 256 * 3, 144 * 1.5,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		float textureWidth1 = static_cast<float>(press_enter->GetTextureWidth());
		float textureHeight1 = static_cast<float>(press_enter->GetTextureHeight());
		//　PressEnterスプライト描画
		press_enter->Render(dc,
			180, 520, 800, 120,
			0, 0, textureWidth1, textureHeight1,
			0,
			1, 1, 1, 1);

		//　press_barスプライト描画
		float textureWidth2 = static_cast<float>(press_bar->GetTextureWidth());
		float textureHeight2 = static_cast<float>(press_bar->GetTextureHeight());
		press_bar->Render(dc,
			235, 520, 800, 120,
			0, 0, textureWidth2, textureHeight2,
			0,
			1, 1, 1, 1);
	}
}