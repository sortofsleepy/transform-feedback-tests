#version 330

in float vColor;
out vec4 glFragColor;
void main(){
    
    glFragColor = vec4(vColor,1.0,0.0,1.0);
}


