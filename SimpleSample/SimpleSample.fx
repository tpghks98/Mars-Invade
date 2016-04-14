//--------------------------------------------------------------------------------------
// File: SimpleSample.fx
//
// The effect file for the SimpleSample sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float3 g_LightDir[8];                  // Light's direction in world space
texture g_MeshTexture;              // Color texture for mesh
texture g_BumpTexture;
float3 g_vViewDir;
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
float3 g_vCameraPos;
float4x4 g_mInvWorldView;
float4x4 g_mProj;    // World * View * Projection matrix

float	g_fAlpha;
float   g_fHitColor;
bool	g_IsBump;
bool	g_IsMap = false;
bool	g_IsDiffuse = false;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
	AddressU = Clamp;
	AddressV = Clamp;
	AddressW = Clamp;
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
sampler BumpTextureSampler =
sampler_state
{
	Texture = <g_BumpTexture>;

};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
	float3 Rim		  : TEXCOORD1;
	float3 Reflect	  : TEXCOORD2;
};


struct OUT_LINE_VS_OUTPUT
{
	float4 Position   : POSITION;   // vertex position 
	float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
	float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};
struct OUT_LINE_PS_OUTPUT
{
	float4 RGBColor : COLOR0;  // Pixel color    
};


OUT_LINE_VS_OUTPUT OUT_LINE_VS(float4 Position : POSITION,
	float3 Normal : NORMAL,
	float2 mTexCoord : TEXCOORD0)
{
	OUT_LINE_VS_OUTPUT Output;
	Output.Position = mul(Position, g_mWorldViewProjection);

	float3 normal = normalize( mul(Normal, (float3x3)g_mInvWorldView) );

	normal = normalize(mul(normal, (float3x3)g_mProj));

	float fLen = 0.005f;

	Output.Position.xy += fLen * Output.Position.z * normal.xy;
	Output.Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Output.TextureUV = mTexCoord;

	return(Output);
}
OUT_LINE_PS_OUTPUT OUT_LINE_PS(VS_OUTPUT Output)
{
	OUT_LINE_PS_OUTPUT Out;
	Out.RGBColor = tex2D(MeshTextureSampler, Output.TextureUV);
	Out.RGBColor.rgb = Output.Diffuse.rgb;

	return Out;
}
//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION,
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;

	
    
	float3	vWorldPosition = mul(vPos.xyz, (float3x3)g_mWorld);
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)

	
	float3 f = normalize(g_vCameraPos - vWorldPosition.xyz);
	float t = length(f) ;

	float3 vViewDir = normalize(g_vCameraPos.xyz - Output.Position.xyz);

	float3 rgb = 0;
	float3 vRim = 0;


//		rgb = max(rgb, dot(vNormalWorldSpace, normalize(f)));
		rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(0, -1, 0))));
		rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(0, 1, 0))));
		rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(0, 1, -1))));
		rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(0, 1, 1))));
		rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(1, 0, 0))));
		rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(-1, 0, 0))));

		rgb = rgb ;
//	rgb = (rgb + vPos.a) / (1 + vPos.a);

	Output.Diffuse.a = 1.0f;

	Output.Diffuse.rgb = rgb ;

    
    // Just copy the texture coordinate through
    Output.TextureUV = vTexCoord0; 
    
	/*
	// Prev

	float Rim = smoothstep(0.5f, 0.9f
	, 1.0f - saturate(dot(vNormalWorldSpace, normalize(g_vViewDir))));
	*/
	float Rim = smoothstep(0.9f, 1.1f
		, 1.0f - saturate(dot(vNormalWorldSpace, float3(0, 1, -1 ))));

	Output.Rim =  pow(Rim, 50);

	Output.Reflect = (reflect(( normalize(  g_vViewDir ))
		, vNormalWorldSpace));

    return Output;           
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In ) 
{	
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
	float4 rgb = tex2D(MeshTextureSampler, In.TextureUV);
		float4 BumpMap = tex2D(BumpTextureSampler, In.TextureUV);


	BumpMap.xy = ((BumpMap.xy * 5) - 1.0f);

	float3 BumpNormal = (BumpMap.rgb);


	float fBumpMultiply = pow(saturate(dot(BumpNormal, g_vViewDir)), 1);



	Output.RGBColor.a = rgb.a;
	Output.RGBColor.rgb =  rgb.rgb;

	float3 specular = 0;
		specular = saturate(dot(In.Reflect, float3(0, 1, 1))) * 0.5f;
	specular = max( specular,
		saturate(dot(In.Reflect, float3(0, 1, 1))) * 0.5f );
	if (g_IsBump)
	{
			Output.RGBColor.rgb *=  fBumpMultiply ;
	}
	if (!g_IsMap)
	{
	//	In.Diffuse = ceil(In.Diffuse * 3.0f) / 3.0f;
		Output.RGBColor.rgb *= pow( In.Diffuse, 1.0f );
		Output.RGBColor.rgb += ( In.Rim );

		Output.RGBColor.rgb += pow( specular , 1.0f );



		Output.RGBColor.r += (1.0f - g_fHitColor) * 0.4f;
		Output.RGBColor.gb *= g_fHitColor;
	}
	else
	{
		Output.RGBColor.rgb = pow(rgb.rgb, 1);

		if (g_IsDiffuse)
		{
			Output.RGBColor.rgb *= ( In.Diffuse * 0.8f + 0.2f );
		}
	}
	Output.RGBColor.a *= g_fAlpha;


    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene
{
	//pass P0
	//{
	//	CULLMODE = CW;
	//	VertexShader = compile vs_2_0 OUT_LINE_VS();
	//	PixelShader = compile ps_2_0 OUT_LINE_PS();
	//}
    pass P0
    {      
//		CULLMODE = CCW;
        VertexShader = compile vs_2_0 RenderSceneVS();
        PixelShader  = compile ps_2_0 RenderScenePS(); 
    }
}
