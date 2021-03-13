#ifdef COMPILE_VS

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec3 a_color;

out vec3 v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0); // 
    v_color = a_color;
}

#else

in vec3 v_color;
out vec4 f_color;

uniform vec3 u_lightColor;

void main()
{
    f_color = vec4(u_lightColor, 1);
}

#endif