#pragma once

//小坐标偏移量
#define SMALL_POS_OFFSET 10000


//屏宽高
#define MAX_SCREEN_GRID 30

#define ROLE_TYPE 0

#define NPC_TYPE 1

#define SAFE_DELETE(ptr) if(ptr) {delete (ptr); (ptr) = NULL;}
#define SAFE_FREE(ptr)	if(ptr) { free(ptr); (ptr) = NULL;}



