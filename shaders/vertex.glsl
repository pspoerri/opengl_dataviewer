#version 150 compatibility

in vec4 in_Position;
//layout(location=1) in vec4 in_Color;
out vec4 ex_Color;

void main(void)
{
  gl_Position = gl_ModelViewProjectionMatrix * vec4(in_Position.xyz, 1.0);
  ex_Color = vec4(in_Position.w,1.0,1.0,1.0);
}
