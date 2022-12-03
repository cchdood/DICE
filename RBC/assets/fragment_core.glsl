#version 330 core
out vec4 FragColor;

in float Face_index;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;

void main() {
    
    if (Face_index == 1.0f) {
        FragColor = texture(texture1, TexCoord);
    }; 
    
    if (Face_index == 2.0f) {
        FragColor = texture(texture2, TexCoord);
    }; 

    if (Face_index == 3.0f) {
        FragColor = texture(texture3, TexCoord);
    }; 
    
    if (Face_index == 4.0f) {
        FragColor = texture(texture4, TexCoord);
    }; 

    if (Face_index == 5.0f) {
        FragColor = texture(texture5, TexCoord);
    }; 
    
    if (Face_index == 6.0f) {
        FragColor = texture(texture6, TexCoord);
    }; 
}