#version 120

uniform sampler2D texture;
uniform int width;
uniform int height;
uniform vec2 pos;
uniform float time;

void main()
{
    // Higher strength means bigger wobble
    float strength = 10;
    // Higher speed means faster wobble
    float flatness = 4;
    float speed = 2;
    float variation = cos(time);

    float x = gl_TexCoord[0].x + (sin( -pos.y/(flatness*strength) + time*speed + gl_TexCoord[0].y/flatness * height/strength ) + 0.5)/width*strength;
    float y = gl_TexCoord[0].y + variation*(cos( pos.x/(flatness*strength) + time*speed + gl_TexCoord[0].x/flatness * width/strength ) + 0.5)/height*strength;
    gl_FragColor = gl_Color * texture2D( texture, vec2( x, y ) );
}
