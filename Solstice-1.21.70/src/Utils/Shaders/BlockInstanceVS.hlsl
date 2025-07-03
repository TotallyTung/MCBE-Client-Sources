// Define the input structure, which matches the input layout you've defined.
struct VSInput
{
    float3 Position : POSITION;
};

// Define the instance input structure.
struct VSInstanceInput
{
    float3 InstPos : InstPos;
    float4 InstCol : InstCol;
};

// Define the output structure, which should include at least the position.
struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
};

// Define the constant buffer structure for per-frame data.
cbuffer PerFrameConstantsBuffer : register(b0)
{
    float4x4 worldViewProj;
};

// The main vertex shader function.
VSOutput vertex_main(VSInput input, VSInstanceInput instanceInput)
{
    VSOutput output;
    
    //Poo shiddy
    output.Position = mul(float4(input.Position + instanceInput.InstPos, 1.0f), worldViewProj);
    output.Color = instanceInput.InstCol;

    return output;
}
