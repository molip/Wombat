#include "ObjMesh.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm>

const char* obj_database = "";	// 定義 mesh 的預設目錄

ObjMesh::ObjMesh(const char* obj_file)
{
	matTotal = 0;		// mat[0] reserved for default meterial
	vTotal = tTotal = nTotal = fTotal = 0;

	Init(obj_file);
}

ObjMesh::ObjMesh()
{
	matTotal = 0;
	vTotal = tTotal = nTotal = fTotal = 0;
}

ObjMesh::~ObjMesh()
{
}

void ObjMesh::LoadMesh(string obj_file)
{
	char	token[100], buf[100], v[5][100];	// v[5] 表示一個 polygon 可以有 5個 vertex
	float	vec[3];

	int	n_vertex, n_texture, n_normal;
	int	cur_tex = 0;				// state variable: 目前所使用的 material

	errno_t err = ::fopen_s(&scene, obj_file.c_str(), "r");
	s_file = obj_file;

	if (!scene)
	{
		cout<< string("Can not open object File \"") << obj_file << "\" !" << endl;
		return;
	}

    std::string strPath;
    size_t n = obj_file.find_last_of('/');
    if (n != std::string::npos)
        strPath = obj_file.substr(0, n + 1);
    //cout << strPath << endl;

	cout<<endl<<obj_file<<endl;

	while(!feof(scene))
	{
		token[0] = 0;
		fscanf_s(scene, "%s", token, sizeof(token));		// 讀 token

		if (!strcmp(token,"g"))
		{
			fscanf_s(scene, "%s", buf, sizeof(buf));
		}

		else if (!strcmp(token,"mtllib"))
		{
			fscanf_s(scene, "%s", mat_file, sizeof(mat_file));
			LoadTex(strPath + string(mat_file));
		}

		else if (!strcmp(token,"usemtl"))
		{
			fscanf_s(scene, "%s", buf, sizeof(buf));
			cur_tex = matMap[s_file+string("_")+string(buf)];
		}

		else if (!strcmp(token,"v"))
		{
			fscanf_s(scene, "%f %f %f", &vec[0], &vec[1], &vec[2]);
			vList.push_back(Vec3(vec));
		}

		else if (!strcmp(token,"vn"))
		{
			fscanf_s(scene, "%f %f %f", &vec[0], &vec[1], &vec[2]);
			nList.push_back(Vec3(vec));
		}
		else if (!strcmp(token,"vt"))
		{
			fscanf_s(scene, "%f %f", &vec[0], &vec[1]);
			tList.push_back(Vec3(vec));
		}

		else if (!strcmp(token,"f"))
		{
			for (int i=0;i<3;i++)		// face 預設為 3，假設一個 polygon 都只有 3 個 vertex
			{
				fscanf_s(scene, "%s", v[i], sizeof(v[i]));
				//printf("[%s]",v[i]);
			}
			//printf("\n");

			Vertex	tmp_vertex[3];		// for faceList structure

			for (int i=0;i<3;i++)		// for each vertex of this face
			{
				char str[20], ch;
				int base,offset;
				base = offset = 0;

				// calculate vertex-list index
				while( (ch=v[i][base+offset]) != '/' && (ch=v[i][base+offset]) != '\0')
				{
					str[offset] = ch;
					offset++;
				}
				str[offset] = '\0';
				n_vertex = atoi(str);
				base += (ch == '\0')? offset : offset+1;
				offset = 0;

				// calculate texture-list index
				while( (ch=v[i][base+offset]) != '/' && (ch=v[i][base+offset]) != '\0')
				{
					str[offset] = ch;
					offset++;
				}
				str[offset] = '\0';
				n_texture = atoi(str);	// case: xxx//zzz，texture 設為 0 (tList 從 1 開始)
				base += (ch == '\0')? offset : offset+1;
				offset = 0;

				// calculate normal-list index
				while( (ch=v[i][base+offset]) != '\0')
				{
					str[offset] = ch;
					offset++;
				}
				str[offset] = '\0';
				n_normal = atoi(str);	// case: xxx/yyy，normal 設為 0 (nList 從 1 開始)

				tmp_vertex[i].v = n_vertex;
				tmp_vertex[i].t = n_texture;
				tmp_vertex[i].n = n_normal;
				tmp_vertex[i].m = cur_tex;
			}

			faceList.push_back(FACE(tmp_vertex[0],tmp_vertex[1],tmp_vertex[2]));
		}

		else if (!strcmp(token,"#"))	  // 註解
			fgets(buf,100,scene);

//		printf("[%s]\n",token);
	}

	if (scene) fclose(scene);

	vTotal = vList.size();
	nTotal = nList.size();
	tTotal = tList.size();
	fTotal = faceList.size();
	printf("vetex: %d, normal: %d, texture: %d, triangles: %d\n",vTotal, nTotal, tTotal, fTotal);
}

void ObjMesh::LoadTex(string tex_file)
{
	char	token[100], buf[100];
	float	r,g,b;

	::fopen_s(&texture, tex_file.c_str(), "r");
	t_file = tex_file;

	if (!texture)
	{
		cout << "Can't open material file \"" << tex_file << "\"!" << endl;
		return;
	}

	cout<<tex_file<<endl;

	int cur_mat;

	while(!feof(texture))
	{
		token[0] = NULL;
		fscanf_s(texture, "%s", token, sizeof(token));		// 讀 token

		if (!strcmp(token,"newmtl"))
		{
			fscanf_s(texture, "%s", buf, sizeof(buf));
			cur_mat = matTotal++;					// 從 mat[1] 開始，mat[0] 空下來給 default material 用
			matMap[s_file+string("_")+string(buf)] = cur_mat; 	// matMap["material_name"] = material_id;
		}

		else if (!strcmp(token,"Ka"))
		{
			fscanf_s(texture, "%f %f %f", &r, &g, &b);
			mat[cur_mat].ambient = Colour(r, g, b);
		}

		else if (!strcmp(token,"Kd"))
		{
			fscanf_s(texture, "%f %f %f", &r, &g, &b);
			mat[cur_mat].diffuse = Colour(r, g, b);
		}

		else if (!strcmp(token,"Ks"))
		{
			fscanf_s(texture, "%f %f %f", &r, &g, &b);
			mat[cur_mat].specular = Colour(r, g, b);
		}

		else if (!strcmp(token,"Ns"))
		{
			fscanf_s(texture, "%f", &r);
			mat[cur_mat].shininess = (int)r;
		}

		else if (!strcmp(token,"#"))	  // 註解
			fgets(buf,100,texture);

//		printf("[%s]\n",token);
	}

	printf("total material:%d\n",matMap.size());

	if (texture) fclose(texture);
}

void ObjMesh::Init(const char* obj_file)
{
	float default_value[3] = {1,1,1};

	vList.push_back(Vec3(default_value));	// 因為 *.obj 的 index 是從 1 開始
	nList.push_back(Vec3(default_value));	// 所以要先 push 一個 default value 到 vList[0],nList[0],tList[0]
	tList.push_back(Vec3(default_value));

	// 定義 default meterial: mat[0]
	matTotal++;

	LoadMesh(string(obj_file));		// 讀入 .obj 檔 (可處理 Material)
}

void ObjMesh::Draw()
{
    glBegin(GL_TRIANGLES);
	for (int i = 0; i < fTotal; ++i)
    {
       for (int j = 0; j < 3; ++j)
       {
            const Vertex& vertex = faceList[i][j];
            GLfloat* pV = vList[vertex.v].ptr;
            GLfloat* pN = nList[vertex.n].ptr;

			mat[vertex.m].Apply();

            glNormal3fv(pN);
            glVertex3fv(pV);
            //printf("%f, f, %f\n", pV[0], pV[1], pV[2]);
       }
    }
    glEnd();
}

void ObjMesh::Scale(float s)
{
	for (size_t i = 0; i < vList.size(); ++i)
        for (int j = 0; j < 3; ++j)
            vList[i].ptr[j] *= s;
}

float ObjMesh::GetHorzRadius() const
{
    float rMax = 0.0;
	for (size_t i = 1; i < vList.size(); ++i) // skip first (default value)
    {
        float x = vList[i][0], z = vList[i][2];
        float r = ::sqrt(x * x + z * z);
        rMax = std::max(r, rMax);
    }
    return rMax;
}

//void ObjMesh::GetBBox()
//{
//	for (size_t i = 0; i < vList.size(); ++i)
//        for (int j = 0; j < 3; ++j)
//            vList[i].ptr[j] *= s;
//}
