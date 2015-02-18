#pragma once

#include "Material.h"

#include <SFML/OpenGL.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

class ObjMesh
{
public:
	ObjMesh();
	ObjMesh(const char* obj_file);
	virtual ~ObjMesh();

    void Draw();
    void Scale(float s);
    float GetHorzRadius() const;

private:
	class Vertex
	{
	public:
		int v;
		int n;
		int t;
		int m;
		Vertex() {};
		Vertex(int v_index, int n_index, int t_index=0, int m_index=0)
		{
			v = v_index;
			n = n_index;
			t = t_index;
			m = m_index;
		}
	};

	class Vec3
	{
	public:
		GLfloat ptr[3];
		Vec3 (GLfloat *v)
		{
			for (int i=0;i<3;i++)
				ptr[i] = v[i];
		}
		GLfloat& operator[](int index)
		{
			return ptr[index];
		}
		const GLfloat& operator[](int index) const
		{
			return ptr[index];
		}
	};

	class FACE
	{
	public:
		Vertex v[3];
		FACE (Vertex &v1, Vertex &v2, Vertex &v3)
		{
			v[0] = v1;
			v[1] = v2;
			v[2] = v3;
		}
		Vertex& operator[](int index)
		{
			return v[index];
		}
	};

	/////////////////////////////////////////////////////////////////////////////
	// Loading Object
	/////////////////////////////////////////////////////////////////////////////

	string  s_file;

	map<string,int> matMap;		// material_name -> material_ID
	vector<Material> matList;

	vector<Vec3>	vList;
	vector<Vec3>	nList;
	vector<Vec3>	tList;
	vector<FACE>	faceList;

	void LoadMesh(const string& scene_file);
	void LoadTex(const string&tex_file);
};
