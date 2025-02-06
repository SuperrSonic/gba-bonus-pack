#include <ogcsys.h>
#include <asndlib.h>

#ifdef WANT_MUSIC
#include "oggplayer.h"
#endif
#include "sound.h"

void Sound_Init(void)
{
	/* Init ASND */
	ASND_Init();
}

s32 Sound_Play_BGM_Main(u8* data, int size)
{
#ifdef WANT_MUSIC
	SetVolumeOgg(200);

	return PlayOgg(data, size, 0, OGG_INFINITE_TIME);
#else
	return 0;
#endif
}

/*
s32 Sound_Play(void)
{
	// Play background music
	return PlayOgg(bgMusic, bgMusic_Len, 0, OGG_INFINITE_TIME);
} */

void Sound_Stop(void)
{
#ifdef WANT_MUSIC
	/* Stop background music */
	if (Sound_IsPlaying())
		StopOgg();
#endif
}

s32 Sound_IsPlaying(void)
{
#ifdef WANT_MUSIC
	/* Check if background music is playing */
	return (StatusOgg() == OGG_STATUS_RUNNING);
#else
	return 0;
#endif
}
