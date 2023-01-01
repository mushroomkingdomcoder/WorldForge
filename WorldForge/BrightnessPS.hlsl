Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	return Texture.Sample(Sampler, tc) * brightness;
}



