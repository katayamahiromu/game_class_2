//--------------------------------------------
//	ハーフランバート拡散反射計算関数
//--------------------------------------------
// normal		: 法線(正規化済み)
// lightVector	: 入射ベクトル(正規化済み)
// lightColor	: 入射光色
// kd			: 反射率(反射の強さ)
float3 ClacHarfLambert(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	float d = saturate(dot(normal, -lightVector) * 0.5 + 0.5);
	return lightColor * d * kd;
}

//--------------------------------------------
//	フォンの鏡面反射計算関数
//--------------------------------------------
// normal		: 法線(正規化済み)
// lightVector	: 入射ベクトル(正規化済み)
// lightColor	: 入射光色
// eyeVector	: 視線ベクトル(正規化済み)
// shininess	: 光沢度(質感調整値)
// ks			: 反射率(反射の強さ)
// 返す値		: 鏡面反射色
float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor,
	float3 eveVector, float shininess, float3 ks)
{
	//入射ベクトルを法線方向に対して反射させたベクトルを算出
	float3 R = reflect(lightVector, normal);

	//反射ベクトルと視線ベクトルで内積
	//内積した結果を０以下であれば０にする
	float d = max(dot(eveVector, R), 0);

	//光沢処理
	//反射は表面がツルツルなほどハイライトは強くなり、範囲が小さく
	//逆に表面がザラザラなほど、ハイライトは弱くなり、範囲は広がる
	//この変化は指数的に変化するため光沢度を乗数としてしよう

	d = pow(d, shininess);

	//入射光色と内積の結果、及び反射率を全て乗算して返却しましょう
	return lightColor * d * ks;
}

//--------------------------------------------
//	トゥーン拡散反射計算関数
//--------------------------------------------
// toonTexture		: トゥーン用U方向ランプテクスチャ
// toonSamplerState	: トゥーン用サンプラーステート
// normal			: 法線(正規化済み)
// lightVector		: 入射ベクトル(正規化済み)
// lightColor		: 入射光色
// kd				: 反射率(反射の強さ)
// 返す値			: 拡散反射色
float3 CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState, float3 normal,
	float3 lightVector, float3 lightColor, float3 kd)
{
	//法線と入射ベクトルからU座標を求める
	float u = saturate(dot(normal, -lightVector) * 0.5f + 0.5f);

	//トゥーンテクスチャから色を取得する
	float3 c = toonTexture.Sample(toonSamplerState, float2(u, 0.1f));

	return lightColor * c * kd;
}