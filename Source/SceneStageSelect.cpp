#include "Graphics/Graphics.h"
#include "SceneStageSelect.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"

//�@������
void SceneStageSelect::Initialize()
{
	//�@�X�v���C�g������
	back = std::make_unique<Sprite>("Data/Sprite/StageSelectBack.png");
	yajirusi = std::make_unique<Sprite>("Data/Sprite/yazirusi.png");
}

//�@�I����
void SceneStageSelect::Finalize()
{
	//�X�v���C�g�I����
	back = nullptr;
	yajirusi = nullptr;
}

// �X�V����
void SceneStageSelect::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���̑���
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
	// ���̈ʒu
	switch (select)
	{
	case 0:
		selectPos = { 0,600 };
		break;
	case 1:
		selectPos = { 200,800 };
		break;
	}

	//�@�G���^�[�L�[���������烍�[�f�B���O������ŃQ�[���V�[���֐؂�ւ�
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		SceneManager::instance().select = select;
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame()));
	}
}

//�@�`�揈��
void SceneStageSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//�@��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.99f,0.99f,0.7f,0.0f }; //�@RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		float textureWidthBack = static_cast<float>(back->GetTextureWidth());
		float textureHeightBack = static_cast<float>(back->GetTextureHeight());

		float textureWidthYajirusi = static_cast<float>(yajirusi->GetTextureWidth());
		float textureHeightYajirusi = static_cast<float>(yajirusi->GetTextureHeight());


		//�@�X�v���C�g�`��
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

	// 2D�f�o�b�OGUI�`��
	{
	}
}