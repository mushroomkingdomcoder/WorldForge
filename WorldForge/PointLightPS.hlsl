Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 light_diffuse;
	float4 light_ambient;
	float2 light_pos;
	float1 quad_att;
	float1 linear_att;
	float1 const_att;
	float1 view_aspect_ratio;
	int2 unused;
	float4 master_brightness;
};

float1 sq(float1 n : Number) : Result
{
	return n * n;
}

float4 Process(float2 tc : TextureCoordinate, float2 wpos : WorldPosition) : SV_TARGET
{
	wpos.x *= view_aspect_ratio;
	float1 dist = sqrt(sq(wpos.x - light_pos.x) + sq(wpos.y - light_pos.y));
	float1 attenuation = 1.0f;
	if (dist != 0.0f)
	{
		attenuation = 1.0f / (quad_att * sq(dist) + linear_att * dist + const_att);
		if (attenuation > 1.0f)
		{
			attenuation = 1.0f;
		}
	}
	float4 texture_color = Texture.Sample(Sampler, tc);
	return (texture_color * (light_diffuse * attenuation) + texture_color * light_ambient) * master_brightness;
}



