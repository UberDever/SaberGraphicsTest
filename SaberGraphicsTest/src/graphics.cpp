#include "../include/graphics.hpp"

ModelId_t Model::registeredModelsCount = 0;
Model::Model(Mesh &mesh)
{
    glGenVertexArrays(1, &info.vao);
    glBindVertexArray(info.vao);

    GLuint vbo_id = 0;

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(mesh.vertices.data()), mesh.vertices.data(), GL_STATIC_DRAW);

    GLuint ibo_id = 0;

    glGenBuffers(1, &ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(mesh.indices.data()), mesh.indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        vxAttr0, // positions
        GL_FLOAT,
        GL_FALSE,
        vxCount * sizeof(GLfloat),
        (GLvoid *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        vxAttr1, // texture coordinates
        GL_FLOAT,
        GL_FALSE,
        vxCount * sizeof(GLfloat),
        (GLvoid *)(vxAttr0 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        vxAttr2, // normals
        GL_FLOAT,
        GL_FALSE,
        vxCount * sizeof(GLfloat),
        (GLvoid *)((vxAttr0 + vxAttr1) * sizeof(GLfloat)));

    info.idx_count = mesh.indices.size();

    info.id = registeredModelsCount;
    registeredModelsCount++;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}