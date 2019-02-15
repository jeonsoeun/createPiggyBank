//#pragma comment(lib,"FreeImage.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include<math.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>
#include "FreeImage.h"

#define MAXNUM 1024

using namespace std;

class Group{
public:
	string matarial; //matarial이름.
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];
	GLfloat Ns;
	Group();
	Group(string str, int a,int d, int s, int ns)
	{
		matarial = str;

	};
};
class BST{
public:
	string index_set;
	int vtx_index;
	int tex_index;
	int nor_index;
	GLuint total_index;
	BST();
	BST::BST(string index_string, int vtx, int tex);
	BST::BST(string index_string, int vtx, int tex, int nor);
	BST::BST(string index_s, int vtx);
	BST(string index_string, int vtx, int tex, int nor, GLuint ind);
};
class Object{
public:
	GLuint VAO[2];
	GLuint VBO[2];
	GLuint VEO[2];
	GLuint VNO[2];
	GLuint TEX[1];

	GLuint VBO_sh;
	GLuint VEO_sh;

	vector<GLfloat> vertices;
	vector<GLuint> indices;
	vector<GLfloat>normal;
	vector<GLfloat> texture_vtx;
	bool use_tex;
	bool use_nor;
	vector<BST> index_tree;
	vector<Group> group;

	GLubyte* texels;
	GLuint sampler[1];
	string imageName;

	vector<GLfloat> shadowVolume;
	vector<GLuint> shadowV_index;
};

bool searchTree(vector<BST> index_tree, string index, int& num); //반환값>> true: 같은거 찾음/ false:못찾음.
bool insertTree(vector<BST>& index_tree, string index, int& o_index/*지금시점에서의 search tree인덱스값을 가져옴.*/, GLuint& t_index/*마지막에 쓰일 인덱스*/);

inline int compareIndex(BST node, int vtx, int tex, int nor)
{
	//반환값>> 1 :  새로운게 더 클때, -1 : 새로운게 더 작을때, 0 끝.
	if (node.vtx_index > vtx)
		return 1;
	else if (node.vtx_index < vtx)
		return -1;

	if (tex != 0)
	{
		if (node.tex_index > tex)
			return 1;
		else if (node.tex_index < tex)
			return -1;
	}

	if (nor != 0)
	{
		if (node.nor_index>nor)
			return 1;
		else if (node.nor_index < nor)
			return -1;
	}

	return 0;
}


void loadImage(string filename, GLubyte*& texel, GLuint& cx, GLuint& cy);
void unloadImage(FIBITMAP* pImage);
int prepareTexture(Object& object);
void checkerboard(Object &object);
