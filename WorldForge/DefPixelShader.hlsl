Texture2D Texture;
SamplerState Sampler;

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	return Texture.Sample(Sampler, tc);
}


