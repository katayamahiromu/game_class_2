#include "Graphics/Graphics.h"
#include "SceneStageSelect.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"

//　初期化
void SceneStageSelect::Initialize()
{
	//　スプライト初期化
	back = std::make_unique<Sprite>("Data/Sprite/StageSelectBack.png");
	yajirusi = std::make_unique<Sprite>("Data/Sprite/yazirusi.png");
}

//　終了化
void SceneStageSelect::Finalize()
{
	//スプライト終了化
	back = nullptr;
	yajirusi = nullptr;
}

// 更新処理
void SceneStageSelect::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// 矢印の操作
	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
	{
		++select;
		if (select < 0) select = 0;
		if (select > 10) select = 10;
	}
	else if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
	{
		--select;
		if (select < 0) select = 0;
		if (select > 10) select = 10;
	}
	// 矢印の位置
	switch (select)
	{
	case 0:
		selectPos = { 0,600 };
		break;
	case 1:
		selectPos = { 200,800 };
		break;
	}

	//　エンターキーを押したらローディングを挟んでゲームシーンへ切り替え
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		SceneManager::instance().select = select;
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame()));
	}
}

//　描画処理
void SceneStageSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//　画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.99f,0.99f,0.7f,0.0f }; //　RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		float textureWidthBack = static_cast<float>(back->GetTextureWidth());
		float textureHeightBack = static_cast<float>(back->GetTextureHeight());

		float textureWidthYajirusi = static_cast<float>(yajirusi->GetTextureWidth());
		float textureHeightYajirusi = static_cast<float>(yajirusi->GetTextureHeight());


		//　スプライト描画
		back->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidthBack, textureHeightBack,
			0,
			1, 1, 1, 1);

		yajirusi->Render(dc,
			selectPos.x, selectPos.y, 100, 100 * 0.5f,
			0, 0, textureWidthYajirusi, textureHeightYajirusi,
			0,
			1, 1, 1, 1);
	}

	// 2DデバッグGUI描画
	{
	}
}