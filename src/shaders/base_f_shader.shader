#version 330 core
in vec3 pointPosition;
in vec3 Normal;

out vec4 FragColor;
uniform vec3 lightPosition = vec3(3.0, 12.0, 12.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 0.4);
uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);

void main() 
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - pointPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}