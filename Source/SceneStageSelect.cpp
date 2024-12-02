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
	triangle = std::make_unique<Sprite>("Data/Sprite/triangle.png");
	line = std::make_unique<Sprite>("Data/Sprite/line.png");
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
	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT && selectNum < maxstage - 1)
	{
		selectNum += 1;
	}
	//if (selectNum >= 5)
	//	//scrollScreenWidth = -1800;
	//	scrollScreenWidth = -1000;
	//else if (selectNum < 5)
	//	scrollScreenWidth = 0;

	for (int i = 0; i < 10; i++)
	{
		/*scale[i] = 1.0f;
		if (pinPositions[selectNum].x == pinPositions[i].x && pinPositions[selectNum].y == pinPositions[i].y)
			scale[i] = 1.4f;*/
		scale[i] = 1.0f;
		if (selectNum==i)
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
		float textureWidthStage = static_cast<float>(stage->GetTextureWidth());
		float textureHeightStage = static_cast<float>(stage->GetTextureHeight());
		float textureWidthPin = static_cast<float>(pin->GetTextureWidth());
		float textureHeightPin = static_cast<float>(pin->GetTextureHeight());
		float textureWidthTriangle = static_cast<float>(triangle->GetTextureWidth());
		float textureHeightTriangle = static_cast<float>(triangle->GetTextureHeight());
		float textureWidthLine = static_cast<float>(line->GetTextureWidth());
		float textureHeightLine = static_cast<float>(line->GetTextureHeight());

		switch (selectNum / 5)
		{
		case 0:
			scrollScreenWidth = 0;
			break;
		case 1:
			scrollScreenWidth = -screenWidth / 6 * 5;
			break;
		case 2:
			scrollScreenWidth = -screenWidth / 6 * 5 * (selectNum / 5);
			break;
		case 3:
			scrollScreenWidth = -screenWidth / 6 * 5 * (selectNum / 5);
			break;
		}

		//　スプライト描画
		for (int i = 0; i < 10; i++)
		{
			/*line->Render(dc,
				linePositions[i].x + scrollScreenWidth, linePositions[i].y, 200, 60,
				0, 0, textureWidthStage, textureHeightStage,
				angle[i],
				1.3f,
				0, 0, 0, 1
			);*/
			//stage->Render(dc,
			//	stagePositions[i].x + scrollScreenWidth, stagePositions[i].y, 200, 60,//ウィンドウ/６-100でぴったり？
			//	0, 0, textureWidthStage, textureHeightStage,
			//	0,
			//	scale[i],
			//	1, 1, 1, 1
			

			stage->Render(dc,
				screenWidth/6*(i+1)-100 + scrollScreenWidth, screenHeight/3* ((i % 2) + 1), 200, 60,
				0, 0, textureWidthStage, textureHeightStage,
				0,
				scale[i],
				1, 1, 1, 1
			);
		}		

		/*pin->Render(dc,
			pinPositions[selectNum].x, pinPositions[selectNum].y + charaMove, 100, 100,
			0, 0, textureWidthPin, textureHeightPin,
			0,
			1.0f,
			1, 1, 1, 1
		);*/
		pin->Render(dc,
			screenWidth / 6 * ((selectNum % 5) + 1)-50, screenHeight / 3 * ((selectNum % 2) + 1) + charaMove-100, 100, 100,
			0, 0, textureWidthPin, textureHeightPin,
			0,
			1.0f,
			1, 1, 1, 1
		);

		if (selectNum < 5)
			triangle->Render(dc,
				screenWidth-100 - charaMove, 300, 100, 100,
				0, 0, textureWidthTriangle, textureHeightTriangle,
				90,
				1.0f,
				1, 1, 1, 1
			);
		else if (selectNum >= 5)
			triangle->Render(dc,
				0 + charaMove, 300, 100, 100,
				0, 0, textureWidthTriangle, textureHeightTriangle,
				-90,
				1.0f,
				1, 1, 1, 1
			);
	}

	// 2DデバッグGUI描画
	{
	}
}