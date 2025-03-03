// 自定义实现的 LookAt 函数
glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    // 1. 已知 Position
    // 2. 计算摄像机方向
    glm::vec3 zaxis = glm::normalize(position - target);
    // 3. 得到正方向的右轴向量
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // 4. 计算摄像机的上轴向量
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // 创建平移矩阵和旋转矩阵
    // 在 glm 中，由于使用列主序，因此访问矩阵元素方式为 mat[col][row]
    glm::mat4 translation = glm::mat4(1.0f); // 默认构造单位矩阵
    translation[3][0] = -position.x; // 第四列，第一行
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // 第一列，第一行
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // 第一列，第二行
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // 第一列，第三行
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;

    // 返回 LookAt 矩阵，组合平移矩阵和旋转矩阵
    return rotation * translation; // 请注意，矩阵相乘遵循从右到左的顺序（先平移后旋转）
}

// 别忘了用你自己的版本替换 glm::lookAt
// view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),
//                    glm::vec3(0.0f, 0.0f, 0.0f),
//                    glm::vec3(0.0f, 1.0f, 0.0f));
view = calculate_lookAt_matrix(glm::vec3(camX, 0.0f, camZ),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
