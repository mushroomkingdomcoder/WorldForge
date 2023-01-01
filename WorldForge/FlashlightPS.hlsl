Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 light_diffuse;
	float4 light_ambient;
	float2 light_pos;
	float1 theta;
	float1 arc_angle;
	float1 quad_att;
	float1 linear_att;
	float1 const_att;
	float1 view_aspect_ratio;
	float4 master_brightness;
};

float1 sq(float1 n : Number) : Result
{
	return n * n;
}

float1 GetAttenuation(float2 pos : PxlPos, float1 qa : QuadAtt, float1 la : LinearAtt, float1 ca : ConstAtt) : Att
{
	float1 dist = sqrt(sq(pos.x) + sq(pos.y));
	float1 att = 1.0f;
	if (dist != 0.0f)
	{
		att = 1.0f / (qa * sq(dist) + la * dist + ca);
		if (att > 1.0f)
		{
			att = 1.0f;
		}
	}
	return att;
}

float4 Process(float2 tc : TextureCoordinate, float2 wpos : WorldPosition) : SV_TARGET
{
	wpos.x *= view_aspect_ratio;
	wpos -= light_pos;
	const float1 pi = 3.1415926535897932384626433832795f;
	const float1 alpha = theta - arc_angle;
	const float1 sin_theta = sin(theta);
	const float1 cos_theta = cos(theta);
	const float1 sin_alpha = sin(alpha);
	const float1 cos_alpha = cos(alpha);
	float1 rise_over_run0 = 999999999999999999.0f;
	if (cos_theta != 0.0f)
	{
		rise_over_run0 = sin_theta / cos_theta;
	}
	float1 rise_over_run1 = 999999999999999999.0f;
	if (cos_alpha != 0.0f)
	{
		rise_over_run1 = sin_alpha / cos_alpha;
	}
	float1 attenuation = 0.0f;
	if (theta <= pi / 2.0f)
	{
		if (alpha >= -pi / 2.0f)
		{
			if (wpos.y <= rise_over_run0 * wpos.x && wpos.y >= rise_over_run1 * wpos.x)
			{
				attenuation = GetAttenuation(wpos, quad_att, linear_att, const_att);
			}
		}
		else
		{
			if (wpos.y <= rise_over_run0 * wpos.x && wpos.y <= rise_over_run1 * wpos.x)
			{
				attenuation = GetAttenuation(wpos, quad_att, linear_att, const_att);
			}
		}
	}
	else
	{
		if (alpha >= pi / 2.0f)
		{
			if (wpos.y >= rise_over_run0 * wpos.x && wpos.y <= rise_over_run1 * wpos.x)
			{
				attenuation = GetAttenuation(wpos, quad_att, linear_att, const_att);
			}
		}
		else
		{
			if (wpos.y >= rise_over_run0 * wpos.x && wpos.y >= rise_over_run1 * wpos.x)
			{
				attenuation = GetAttenuation(wpos, quad_att, linear_att, const_att);
			}
		}
	}
	float4 texture_color = Texture.Sample(Sampler, tc);
	return (texture_color * (light_diffuse * attenuation) + texture_color * light_ambient) * master_brightness;
}
