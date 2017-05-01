/* Project:		GeometryRun
   File Name:	GameObjectManager.h
   Author:		�Ƽ�ά
   Date:
   Purpose:		��Ϸ������� */

#ifndef _GameObjectManager
#define _GameObjectManager

#define ERROR -1
#define OK 1
#define FLAG_ACTIVE 1
#define FLAG_INACTIVE 0

#include <stdio.h>
#include <stdbool.h>
#include "GameStateList.h"
#include "System.h"
#include "AEEngine.h"
#include "GameObjectManager.h"

typedef int Status;

#define MAXPROPERTIES 5

typedef struct
{
	char name[10];
	int value;
}Property;

enum objType
{
	player = 0,
	monster,
	block,
	boss
};

// ��Ϸ�������/�ṹ
typedef struct
{
	unsigned long		type;		// ��Ϸ��������
	AEGfxVertexList*	pMesh;		// ��״
}GameObjBase;

// ��Ϸ������/�ṹ
typedef struct
{
	GameObjBase*		pObject;	// ָ����ࣨԭʼ��״�����ͣ�
	unsigned long		flag;		// ���־
	float				scale;		// �ߴ�
	AEVec2				posCurr;	// ��ǰλ��
	AEVec2				velCurr;	// ��ǰ�ٶ�
	float				dirCurr;	// ��ǰ����
	AEMtx33				transform;	// �任����ÿһ֡����ҪΪÿһ���������
	Property			properties[MAXPROPERTIES]; // �ö��������
	int					propertyCount;			   // �ö������Եĸ���
}GameObj;

typedef struct INSNODE
{
	GameObj gameobj;
	struct INSNODE *next;
	struct INSNODE *pre;
}insNode;

typedef struct
{
	int count;
	insNode *head;
	insNode *tail;
}*GameObjList;

typedef struct BASENODE
{
	GameObjBase gameobj_base;
	struct BASENODE *next;
	struct BASENODE *pre;
	GameObjList gameobj_list;
}baseNode;

typedef struct
{
	int count;
	baseNode *head;
	baseNode *tail;
}*GameObjBaseList;


// ��Ϸ��������
static GameObjBaseList theBaseList;

Status InitialGameObjList(GameObjList *L);

Status InitialGameObjBaseList(GameObjBaseList *L);

Status DestroyGameObjList(GameObjList *L);

Status DestroyGameObjBaseList(GameObjBaseList *L);

Status ClearGameObjList(GameObjList L);

Status ClearGameObjBaseList(GameObjBaseList L);

bool ListIsEmpty(GameObjList L);

bool BaseListIsEmpty(GameObjBaseList L);

int ListLength(GameObjList L);

int BaseListLength(GameObjBaseList L);

GameObj* CreateGameObj(unsigned long theType, float scale, AEVec2* pPos, AEVec2* pVel, float dir, GameObjBaseList L, int thePropertyCount, Property* theProperties);

Status CreateGameObjBase(unsigned long theType, AEGfxVertexList* theMesh, GameObjBaseList L);

Status GameObjDelete(GameObj* theGameObj);

Status ListTraverse(GameObjList L, void(*visit)());

Status BaseListTraverse(GameObjBaseList L, void(*visit)());

#endif 
