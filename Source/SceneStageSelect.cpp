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
	stage = std::make_unique<Sprite>("Data/Sprite/-removebg-preview.png");
	pin = std::make_unique<Sprite>("Data/Sprite/pin.jpg");
}

//　終了化
void SceneStageSelect::Finalize()
{
	//スプライト終了化
	//追加
	stage = nullptr;
	pin = nullptr;
}

// 更新処理
void SceneStageSelect::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_LEFT && selectNum > 0)
	{
		selectNum -= 1;
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT && selectNum < 9)
	{
		selectNum += 1;
	}
	if (selectNum >= 5)
		scrollScreenWidth = -1800;
	else if (selectNum < 5)
		scrollScreenWidth = 0;

	for (int i = 0; i < 10; i++)
	{
		scale[i] = 1.0f;
		if (pinPositions[selectNum].x == pinPositions[i].x && pinPositions[selectNum].y == pinPositions[i].y)
			scale[i] = 1.4f;
	}

	if (moveflg) {
		charaMove += 15.0f * elapsedTime; 
		if (charaMove >= 15.0f) {
			moveflg = false;
		}
	}
	else {
		charaMove -= 15.0f * elapsedTime; 
		if (charaMove <= 0.0f) {
			moveflg = true;
		}
	}

	//　エンターキーを押したらローディングを挟んでゲームシーンへ切り替え
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame(selectNum)));
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

		/*float textureWidthBack = static_cast<float>(back->GetTextureWidth());
		float textureHeightBack = static_cast<float>(back->GetTextureHeight());*/

		float textureWidthPin = static_cast<float>(pin->GetTextureWidth());
		float textureHeightPin = static_cast<float>(pin->GetTextureHeight());


		//　スプライト描画
		/*back->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidthBack, textureHeightBack,
			0,
			1, 1, 1, 1);*/

		//追加
		for (int i = 0; i < 10; i++)
		{
			stage->Render(dc,
				stagePositions[i].x + scrollScreenWidth, stagePositions[i].y, 200, 60,
				0, 0, spriteSize.x, spriteSize.y,
				0,
				scale[i],
				1, 1, 1, 1
			);
		}

		pin->Render(dc,
			pinPositions[selectNum].x, pinPositions[selectNum].y + charaMove, 100, 100,
			0, 0, textureWidthPin, textureHeightPin,
			0,
			1.0f,
			1, 1, 1, 1
		);
	}

	// 2DデバッグGUI描画
	{
	}
}