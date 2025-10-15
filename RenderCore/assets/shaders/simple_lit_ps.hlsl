// ===========================================================
// Simple Lit Pixel Shader (fixed light, no extra buffers)
// ===========================================================

cbuffer ColorBuffer : register(b0)
{
    float4 baseColor; // Tint color from CPU
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0;
};

// Fixed light parameters
static const float3 LightDirection = normalize(float3(0.577f, -0.577f, 0.577f)); // 45° down-right
static const float3 LightColor = float3(1.0f, 0.95f, 0.85f); // Warm white light
static const float AmbientIntensity = 0.15f;

// Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
    float3 N = normalize(input.normal);
    float NdotL = saturate(dot(N, -LightDirection));

    float3 diffuse = LightColor * baseColor.rgb * NdotL;
    float3 ambient = AmbientIntensity * baseColor.rgb;

    float3 finalColor = diffuse + ambient;

    return float4(finalColor, baseColor.a);
}