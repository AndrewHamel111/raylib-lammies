#include "resources.h"
#include "card/resources.h"
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

Texture2D tex_card_small[4][13];
Texture2D tex_card_small_extra[3]; // Back, Joker Black, Joker Red
Texture2D tex_card_large[4][13];
Texture2D tex_card_large_extra[3]; // Back, Joker Black, Joker Red

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

static void LoadUIAssets(void)
{
	fnt_receipt = LoadFontInternal("resources/BMREA___.TTF");
	fnt_pop = LoadFontInternal("resources/gomarice_rockin_record.ttf");
}

static void LoadAudio(void)
{
	music_bgm = LoadMusicInternal("resources/sound/music.wav");

//	sfx_example = LoadSoundInternal("resources/sound/example.ogg");
//	SetSoundVolume(sfx_example, 0.65f);
}

static void LoadRandomizableSounds(void)
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

static void LoadCardTextures(void)
{
	for (int suit = 0; suit < 4; suit++)
	{
		const char* suitName = SuitName_Lower(suit);
		for (int rank = 0; rank < 13; rank++)
		{
			tex_card_small[suit][rank] = LoadTextureInternal(TextFormat("resources/textures/cards/small/%s/%d.png", suitName, rank));
			tex_card_large[suit][rank] = LoadTextureInternal(TextFormat("resources/textures/cards/large/%s/%d.png", suitName, rank));
		}
	}

	tex_card_small_extra[0] = LoadTextureInternal("resources/textures/cards/small/back.png");
	tex_card_small_extra[1] = LoadTextureInternal("resources/textures/cards/small/joker_black.png");
	tex_card_small_extra[2] = LoadTextureInternal("resources/textures/cards/small/joker_red.png");

	tex_card_large_extra[0] = LoadTextureInternal("resources/textures/cards/large/back.png");
	tex_card_large_extra[1] = LoadTextureInternal("resources/textures/cards/large/joker_black.png");
	tex_card_large_extra[2] = LoadTextureInternal("resources/textures/cards/large/joker_red.png");
}

void LoadResources(void)
{
#if USE_RRES
	RRES_Init();
#endif

	LoadUIAssets();
	LoadCardTextures();

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

Texture2D GetCardSmall(const Card* card)
{
	if (card->suit < 0 || card->suit > 3)
	{
		TraceLog(LOG_WARNING, "GetCardSmall failed due to invalid suit!");
		return (Texture2D){0};
	}

	if (card->rank < Joker || card->rank > King)
	{
		TraceLog(LOG_WARNING, "GetCardSmall failed due to invalid rank!");
		return (Texture2D){0};
	}

	if (card->rank == Joker)
	{
		int suit = (card->suit == Diamonds || card->suit == Hearts) ? 2 : 1;
		return tex_card_small_extra[suit];
	}

	return tex_card_small[card->suit][card->rank];
}

Texture2D GetCardLarge(const Card* card)
{
	if (card->suit < 0 || card->suit > 3)
	{
		TraceLog(LOG_WARNING, "GetCardLarge failed due to invalid suit!");
		return (Texture2D){0};
	}

	if (card->rank < Joker || card->rank > King)
	{
		TraceLog(LOG_WARNING, "GetCardLarge failed due to invalid rank!");
		return (Texture2D){0};
	}

	if (card->rank == Joker)
	{
		int suit = (card->suit == Diamonds || card->suit == Hearts) ? 2 : 1;
		return tex_card_large_extra[suit];
	}

	return tex_card_large[card->suit][card->rank];
}

Texture2D GetCardBackSmall(void)
{
	return tex_card_small_extra[0];
}

Texture2D GetCardBackLarge(void)
{
	return tex_card_large_extra[0];
}

Texture2D GetCardJokerSmall(bool red)
{
	return red ? tex_card_small_extra[2] : tex_card_small_extra[1] ;
}

Texture2D GetCardJokerLarge(bool red)
{
	return red ? tex_card_large_extra[2] : tex_card_large_extra[1] ;
}

Rectangle GetCardSourceSmall(void)
{
	return R(6, 2, 20, 29);
}

Rectangle GetCardSourceLarge(void)
{
	return R(11, 2, 42, 60);
}

Texture2D GetCardValue(Suit suit, Rank rank, bool small)
{
	Texture2D result;
	return result;
}
