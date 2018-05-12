#version 150
// rotateを変えればいいじゃないか(ひらひら)

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;

void main() {
    texCoordVarying = vec2(texcoord.x, texcoord.y);
    gl_Position = modelViewProjectionMatrix * position;
}

// 頂点の場所をいじる
//#version 150
//
//uniform mat4 modelViewProjectionMatrix;
//
//in vec4 position;
//in vec2 texcoord;
//
//uniform sampler2D tex;
//uniform float time;
//
//out vec2 texCoordVarying;
//
//void main() {
//    vec4 modifiedPosition = modelViewProjectionMatrix * position;
//
//    float scale = sin(time)*100;
//
//    float displacementY = texture(tex, texcoord).r;
//    modifiedPosition.x += displacementY * scale;
//
//    gl_Position = modifiedPosition;
//
//    texCoordVarying = texcoord;
//}


