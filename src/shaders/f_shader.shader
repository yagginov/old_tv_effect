#version 330 core

uniform vec2 scale = vec2(7.0, 12.0);

out vec4 FragColor;

void main() {
    vec4 original_color = vec4(1.0, 1.0, 1.0, 1.0);

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