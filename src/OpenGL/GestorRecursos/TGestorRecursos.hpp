#ifndef TGESTORRECURSOS_H
#define TGESTORRECURSOS_H

#include <iostream>

#include <vector>
#include <map>
#include "libGL.hpp"

class TRecurso;
class TRecursoMalla;
class TRecursoTextura;
class TRecursoMaterial;
class mesh;

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

class TGestorRecursos
{
public:
	~TGestorRecursos();

	//-----------ASSIMP-----------//
	std::vector<mesh*> loadMesh(const char* nombre, bool sta, bool anim);
	void processNode(aiNode* node, const aiScene* scene);


	// ASSIMP:MALLA
	TRecursoMalla* processMesh(aiMesh* mesh, const aiScene* scene, const char* nombre);

	// ASSIMP:MATERIAL
	TRecursoMaterial* getRecursoMaterial(const char* nombre, const aiMaterial* mat);
	TRecursoMaterial* processMaterial(aiMesh* mesh, const aiScene* scene);

	// ASSIMP:TEXTURA
	TRecursoTextura* processTextures(aiMesh* mesh, const aiScene* scene);

	void loadTexture(const aiScene* scene, const aiMesh* mesh, aiTextureType textureType, GLuint& textName, TRecursoTextura* recText);
	GLuint textureFromFile(const std::string& file);

	// METODOS GET
	TRecursoMalla* getMalla(const char* name);
	TRecursoTextura* getRecursoTextura(const char* nombre, aiMesh* mesh, const aiScene* scene);
	TRecursoMalla* getRecursoMalla(const char* nombre, aiMesh* mesh, const aiScene* scene);	

	std::vector<TRecurso*> getRecursoMallas();

private:

	std::vector<mesh*> objMeshes;
	std::vector<TRecurso*> recursoMallas;
	std::vector<TRecurso*> recursoMaterials;
	std::vector<TRecurso*> recursoTexturas;
	std::string* dir;
	std::map<std::string, GLuint> textures;
	bool statico;
	bool animation;
	int animationCount;
};
#endif