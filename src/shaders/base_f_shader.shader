#version 330 core
in vec3 pointPosition;
in vec3 Normal;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 lightPosition = vec3(2.0, 1.0, 13.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);
uniform vec3 eyePos = vec3(0.0, 0.0, 3.0);
uniform sampler2D texture_01;

void main() 
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - pointPosition);
    vec3 eyeDir = normalize(eyePos - pointPosition);

    vec3 textureColor = texture(texture_01, texCoord).xyz;

    float textureIntensity = smoothstep(0.0, 0.2, length(textureColor) / 3.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * textureColor;

    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor * textureColor;

    vec3 reflectDir = reflect(-lightDir, norm);

    float shininess = mix(32.0, 5.0, textureIntensity); 
    float spec = pow(max(dot(eyeDir, reflectDir), 0.0), shininess);
    
    vec3 specular = spec * lightColor * (1.0 - textureIntensity); 

    vec3 result = ambient + diffuse + specular; 
    FragColor = vec4(result, 1.0);
}
