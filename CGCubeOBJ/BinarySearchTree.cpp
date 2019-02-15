#pragma once
#include"OBJ.h"

BST::BST(){}
BST::BST(string index_string, int vtx, int tex,int nor)
{
	index_set = index_string;
	vtx_index = vtx;
	tex_index = tex;
	nor_index = nor;
}
BST::BST(string index_s, int vtx)
{
	index_set = index_s;
	vtx_index = vtx;
	tex_index = 0;
	nor_index = 0;
}
BST::BST(string index_string, int vtx, int tex)
{
	index_set = index_string;
	vtx_index = vtx;
	tex_index = tex;
	nor_index = 0;
}
BST::BST(string index_string, int vtx, int tex, int nor, GLuint ind)
{
	index_set = index_string;
	vtx_index = vtx;
	tex_index = tex;
	nor_index = 0;
	total_index = ind;
}

bool searchTree(vector<BST> index_tree, string index, int& num) //반환값>> true: 같은거 찾음/ false:못찾음.
{
	int start = 0; //현재 비교가능성이 있는 범위의 시작점
	int end = index_tree.size() - 1; //끝점
	int number = 0;

	char temp[1024];
	char* a;
	strcpy_s(temp, 1024, index.data());
	strtok_s(temp, "/", &a);
	int vtx = atoi(temp);
	int tex = atoi(a);

	char* b;
	strtok_s(a, "/", &b);
	int nor = atoi(b);

	for (int i = 0;; i++)
	{
		if (i % 10 == 0 && i != 0)
			cout << "\t Search Depth = " << i << "\n";
		number = (start + end) / 2;
		if (start > end)
		{
			cout << "시작점: " << start << "  끝점: " << end << "에서 오류." << endl;
			return false;
		}
		if (end - start < 10)
		{
			for (int j = 0; j < 10; j++)
			{
				int check = compareIndex(index_tree[j + start], vtx, tex, nor);
				if (check == 0)
				{
					num = start + j;
					return true;
				}
				else if (check == -1)
				{
					num = start + j;
					return false;
				}
			}
			num = end + 1;
			return false;
		}
		int check = compareIndex(index_tree[number], vtx, tex, nor);
		if (check == 0)
		{
			num = number;
			return true;
		}
		else if (check == 1)
		{
			start = number + 1;
		}
		else if (check == -1)
		{
			end = number - 1;
		}
	}
	num = -1;
	return false;
}

bool insertTree(vector<BST>& index_tree, string index, int& o_index/*vtx-vt세트 만들기 위한 인덱스.*/, GLuint& t_index/*마지막에 쓰일 인덱스*/)
{
	char temp[1024];
	char* a;
	strcpy_s(temp, 1024, index.data());
	strtok_s(temp, "/", &a);
	int vtx = atoi(temp);
	int tex = atoi(a);

	int result = 0;

	//if (index_tree.size() % 100 == 0)
	//	cout << "Index Count = " << index_tree.size() << "\n";

	if (index_tree.size() == 0)
	{
 		BST insert_item(index, vtx, tex,-1, t_index);
		index_tree.insert(index_tree.begin(), insert_item);
		o_index = 0;
		
		return true;
	}
	else if (!searchTree(index_tree, index, result))//searchTree에서 못찾은 것만 insert함.
	{
		BST insert_item(index, vtx, tex,-1, t_index);
		index_tree.insert(index_tree.begin() + result, insert_item);

		o_index = result;
		return true;
	}
	else//찾은거는 그냥 반환.
	{
		if (result != -1)
		{
			t_index = index_tree[result].total_index;
			o_index = result;
			return false;
		}
	}
	return false;
}
