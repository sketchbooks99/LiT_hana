#version 150

uniform sampler2D tex;
uniform float alpha;
in vec2 texCoordVarying;
out vec4 outputColor;

vec4 color;


void main() {
    color = texture(tex, texCoordVarying);
    
    if(color.a > 0) color.a = alpha;
    
    outputColor = color;
}

