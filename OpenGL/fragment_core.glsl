#version 440


struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 fs_color;

uniform Material material;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	fs_color = texture(texture0, vs_texcoord)*texture(texture1, vs_texcoord);
	//vec4(vs_color, 1.f);
}
