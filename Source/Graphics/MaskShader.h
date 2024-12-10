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
	//閾値を送るための定数バッファ用構造体
	struct  CbMask
	{
		float dissolveThreshold;//ディゾルブ量
		float edgThreshold;//緑の閾値
		DirectX::XMFLOAT2 dummy;
		DirectX::XMFLOAT4 edgColor;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> maskConstantBuffer; //定数バッファ

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStaencilSatate;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};