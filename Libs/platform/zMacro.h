#pragma once

//С����ƫ����
#define SMALL_POS_OFFSET 10000


//�����
#define MAX_SCREEN_GRID 30
//����
#define SCREEN_GRID_WIDTH MAX_SCREEN_GRID
//����
#define SCREEN_GRID_HEIGHT MAX_SCREEN_GRID

#define ROLE_TYPE 0

#define NPC_TYPE 1

#define SAFE_DELETE(ptr) if(ptr) {delete (ptr); (ptr) = NULL;}
#define SAFE_FREE(ptr)	if(ptr) { free(ptr); (ptr) = NULL;}



