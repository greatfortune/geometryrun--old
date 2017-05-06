/* Project:		GeometryRun
File Name:	GameObjectManager.c
Author:		黄嘉维
Date:
Purpose:		游戏对象管理 */

#include "GameObjectManager.h"

static baseNode* GetBaseNodeWithType(unsigned long theType, GameObjBaseList L);

Status InitialGameObjList(GameObjList *L)
{
	*L = (GameObjList *)malloc(sizeof(GameObjList*));
	if (!*L)
		return ERROR;
	(*L)->head = (insNode *)malloc(sizeof(insNode));
	(*L)->tail = (insNode *)malloc(sizeof(insNode));
	AE_ASSERT_MESG((*L)->head, "Fail to malloc");
	AE_ASSERT_MESG((*L)->tail, "Fail to malloc");
	(*L)->head->next = (*L)->tail;
	(*L)->head->pre = (*L)->tail;
	(*L)->tail->next = (*L)->head;
	(*L)->tail->pre = (*L)->head;
	(*L)->count = 0;
	return OK;
}

Status InitialGameObjBaseList(GameObjBaseList *L)
{
	*L = (GameObjBaseList *)malloc(sizeof(GameObjBaseList*));
	if (!*L)
		return ERROR;
	(*L)->head = (baseNode *)malloc(sizeof(baseNode));
	(*L)->tail = (baseNode *)malloc(sizeof(baseNode));
	AE_ASSERT_MESG((*L)->head, "Fail to malloc");
	AE_ASSERT_MESG((*L)->tail, "Fail to malloc");
	(*L)->head->next = (*L)->tail;
	(*L)->head->pre = (*L)->tail;
	(*L)->tail->next = (*L)->head;
	(*L)->tail->pre = (*L)->head;
	(*L)->count = 0;
	return OK;
}

Status DestroyGameObjList(GameObjList *L)
{
	insNode *pt1 = (*L)->head, *pt2 = pt1;
	while (pt2 != (*L)->tail)
	{
		pt2 = pt1->next;
		free(pt1);
		pt1 = pt2;
	}
	free(pt2);
	free(*L);
	return OK;
}

Status DestroyGameObjBaseList(GameObjBaseList *L)
{
	baseNode *pt1 = (*L)->head, *pt2 = pt1;
	while (pt2 != (*L)->tail)
	{
		pt2 = pt1->next;
		DestroyGameObjList(pt1->gameobj_list);
		free(pt1);
		pt1 = pt2;
	}
	free(pt2);
	free(*L);
	return OK;
}

Status ClearGameObjList(GameObjList L)
{
	insNode *pt1 = L->head->next, *pt2 = pt1;
	while (pt2 != L->tail)
	{
		pt2 = pt1->next;
		free(pt1);
		pt1 = pt2;
	}
	L->head->next = L->tail;
	L->count = 0;
	return OK;
}

Status ClearGameObjBaseList(GameObjBaseList L)
{
	baseNode *pt1 = L->head->next, *pt2 = pt1;
	while (pt2 != L->tail)
	{
		pt2 = pt1->next;
		DestroyGameObjList(pt1->gameobj_list);
		free(pt1);
		pt1 = pt2;
	}
	L->head->next = L->tail;
	L->count = 0;
	return OK;
}

bool ListIsEmpty(GameObjList L)
{
	if (L->head->next == L->tail)
		return true;
	else
		return false;
}

bool BaseListIsEmpty(GameObjBaseList L)
{
	if (L->head->next == L->tail)
		return true;
	else
		return false;
}

int ListLength(GameObjList L)
{
	return L->count;
}

int BaseListLength(GameObjBaseList L)
{
	return L->count;
}

Status GameObjDelete(GameObj* theGameObj)
{
	AE_ASSERT_MESG(theGameObj->flag, "Trying to delete a inactive gameobject");
	theGameObj->flag = FLAG_INACTIVE;

	return OK;
}

//遍历函数，可能无用
Status ListTraverse(GameObjList L, void(*visit)())
{
	insNode *pt = L->head->next;
	while (pt != L->tail)
	{
		visit(&(pt));
		pt = pt->next;
	}
	return OK;
}

//遍历函数，可能无用
Status BaseListTraverse(GameObjBaseList L, void(*visit)())
{
	baseNode *pt = L->head->next;
	while (pt != L->tail)
	{
		visit(&(pt));
		pt = pt->next;
	}
	return OK;
}


static baseNode* GetBaseNodeWithType(unsigned long theType, GameObjBaseList L)
{
	baseNode *pBaseNode = NULL;
	for (pBaseNode = L->head->next;; pBaseNode = pBaseNode->next)
	{
		if (pBaseNode == L->tail)
			return NULL;
		else if (pBaseNode->gameobj_base.type == theType)
			return pBaseNode;
	}
}

//创建新实例对象
GameObj* CreateGameObj(unsigned long theType, float scale, Vector2* pPos, Vector2* pVel, float dir, GameObjBaseList L, int thePropertyCount, Property* theProperties)
{
	Vector2 zero = { 0.0f, 0.0f };
	baseNode *pBaseNode = GetBaseNodeWithType(theType, L);

	insNode *pt1 = pBaseNode->gameobj_list->head, *pt2 = pt1->next, *pInstNode = NULL;
	int flag = 0, i;

	// 找非活动对象的位置
	for (pt1 = pBaseNode->gameobj_list->head->next; pt1 != pBaseNode->gameobj_list->tail; pt1 = pt1->next)
	{
		if (pt1->gameobj.flag == FLAG_INACTIVE)
		{
			flag = 1;
			break;
		}
	}
	if (flag)
		pInstNode = pt1;
	else
	{
		pInstNode = (insNode *)malloc(sizeof(insNode));
		AE_ASSERT_MESG(pInstNode, "Fail to malloc");
	}
	pt1->next = pInstNode;
	pInstNode->next = pt2;
	pInstNode->gameobj.pObject = &(pBaseNode->gameobj_base);
	pInstNode->gameobj.flag = FLAG_ACTIVE;
	pInstNode->gameobj.scale = scale;
	pInstNode->gameobj.posCurr = pPos ? *pPos : zero;
	pInstNode->gameobj.velCurr = pVel ? *pVel : zero;
	pInstNode->gameobj.dirCurr = dir;
	pInstNode->gameobj.propertyCount = thePropertyCount;
	for (i = 0; i < thePropertyCount; i++)
		pInstNode->gameobj.properties[i] = theProperties[i];
	// 返回新创建的对象实例
	return &(pInstNode->gameobj);
}

//创建新基类
Status CreateGameObjBase(unsigned long theType, AEGfxVertexList* theMesh, GameObjBaseList L)
{
	baseNode *pBaseNode = (baseNode *)malloc(sizeof(baseNode));
	baseNode *pt1 = L->head, *pt2 = pt1->next;
	AE_ASSERT_MESG(pBaseNode, "Fail to malloc");

	pt1->next = pBaseNode;
	pBaseNode->next = pt2;

	pBaseNode->gameobj_base.type = theType;
	pBaseNode->gameobj_base.pMesh = theMesh;

	return OK;
}

