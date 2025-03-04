// Vertex shader:
// ================
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightingColor; // resulting color from lighting calculations

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // gouraud shading
    // ------------------------
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;      

    LightingColor = ambient + diffuse + specular;
}


// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;

in vec3 LightingColor; 

uniform vec3 objectColor;

void main()
{
   FragColor = vec4(LightingColor * objectColor, 1.0);
}

/*
那么我们看到了什么呢？
你可以亲自观察（或者参考提供的图像）到立方体前面两个三角形之间明显的区别。
这个“条纹”之所以可见，是由于片元插值的缘故。
从示例图像中我们可以看到，立方体前面板的右上顶点有镜面高光。
由于底部右侧三角形的右上顶点被照亮，而该三角形另外两个顶点未被照亮，所以明亮的数值通过插值传递到了另外两个顶点。
上部左侧三角形也发生了同样的情况。由于中间的片元颜色并非直接来自光源，而是插值的结果，所以中间片元的光照计算不正确，
导致上部左侧三角形与底部右侧三角形在亮度上发生冲突，从而在两个三角形之间出现了一条明显的条纹。

当使用更复杂的形状时，这种效果会更加明显。
*/
