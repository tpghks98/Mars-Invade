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

float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix

float4x4 g_LightView;
float4x4 g_LightProj;
texture g_MeshTexture;

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
//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
	float4 mClipPosition : TEXCOORD1;
	float2 vTex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0 )
{
    VS_OUTPUT Output;

	Output.mClipPosition = mul(vPos, g_mWorld);
	Output.mClipPosition = mul(Output.mClipPosition, g_LightView);
	Output.mClipPosition = mul(Output.mClipPosition, g_LightProj);
	Output.Position = Output.mClipPosition;
	Output.vTex = vTexCoord0;

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
	float depth = In.mClipPosition.z / In.mClipPosition.w;

	PS_OUTPUT ps;
	ps.RGBColor = float4(depth.xxx, tex2D(MeshTextureSampler, In.vTex).a);
	return ps;
}


//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene
{
	pass CreateShadow
	{
		VertexShader = compile vs_3_0 RenderSceneVS();
		PixelShader = compile ps_3_0 RenderScenePS();
	}
}