#include <stdio.h>
#include "GameStateList.h"
#include "System.h"
#include "Level1.h"
#include "AEEngine.h"
#include "AEInput.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

static GameObj* pHero;
Vector2D zero = { 0.0f, 0.0f };

void Load1(void)
{
	GameObjBase* pObjBase;
	theBaseList = NULL;
	// 初始化游戏对象基类的实例列表
	InitialGameObjBaseList(&theBaseList);

	// 初始化游戏对象类的实例列表

	// 创建基类的实例	

	// =======================
	// 背景(平台)：尺寸很小，简化成三角形定义
	// =======================

	AEGfxMeshStart();
	AEGfxTriAdd(
		-2.0f, 0.1f, COLOR_BACKGROUND, 0.0f, 0.0f,
		-2.0f, -0.1f, COLOR_BACKGROUND, 0.0f, 0.0f,
		2.0f, 0.1f, COLOR_BACKGROUND, 0.0f, 0.0f);
	AEGfxTriAdd(
		2.0f, -0.1f, COLOR_BACKGROUND, 0.0f, 0.0f,
		-2.0f, -0.1f, COLOR_BACKGROUND, 0.0f, 0.0f,
		2.0f, 0.1f, COLOR_BACKGROUND, 0.0f, 0.0f);

	CreateGameObjBase(TYPE_BACKGROUND, AEGfxMeshEnd(), theBaseList);

	// =========================
	// 主角：用12个三角形拼成一个“圆形”
	// =========================

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		1.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.866f, 0.5f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.866f, 0.5f, COLOR_PLAYER, 0.0f, 0.0f,
		0.5f, 0.866f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.5f, 0.866f, COLOR_PLAYER, 0.0f, 0.0f,
		0.0f, 1.0f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.0f, 1.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.5f, 0.866f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.5f, 0.866f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.866f, 0.5f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.866f, 0.5f, COLOR_PLAYER, 0.0f, 0.0f,
		-1.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-1.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.866f, -0.5f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.866f, -0.5f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.5f, -0.866f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		-0.5f, -0.866f, COLOR_PLAYER, 0.0f, 0.0f,
		0.0f, -1.0f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.0f, -1.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.5f, -0.866f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.5f, -0.866f, COLOR_PLAYER, 0.0f, 0.0f,
		0.866f, -0.5f, COLOR_PLAYER, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f,
		0.866f, -0.5f, COLOR_PLAYER, 0.0f, 0.0f,
		1.0f, 0.0f, COLOR_PLAYER, 0.0f, 0.0f);

	CreateGameObjBase(TYPE_PLAYER, AEGfxMeshEnd(), theBaseList);

	// ========================
	// 障碍物：3个三角形拼接的五边形
	// ========================

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.7f, -0.1f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.6f, 0.4f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.7f, -0.1f, 0xFFFFFF00, 0.0f, 0.0f,
		0.6f, 0.4f, 0xFFFFFF00, 0.0f, 0.0f,
		0.25f, -0.15f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.7f, -0.1f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.2f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.25f, -0.15f, 0xFFFFFF00, 0.0f, 0.0f);
	CreateGameObjBase(TYPE_BLOCK, AEGfxMeshEnd(), theBaseList);

}

void Ini1(void)
{
	
	GameObj* pObj;
	int i;
	
	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// 对象实例化：
	// 玩家对象实例化
	pHero = CreateGameObj(TYPE_PLAYER, SIZE_HERO, zero, zero, 0, theBaseList, 0, NULL);
	AE_ASSERT(pHero);
	// 背景对象实例化
	pObj = CreateGameObj(TYPE_BACKGROUND, SIZE_BACKGROUND, zero, zero, 0, theBaseList, 0, NULL);
	AE_ASSERT(pHero);
	// 小行星对象实例化 并 初始化
	for ( i = 0; i < BLOCK_NUM; i++)
	{
		// 实例化
		pObj = CreateGameObj(TYPE_BLOCK, SIZE_BLOCK, zero, zero, 0, theBaseList, 0, NULL);
		AE_ASSERT(pObj);

		// 初始化: 坐标位置 朝向和尺寸大小
		switch (i)
		{
			case 0: 
				pObj->posCurr.x = AEGfxGetWinMaxX();
				pObj->posCurr.y = 100.0f; 
				break;
			case 1:
				pObj->posCurr.x = 100.0f; 
				pObj->posCurr.y = AEGfxGetWinMaxY();
				break;
			case 2:
				pObj->posCurr.x = AEGfxGetWinMinX();
				pObj->posCurr.y = 100.0f;
				break;
		}
		// 朝向
		pObj->dirCurr = acosf( pObj->posCurr.x / ((float)sqrt(pObj->posCurr.x*pObj->posCurr.x + pObj->posCurr.y * pObj->posCurr.y)) ) - PI;

		pObj->scale = (i+1) * 10.0f;		
	}

}

void Free1(void)
{
	baseNode* pbaseNode = theBaseList->head;
	insNode* pinsNode;
	while (pbaseNode!=theBaseList->tail) {
		pinsNode = pbaseNode->gameobj_list->head->next;
		while(pinsNode!=pbaseNode->gameobj_list->tail){
			GameObjDelete(&(pinsNode->gameobj));
			pinsNode = pinsNode->next;
		}
		pbaseNode = pbaseNode->next;
	}
}

void Unload1(void)
{
	baseNode* pbaseNode = theBaseList->head;
	insNode* pinsNode;
	while (pbaseNode != theBaseList->tail) {
		DestroyGameObjList(&(pbaseNode->gameobj_list));
		AEGfxMeshFree(pbaseNode->gameobj_base.pMesh);
		pbaseNode = pbaseNode->next;
	}
	DestroyGameObjBaseList(theBaseList);

}
