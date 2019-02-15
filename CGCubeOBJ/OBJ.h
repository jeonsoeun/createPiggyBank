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
	string matarial; //matarial�̸�.
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

bool searchTree(vector<BST> index_tree, string index, int& num); //��ȯ��>> true: ������ ã��/ false:��ã��.
bool insertTree(vector<BST>& index_tree, string index, int& o_index/*���ݽ��������� search tree�ε������� ������.*/, GLuint& t_index/*�������� ���� �ε���*/);

inline int compareIndex(BST node, int vtx, int tex, int nor)
{
	//��ȯ��>> 1 :  ���ο�� �� Ŭ��, -1 : ���ο�� �� ������, 0 ��.
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
