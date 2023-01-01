Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	float4 color = Texture.Sample(Sampler, tc);
	color.x = 1.0f - color.x;
	color.y = 1.0f - color.y;
	color.z = 1.0f - color.z;
	return color * brightness;
}