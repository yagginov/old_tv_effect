#version 330 core

in vec2 texCoord;

uniform vec2 scale = vec2(4.0, 4.0);
uniform sampler2D renderedTexture;

out vec4 FragColor;

void main() {
    vec4 original_color = texture(renderedTexture, texCoord);

    vec2 stripe_index = mod(floor(gl_FragCoord.xy / scale), 4.0);

    if (stripe_index.x == 0.0 || stripe_index.y == 0.0) 
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } 
    else 
    {
        if (stripe_index.x == 1.0) 
        {
            FragColor = vec4(original_color.x, 0.0, 0.0, 1.0);
        } 
        else if (stripe_index.x == 2.0) 
        {
            FragColor = vec4(0.0, original_color.y, 0.0, 1.0);
        } 
        else 
        {
            FragColor = vec4(0.0, 0.0, original_color.z, 1.0);
        }
    }
}