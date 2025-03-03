// Vertex shader:
// ==============
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// out vec3 ourColor;
out vec3 ourPosition;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    // ourColor = aColor;
    ourPosition = aPos;
}

// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;
// in vec3 ourColor;
in vec3 ourPosition;

void main()
{
    FragColor = vec4(ourPosition, 1.0);    // note how the position value is linearly interpolated to get all the different colors
}
/*
回答问题：你知道为什么左下角是黑色的吗？
-- --------------------------------------------------------------------
想一想：我们的片段着色器输出的颜色等于三角形的（经过插值的）坐标。
那么，三角形左下角的坐标是多少呢？是 (-0.5f, -0.5f, 0.0f)。由于 xy 的值为负，它们被限制为 0.0f。
从这一点开始，一直到三角形中心，值都会重新通过插值变为正数。而值为 0.0f 自然对应黑色，这就解释了三角形黑色的一侧。
*/
