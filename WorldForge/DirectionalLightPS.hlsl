Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 light_diffuse;
	float4 light_ambient;
	float3 light_direction;
	int1 unused;
	float4 master_brightness;
};

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	float1 attenuation = light_direction.z;
	if (attenuation < 0.0f)
	{
		attenuation = 0.0f;
	}
	if (attenuation > 1.0f)
	{
		attenuation = 1.0f;
	}
	float4 texture_color = Texture.Sample(Sampler, tc);
	return (texture_color * (light_diffuse * attenuation) + texture_color * light_ambient) * master_brightness;
}