struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

float4 pixel_main(PixelInput input) : SV_Target
{
    return input.Color;
}