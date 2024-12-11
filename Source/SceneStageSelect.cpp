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
	stageNum[0] = std::make_unique<Sprite>("Data/Sprite/stageNum/1.png");
	stageNum[1] = std::make_unique<Sprite>("Data/Sprite/stageNum/2.png");
	stageNum[2] = std::make_unique<Sprite>("Data/Sprite/stageNum/3.png");
	stageNum[3] = std::make_unique<Sprite>("Data/Sprite/stageNum/4.png");
	stageNum[4] = std::make_unique<Sprite>("Data/Sprite/stageNum/5.png");
	stageNum[5] = std::make_unique<Sprite>("Data/Sprite/stageNum/6.png");
	stageNum[6] = std::make_unique<Sprite>("Data/Sprite/stageNum/7.png");
	stageNum[7] = std::make_unique<Sprite>("Data/Sprite/stageNum/8.png");
	stageNum[8] = std::make_unique<Sprite>("Data/Sprite/stageNum/9.png");
	stageNum[9] = std::make_unique<Sprite>("Data/Sprite/stageNum/10.png");
	stageNum[10] = std::make_unique<Sprite>("Data/Sprite/stageNum/11.png");
	stageNum[11] = std::make_unique<Sprite>("Data/Sprite/stageNum/12.png");
	stageNum[12] = std::make_unique<Sprite>("Data/Sprite/stageNum/13.png");
	stageNum[13] = std::make_unique<Sprite>("Data/Sprite/stageNum/14.png");
	stageNum[14] = std::make_unique<Sprite>("Data/Sprite/stageNum/15.png");
	stageNum[15] = std::make_unique<Sprite>("Data/Sprite/stageNum/16.png");
	stageNum[16] = std::make_unique<Sprite>("Data/Sprite/stageNum/17.png");
	stageNum[17] = std::make_unique<Sprite>("Data/Sprite/stageNum/18.png");
	stageNum[18] = std::make_unique<Sprite>("Data/Sprite/stageNum/19.png");
	stageNum[19] = std::make_unique<Sprite>("Data/Sprite/stageNum/20.png");

	BGM = Audio::Instance().LoadAudioSource("Data/Audio/BGM/gentle-wind-ai-201031.wav");
	select_note = Audio::Instance().LoadAudioSource("Data/Audio/SE/choice.wav");
	decide = Audio::Instance().LoadAudioSource("Data/Audio/SE/decide.wav");

	BGM->Play(true);
}

//　終了化
void SceneStageSelect::Finalize()
{

}

// 更新処理
void SceneStageSelect::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX() * -1;

	//ピン移動処理
	if (ax > 0 && selectNum > 0 && ChangeFlg(elapsedTime) == false)
	{
		selectNum -= 1;
		cooltimeFlg = true;
		select_note->DC_Play();
	}
	if (ax < 0 && selectNum < maxStage - 1 && ChangeFlg(elapsedTime) == false)
	{
		selectNum += 1;
		cooltimeFlg = true;
		select_note->DC_Play();
	}

	if (gamePad.GetButtonDown() & GamePad::GamePad::BTN_START)
	{

	}

	//選択中のステージのサイズを大きく
	ChangeSize(elapsedTime);

	//キャラ上下に動かす
	ShiftChara(elapsedTime);

	//　エンターキーを押したらローディングを挟んでゲームシーンへ切り替え
	if (gamePad.GetButtonDown() & GamePad::BTN_A)chengeFlag = true;

	if (chengeFlag && !decide->IsPlay())
	{
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame(selectNum)));
	}

	if (gamePad.GetButtonDown() & GamePad::GamePad::BTN_START)
	{
		SceneManager::instance().ChengeScene(new SceneTitle);
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

		//画面スクロール
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
		for (int i = 0; i < maxStage; i++)
		{
			float textureWidthNum = static_cast<float>(stageNum[i]->GetTextureWidth());
			float textureHeightNum = static_cast<float>(stageNum[i]->GetTextureHeight());

			stageNum[i]->Render(dc,
				screenWidth / 6 * (i + 1) - 120 + scrollScreenWidth, screenHeight / 3 * ((i % 2) + 1) + 20, 700, 150,
				0, 0, textureWidthNum, textureHeightNum,
				0,
				1, 1, 1, 1
			);
			stage->Render(dc,
				screenWidth / 6 * (i + 1) - 100 + scrollScreenWidth, screenHeight / 3 * ((i % 2) + 1), 200, 60,
				0, 0, textureWidthStage, textureHeightStage,
				0,
				scale[i],
				1, 1, 1, 1
			);
		}

		pin->Render(dc,
			screenWidth / 6 * ((selectNum % 5) + 1) - 50, screenHeight / 3 * ((selectNum % 2) + 1) + charaMove - 100, 100, 100,
			0, 0, textureWidthPin, textureHeightPin,
			0,
			1, 1, 1, 1
		);

		//右三角
		if (selectNum < maxStage - 5)
			triangle->Render(dc,
				screenWidth - 100 - charaMove, 300, 100, 100,
				0, 0, textureWidthTriangle, textureHeightTriangle,
				90,
				1, 1, 1, 1
			);
		//左三角
		if (selectNum >= 5)
			triangle->Render(dc,
				0 + charaMove, 300, 100, 100,
				0, 0, textureWidthTriangle, textureHeightTriangle,
				-90,
				1, 1, 1, 1
			);
	}


	// 2DデバッグGUI描画
	{
		DrawDebugGui();
	}
}

void SceneStageSelect::DrawDebugGui()
{
	ImGui::Begin("Select");
	ImGui::SliderInt("Stage Number", &debug_int, 0, 30);
	if (ImGui::Button("go game scene"))
	{
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame(debug_int)));
	}

	ImGui::End();
}

bool SceneStageSelect::ChangeFlg(float elapsedTime)
{
	if (cooltimeFlg == true)
	{
		cooltime += 1.0f * elapsedTime;
	}
	if (cooltime > 0.5f)
	{
		cooltime = 0.0f;
		cooltimeFlg = false;
	}
	return cooltimeFlg;
}

void SceneStageSelect::ShiftChara(float elapsedTime)
{
	if (moveFlg) {
		charaMove += 15.0f * elapsedTime;
		if (charaMove >= 15.0f) {
			moveFlg = false;
		}
	}
	else {
		charaMove -= 15.0f * elapsedTime;
		if (charaMove <= 0.0f) {
			moveFlg = true;
		}
	}
}

void SceneStageSelect::ChangeSize(float elapsedTime)
{
	for (int i = 0; i < maxStage; i++)
	{
		scale[i] = 1.0f;
		if (selectNum == i)
			scale[i] = 1.4f;
	}
}