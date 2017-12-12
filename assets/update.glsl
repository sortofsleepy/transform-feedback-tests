
#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in float phi;
layout (location = 2) in float theta;
layout (location = 3) in float phiSpeed;
layout (location = 4) in float thetaSpeed;


out vec3 oPos;
out float oPhi;
out float oTheta;
out float oPhiSpeed;
out float oThetaSpeed;

void main(){
    
    vec3 pos = position;
    
    pos *= vec3(0.);
    
    
    oPos = pos;
    oPhi = phi;
    oTheta = theta;
    oPhiSpeed = phiSpeed;
    oThetaSpeed = thetaSpeed;

    
    
}
