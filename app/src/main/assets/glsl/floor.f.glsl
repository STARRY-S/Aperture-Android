#version 300 es
precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
    vec3 ambient  = 0.8  * vec3(texture(ourTexture, TexCoord));
    // FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(ambient, 1.0);
    // FragColor = vec4(ourColor, 1.0);
}
