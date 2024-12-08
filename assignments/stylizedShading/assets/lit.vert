#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec3 WorldNormal;
out vec2 TexCoord;
out vec3 WorldPos;
out vec4 ClipSpace;
flat out int CubeFace; // 1 = right, 2 = left, 3 = front, 4 = back, 5 = top

uniform mat4 _Model;
uniform mat4 _ViewProjection;
uniform int _WallCount;
uniform int _MazeSize;
uniform int _ActiveTexture;

void main(){
    vec4 worldPos = _Model * vec4(aPos,1.0);
	WorldPos = worldPos.xyz;
	WorldNormal = mat3(transpose(inverse(_Model))) * aNormal;

	// floor plane specifc
	if (_ActiveTexture == 1)
	{
		TexCoord = vec2(aUV.x * _MazeSize, aUV.y * _MazeSize);
	}
	// if this is the left or right wall, we dont want to modify its uv
	else if (aNormal.x != 0)
	{
		vec2 rotatedUV;

		// rotate uv coordinates to display texture at proper orientation
		if (aNormal.x > 0)
		{
			rotatedUV = vec2(-aUV.y, aUV.x);
		}
		else
		{
			rotatedUV = vec2(aUV.y, -aUV.x);
		}

		CubeFace = 0;
		TexCoord = vec2(rotatedUV.x, rotatedUV.y);
	}
	// if its the top face, we need to modify the v axis
	else if (aNormal.y != 0)
	{
		TexCoord = vec2(aUV.x, aUV.y * _WallCount);
		CubeFace = 5;
	}
	// if its the front or back face, we need to modify the u axis
	else if (aNormal.z != 0)
	{
		CubeFace = 0;
		TexCoord = vec2(aUV.x * _WallCount, aUV.y);
	}
	else
	{
		TexCoord = vec2(aUV.x, aUV.y);
	}

	ClipSpace = _ViewProjection * worldPos;

	gl_Position = ClipSpace;
}