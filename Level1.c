/* Project:		GProj_Asteroid
   File Name:	Level1.c
   Author:		闷声发大财
   Date:		
   Purpose:		关卡1  */

#include <stdio.h>
#include "GameStateList.h"
#include "System.h"
#include "Level1.h"
#include "AEEngine.h"
#include "Input.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
//Player
#define MOVE_VELOCITY_HERO 40.0f
#define MOVE_VELOCITY_ENEMY 75.0f
#define JUMP_VELOCITY 50.0f
#define GRAVITY 20.0f

Vector2D zero = { 0.0f, 0.0f };

// Player对象：因为是Player，所以单独声明，方便程序设计
static GameObj* pHero;

//jumpCheck:跳跃次数，用于二级跳
int jumpCheck = 0;


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

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
		-4.0f, 0.04f, COLOR_BACKGROUND, 0.0f, 0.0f,
		-4.0f, -0.04f, COLOR_BACKGROUND, 0.0f, 0.0f,
		4.0f, 0.04f, COLOR_BACKGROUND, 0.0f, 0.0f);
	AEGfxTriAdd(
		4.0f, -0.04f, COLOR_BACKGROUND, 0.0f, 0.0f,
		-4.0f, -0.04f, COLOR_BACKGROUND, 0.0f, 0.0f,
		4.0f, 0.04f, COLOR_BACKGROUND, 0.0f, 0.0f);

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
	Vector2D iniPosition_Player = {0.0f, 30.0f};
	Vector2D iniPosition_Block[3];
	iniPosition_Block[0].x = 70.0f;
	iniPosition_Block[0].y = 170.0f;
	iniPosition_Block[1].x = 120.0f;
	iniPosition_Block[1].y = 30.0f;
	iniPosition_Block[2].x = 40.0f;
	iniPosition_Block[2].y = 270.0f;


	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// 对象实例化：
	// 玩家对象实例化
	pHero = CreateGameObj(TYPE_PLAYER, SIZE_HERO, iniPosition_Player, zero, 0, theBaseList, 0, NULL);
	AE_ASSERT(pHero);
	// 背景对象实例化
	pObj = CreateGameObj(TYPE_BACKGROUND, SIZE_BACKGROUND, zero, zero, 0, theBaseList, 0, NULL);
	AE_ASSERT(pHero);
	// 小行星对象实例化 并 初始化
	for (i = 0; i < BLOCK_NUM; i++)
	{
		// 实例化
		pObj = CreateGameObj(TYPE_BLOCK, SIZE_BLOCK, iniPosition_Block[i], zero, 0, theBaseList, 0, NULL);
		AE_ASSERT(pObj);

		// 朝向
		pObj->dirCurr = acosf(pObj->posCurr.x / ((float)sqrt(pObj->posCurr.x*pObj->posCurr.x + pObj->posCurr.y * pObj->posCurr.y))) - PI;

		pObj->scale = (i + 1) * 10.0f;
	}

}

void Update1(void)
{
	unsigned long i;
	baseNode *pbasenode, *pbasenode2;
	insNode *pinsnode, *pinsnode2;
	GameObjList tlist, tlist2;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;

	// ==========================================================================================
	// 获取窗口四条边的坐标，当窗口发生移动或缩放，以下值会发生变化
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	// ======================
	// 帧时间：Unity中的dt
	// ======================
	frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// 游戏逻辑响应输入
	// =========================

	// 状态切换
	if (KeyPressed[KeyR] == TRUE)
	{
		Next = GS_Restart;
		return;
	}
	if (KeyPressed[KeyESC] == TRUE)
	{
		Next = GS_Quit;
		return;
	}
	if (KeyPressed[Key2] == TRUE)
	{
		Next = GS_L2;
		return;
	}

	// 控制玩家player左右移动 及 跳跃(匀速)
	if (KeyPressed[KeyRight] == TRUE)
	{
		pHero->velCurr.x = MOVE_VELOCITY_HERO;
	}
	else
		if (KeyPressed[KeyLeft] == TRUE)
		{
			pHero->velCurr.x = -MOVE_VELOCITY_HERO;
		}
		else
			pHero->velCurr.x = 0.f;
	if (KeyPressed[KeySpace] == TRUE)
	{

		if (jumpCheck<2)
			pHero->velCurr.y = JUMP_VELOCITY;
		jumpCheck++;
	}
	pHero->posCurr.x += frameTime * pHero->velCurr.x;
	pHero->posCurr.y += frameTime * pHero->velCurr.y;
	// Player跳起后的重力效应
	if (jumpCheck > 0)
		pHero->velCurr.y -= GRAVITY * frameTime;

	/*
	// 空格键射击(创建一个子弹对象)
	if (AEInputCheckTriggered(VK_SPACE))
	{
	GameObj* pBullet;

	// create a bullet
	pBullet = CreateGameObj(TYPE_BULLET, 3.0f, zero, zero, 0.0f, theBaseList, 0, NULL);
	AE_ASSERT(pBullet);
	pBullet->posCurr = spShip->posCurr;
	pBullet->dirCurr = spShip->dirCurr;
	}

	// M键发射导弹
	if (AEInputCheckTriggered('M'))
	{
	GameObj* spMissile;

	// 只允许生成一个导弹
	if (!flag_missile)
	{
	// 创建导弹对象
	spMissile = CreateGameObj(TYPE_MISSILE, 10.0f, zero, zero, 0.0f, theBaseList, 0, NULL);
	AE_ASSERT(spMissile);
	spMissile->posCurr = spShip->posCurr;
	spMissile->dirCurr = spShip->dirCurr;

	flag_missile = 1;
	}
	}
	*/
	/*

	// ==================================================
	// 更新所有其它（非player控制）活动对象的（位置等）
	// ==================================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
	tlist = pbasenode->gameobj_list;
	for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
	{
	GameObj* pInst = &(pinsnode->gameobj);
	Vector2D added;

	// 遇到非活动对象，不处理
	if ((pInst->flag & FLAG_ACTIVE) == 0)
	continue;

	// 更新小行星位置
	if (pInst->pObject->type == TYPE_ASTEROID)
	{
	Vector2DSet(&added, 5.0f * (float)(frameTime)* cosf(pInst->dirCurr), 5.0f * (float)(frameTime)* sinf(pInst->dirCurr));
	Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
	}

	// 更新子弹位置
	if (pInst->pObject->type == TYPE_BULLET)
	{
	Vector2DSet(&added, BULLET_SPEED * (float)(frameTime)* cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime)* sinf(pInst->dirCurr));
	Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
	}

	// 更新导弹位置
	if (pInst->pObject->type == TYPE_MISSILE)
	{
	Vector2DSet(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
	Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
	}
	}
	}

	// ===================================
	// 其它更新
	// Example:
	//		-- 超出屏幕在对向出现（飞船 小行星）
	//		-- 超出屏幕删除（子弹）
	//		-- 导弹：计算新方向
	// ===================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
	tlist = pbasenode->gameobj_list;
	for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
	{
	GameObj* pInst = &(pinsnode->gameobj);

	// 不理会非活动对象
	if ((pInst->flag & FLAG_ACTIVE) == 0)
	continue;

	// 飞船：Wrap
	if (pInst->pObject->type == TYPE_SHIP)
	{
	pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
	pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
	continue;
	}

	// 小行星：Wrap
	if (pInst->pObject->type == TYPE_ASTEROID)
	{
	pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
	pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
	continue;
	}

	// 删除超出屏幕边界的子弹
	if (pInst->pObject->type == TYPE_BULLET)
	{
	if ( (pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY) )
	pInst->flag = 0;
	continue;
	}

	// 如导弹目标已击毁，则更新导弹方向
	if ( pInst->pObject->type == TYPE_MISSILE )
	{
	GameObj* pTarget;
	int j;
	float angle;
	Vector2D newv;

	//if ( missile_target == -1 )
	//{
	//	// 搜索确定新目标
	//	for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
	//	{
	//		pTarget = sGameObjList + j;

	//		// 跳过非活动对象或非小行星对象
	//		if ((pTarget->flag & FLAG_ACTIVE) == 0 || pTarget->pObject->type != TYPE_ASTEROID)
	//			continue;
	//
	//		missile_target = j;
	//		break;
	//	}
	//
	//	// 找到目标
	//	if (missile_target > -1)
	//	{
	//		// 确定方向
	//		Vector2Sub(&newv,&(pTarget->posCurr),&(pInst->posCurr));
	//		if ( newv.x != 0)
	//			angle = atanf(newv.y/newv.x);
	//		else
	//			angle = 0;
	//
	//		// 差向量在第二、三象限
	//		if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0) )
	//			angle += PI;
	//		// 差向量在第四象限
	//		if ( newv.x > 0 && newv.y < 0)
	//			angle = 2*PI + angle;

	//		pInst->dirCurr = angle;
	//	}
	//	else
	//		// 销毁导弹
	//		pInst->flag = 0;
	//}
	}
	}
	}
	*/

	// ====================
	// 碰撞检测
	// ====================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			GameObj* pInstOther;

			// 不处理非活动对象
			if (pInst->flag == FLAG_INACTIVE)
				continue;

			// Player 与 background / block 的碰撞检测
			if (pInst->pObject->type == TYPE_PLAYER)
			{
				for (pbasenode2 = theBaseList->head->next; pbasenode2 != theBaseList->tail; pbasenode2 = pbasenode2->next)
				{
					tlist2 = pbasenode2->gameobj_list;
					for (pinsnode2 = tlist2->head->next; pinsnode2 != tlist2->tail; pinsnode2 = pinsnode2->next)
					{
						pInstOther = &(pinsnode2->gameobj);

						// 跳过非活动对象和自身
						if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_PLAYER))
							continue;

						// Player vs. Block
						if (pInstOther->pObject->type == TYPE_BLOCK)
						{
							// 碰撞检测
							if (StaticCircleToStaticCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
							{
								// 碰到BLOCK死亡
								// 重新开始关卡
								Next = GS_Restart;
								continue;
							}
						}

						// Player vs. Background
						if (pInstOther->pObject->type == TYPE_BACKGROUND)
						{
							//是否在平台上
							if ((pInst->posCurr.y - pInstOther->posCurr.y) <= (pInst->scale / 2 + pInstOther->scale / 2))
							{
								jumpCheck = 0;
								pInst->velCurr.y = 0;
								pInst->posCurr.y = pInstOther->posCurr.y + pInst->scale / 2 + pInstOther->scale / 2;
							}
						}
					}
				}
			}
		}
	}

	// =====================================
	// 计算所有对象的2D变换矩阵
	// =====================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			Matrix2D		 trans, rot, scale;

			// 不处理非活动对象
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// 缩放矩阵
			Matrix2DScale(&scale, pInst->scale, pInst->scale);
			// 旋转矩阵
			Matrix2DRotDeg(&rot, pInst->dirCurr);
			// 平移矩阵
			Matrix2DTranslate(&trans, pInst->posCurr.x, pInst->posCurr.y);
			// 以正确的顺序连乘以上3个矩阵形成2维变换矩阵
			Matrix2DConcat(&(pInst->transform), &trans, &rot);
			Matrix2DConcat(&(pInst->transform), &(pInst->transform), &scale);
		}
	}
}


void Draw1(void)
{
	baseNode *pbasenode;
	insNode *pinsnode;
	GameObjList tlist;
	char strBuffer[1024];
	unsigned long i;
	double frameTime;

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 逐个绘制对象列表中的所有对象
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);

			// 跳过非活动对象
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// 设置绘制模式(Color or texture)
			AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
			// 设置所有参数(Color blend, textures, etc..)
			AEGfxSetBlendMode(AE_GFX_RM_COLOR);
			// 设置对象的2D变换矩阵，使用函数：AEGfxSetTransform
			AEGfxSetTransform(pInst->transform.m);
			// 绘制当前对象，使用函数：AEGfxMeshDraw
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void Free1(void)
{
	int i = 0;
	baseNode *pbasenode;
	insNode *pinsnode;
	GameObjList tlist;

	// 使用函数gameObjDestroy删除列表中的对象
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			GameObjDelete(pInst);
		}
	}
}

void Unload1(void)
{
	DestroyGameObjBaseList(theBaseList);
}
