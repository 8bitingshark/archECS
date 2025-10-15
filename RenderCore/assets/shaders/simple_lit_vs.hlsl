// ===========================================================
// Simple Lit Vertex Shader (fixed light, row-major friendly)
// ===========================================================

cbuffer MatrixBuffer : register(b0)
{
    row_major matrix worldMatrix;
    row_major matrix viewMatrix;
    row_major matrix projectionMatrix;
};

// Input layout (match with C++)
struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

// Output to pixel shader
struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0;
};

// Vertex Shader
PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    // Transform vertex position to world space
    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    output.worldPos = worldPos.xyz;

    // Project to clip space
    float4 viewPos = mul(worldPos, viewMatrix);
    output.position = mul(viewPos, projectionMatrix);

    // Transform and normalize normal
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

    return output;
}