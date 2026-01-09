#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;

out vec3 ourColor;
out vec2 texCoord;

uniform float sin_pos;
uniform float cos_pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   if (int(aPos.x)%2==0 || int(aPos.z)%2==0){
	   gl_Position= projection * view * model * vec4(aPos.x+ cos_pos/3.0,sin_pos/8.0,aPos.z+ cos_pos/3.0,1.0f);
   }else{
      gl_Position= projection * view * model * vec4(aPos.x+ cos_pos/3.0,0,aPos.z,1.0f);

   }
   ourColor= aCol;
   texCoord= aTex;
}