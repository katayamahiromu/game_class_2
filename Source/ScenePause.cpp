#include"ScenePause.h"
#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneManager.h"
#include"SceneStageSelect.h"

void ScenePause::Initialize()
{
	mask = std::make_unique<Sprite>();
	oasobi = std::make_unique<Sprite>("Data/Sprite/OASOBI.png");
	command[0] = std::make_unique<Sprite>("Data/Sprite/continue.png");
	command[1] = std::make_unique<Sprite>("Data/Sprite/Instructions.png");
	command[2] = std::make_unique<Sprite>("Data/Sprite/stage selection.png");
	back = std::make_unique<Sprite>();

	select_note = Audio::Instance().LoadAudioSource("Data/Audio/SE/choice.wav");
	decide = Audio::Instance().LoadAudioSource("Data/Audio/SE/decide.wav");
}

void ScenePause::Finalize()
{

}

void ScenePause::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_START) closeFlag = true;

	//移動
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		section++;
		select_note->DC_Play();
	}
	else if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		section--;
		select_note->DC_Play();
	}
	//範囲制限
	if (section < CONTINNUE) section = STAGE_SELECTION;
	else if (section > STAGE_SELECTION) section = CONTINNUE;

	//決定した時のそれぞれの行動
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		decide->DC_Play();

		switch (section)
		{
		case CONTINNUE:
			closeFlag = true;
			break;
		case INSTRUCTION:
			//後々説明用の画像を出す
			break;
		case STAGE_SELECTION:
			chengSceneFlag = true;
			break;
		}
	}

	if (chengSceneFlag && !decide->IsPlay()) SceneManager::instance().ChengeScene(new SceneStageSelect);
}

void ScenePause::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11RasterizerState* rs = graphics.GetRasterizerState();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 0.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	back->Render(dc,
		0, 0, 1280, 720,
		0, 0, 1280, 720, 0,
		1.0f, 1.0f, 1.0f, 1.0f);

	oasobi->Render(dc,
		0, 0, 1280, 187,
		0, 0, 1280, 187, 0,
		1.0f, 1.0f, 1.0f, 1.0f
		);

	mask->Render(dc,
		0, 0, 1280, 720,
		0, 0, 1280, 720, 0,
		0, 0, 0, alpha);

	for (int i = 0;i < MAX_COMMAND;++i)
	{
		section == i ? section_alpha = 1.0f: section_alpha = 0.5;
		command[i]->Render(dc,
			pos[i].x, pos[i].y, pos[i].z, pos[i].w,
			0.0f, 0.0f, command[i]->GetTextureWidth(), command[i]->GetTextureHeight(), 0.0f,
			1.0f, 1.0f, 1.0f, section_alpha);
	}

	DebugGui();
}

void ScenePause::DebugGui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Pause", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);
	}
	ImGui::End();
}


void ScenePause::SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>Srv)
{
	gameSrv = Srv.Get();
	back->SetShaderResourceView(gameSrv, 1280, 720);
}