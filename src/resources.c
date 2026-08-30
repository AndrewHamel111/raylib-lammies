#include "resources.h"
#include "utility.h"

#if USE_RRES

#include <stdlib.h>

#include "rres-raylib.h"

const char* rresPath = "bin/resources.rres";
rresCentralDir dir;
rresResourceChunk chunk;
static void RRES_Init()
{
	dir = rresLoadCentralDirectory(rresPath);

	if (dir.count == 0)
	{
		TraceLog(LOG_FATAL, "No Central Directory found, cannot load from RRES!");
	}

	TraceLog(LOG_INFO, TextFormat("Files found:"));
	for (int i = 0; i < dir.count; i++)
	{
		TraceLog(LOG_INFO, TextFormat("%s", dir.entries[i].fileName));
	}
}

void** music_data = NULL;
int music_data_count = 0;

#endif

Font fnt_receipt;
Font fnt_pop;

Music music_bgm;

#define SFX_DISCARD_COUNT 3
Sound sfx_discard[SFX_DISCARD_COUNT];
int discard_last_index = 0;

#define SFX_SLIDE_COUNT 6
Sound sfx_slide[SFX_SLIDE_COUNT];
int slide_last_index = 0;

#define SFX_DOORBELL_COUNT 3
Sound sfx_doorbell[SFX_DOORBELL_COUNT];
int doorbell_last_index = 0;

static Texture2D LoadTextureInternal(const char* path)
{
#if USE_RRES
	chunk = rresLoadResourceChunk(rresPath, rresGetResourceId(dir, path));
	UnpackResourceChunk(&chunk);
	Image image = LoadImageFromResource(chunk);
	rresUnloadResourceChunk(chunk);
#else
	Image image = LoadImage(TextFormat("%s%s", "../", path));
#endif

	return LoadTextureFromImage(image);
}

static Music LoadMusicInternal(const char* path)
{
#if USE_RRES
	chunk = rresLoadResourceChunk(rresPath, rresGetResourceId(dir, path));
	unsigned int size = 0;
	void* data = LoadDataFromResource(chunk, &size);
	int newCount = music_data_count + 1;
	music_data = MemRealloc(music_data, sizeof(void*) * newCount);
	music_data[music_data_count] = MemAlloc(size);
	memcpy(music_data[music_data_count], data, size);
	Music music = LoadMusicStreamFromMemory(".wav", music_data[music_data_count], size);
	music_data_count = newCount;
	MemFree(data);
	rresUnloadResourceChunk(chunk);
	return music;
#else
	return LoadMusicStream(TextFormat("%s%s", "../", path));
#endif
}

static Sound LoadSoundInternal(const char* path)
{
#if USE_RRES
	chunk = rresLoadResourceChunk(rresPath, rresGetResourceId(dir, path));
	int result = UnpackResourceChunk(&chunk);
	Wave wave = LoadWaveFromResource(chunk);
	rresUnloadResourceChunk(chunk);
#else
	Wave wave = LoadWave(TextFormat("%s%s", "../", path));
#endif

	return LoadSoundFromWave(wave);
}

static Font LoadFontInternal(const char* path)
{
#if USE_RRES
	// .ttf required to be "RAW" in rrespacker!
	chunk = rresLoadResourceChunk(rresPath, rresGetResourceId(dir, path));
	unsigned int size;
	void* data = LoadDataFromResource(chunk, &size);
	Font font = LoadFontFromMemory(".ttf", data, size, 128, NULL, 0);
	MemFree(data);
	rresUnloadResourceChunk(chunk);
	return font;
#else
	return LoadFontEx(TextFormat("%s%s", "../", path), 128, NULL, 0);
#endif
}

void LoadUIAssets(void)
{
	fnt_receipt = LoadFontInternal("resources/BMREA___.TTF");
	fnt_pop = LoadFontInternal("resources/gomarice_rockin_record.ttf");
}

void LoadAudio(void)
{
	music_bgm = LoadMusicInternal("resources/sound/music.wav");

//	sfx_example = LoadSoundInternal("resources/sound/example.ogg");
//	SetSoundVolume(sfx_example, 0.65f);
}

void LoadRandomizableSounds(void)
{
	for (int i = 0; i < SFX_DISCARD_COUNT; i++)
	{
		sfx_discard[i] = LoadSoundInternal(TextFormat("resources/sound/discard/%d.ogg", i + 1));
	}

	for (int i = 0; i < SFX_SLIDE_COUNT; i++)
	{
		sfx_slide[i] = LoadSoundInternal(TextFormat("resources/sound/slide/%d.ogg", i + 1));
	}

	for (int i = 0; i < SFX_DOORBELL_COUNT; i++)
	{
		sfx_doorbell[i] = LoadSoundInternal(TextFormat("resources/sound/bell/%d.ogg", i + 1));
	}
}

void LoadResources(void)
{
#if USE_RRES
	RRES_Init();
#endif

	LoadUIAssets();

	LoadAudio();
	LoadRandomizableSounds();
}

Font GetReceiptFont(void)
{
	return GetFontDefault();
	//return fnt_receipt;
}

Font GetPopFont(void)
{
	return fnt_pop;
}

Music GetBGM(void)
{
	return music_bgm;
}

Sound GetRandomizedSFX(Sound* array, int count, int* lastIndex)
{
	int index = (*lastIndex + GetRandomValue(1, count - 1)) % count;
	*lastIndex = index;
	return array[index];
}

Sound GetSlideSFX(void)
{
	return GetRandomizedSFX(sfx_slide, SFX_SLIDE_COUNT, &slide_last_index);
}

Sound GetDiscardSFX(void)
{
	return GetRandomizedSFX(sfx_discard, SFX_DISCARD_COUNT, &discard_last_index);
}

Sound GetDoorbellSFX(void)
{
	return GetRandomizedSFX(sfx_doorbell, SFX_DOORBELL_COUNT, &doorbell_last_index);
}
