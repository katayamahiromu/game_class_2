#pragma once
#include<d3d11.h>
#include <DirectXMath.h>

//色調補正情報
struct ColorGradingData
{
	float hueShift = 0;// 色相調整
	float saturation = 1;// 彩度調整
	float brightness = 1;// 明度調整
	float dummy = 0;
};

// レンダーコンテキスト
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;

	ColorGradingData colorGradingData;
};