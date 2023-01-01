Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	return Texture.Sample(Sampler, float2(1.0f - tc.x, 1.0f - tc.y)) * brightness;
}



