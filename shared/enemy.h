#ifndef ENEMY_H
#define ENEMY_H

#pragma pack(push, 1)
typedef struct {
   int Version;
	int Count;
} EnemyHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	char Name[20];
	int Health;
} Enemy;
#pragma pack(pop)

#endif
