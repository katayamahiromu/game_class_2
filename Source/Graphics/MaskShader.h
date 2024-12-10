#pragma once
#include<memory>
#include<wrl.h>
#include"Graphics/Shader.h"
#include"Sprite.h"

class MaskShader
{
public:
	MaskShader(ID3D11Device*device);
	~MaskShader(){};


	void Begin(const RenderContext& rc);
	void Draw(const RenderContext& rc, const Sprite* sprite);
	void End(const RenderContext& rc);
private:
	//臒l�𑗂邽�߂̒萔�o�b�t�@�p�\����
	struct  CbMask
	{
		float dissolveThreshold;//�f�B�]���u��
		float edgThreshold;//�΂�臒l
		DirectX::XMFLOAT2 dummy;
		DirectX::XMFLOAT4 edgColor;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> maskConstantBuffer; //�萔�o�b�t�@

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStaencilSatate;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};