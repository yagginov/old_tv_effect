#version 330 core
in vec3 pointPosition;
in vec3 Normal;
in vec2 texCoord;

out vec4 FragColor;
uniform vec3 lightPosition = vec3(12.0, 25.0, 13.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);
uniform sampler2D texture_01;

void main() 
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - pointPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * texture(texture_01, texCoord).xyz;
    FragColor = vec4(result, 1.0);
}