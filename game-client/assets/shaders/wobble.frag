#version 120

uniform sampler2D texture;
uniform int width;
uniform int height;
uniform float time;

void main()
{
    // Higher strength means bigger wobble
    float strength = 8;
    // Higher speed means faster wobble
    float speed = 2;

    float x = gl_TexCoord[0].x + (sin( time*speed + gl_TexCoord[0].y * width/strength ) + 0.5)/width*strength;
    float y = gl_TexCoord[0].y + (cos( time*speed + gl_TexCoord[0].x * height/strength ) + 0.5)/height*strength;
    gl_FragColor = gl_Color * texture2D( texture, vec2( x, y ) );
}
