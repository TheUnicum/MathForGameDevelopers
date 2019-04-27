#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_model;
uniform mat4 u_mvp;
uniform mat3 u_transInvers_model;

void main()
{
	FragPos = vec3(u_model * vec4(aPos, 1.0f));
	Normal = u_transInvers_model * aNormal;
	TexCoords = aTexCoords;

	gl_Position = u_mvp * vec4(aPos, 1.0f);
};

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
	sampler2D emission1;
	float shininess;
};

uniform Material material;

uniform vec3 u_Box_color;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

void main()
{
	//FragColor = vec4(texture(material.diffuse1, TexCoords));
	//FragColor = vec4(u_Box_color, 1.0f);
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * u_lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * u_lightColor;

	// specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(u_viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * u_lightColor;

	vec3 result = (ambient + diffuse + specular) * u_Box_color;
	FragColor = vec4(result, 1.0f);



};

