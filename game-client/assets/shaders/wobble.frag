#version 120

uniform sampler2D texture;
uniform int width;
uniform int height;
uniform vec2 pos;
uniform float time;

void main()
{
    // Higher strength means bigger wobble
    float strength = 6;
    // Higher speed means faster wobble
    float speed = 4;

    // Got some magic number smell here (the /8). I can't help it!
    float x = gl_TexCoord[0].x + (sin( pos.y/8 + time*speed + gl_TexCoord[0].y * height/strength ) + 0.5)/width*strength;
    float y = gl_TexCoord[0].y + (cos( pos.x/8 + time*speed + gl_TexCoord[0].x * width/(strength*cos(time)) ) + 0.5)/height*strength;
    gl_FragColor = gl_Color * texture2D( texture, vec2( x, y ) );
}
