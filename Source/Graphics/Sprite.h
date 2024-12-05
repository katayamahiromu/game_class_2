#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

// �X�v���C�g
class Sprite
{
public:
	Sprite();
	Sprite(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view);
	Sprite(const char* filename);
	~Sprite();

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// �`����s
	void Render(ID3D11DeviceContext *dc,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// �e�N�X�`�����擾
	int GetTextureWidth() const { return textureWidth; }

	// �e�N�X�`�������擾
	int GetTextureHeight() const { return textureHeight; }

	//�V�F�[�_�[���\�[�X�r���[�̎擾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResourceView.Get(); }
	void SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);

	//���_�V�F�[�_�[�̎擾
	Microsoft::WRL::ComPtr<ID3D11VertexShader>GetVertexShader() { return vertexShader.Get(); }

	//�C���v�b�g���C�A�E�g�̎擾
	Microsoft::WRL::ComPtr<ID3D11InputLayout>GetInputLayout() { return inputLayout; }
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;

	D3D11_TEXTURE2D_DESC texture2d_desc;
	int textureWidth = 0;
	int textureHeight = 0;
	bool isLoadFile = true;
};