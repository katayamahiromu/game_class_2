#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneStageSelect.h"
#include"SceneManager.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include "Framework.h"

//������
void SceneTitle::Initialize()
{
	//�X�v���C�g������
	sprite = new Sprite("Data/Sprite/Title.png");
	start = new Sprite("Data/Sprite/start.png");
	end = new Sprite("Data/Sprite/end.png");
	test = Audio::Instance().MakeSubMix();
	Cdur = Audio::Instance().LoadAudioSource("Data/Audio/SE.wav");
	Cdur->Set_Submix_voice(test->Get_Submix_Voice());
	test->SetVolum(1.0f);
	test->equalizer();
	//Cdur->Play(false);
}

//�I����
void SceneTitle::Finalize()
{
	//�X�v���C�g�I����
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	//�ǉ�
	if (start != nullptr)
	{
		delete start;
		start = nullptr;
	}
	if (end != nullptr)
	{
		delete end;
		end = nullptr;
	}
}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	
	SceneManager::instance().SetEndFlg(finalizeflg);

	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		if (scale[0] == 1.0f)
		{
			scale[0] = 1.4f;
			scale[1] = 1.0f;
			finalizeflg = false;
		}
		else if (scale[1] == 1.0f)
		{
			scale[1] = 1.4f;
			scale[0] = 1.0f;
			finalizeflg = true;
		}
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		if (scale[0] == 1.0f)
		{
			scale[0] = 1.4f;
			scale[1] = 1.0f;
			finalizeflg = false;
		}
		else if (scale[1] == 1.0f)
		{
			scale[1] = 1.4f;
			scale[0] = 1.0f;
			finalizeflg = true;
		}
	}

	//�Ȃɂ��{�^������������Q�[���V�[���ւ̐؂�ւ�
	const GamePadButton anyButton =
		  GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;
	if (gamePad.GetButtonDown() & anyButton&& finalizeflg ==false) {
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneStageSelect));
		//SceneManager::instance().ChengeScene(new SceneGame);
	}
}

//�`�揈��
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//��ʃN���A�������_�[�^�[�Q�b�g
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(sprite->GetTextureWidth());
		float textureHeight = static_cast<float>(sprite->GetTextureHeight());
		float textureWidthStart = static_cast<float>(start->GetTextureWidth());
		float textureHeightStart = static_cast<float>(start->GetTextureHeight());
		float textureWidthEnd = static_cast<float>(end->GetTextureWidth());
		float textureHeightEnd = static_cast<float>(end->GetTextureHeight());

		//�^�C�g���X�v���C�g
		sprite->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1,
			1, 1, 1, 1
		);
		start->Render(dc,
			screenWidth / 2 - spritesize.x / 2, screenHeight / 10 * 6, spritesize.x, spritesize.y,
			0, 0, textureWidthStart, textureHeightStart,
			0,
			scale[0],
			1, 1, 1, 1
		);
		end->Render(dc,
			screenWidth / 2 - spritesize.x / 2, screenHeight / 10 * 8, spritesize.x, spritesize.y,
			0, 0, textureWidthEnd, textureHeightEnd,
			0,
			scale[1],
			1, 1, 1, 1
		);
	}
}