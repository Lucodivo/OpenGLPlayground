#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "LearnOpenGLPlatform.h"

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture
{
  uint32 id;
  std::string type;
  std::string path;
};

class Mesh
{
public:
  std::vector<Vertex> vertices;
  std::vector<uint32> indices;
  std::vector<Texture> textures;
  uint32 VAO;

  Mesh(std::vector<Vertex> vertices, std::vector<uint32> indices, std::vector<Texture> textures)
  {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
  }

  void Draw(Shader shader)
  {
    uint32 diffuseNr = 1;
    uint32 specularNr = 1;
    for (uint32 i = 0; i < textures.size(); i++)
    {
      Texture texture = textures[i];
      glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
      // retrieve texture number (the N in diffuse_textureN)
      std::string number;
      std::string name = texture.type;
      if (name == "diffTexture")
        number = std::to_string(diffuseNr++);
      else if (name == "specTexture")
        number = std::to_string(specularNr++);

      const std::string uniformName = "material." + name + number;
      shader.setUniform(uniformName, i);
      glBindTexture(GL_TEXTURE_2D, texture.id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  uint32 VBO, EBO;

  void setupMesh()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // vertex texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    // unbind VBO, VAO, & EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
};
