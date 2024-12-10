#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"SceneGame.h"

//初期化
void SceneLoading::Initialize()
{
	//スプライトの初期化
	kaiten = std::make_unique<Sprite>("Data/Sprite/Loadingkaiten.png");
	loading = std::make_unique<Sprite>("Data/Sprite/Loading.png");
	back = std::make_unique<Sprite>("Data/Sprite/Teppan.png");
	//スレッド開始
	thread = new std::thread(SceneLoading::LoadingThread, this);
}

//終了化
void SceneLoading::Finalize()
{
	//スレッド終了化
	thread->join();
	if (thread != nullptr)
	{
		delete thread;
		thread = nullptr;
	}
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	//次のシーンが完了したらシーンを切り替える
	if (nextScene->IsReady())
	{
		SceneManager::instance().ChengeScene(nextScene);
	}
}

//描画処理
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆ターゲット設定
	FLOAT color[] = { 0.0f,0.0f,0.0f,0.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//2Dスプライト描画
	{
		//画面右下にローディングアイコン描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		float textureWidth = static_cast<float>(back->GetTextureWidth());
		float textureHeight = static_cast<float>(back->GetTextureHeight());
		float positionX = screenWidth - textureWidth;
		float positionY = screenHeight - textureHeight;

		back->Render(dc,
			positionX,positionY,textureWidth,textureHeight,
			0,0,textureWidth,textureHeight,
			0,
			1,1,1,1
		);

		float textureWidth2 = static_cast<float>(loading->GetTextureWidth());
		float textureHeight2 = static_cast<float>(loading->GetTextureHeight());
		float positionX2 = screenWidth - textureWidth2;
		float positionY2 = screenHeight - textureHeight2;

		loading->Render(dc,
			positionX2, positionY2, textureWidth2, textureHeight2,
			0, 0, textureWidth2, textureHeight2,
			0,
			1, 1, 1, 1
		);

		float textureWidth3 = static_cast<float>(kaiten->GetTextureWidth());
		float textureHeight3 = static_cast<float>(kaiten->GetTextureHeight());
		float positionX3 = screenWidth - textureWidth3;
		float positionY3 = screenHeight - textureHeight3;
		kaiten->Render(dc,
			positionX3, positionY3, textureWidth3, textureHeight3,
			0, 0, textureWidth3, textureHeight3,
			angle,
			1, 1, 1, 1
		);

	}
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	scene->nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetReady();
}