/* Project:		GProj_Asteroid
   File Name:	Level1.c
   Author:		刘芳
   Date:		
   Purpose:		关卡1  */

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

#define SHIP_INITIAL_NUM			3		// 飞船的lives数目
#define ASTEROID_NUM				3		// 小行星数目
#define SHIP_SIZE					30.0f	// 飞船尺寸
#define SHIP_ACCEL_FORWARD			50.0f	// 飞船前向加速度(m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f	// 飞船后向加速度(m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// 飞船旋转速度(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// 导弹旋转速度(degree/second)

#define BULLET_SPEED				100.0f		// 子弹沿当前方向的速度 (m/s)

#define FLAG_ACTIVE					0x00000001  // 活动对象标志


// 飞船对象：因为是Player，所以单独声明，方便程序设计
static GameObj* spShip;

// 剩余的飞船lives (lives 0 = game over)
static long	sShipLives;	

// the score = 击毁的小行星asteroid个数
static unsigned long	sScore;	

// 只允许一个巡航导弹生成，因此设置一个生成标志
int flag_missile = 0;
static int missile_target = -1;  // 导弹目标在对象列表中的下标

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// 创建/删除游戏对象

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
	// =====================
	// 飞船
	// =====================
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0x01FF0000, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f); 
	CreateGameObjBase(TYPE_SHIP, AEGfxMeshEnd(), theBaseList);
	
	// =======================
	// 子弹：尺寸很小，简化成三角形定义
	// =======================

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	CreateGameObjBase(TYPE_BULLET, AEGfxMeshEnd(), theBaseList);

	// =========================
	// 小行星：用六个三角形拼成一个“圆形”
	// =========================

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.5f,  0.0f, 0xFF0000FF, 0.0f, 0.0f,
		0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		-0.5f, 0.0f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.5f,  0.0f, 0xFF0000FF, 0.0f, 0.0f,
		0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		-0.5f, 0.0f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f);

	CreateGameObjBase(TYPE_ASTEROID, AEGfxMeshEnd(), theBaseList);
	// ========================
	// 导弹：两个三角形拼接的菱形
	// ========================

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.5f,  0.0f, 0xFFFFFF00, 0.0f, 0.0f, 
		0.0f,  0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f,  0.0f, 0xFFFFFF00, 0.0f, 0.0f, 
		 0.0f,   0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		 0.0f,  -0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	CreateGameObjBase(TYPE_MISSILE, AEGfxMeshEnd(), theBaseList);
}

void Ini1(void)
{
	GameObj* pObj;
	int i;
	
	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// 对象实例化：游戏开始只有飞船和小行星需要实例化
	// 飞船对象实例化
	spShip = CreateGameObj(TYPE_SHIP, SHIP_SIZE, 0, 0, 0,theBaseList,0, NULL);
	AE_ASSERT(spShip);
	// 小行星对象实例化 并 初始化
	for ( i = 0; i < ASTEROID_NUM; i++)
	{
		// 实例化
		pObj = CreateGameObj(TYPE_ASTEROID, 1.0f, 0, 0, 0, theBaseList, 0, NULL);
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

	// 分数及飞船数目初始化
	sScore = 0;
	sShipLives = SHIP_INITIAL_NUM;

	// 未生成导弹
	flag_missile = 0;
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
	if(AEInputCheckTriggered('R'))
	{
		Next = GS_Restart;
		return;
	}
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		Next = GS_Quit;
		return;
	}
	if(AEInputCheckTriggered('2'))
	{
		Next = GS_L2;
		return;
	}

	// 飞船移动
	if (AEInputCheckCurr(VK_UP))
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);

		// 加速运动，所以要更新速度
		spShip->velCurr.x += SHIP_ACCEL_FORWARD * (float)(frameTime);
		spShip->velCurr.y += SHIP_ACCEL_FORWARD * (float)(frameTime);
		// 根据新速度更新位置
		spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	if (AEInputCheckCurr(VK_DOWN))
	{
		AEVec2 added;
		AEVec2Set(&added, -cosf(spShip->dirCurr), -sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);

		// 速度更新
		spShip->velCurr.x += SHIP_ACCEL_BACKWARD * (float)(frameTime);
		spShip->velCurr.y += SHIP_ACCEL_BACKWARD * (float)(frameTime);

		// 位置更新
		spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		// 逆时针旋转
		spShip->dirCurr += SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{
		// 顺时针旋转
		spShip->dirCurr -= SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	// 空格键射击(创建一个子弹对象)
	if (AEInputCheckTriggered(VK_SPACE))
	{
		GameObj* pBullet;

		// create a bullet
		pBullet = CreateGameObj(TYPE_BULLET, 3.0f, 0, 0, 0.0f, theBaseList, 0, NULL);
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
			spMissile = CreateGameObj(TYPE_MISSILE, 10.0f, 0, 0, 0.0f, theBaseList, 0, NULL);
			AE_ASSERT(spMissile);
			spMissile->posCurr = spShip->posCurr;
			spMissile->dirCurr = spShip->dirCurr;

			flag_missile = 1;
		}
	}

	// ==================================================
	// 更新所有其它（非player控制）活动对象的（位置等）
	// ==================================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			AEVec2 added;

			// 遇到非活动对象，不处理
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// 更新小行星位置
			if (pInst->pObject->type == TYPE_ASTEROID)
			{
				AEVec2Set(&added, 5.0f * (float)(frameTime)* cosf(pInst->dirCurr), 5.0f * (float)(frameTime)* sinf(pInst->dirCurr));
				AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
			}

			// 更新子弹位置
			if (pInst->pObject->type == TYPE_BULLET)
			{
				AEVec2Set(&added, BULLET_SPEED * (float)(frameTime)* cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime)* sinf(pInst->dirCurr));
				AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
			}

			// 更新导弹位置
			if (pInst->pObject->type == TYPE_MISSILE)
			{
				AEVec2Set(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
				AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
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
				AEVec2 newv;

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
				//		AEVec2Sub(&newv,&(pTarget->posCurr),&(pInst->posCurr));
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
			int j;

			// 不处理非活动对象
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// asteroid 与 ship / bullet / missile 的碰撞检测
			if (pInst->pObject->type == TYPE_ASTEROID)
			{
				for (pbasenode2 = theBaseList->head->next; pbasenode2 != theBaseList->tail; pbasenode2 = pbasenode2->next)
				{
					tlist2 = pbasenode2->gameobj_list;
					for (pinsnode2 = tlist2->head->next; pinsnode2 != tlist2->tail; pinsnode2 = pinsnode2->next)
					{
						pInstOther = &(pinsnode2->gameobj);

						// 跳过非活动对象和小行星自身
						if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ASTEROID))
							continue;

						// asteroid vs. ship
						if (pInstOther->pObject->type == TYPE_SHIP)
						{
							// 碰撞检测
							if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
							{
								// 飞船击毁
								sShipLives -= 1;

								if (sShipLives <= 0)
									// 重新开始关卡
									Next = GS_Restart;
								else
								{
									// 更新位置
									pInstOther->posCurr.x = 100.0f;
									pInstOther->posCurr.y = winMinY;
								}
							}
							continue;
						}
						// asteroid vs. bullet
						if (pInstOther->pObject->type == TYPE_BULLET)
						{
							// 发生碰撞，则两者都销毁
							if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
							{
								pInstOther->flag = 0;
								pInst->flag = 0;
								sScore += 100;
							}
							continue;
						}
						// asteroid vs. missile
						if (pInstOther->pObject->type == TYPE_MISSILE)
						{
							// collision detection
							if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
							{
								pInst->flag = 0;
								missile_target = -1;
								sScore += 500;
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
			AEMtx33		 trans, rot, scale;

			// 不处理非活动对象
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// 缩放矩阵
			AEMtx33Scale(&scale, pInst->scale, pInst->scale);
			// 旋转矩阵
			AEMtx33Rot(&rot, pInst->dirCurr);
			// 平移矩阵
			AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
			// 以正确的顺序连乘以上3个矩阵形成2维变换矩阵
			AEMtx33Concat(&(pInst->transform), &trans, &rot);
			AEMtx33Concat(&(pInst->transform), &(pInst->transform), &scale);
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
	int i = 0;

	//// 卸载对象形状定义资源，使用函数：AEGfxMeshFree
	//for (i = 0; i < GAME_OBJ_BASE_NUM_MAX; i++)
	//{
	//	GameObjBase* pGamObjBase = sGameObjBaseList + i;
	//	AEGfxMeshFree(pGamObjBase->pMesh);
	//}
}
