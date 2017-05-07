/* Project:		GeometryRun
File Name:	GameObjectManager.h
Author:		黄嘉维
Date:
Purpose:		游戏对象管理 */

#ifndef _GameObjectManager
#define _GameObjectManager

#define OK 1
#define FLAG_ACTIVE 1
#define FLAG_INACTIVE 0

#include <stdio.h>
#include <stdbool.h>
#include "GameStateList.h"
#include "System.h"
#include "AEEngine.h"
#include "GameObjectManager.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "Math2D.h"

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
	background,
	monster,
	block,
	boss
};

// 游戏对象基类/结构
typedef struct
{
	unsigned long		type;		// 游戏对象类型
	AEGfxVertexList*	pMesh;		// 形状
}GameObjBase;

// 游戏对象类/结构
typedef struct
{
	GameObjBase*		pObject;	// 指向基类（原始形状和类型）
	unsigned long		flag;		// 活动标志
	float				scale;		// 尺寸
	Vector2D			posCurr;	// 当前位置
	Vector2D			velCurr;	// 当前速度
	float				dirCurr;	// 当前方向
	Matrix2D			transform;	// 变换矩阵：每一帧都需要为每一个对象计算
	Property			properties[MAXPROPERTIES]; // 该对象的属性
	int					propertyCount;			   // 该对象属性的个数
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


// 游戏对象链表
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

GameObj* CreateGameObj(unsigned long theType, float scale, Vector2D Pos, Vector2D Vel, float dir, GameObjBaseList L, int thePropertyCount, Property* theProperties);

Status CreateGameObjBase(unsigned long theType, AEGfxVertexList* theMesh, GameObjBaseList L);

Status GameObjDelete(GameObj* theGameObj);

Status ListTraverse(GameObjList L, void(*visit)());

Status BaseListTraverse(GameObjBaseList L, void(*visit)());

#endif 
