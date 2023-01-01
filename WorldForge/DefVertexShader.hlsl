struct Vertex
{
	float2 tc : TextureCoordinate;
	float2 wpos : WorldPosition;
	float4 pos : SV_POSITION;
};

Vertex Process(float2 pos : Position, float2 tc : TextureCoordinate)
{
	Vertex vtx;
	vtx.tc = tc;
	vtx.wpos = pos;
	vtx.pos = float4(pos, 0.0f, 1.0f);
	return vtx;
}