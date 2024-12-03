#include"ScenePause.h"
#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneManager.h"


void ScenePause::Initialize()
{
	mask = std::make_unique<Sprite>();
	command[0] = std::make_unique<Sprite>("Data/Sprite/continue.png");
	command[1] = std::make_unique<Sprite>("Data/Sprite/Instructions.png");
	command[2] = std::make_unique<Sprite>("Data/Sprite/stage selection.png");
	back = std::make_unique<Sprite>();
}

void ScenePause::Finalize()
{

}

void ScenePause::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_START) closeFlag = true;

	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)section++;
	else if (gamePad.GetButtonDown() & GamePad::BTN_UP)section--;

	if (section < CONTINNUE) section = STAGE_SELECTION;
	else if (section > STAGE_SELECTION) section = CONTINNUE;
}

void ScenePause::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11RasterizerState* rs = graphics.GetRasterizerState();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	back->Render(dc,
		0, 0, 1280, 720,
		0, 0, 1280, 720, 0,
		1.0f, 1.0f, 1.0f, 1.0f);

	mask->Render(dc,
		0, 0, 1280, 720,
		0, 0, 1280, 720, 0,
		0, 0, 0, alpha);

	/*command[0]->Render(dc,
		45.0f, 0.0f, 1280.0f, 260.0f,
		0.0f, 0.0f, command[0]->GetTextureWidth(),command[0]->GetTextureHeight(), 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);

	command[1]->Render(dc,
		285.0f, 260.0f, 800, 260.0f,
		0.0f, 0.0f, command[1]->GetTextureWidth(), command[1]->GetTextureHeight(), 0.0f,
		1.0f, 1.0f, 1.0f, 0.5f);

	command[2]->Render(dc,
		285, 520.0f, 800, 260.0f,
		0.0f, 0.0f, command[2]->GetTextureWidth(), command[2]->GetTextureHeight(), 0.0f,
		1.0f, 1.0f, 1.0f, 0.5f);*/

	for (int i = 0;i < MAX_COMMAND;++i)
	{
		if (section == i)section_alpha = 1.0f;
		command[i]->Render(dc,
			pos[i].x, pos[i].y, pos[i].z, pos[i].w,
			0.0f, 0.0f, command[i]->GetTextureWidth(), command[i]->GetTextureHeight(), 0.0f,
			1.0f, 1.0f, 1.0f, section_alpha);
		section_alpha = 0.5;
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