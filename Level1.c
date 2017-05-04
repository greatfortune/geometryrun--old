/* Project:		GProj_Asteroid
   File Name:	Level1.c
   Author:		����
   Date:		
   Purpose:		�ؿ�1  */

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

#define SHIP_INITIAL_NUM			3		// �ɴ���lives��Ŀ
#define ASTEROID_NUM				3		// С������Ŀ
#define SHIP_SIZE					30.0f	// �ɴ��ߴ�
#define SHIP_ACCEL_FORWARD			50.0f	// �ɴ�ǰ����ٶ�(m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f	// �ɴ�������ٶ�(m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// �ɴ���ת�ٶ�(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// ������ת�ٶ�(degree/second)

#define BULLET_SPEED				100.0f		// �ӵ��ص�ǰ������ٶ� (m/s)

#define FLAG_ACTIVE					0x00000001  // ������־


// �ɴ�������Ϊ��Player�����Ե�������������������
static GameObj* spShip;

// ʣ��ķɴ�lives (lives 0 = game over)
static long	sShipLives;	

// the score = ���ٵ�С����asteroid����
static unsigned long	sScore;	

// ֻ����һ��Ѳ���������ɣ��������һ�����ɱ�־
int flag_missile = 0;
static int missile_target = -1;  // ����Ŀ���ڶ����б��е��±�

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// ����/ɾ����Ϸ����

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Load1(void)
{
	GameObjBase* pObjBase;
	theBaseList = NULL;
	// ��ʼ����Ϸ��������ʵ���б�
	InitialGameObjBaseList(&theBaseList);

	// ��ʼ����Ϸ�������ʵ���б�


	// ���������ʵ��	
	// =====================
	// �ɴ�
	// =====================
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0x01FF0000, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f); 
	CreateGameObjBase(TYPE_SHIP, AEGfxMeshEnd(), theBaseList);
	
	// =======================
	// �ӵ����ߴ��С���򻯳������ζ���
	// =======================

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	CreateGameObjBase(TYPE_BULLET, AEGfxMeshEnd(), theBaseList);

	// =========================
	// С���ǣ�������������ƴ��һ����Բ�Ρ�
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
	// ����������������ƴ�ӵ�����
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
	
	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ����ʵ��������Ϸ��ʼֻ�зɴ���С������Ҫʵ����
	// �ɴ�����ʵ����
	spShip = CreateGameObj(TYPE_SHIP, SHIP_SIZE, 0, 0, 0,theBaseList,0, NULL);
	AE_ASSERT(spShip);
	// С���Ƕ���ʵ���� �� ��ʼ��
	for ( i = 0; i < ASTEROID_NUM; i++)
	{
		// ʵ����
		pObj = CreateGameObj(TYPE_ASTEROID, 1.0f, 0, 0, 0, theBaseList, 0, NULL);
		AE_ASSERT(pObj);

		// ��ʼ��: ����λ�� ����ͳߴ��С
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
		// ����
		pObj->dirCurr = acosf( pObj->posCurr.x / ((float)sqrt(pObj->posCurr.x*pObj->posCurr.x + pObj->posCurr.y * pObj->posCurr.y)) ) - PI;

		pObj->scale = (i+1) * 10.0f;		
	}

	// �������ɴ���Ŀ��ʼ��
	sScore = 0;
	sShipLives = SHIP_INITIAL_NUM;

	// δ���ɵ���
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
	// ��ȡ���������ߵ����꣬�����ڷ����ƶ������ţ�����ֵ�ᷢ���仯
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	// ======================
	// ֡ʱ�䣺Unity�е�dt
	// ======================
	frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// ��Ϸ�߼���Ӧ����
	// =========================
	
	// ״̬�л�
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

	// �ɴ��ƶ�
	if (AEInputCheckCurr(VK_UP))
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);

		// �����˶�������Ҫ�����ٶ�
		spShip->velCurr.x += SHIP_ACCEL_FORWARD * (float)(frameTime);
		spShip->velCurr.y += SHIP_ACCEL_FORWARD * (float)(frameTime);
		// �������ٶȸ���λ��
		spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	if (AEInputCheckCurr(VK_DOWN))
	{
		AEVec2 added;
		AEVec2Set(&added, -cosf(spShip->dirCurr), -sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);

		// �ٶȸ���
		spShip->velCurr.x += SHIP_ACCEL_BACKWARD * (float)(frameTime);
		spShip->velCurr.y += SHIP_ACCEL_BACKWARD * (float)(frameTime);

		// λ�ø���
		spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		// ��ʱ����ת
		spShip->dirCurr += SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{
		// ˳ʱ����ת
		spShip->dirCurr -= SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	// �ո�����(����һ���ӵ�����)
	if (AEInputCheckTriggered(VK_SPACE))
	{
		GameObj* pBullet;

		// create a bullet
		pBullet = CreateGameObj(TYPE_BULLET, 3.0f, 0, 0, 0.0f, theBaseList, 0, NULL);
		AE_ASSERT(pBullet);
		pBullet->posCurr = spShip->posCurr;
		pBullet->dirCurr = spShip->dirCurr;
	}

	// M�����䵼��
	if (AEInputCheckTriggered('M'))
	{
		GameObj* spMissile;

		// ֻ��������һ������
		if (!flag_missile)
		{
			// ������������
			spMissile = CreateGameObj(TYPE_MISSILE, 10.0f, 0, 0, 0.0f, theBaseList, 0, NULL);
			AE_ASSERT(spMissile);
			spMissile->posCurr = spShip->posCurr;
			spMissile->dirCurr = spShip->dirCurr;

			flag_missile = 1;
		}
	}

	// ==================================================
	// ����������������player���ƣ������ģ�λ�õȣ�
	// ==================================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			AEVec2 added;

			// �����ǻ���󣬲�����
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// ����С����λ��
			if (pInst->pObject->type == TYPE_ASTEROID)
			{
				AEVec2Set(&added, 5.0f * (float)(frameTime)* cosf(pInst->dirCurr), 5.0f * (float)(frameTime)* sinf(pInst->dirCurr));
				AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
			}

			// �����ӵ�λ��
			if (pInst->pObject->type == TYPE_BULLET)
			{
				AEVec2Set(&added, BULLET_SPEED * (float)(frameTime)* cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime)* sinf(pInst->dirCurr));
				AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
			}

			// ���µ���λ��
			if (pInst->pObject->type == TYPE_MISSILE)
			{
				AEVec2Set(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
				AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
			}
		}
	}

	// ===================================
	// ��������
	// Example:
	//		-- ������Ļ�ڶ�����֣��ɴ� С���ǣ�
	//		-- ������Ļɾ�����ӵ���
	//		-- �����������·���
	// ===================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);

			// �����ǻ����
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;
		
			// �ɴ���Wrap
			if (pInst->pObject->type == TYPE_SHIP)
			{
				pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
				pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
				continue;
			}

			// С���ǣ�Wrap
			if (pInst->pObject->type == TYPE_ASTEROID)
			{
				pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
				pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
				continue;
			}

			// ɾ��������Ļ�߽���ӵ�
			if (pInst->pObject->type == TYPE_BULLET)
			{
				if ( (pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY) )
					pInst->flag = 0;
				continue;
			}

			// �絼��Ŀ���ѻ��٣�����µ�������
			if ( pInst->pObject->type == TYPE_MISSILE )
			{
				GameObj* pTarget;
				int j;
				float angle;
				AEVec2 newv;

				//if ( missile_target == -1 )
				//{
				//	// ����ȷ����Ŀ��
				//	for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
				//	{
				//		pTarget = sGameObjList + j;

				//		// �����ǻ������С���Ƕ���
				//		if ((pTarget->flag & FLAG_ACTIVE) == 0 || pTarget->pObject->type != TYPE_ASTEROID)
				//			continue;
				//	
				//		missile_target = j;
				//		break;
				//	}
				//
				//	// �ҵ�Ŀ��
				//	if (missile_target > -1)
				//	{
				//		// ȷ������
				//		AEVec2Sub(&newv,&(pTarget->posCurr),&(pInst->posCurr));
				//		if ( newv.x != 0)
				//			angle = atanf(newv.y/newv.x);
				//		else
				//			angle = 0;
				//
				//		// �������ڵڶ���������
				//		if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0) )
				//			angle += PI;
				//		// �������ڵ�������
				//		if ( newv.x > 0 && newv.y < 0)
				//			angle = 2*PI + angle;

				//		pInst->dirCurr = angle;
				//	}
				//	else
				//		// ���ٵ���
				//		pInst->flag = 0;
				//}
			}
		}
	}

	// ====================
	// ��ײ���
	// ====================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			GameObj* pInstOther;
			int j;

			// ������ǻ����
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// asteroid �� ship / bullet / missile ����ײ���
			if (pInst->pObject->type == TYPE_ASTEROID)
			{
				for (pbasenode2 = theBaseList->head->next; pbasenode2 != theBaseList->tail; pbasenode2 = pbasenode2->next)
				{
					tlist2 = pbasenode2->gameobj_list;
					for (pinsnode2 = tlist2->head->next; pinsnode2 != tlist2->tail; pinsnode2 = pinsnode2->next)
					{
						pInstOther = &(pinsnode2->gameobj);

						// �����ǻ�����С��������
						if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ASTEROID))
							continue;

						// asteroid vs. ship
						if (pInstOther->pObject->type == TYPE_SHIP)
						{
							// ��ײ���
							if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
							{
								// �ɴ�����
								sShipLives -= 1;

								if (sShipLives <= 0)
									// ���¿�ʼ�ؿ�
									Next = GS_Restart;
								else
								{
									// ����λ��
									pInstOther->posCurr.x = 100.0f;
									pInstOther->posCurr.y = winMinY;
								}
							}
							continue;
						}
						// asteroid vs. bullet
						if (pInstOther->pObject->type == TYPE_BULLET)
						{
							// ������ײ�������߶�����
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
	// �������ж����2D�任����
	// =====================================
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);
			AEMtx33		 trans, rot, scale;

			// ������ǻ����
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// ���ž���
			AEMtx33Scale(&scale, pInst->scale, pInst->scale);
			// ��ת����
			AEMtx33Rot(&rot, pInst->dirCurr);
			// ƽ�ƾ���
			AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
			// ����ȷ��˳����������3�������γ�2ά�任����
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

	// ������ƶ����б��е����ж���
	for (pbasenode = theBaseList->head->next; pbasenode != theBaseList->tail; pbasenode = pbasenode->next)
	{
		tlist = pbasenode->gameobj_list;
		for (pinsnode = tlist->head->next; pinsnode != tlist->tail; pinsnode = pinsnode->next)
		{
			GameObj* pInst = &(pinsnode->gameobj);

			// �����ǻ����
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// ���û���ģʽ(Color or texture)
			AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
			// �������в���(Color blend, textures, etc..)
			AEGfxSetBlendMode(AE_GFX_RM_COLOR);
			// ���ö����2D�任����ʹ�ú�����AEGfxSetTransform
			AEGfxSetTransform(pInst->transform.m);
			// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
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

	// ʹ�ú���gameObjDestroyɾ���б��еĶ���
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

	//// ж�ض�����״������Դ��ʹ�ú�����AEGfxMeshFree
	//for (i = 0; i < GAME_OBJ_BASE_NUM_MAX; i++)
	//{
	//	GameObjBase* pGamObjBase = sGameObjBaseList + i;
	//	AEGfxMeshFree(pGamObjBase->pMesh);
	//}
}
