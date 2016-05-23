#version 330

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vTex;

out vec2 fTex;

uniform float posx;
uniform float posy;
uniform float width;
uniform float height;
uniform int flip;

void main()
{
    gl_Position = vec4(vPos.x * width + posx, vPos.y * height + posy, 0.0, 1.0);
	if (flip == 0)
		fTex = vTex;
	else
		fTex = vec2(1.0 - vTex.x, vTex.y);
}
