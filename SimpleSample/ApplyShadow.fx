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
       // Color texture for mesh
texture g_MeshTexture;
texture g_BumpTexture;

sampler MeshTextureSampler =
sampler_state
{
	Texture = <g_MeshTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture g_sh;

sampler2D ShSampler =
sampler_state
{
	Texture = <g_sh>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler BumpTextureSampler =
sampler_state
{
	Texture = <g_BumpTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

bool	g_IsBump;


float4x4 g_LightView;
float4x4 g_LightProj;

float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
	float4 mClipPosition : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0 )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;

	Output.mClipPosition = mul(vPos, g_mWorld);
	Output.mClipPosition = mul(Output.mClipPosition, g_LightView);
	Output.mClipPosition = mul(Output.mClipPosition, g_LightProj);

	Output.Position = mul(vPos, g_mWorldViewProjection);

	vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld));

	float3 rgb = max( 0 , dot(vNormalWorldSpace, float3(0, 1, 0)) );
	rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(0, -1, 0))));
	rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(0, 1, 0))));
	rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(-1, 0, 0))));
	rgb = max(rgb, dot(vNormalWorldSpace, normalize(float3(1, 0, 0))));

	Output.Diffuse.rgb = rgb;
	Output.Diffuse.a = 1.0f;

	Output.TextureUV = vTexCoord0;


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
		Output.RGBColor = rgb;
	float2 uv = In.mClipPosition.xy / In.mClipPosition.w;
		uv.y = -uv.y;
	uv = uv * 0.5 + 0.5;

	float shadowDepth = tex2D(ShSampler, uv).r;
	float currentDepth = In.mClipPosition.z / In.mClipPosition.w;

	if (currentDepth > shadowDepth + 0.0000125f)
	{
		Output.RGBColor.rgb *= 0.75f;
	}
	if (g_IsBump)
	{
		float4 BumpMap = tex2D(BumpTextureSampler, In.TextureUV);

		BumpMap.xy = ((BumpMap.xy * 5.0f) - 1.0f);

		float3 BumpNormal = (BumpMap.rgb);


			float fBumpMultiply = pow(saturate(dot(BumpNormal, float3( 0, 1 , 0 ))), 1);
//			Output.RGBColor.rgb *=  fBumpMultiply ;

	}

	Output.RGBColor = Output.RGBColor * In.Diffuse ;

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene
{
	pass P0
    {          
		CULLMODE = CCW;
        VertexShader = compile vs_2_0 RenderSceneVS();
        PixelShader  = compile ps_2_0 RenderScenePS(); 
    }
}
