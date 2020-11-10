#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb/stb_image.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "Mesh.h"
#include "LearnOpenGLPlatform.h"
#include "common/Util.h"

class Model
{
public:
  std::vector<Mesh*> meshes;

  Model(const char* path)
  {
    loadModel(path);
  }

  ~Model() {
    for(Mesh* mesh : meshes) { delete mesh; }
    for(Texture texture : texturesLoaded) { glDeleteTextures(1, &texture.id); }
  }

  void Draw(Shader shader)
  {
    for (uint32 i = 0; i < meshes.size(); i++) meshes[i]->Draw(shader);
  }

private:
  std::vector<Texture> texturesLoaded;
  std::string directory;

  void loadModel(std::string path)
  {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode* node, const aiScene* scene)
  {
    // process all the node's meshes (if any)
    for (uint32 i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[i]];
      Mesh* mesh;
      processMesh(assimpMesh, scene, &mesh);
      meshes.push_back(mesh);
    }
    // then do the same for each of its children
    for (uint32 i = 0; i < node->mNumChildren; i++)
    {
      processNode(node->mChildren[i], scene);
    }
  }

  void processMesh(const aiMesh* assimpMesh, const aiScene* scene, Mesh** mesh)
  {
    std::vector<Vertex> vertices;
    for (uint32 i = 0; i < assimpMesh->mNumVertices; i++)
    {
      Vertex vertex;
      vertex.Position = glm::vec3(assimpMesh->mVertices[i].x, assimpMesh->mVertices[i].y, assimpMesh->mVertices[i].z);
      vertex.Normal = glm::vec3(assimpMesh->mNormals[i].x, assimpMesh->mNormals[i].y, assimpMesh->mNormals[i].z);
      vertex.TexCoords = assimpMesh->HasTextureCoords(0) ?
                         glm::vec2(assimpMesh->mTextureCoords[0][i].x, assimpMesh->mTextureCoords[0][i].y) :
                         glm::vec2(0.0f, 0.0f);

      vertices.push_back(vertex);
    }

    // process indices
    std::vector<uint32> indices;
    for (uint32 i = 0; i < assimpMesh->mNumFaces; i++)
    {
      aiFace face = assimpMesh->mFaces[i];
      indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }

    // process material
    std::vector<Texture> textures;
    if (assimpMesh->mMaterialIndex >= 0)
    {
      aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];
      std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffTexture");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specTexture");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    *mesh = new Mesh(vertices, indices, textures);
  }

  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
  {
    std::vector<Texture> textures;

    char filename[128]; // NOTE: Hard limit on file directory size
    memcpy(filename, directory.c_str(), directory.length());
    filename[directory.length()] = '/';

    for (uint32 i = 0; i < mat->GetTextureCount(type); i++)
    {
      aiString fileName;
      mat->GetTexture(type, i, &fileName);
      bool skip = false;
      for (uint32 j = 0; j < texturesLoaded.size(); j++)
      {
        if (std::strcmp(texturesLoaded[j].path.data(), fileName.C_Str()) == 0)
        {
          textures.push_back(texturesLoaded[j]);
          skip = true;
          break;
        }
      }
      if (!skip)
      {   // if texture hasn't been loaded already, load it
        uint32 fileNameSize = directory.length() + fileName.length + 2;
        memcpy(filename + directory.length() + 1, fileName.C_Str(), fileName.length);
        filename[fileNameSize-1] = '\0';

        Texture texture;
        load2DTexture(filename, texture.id, false, false);
        texture.type = typeName;
        texture.path = fileName.C_Str();
        textures.push_back(texture);
        texturesLoaded.push_back(texture); // add to loaded textures
      }
    }
    return textures;
  }
};