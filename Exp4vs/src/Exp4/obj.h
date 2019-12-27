#ifndef _OBJ_H 
#define _OBJ_H 

#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <fstream> 
#include <vector>

using namespace std;

#define   MAX_TEXTURES    100     // 最大的纹理数目

// 定义3D点的类，用于保存模型中的顶点 
class CVector3 
{ 
public: 
float x, y, z; 
};

// 定义2D点类，用于保存模型的UV纹理坐标 
class CVector2 
{ 
public: 
float x, y; 
};

// 面的结构定义 
struct tFace 
{ 
int vertIndex[4];    // 顶点索引 
int coordIndex[4];    // 纹理坐标索引 
};

// 材质信息结构体 
struct tMaterialInfo 
{ 
char strName[255];    // 材质名称 
char strFile[255];    // 如果存在纹理映射，则表示纹理文件名称 
BYTE color[3];     // 对象的RGB颜色 
int   texureId;     // 纹理ID 
float uTile;     // u 重复 
float vTile;     // v 重复 
float uOffset;       // u 纹理偏移 
float vOffset;     // v 纹理偏移 
} ;

// 对象信息结构体 
struct t3DObject 
{ 
int numOfVerts;    // 模型中顶点的数目 
int numOfFaces;    // 模型中面的数目 
int numTexVertex;    // 模型中纹理坐标的数目 
int materialID;    // 纹理ID 
bool bHasTexture;    // 是否具有纹理映射 
char strName[255];    // 对象的名称 
CVector3 *pVerts;    // 对象的顶点 
CVector3 *pNormals;   // 对象的法向量 
CVector2 *pTexVerts;   // 纹理UV坐标 
tFace *pFaces;     // 对象的面信息 
};

// 模型信息结构体 
struct t3DModel 
{ 
int numOfObjects;      // 模型中对象的数目 
int numOfMaterials;      // 模型中材质的数目 
vector<tMaterialInfo> pMaterials; // 材质链表信息 
vector<t3DObject> pObject;    // 模型中对象链表信息 
}; 

// 下面是读入obj文件的类 
class CLoadObj 
{ 
public:
CLoadObj();
~CLoadObj() {};
public: 
// 将obj文件中的信息读入到模型中 
bool ImportObj(t3DModel *pModel, char *strFileName); 

// 由于在obj文件中没有纹理/材质名称，只能手动设置 
// materialID是在模型材质列表中的索引号 
void SetObjectMaterial(t3DModel *pModel, int whichObject, int materialID); 

// 添加材质 
void AddMaterial(t3DModel *pModel, char *strName, char *strFile, 
      int r = 255,      int g = 255,   int b = 255);


protected:
// 读入对象，在ImportObj()中将调用该函数 
void ReadObjFile(t3DModel *pModel);

// 读入顶点信息，在ReadObjFile()中调用该函数 
void ReadVertexInfo();

// 读入面信息，在ReadObjFile()中调用该函数 
void ReadFaceInfo();

//获取每一行开头的关键字
void GetKeyWord(char* keyword);

// 完成面信息的读入之后调用该函数 
void FillInObjectInfo(t3DModel *pModel);

// 计算顶点的法向量 
void ComputeNormals(t3DModel *pModel); 
private: 

// 需要读入的文件指针 
FILE *m_FilePointer; 

// 顶点链表 
vector<CVector3> m_pVertices; 

// 面链表 
vector<tFace> m_pFaces; 

// UV坐标链表 
vector<CVector2> m_pTextureCoords; 

// 当前对象是否具有纹理坐标 
bool m_bObjectHasUV; 

// 当前读入的对象是否是面 
bool m_bJustReadAFace; 
}; 

#endif
