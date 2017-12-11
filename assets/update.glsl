
#version 330 core


layout (location = 0) in vec3 position;
//layout (location = 1) in float phi;
//layout (location = 2) in float theta;


out vec3 oPos;
//out float oPhi;
//out float oTheta;

void main(){
    
    oPos = position;
    //oPhi = phi;
    //oTheta = theta;
}
