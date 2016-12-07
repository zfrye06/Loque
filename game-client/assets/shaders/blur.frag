#version 120

uniform sampler2D texture;
uniform vec2 resolution;
uniform vec2 direction;

// Stolen from https://github.com/Jam3/glsl-fast-gaussian-blur kinda
void main()
{
    vec2 uv = vec2(gl_FragCoord.xy / resolution.xy);
	vec4 color = vec4(0.0);
	vec2 off1 = vec2(1.3846153846) * direction;
	vec2 off2 = vec2(3.2307692308) * direction;
	color += texture2D(texture, uv) * 0.2270270270;
	color += texture2D(texture, uv + (off1 / resolution)) * 0.3162162162;
	color += texture2D(texture, uv - (off1 / resolution)) * 0.3162162162;
	color += texture2D(texture, uv + (off2 / resolution)) * 0.0702702703;
	color += texture2D(texture, uv - (off2 / resolution)) * 0.0702702703;
    gl_FragColor = color;
}
