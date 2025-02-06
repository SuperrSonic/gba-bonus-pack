#ifndef _SOUND_H_
#define _SOUND_H_

/* Prototypes */
void Sound_Init(void);
s32  Sound_Play(void);
s32  Sound_Play_Ring(void);
s32  Sound_Play_Coin(void);
s32  Sound_Play_Rupee(void);
s32  Sound_Play_Cashin1(void);
s32 Sound_Play_BGM_Main(u8* data, int size);
void Sound_Stop(void);
s32  Sound_IsPlaying(void);

#endif

