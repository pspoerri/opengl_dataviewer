#version 150

precision hghp float;
in vec4 in_Position;
out vec4 fragColor;

void main(void)
  fragColor = vec4(in_Position.w,0.0,0.0,1.0);
}
