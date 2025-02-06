/* Copyright (c) 2022-2025 Diego A.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gccore.h>
#include <ogc/machine/processor.h>
#include <fat.h>

#include "so_dtt_ogg.h"

#ifdef HW_RVL
#include <sdcard/wiisd_io.h>
#else
#include <sdcard/gcsd.h>
#endif

#include <ogcsys.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

// for directory parsing and low-level file I/O
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "input.h"
#if WANT_CHAO
#include <asndlib.h>
#include "chao_names.h"
#include "crypt_table.h"
#include "sound.h"

#include "SRLoseRings_pcm.h"
#include "SRidersRing_pcm.h"
#include "PMarioCoin_pcm.h"
#include "WWRupee_pcm.h"
#include "WWRupeeRed_pcm.h"
#include "WW_reg_pcm.h"
#include "TCGRing_pcm.h"
#include "TCGAsk_pcm.h"
#include "TCGOK_pcm.h"
#include "ALTTP_pcm.h"
#include "SA1up_pcm.h"

#ifdef WANT_NET
#include <network.h>
#include "http.h"
#endif

#endif

#define SYSMEM1_SIZE 0x01800000

static const unsigned fstPTR = 0x424; // for searching for chao.bin in SADX

//Wii Remote battery
int batt_lvl = 4;

#ifdef WANT_NET
//use to store last octet of Wii's current ip
static char WiiIP[16] = {0};
static char WiiOct[4] = {0};
#endif
static bool want3DS = false;
static bool sendChao = false;

static char hexaeco_path[] ATTRIBUTE_ALIGN(32) = "disk:/private/other/savefiles/";

// PKM AGB
//Ruby and Sapphire
typedef struct _PKData
{
	// A SAVE
	u8 dataUnk1[0x1C000];
/*	u8 dataUnk1[0x1000];
	u8 dataUnk2[0x1000];
	union
	{
		u8 dataA12[0xF80];
		struct
		{
			u8 chkA12[0xF80];
		} ATTRIBUTE_PACKED;
	};
	u8 padA12[0x74];
	u16 section_id_A12;
	u16 checksum_A12; //le
	u32 signature_A12;
	u32 index_A12;
	union
	{
		u8 dataA13[0x7D0];
		struct
		{
			u8 chkA13[0x7D0];
		} ATTRIBUTE_PACKED;
	};
	u8 padA13[0x824];
	u16 section_id_A13;
	u16 checksum_A13; //le
	u32 signature_A13;
	u32 index_A13;
	union
	{
		u8 dataA00[0xF2C];
		struct
		{
			u8 unk1_A00[0x19];
			u16 natDexUnA_A;
			u8 unk2_A00[0xD];
			u8 dexOwnedA[0x31];
			u8 unk3_A00[0x3];
			u8 dexSeenA_A[0x31];
			u8 unk4_A00[0xE9F];
		} ATTRIBUTE_PACKED;
	};
	u8 padA00[0xC8];
	u16 section_id_A00;
	u16 checksum_A00; //le
	u32 signature_A00;
	u32 index_A00;
	union
	{
		u8 dataA01[0xF80];
		struct
		{
			u8 chk1_A01[0x234];
			u8 party_A;
			u8 unk1_A01;
			u8 unk2_A01;
			u8 unk3_A01;
			u8 poke1_A[0x64];
			u8 poke2_A[0x64];
			u8 poke3_A[0x64];
			u8 poke4_A[0x64];
			u8 poke5_A[0x64];
			u8 poke6_A[0x64];
			u32 money_A; //le
			u16 coins_A; //le
			u8 chk2_A01[0x4A2];
			u8 dexSeenA_B[0x31];
			u8 chk3_A01[0x617];
		} ATTRIBUTE_PACKED;
	};
	u8 padA01[0x74];
	u16 section_id_A01;
	u16 checksum_A01; //le
	u32 signature_A01;
	u32 index_A01;
	union
	{
		u8 dataA02[0xF80];
		struct
		{
			u8 unk1_A02[0x3A6];
			u8 natDexUnA_B;
			u8 unk2_A02[0xA5];
			u16 natDexUnA_C;
			u8 unk3_A02[0xB32];
		} ATTRIBUTE_PACKED;
	};
	u8 padA02[0x74];
	u16 section_id_A02;
	u16 checksum_A02; //le
	u32 signature_A02;
	u32 index_A02;
	union
	{
		u8 dataA03[0xF80];
		struct
		{
			u8 unk1_A03[0xF80];
		} ATTRIBUTE_PACKED;
	};
	u8 padA03[0x74];
	u16 section_id_A03;
	u16 checksum_A03; //le
	u32 signature_A03;
	u32 index_A03;
	union
	{
		u8 dataA04[0xF08];
		struct
		{
			u8 unk1_A04[0xC0C];
			u8 dexSeenA_C[0x31];
			u8 unk2_A04[0x2CB];
		} ATTRIBUTE_PACKED;
	};
	u8 padA04[0xEC];
	u16 section_id_A04;
	u16 checksum_A04; //le
	u32 signature_A04;
	u32 index_A04;
	
	//don't care
	u8 padArest[0x8000];
	
	// B SAVE
	union
	{
		u8 dataB00[0xF2C];
		struct
		{
			u8 unk1_B00[0x19];
			u16 natDexUnB_A;
			u8 unk2_B00[0xD];
			u8 dexOwnedB[0x31];
			u8 unk3_B00[0x3];
			u8 dexSeenB_A[0x31];
			u8 unk4_B00[0xE9F];
		} ATTRIBUTE_PACKED;
	};
	u8 padB00[0xC8];
	u16 section_id_B00;
	u16 checksum_B00; //le
	u32 signature_B00;
	u32 index_B00;
	union
	{
		u8 dataB01[0xF80];
		struct
		{
			u8 chk1_B01[0x234];
			u8 party_B;
			u8 unk1_B01;
			u8 unk2_B01;
			u8 unk3_B01;
			u8 poke1_B[0x64];
			u8 poke2_B[0x64];
			u8 poke3_B[0x64];
			u8 poke4_B[0x64];
			u8 poke5_B[0x64];
			u8 poke6_B[0x64];
			u32 money_B; //le
			u16 coins_B; //le
			u8 chk2_B01[0x4A2];
			u8 dexSeenB_B[0x31];
			u8 chk3_B01[0x617];
		} ATTRIBUTE_PACKED;
	};
	u8 padB01[0x74];
	u16 section_id_B01;
	u16 checksum_B01; //le
	u32 signature_B01;
	u32 index_B01;
	union
	{
		u8 dataB02[0xF80];
		struct
		{
			u8 unk1_B02[0x3A6];
			u8 natDexUnB_B;
			u8 unk2_B02[0xA5];
			u16 natDexUnB_C;
			u8 unk3_B02[0xB32];
		} ATTRIBUTE_PACKED;
	};
	u8 padB02[0x74];
	u16 section_id_B02;
	u16 checksum_B02; //le
	u32 signature_B02;
	u32 index_B02;
	union
	{
		u8 dataB03[0xF80];
		struct
		{
			u8 unk1_B03[0xF80];
		} ATTRIBUTE_PACKED;
	};
	u8 padB03[0x74];
	u16 section_id_B03;
	u16 checksum_B03; //le
	u32 signature_B03;
	u32 index_B03;
	union
	{
		u8 dataB04[0xF08];
		struct
		{
			u8 unk1_B04[0xC0C];
			u8 dexSeenB_C[0x31];
			u8 unk2_B04[0x2CB];
		} ATTRIBUTE_PACKED;
	};
	u8 padB04[0xEC];
	u16 section_id_B04;
	u16 checksum_B04; //le
	u32 signature_B04;
	u32 index_B04; */
} PKData;

//Emerald
typedef struct _EMData
{
	// A SAVE
	u8 dataUnk1[0x1C000];
	//u8 dataUnk1[0x1000]; //pc buffs
/*	u8 dataUnk2[0x1000];
	u8 dataUnk3[0x1000];
	u8 dataUnk4[0x1000];
	u8 dataUnk5[0x1000];
	u8 dataUnk6[0x1000];
	union
	{
		u8 dataA00[0xF2C];
		struct
		{
			u8 unk1_A00[0x19];
			u16 natDexUnA_A;
			u8 unk2_A00[0xD];
			u8 dexOwnedA[0x31];
			u8 unk3_A00[0x3];
			u8 dexSeenA_A[0x31];
			u8 unk4_A00[0xE9F];
		} ATTRIBUTE_PACKED;
	};
	u8 padA00[0xC8];
	u16 section_id_A00;
	u16 checksum_A00; //le
	u32 signature_A00;
	u32 index_A00;
	union
	{
		u8 dataA01[0xF80];
		struct
		{
			u8 chk1_A01[0x234];
			u8 party_A;
			u8 unk1_A01;
			u8 unk2_A01;
			u8 unk3_A01;
			u8 poke1_A[0x64];
			u8 poke2_A[0x64];
			u8 poke3_A[0x64];
			u8 poke4_A[0x64];
			u8 poke5_A[0x64];
			u8 poke6_A[0x64];
			u32 money_A; //le
			u16 coins_A; //le
			u8 chk2_A01[0x4F2];
			u8 dexSeenA_B[0x31];
			u8 chk3_A01[0x5C7];
		} ATTRIBUTE_PACKED;
	};
	u8 padA01[0x74];
	u16 section_id_A01;
	u16 checksum_A01; //le
	u32 signature_A01;
	u32 index_A01;
	union
	{
		u8 dataA02[0xF80];
		struct
		{
			u8 unk1_A02[0x402];
			u8 natDexUnA_B;
			u8 unk2_A02[0xA5];
			u16 natDexUnA_C;
			u8 unk3_A02[0xAD6];
		} ATTRIBUTE_PACKED;
	};
	u8 padA02[0x74];
	u16 section_id_A02;
	u16 checksum_A02; //le
	u32 signature_A02;
	u32 index_A02;
	union
	{
		u8 dataA03[0xF80];
		struct
		{
			u8 unk1_A03[0xF80];
		} ATTRIBUTE_PACKED;
	};
	u8 padA03[0x74];
	u16 section_id_A03;
	u16 checksum_A03; //le
	u32 signature_A03;
	u32 index_A03;
	union
	{
		u8 dataA04[0xF08];
		struct
		{
			u8 unk1_A04[0xCA4];
			u8 dexSeenA_C[0x31];
			u8 unk2_A04[0x233];
		} ATTRIBUTE_PACKED;
	};
	u8 padA04[0xEC];
	u16 section_id_A04;
	u16 checksum_A04; //le
	u32 signature_A04;
	u32 index_A04;
	
	//don't care
	u8 padArest[0x8000];
	
	// B SAVE
	union
	{
		u8 dataB00[0xF2C];
		struct
		{
			u8 unk1_B00[0x19];
			u16 natDexUnB_A;
			u8 unk2_B00[0xD];
			u8 dexOwnedB[0x31];
			u8 unk3_B00[0x3];
			u8 dexSeenB_A[0x31];
			u8 unk4_B00[0xE9F];
		} ATTRIBUTE_PACKED;
	};
	u8 padB00[0xC8];
	u16 section_id_B00;
	u16 checksum_B00; //le
	u32 signature_B00;
	u32 index_B00;
	union
	{
		u8 dataB01[0xF80];
		struct
		{
			u8 chk1_B01[0x234];
			u8 party_B;
			u8 unk1_B01;
			u8 unk2_B01;
			u8 unk3_B01;
			u8 poke1_B[0x64];
			u8 poke2_B[0x64];
			u8 poke3_B[0x64];
			u8 poke4_B[0x64];
			u8 poke5_B[0x64];
			u8 poke6_B[0x64];
			u32 money_B; //le
			u16 coins_B; //le
			u8 chk2_B01[0x4F2];
			u8 dexSeenB_B[0x31];
			u8 chk3_B01[0x5C7];
		} ATTRIBUTE_PACKED;
	};
	u8 padB01[0x74];
	u16 section_id_B01;
	u16 checksum_B01; //le
	u32 signature_B01;
	u32 index_B01;
	union
	{
		u8 dataB02[0xF80];
		struct
		{
			u8 unk1_B02[0x402];
			u8 natDexUnB_B;
			u8 unk2_B02[0xA5];
			u16 natDexUnB_C;
			u8 unk3_B02[0xAD6];
		} ATTRIBUTE_PACKED;
	};
	u8 padB02[0x74];
	u16 section_id_B02;
	u16 checksum_B02; //le
	u32 signature_B02;
	u32 index_B02;
	union
	{
		u8 dataB03[0xF80];
		struct
		{
			u8 unk1_B03[0xF80];
		} ATTRIBUTE_PACKED;
	};
	u8 padB03[0x74];
	u16 section_id_B03;
	u16 checksum_B03; //le
	u32 signature_B03;
	u32 index_B03;
	union
	{
		u8 dataB04[0xF08];
		struct
		{
			u8 unk1_B04[0xCA4];
			u8 dexSeenB_C[0x31];
			u8 unk2_B04[0x233];
		} ATTRIBUTE_PACKED;
	};
	u8 padB04[0xEC];
	u16 section_id_B04;
	u16 checksum_B04; //le
	u32 signature_B04;
	u32 index_B04;
	u8 resto[0x5000]; */
} EMData;

//FireRed/LeafGreen
typedef struct _FGData
{
	u8 dataUnk1[0x1C000];
} FGData;

PKData * poke_main_save = NULL;
EMData * emerald_main_save = NULL;
FGData * frlg_main_save = NULL;

typedef struct _DKMData
{
	//u32 PID;
	//u32 OID;
	//u8 other[0x18];
	u8 dat0[0x18+8];
	union
	{
		u8 data[0x30]; //encrypted data
		struct
		{
			u16 internal_ID; //le
			u8 etc[0x2E];
		} ATTRIBUTE_PACKED;
	};
	u8 unk[0x14];
} DKMData;

DKMData * decrypted_mon = NULL;
DKMData * sdata = NULL;

int unshuffleStructure[24][4] = {
    {0,1,2,3,},
    {0,1,3,2,},
    {0,2,1,3,},
    {0,3,1,2,},
    {0,2,3,1,},
    {0,3,2,1,},
    {1,0,2,3,},
    {1,0,3,2,},
    {2,0,1,3,},
    {3,0,1,2,},
    {2,0,3,1,},
    {3,0,2,1,},
    {1,2,0,3,},
    {1,3,0,2,},
    {2,1,0,3,},
    {3,1,0,2,},
    {2,3,0,1,},
    {3,2,0,1,},
    {1,2,3,0,},
    {1,3,2,0,},
    {2,1,3,0,},
    {3,1,2,0,},
    {2,3,1,0,},
    {3,2,1,0,}
};

u16 table3_National[] =
{
              0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
             20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
             40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
             60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
             80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
            100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
            120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
            160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
            180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
            200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
            220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
            240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 252, 253, 254,
            255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274,
            275, 290, 291, 292, 276, 277, 285, 286, 327, 278, 279, 283, 284, 320, 321, 300, 301, 352, 343, 344,
            299, 324, 302, 339, 340, 370, 341, 342, 349, 350, 318, 319, 328, 329, 330, 296, 297, 309, 310, 322,
            323, 363, 364, 365, 331, 332, 361, 362, 337, 338, 298, 325, 326, 311, 312, 303, 307, 308, 333, 334,
            360, 355, 356, 315, 287, 288, 289, 316, 317, 357, 293, 294, 295, 366, 367, 368, 359, 353, 354, 336,
            335, 369, 304, 305, 306, 351, 313, 314, 345, 346, 347, 348, 280, 281, 282, 371, 372, 373, 374, 375,
            376, 377, 378, 379, 382, 383, 384, 380, 381, 385, 386, 358,
};

int Chksum(u8 *Data, int length)
{
	int Chk,i,tmp;
	length = length;
	Chk=0;
	for(i=0; i < (length & ~3); i+=4)
		Chk += __lwbrx(Data, i);

	tmp = Chk>>16;
	tmp +=Chk;

	Chk = (tmp&0xFFFF);

    //printf ("Checksum: 0x%04X\n", Chk);
	return Chk;
}

#include "jirachi_adamant_bin.h"
#include "jirachi_lax_channel_bin.h"
#include "jirachi_lonely_meteo_bin.h"
#include "jirachi_naive_bin.h"
#include "jirachi_naughty_bin.h"
#include "jirachi_serious_channel_bin.h"

#include "jirachi_shiny_bashful_bin.h"
#include "jirachi_shiny_careful_bin.h"
#include "jirachi_shiny_channel_bin.h"
#include "jirachi_shiny_docile_bin.h"
#include "jirachi_shiny_hasty_bin.h"
#include "jirachi_shiny_jolly_bin.h"
#include "jirachi_shiny_lonely_bin.h"
#include "jirachi_shiny_naughty_bin.h"
#include "jirachi_shiny_serious_bin.h"
#include "jirachi_shiny_timid_bin.h"
// where the heck did I get all these shinies from??

#include "mystry_mew_bashful_bin.h"
#include "mystry_mew_calm_bin.h"
#include "mystry_mew_careful_bin.h"
#include "mystry_mew_docile_bin.h"
#include "mystry_mew_hardy_bin.h"
#include "mystry_mew_impish_bin.h"
#include "mystry_mew_jolly_bin.h"
#include "mystry_mew_mild_bin.h"
#include "mystry_mew_quiet_bin.h"
#include "mystry_mew_quirky_bin.h"
#include "mystry_mew_sassy_bin.h"
#include "mystry_mew_timid_bin.h"

#include "aura_mew_bold_bin.h"
#include "aura_mew_rash_bin.h"
#include "aura_mew_relaxed_bin.h"

#include "colos_celebi_hardy_bin.h"
#include "colos_pikachu_bashful_bin.h"

#include "box_pichu_careful_bin.h"
#include "box_skitty_naughty_bin.h"
#include "box_swablu_calm_bin.h"
#include "box_zigzag_hasty_bin.h"

static bool wantShinyJirachi = false;
//if E get Aurora/Eon/Mystic/Old Sea Map ticket
//if Ruby/Saphh get Eon ticket
//if FR/LG get Aurora/Mystic ticket

static bool wantMew;
static bool wantCelebi;  //unused AGETO
static bool wantPikachu; //unused COLOS
static bool wantSwablu;  //Box Eggs
static bool wantZigzag;
static bool wantSkitty;
static bool wantPichu;
static bool wantEvents;
static bool wantNat;

u8 langPKM = 2; // 2: EN, 7: ES, 3: FR, 4: IT, 5: DE
u16 gen3ID = 384; //384 = Jirachi
u8 curr[0x64];
u8 currShin[0x64];

void unshufflePkmData(unsigned int key, char *data_in, char *data_out)
{
    int x, y, algorythm, blockSize;

    algorythm = key % 24;
    blockSize = 12;

    for(x = 0; x < 4; x++)
    {
        int pos = unshuffleStructure[algorythm][x];
        for(y = 0; y < blockSize; y++)
        {
            data_out[x*blockSize+y] = data_in[pos*blockSize+y]&0xFF;
        }
    }
}

char *readBytes(char *data, int offset, int count)
{
    char* bytes = 0;
    int x;
    for(x = 0; x < count; x++)
    {
        bytes += ((data[offset + x] & 0xFF) << (8 * x));
    }
    return bytes;
}

void pkmGBAcrypt(unsigned int seed, char *data_in, char *data_out)
{
    int x;
    unsigned int tempData;
    for(x = 0; x < 12; x++)
    {
            tempData = seed ^ (unsigned int)readBytes(data_in, 4*x, 4);

            data_out[4*x +0] = tempData;
            data_out[4*x +1] = tempData>>8;
            data_out[4*x +2] = tempData>>16;
            data_out[4*x +3] = tempData>>24;
    }
}

void PickJirachi()
{
	int rand_norm = rand() % (6 + 1 - 1) + 1;
	switch (rand_norm) {
		case 1: memcpy(curr, jirachi_adamant_bin, 0x64);
		break;
		case 2: memcpy(curr, jirachi_lax_channel_bin, 0x64);
		break;
		case 3: memcpy(curr, jirachi_lonely_meteo_bin, 0x64);
		break;
		case 4: memcpy(curr, jirachi_naive_bin, 0x64);
		break;
		case 5: memcpy(curr, jirachi_naughty_bin, 0x64);
		break;
		case 6: memcpy(curr, jirachi_serious_channel_bin, 0x64);
		break;
	}

	//shiny
	int rand_shiny = rand() % (10 + 1 - 1) + 1;
	switch (rand_shiny) {
		case 1: memcpy(currShin, jirachi_shiny_bashful_bin, 0x64);
		break;
		case 2: memcpy(currShin, jirachi_shiny_careful_bin, 0x64);
		break;
		case 3: memcpy(currShin, jirachi_shiny_channel_bin, 0x64);
		break;
		case 4: memcpy(currShin, jirachi_shiny_docile_bin, 0x64);
		break;
		case 5: memcpy(currShin, jirachi_shiny_hasty_bin, 0x64);
		break;
		case 6: memcpy(currShin, jirachi_shiny_jolly_bin, 0x64);
		break;
		case 7: memcpy(currShin, jirachi_shiny_lonely_bin, 0x64);
		break;
		case 8: memcpy(currShin, jirachi_shiny_naughty_bin, 0x64);
		break;
		case 9: memcpy(currShin, jirachi_shiny_serious_bin, 0x64);
		break;
		case 10: memcpy(currShin, jirachi_shiny_timid_bin, 0x64);
		break;
	}
	int rand_val = rand() % (100 + 1 - 1) + 1;
	if(rand_val > 90) { //shiny get!
		memcpy(curr, currShin, 0x64);
		wantShinyJirachi = true;
	}
	
	//argument pokemon
	if(wantCelebi)
		memcpy(curr, colos_celebi_hardy_bin, 0x64);
	else if(wantPikachu)
		memcpy(curr, colos_pikachu_bashful_bin, 0x64);
	else if(wantPichu)
		memcpy(curr, box_pichu_careful_bin, 0x64);
	else if(wantSkitty)
		memcpy(curr, box_skitty_naughty_bin, 0x64);
	else if(wantSwablu)
		memcpy(curr, box_swablu_calm_bin, 0x64);
	else if(wantZigzag)
		memcpy(curr, box_zigzag_hasty_bin, 0x64);
	else if(wantMew) { // 3 unique aura, 12 unique mystery
		int rand_mew = rand() % (15 + 1 - 1) + 1;
		switch(rand_mew) {
		case 1: memcpy(curr, aura_mew_bold_bin, 0x64);
		break;
		case 2: memcpy(curr, aura_mew_rash_bin, 0x64);
		break;
		case 3: memcpy(curr, aura_mew_relaxed_bin, 0x64);
		break;
		case 4: memcpy(curr, mystry_mew_bashful_bin, 0x64);
		break;
		case 5: memcpy(curr, mystry_mew_docile_bin, 0x64);
		break;
		case 6: memcpy(curr, mystry_mew_impish_bin, 0x64);
		break;
		case 7: memcpy(curr, mystry_mew_sassy_bin, 0x64);
		break;
		case 8: memcpy(curr, mystry_mew_hardy_bin, 0x64);
		break;
		case 9: memcpy(curr, mystry_mew_calm_bin, 0x64);
		break;
		case 10: memcpy(curr, mystry_mew_quirky_bin, 0x64);
		break;
		case 11: memcpy(curr, mystry_mew_careful_bin, 0x64);
		break;
		case 12: memcpy(curr, mystry_mew_jolly_bin, 0x64);
		break;
		case 13: memcpy(curr, mystry_mew_mild_bin, 0x64);
		break;
		case 14: memcpy(curr, mystry_mew_quiet_bin, 0x64);
		break;
		case 15: memcpy(curr, mystry_mew_timid_bin, 0x64);
		break;
		}
	}
	
	//Set language
	if(langPKM != 0)
		curr[0x12] = langPKM;
	
	//pokedex
	DKMData * decrypted_mon = (DKMData *)memalign(32, sizeof(DKMData));
	if(!decrypted_mon) {
		exit(1);
	}
	//get current pokemon
	memcpy(decrypted_mon, curr, 0x64);
	
	int part1Size = 0x20, part2Size = 0x30, part3Size = 0x14;
	char *partData1, *partData2_in, *partData2, *partData2_out, *partData3;
	unsigned int tid, pid, encryptionKey = 0;

	partData1 = (char *)malloc(part1Size);
    if(partData1 == NULL)
    {
        ;
    }
    memcpy(partData1, &decrypted_mon->dat0[0], 0x18+8);
    partData2_in = (char *)malloc(part2Size);
    if(partData2_in == NULL)
    {
        ;
    }
	memcpy(partData2_in, &decrypted_mon->data[0], 0x30);
    partData3 = (char *)malloc(part3Size);
    if(partData3 == NULL)
    {
        ;
    }
	memcpy(partData3, &decrypted_mon->unk[0], 0x14);
    //get pid
    pid = (unsigned int)readBytes(partData1, 0, 4);
    partData2 = (char *)malloc(part2Size);
    if(partData2 == NULL)
    {
        ;
    }
    partData2_out = (char *)malloc(part2Size);
    if(partData2_out == NULL)
    {
        ;
    }
	tid = (unsigned int)readBytes(partData1, 4, 4);
	encryptionKey = pid ^ tid;
	
	unshufflePkmData(pid, partData2_in, partData2);
    pkmGBAcrypt(encryptionKey, partData2, partData2_out);

	memcpy(&decrypted_mon->data[0], partData2_out, 0x30);
	
	//convert ID to national dex and use it
	//u16 gen3ID = (sdata->internal_ID>>8) | (sdata->internal_ID<<8);
	//u16 gen3ID = 384;
	gen3ID = decrypted_mon->internal_ID;
	gen3ID = (gen3ID>>8) | (gen3ID<<8);
	gen3ID = table3_National[gen3ID] - 1;
	
	//if(gen3ID != 384)
		//missionFailed();

#if 0 //the avoid problems version
	if(wantSwablu)
		gen3ID = 332;
	else if(wantZigzag)
		gen3ID = 262;
	else if(wantSkitty)
		gen3ID = 299;
	else if(wantPichu)
		gen3ID = 171;
	else if(wantPikachu)
		gen3ID = 24;
	else if(wantCelebi)
		gen3ID = 250;
	else if(wantMew)
		gen3ID = 150;
	else
		gen3ID = 384; //jirachi
#endif
}

//Now XD
static const s8 substructureOrder[16] = { 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,15,14 };
static const u16 substructureMaxSizes[16] = /* default sizes */
{
 0x88, 0x978, 0xbc50, 0x4a, 0xcc, 0x1774,
 0x364, 0x2400, 0x6b4, 0xb, 0x400, 0x800, 0x1000, 0x2000,
 0x2298, 0xc800
};
static const u16 substructureOffsetIncrements[16] =
{
 0xa0, 0x990, 0xbc60, 0x60, 0xe0, 0x1790,
 0x380, 0x2410, 0x6d0, 0x20, 0x410, 0x810,
 0x1010, 0x2010, 0x22b0, 0xc810
};

//Now PBR, NOTE: this code is from TuxSH
void encryptSaveSlotAndUpdateChecksums(u8* in, u8* out) {
	//checkDataChecksum(in, 0x100, 8);
	//checkDataChecksum(in, 0x1C0000, 0x1BFF80); // end - 0x80
	//PBRencryptData(in, out, 0x1C0000);
	return;
}

static void *xfb = NULL;

static void initialize(GXRModeObj *rmode)
{
	//static void *xfb = NULL;

	if (xfb)
		free(MEM_K1_TO_K0(xfb));

	xfb = SYS_AllocateFramebuffer(rmode);
	VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
	xfb = MEM_K0_TO_K1(xfb);
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	VIDEO_WaitVSync();
}

u16 programChk = 0;
char name[] = "Diego A.";
char quote[] = "MISAKANETWORK";

static bool shutupOmochao = false;
static bool networkEnabled = true;
static bool showIP = false;
static bool android = false;
static signed netDevice = 1; //3DS default
char chaoDL[256] = { 0 };

//variables
char sa2_path[256] = { 0 }; // Sonic Advance 2
char sa1_path[256] = { 0 }; // Sonic Advance 1
char spp_path[256] = { 0 }; // Sonic Pinball Party

char sa2b_path[256]  = { 0 }; // Sonic Adventure 2 Battle
char sadx_path[256]  = { 0 }; // Sonic Adventure DX
char mcd_path[256]   = { 0 }; // Memory Card file holding both SADX/SA2B as contiguous data
char chao_path[256]  = { 0 }; // Chao file on storage device that will be imported into SADX/SA2B
char music_path[256] = { 0 }; // OGG Vorbis music.
char tcg_path[256] = { 0 };   // SADX TCG ROM, used for writing/reading
char sadx_iso[256] = { 0 };   // SADX ISO used for extracting TCG ROM

//bool rings = false;
//bool chao = false;
//bool egg = false;
//bool fruit = false;
u8 slot = 1;
u8 mode = 0;
static bool askImport = false;  //false by default
static bool verbose = false;
static bool randMusic = false;
//static bool sa1_active = false; //use these to determine if a path is set
//static bool sa2_active = false;
//static bool spp_active = false;

u16 slot_size = 0x800;
u8 name_offset = 0x12; //nameless chao currently used to detect empty slot.
u16 skipGChdr = 0x3080;
u8 chao_type = 0x80; //use to detect empty slot

// SA2B slots
#define CHAO_SLOT_1 0x3AE4
#define CHAO_SLOT_2 CHAO_SLOT_1+slot_size
#define CHAO_SLOT_3 CHAO_SLOT_2+slot_size
#define CHAO_SLOT_4 CHAO_SLOT_3+slot_size
#define CHAO_SLOT_5 CHAO_SLOT_4+slot_size
#define CHAO_SLOT_6 CHAO_SLOT_5+slot_size
#define CHAO_SLOT_7 CHAO_SLOT_6+slot_size
#define CHAO_SLOT_8 CHAO_SLOT_7+slot_size
#define CHAO_SLOT_9 CHAO_SLOT_8+slot_size
#define CHAO_SLOT_10 CHAO_SLOT_9+slot_size
#define CHAO_SLOT_11 CHAO_SLOT_10+slot_size
#define CHAO_SLOT_12 CHAO_SLOT_11+slot_size
#define CHAO_SLOT_13 CHAO_SLOT_12+slot_size
#define CHAO_SLOT_14 CHAO_SLOT_13+slot_size
#define CHAO_SLOT_15 CHAO_SLOT_14+slot_size
#define CHAO_SLOT_16 CHAO_SLOT_15+slot_size
#define CHAO_SLOT_17 CHAO_SLOT_16+slot_size
#define CHAO_SLOT_18 CHAO_SLOT_17+slot_size
#define CHAO_SLOT_19 CHAO_SLOT_18+slot_size
#define CHAO_SLOT_20 CHAO_SLOT_19+slot_size
#define CHAO_SLOT_21 CHAO_SLOT_20+slot_size
#define CHAO_SLOT_22 CHAO_SLOT_21+slot_size
#define CHAO_SLOT_23 CHAO_SLOT_22+slot_size
#define CHAO_SLOT_24 CHAO_SLOT_23+slot_size

// SADX slots
#define DX_CHAO_SLOT_1 0x3898
#define DX_CHAO_SLOT_2 DX_CHAO_SLOT_1+slot_size
#define DX_CHAO_SLOT_3 DX_CHAO_SLOT_2+slot_size
#define DX_CHAO_SLOT_4 DX_CHAO_SLOT_3+slot_size
#define DX_CHAO_SLOT_5 DX_CHAO_SLOT_4+slot_size
#define DX_CHAO_SLOT_6 DX_CHAO_SLOT_5+slot_size
#define DX_CHAO_SLOT_7 DX_CHAO_SLOT_6+slot_size
#define DX_CHAO_SLOT_8 DX_CHAO_SLOT_7+slot_size
#define DX_CHAO_SLOT_9 DX_CHAO_SLOT_8+slot_size
#define DX_CHAO_SLOT_10 DX_CHAO_SLOT_9+slot_size
#define DX_CHAO_SLOT_11 DX_CHAO_SLOT_10+slot_size
#define DX_CHAO_SLOT_12 DX_CHAO_SLOT_11+slot_size
#define DX_CHAO_SLOT_13 DX_CHAO_SLOT_12+slot_size
#define DX_CHAO_SLOT_14 DX_CHAO_SLOT_13+slot_size
#define DX_CHAO_SLOT_15 DX_CHAO_SLOT_14+slot_size
#define DX_CHAO_SLOT_16 DX_CHAO_SLOT_15+slot_size
#define DX_CHAO_SLOT_17 DX_CHAO_SLOT_16+slot_size
#define DX_CHAO_SLOT_18 DX_CHAO_SLOT_17+slot_size
#define DX_CHAO_SLOT_19 DX_CHAO_SLOT_18+slot_size
#define DX_CHAO_SLOT_20 DX_CHAO_SLOT_19+slot_size
#define DX_CHAO_SLOT_21 DX_CHAO_SLOT_20+slot_size
#define DX_CHAO_SLOT_22 DX_CHAO_SLOT_21+slot_size
#define DX_CHAO_SLOT_23 DX_CHAO_SLOT_22+slot_size
#define DX_CHAO_SLOT_24 DX_CHAO_SLOT_23+slot_size

#if WANT_CHAO
void setOption(char* key, char* valuePointer){
	bool isString = valuePointer[0] == '"';
	//char value = 0;
	
	if(isString) {
		char* p = valuePointer++;
		while(*++p != '"');
		*p = 0;
	} //else
		//value = atoi(valuePointer);
	
	unsigned int i = 0;
	for(i=0; i<18; i++){
		if(!strcmp("memcard-path", key)) {
			sprintf(mcd_path, "%s", valuePointer);
		} else if(!strcmp("mode", key)) {
				if(!strcmp(valuePointer, "transporter"))
					mode = 1;
				else if(!strcmp(valuePointer, "rare"))
					mode = 2;
				else if(!strcmp(valuePointer, "exporter"))
					mode = 3;
				else if(!strcmp(valuePointer, "shop"))
					mode = 4;
				else if(!strcmp(valuePointer, "gba"))
					mode = 5;
				else if(!strcmp(valuePointer, "ring"))
					mode = 6;
				else if(!strcmp(valuePointer, "reset"))
					mode = 7;
				else if(!strcmp(valuePointer, "tcg"))
					mode = 8;
		} else if(!strcmp("chao-path", key)) {
			//whether to ask if you want to import a chao file.
			sprintf(chao_path, "%s", valuePointer);
			if(chao_path[0] != 0)
				askImport = true;
		} else if(!strcmp("sa1-path", key)) {
			sprintf(sa1_path, "%s", valuePointer);
		} else if(!strcmp("sa2-path", key)) {
			sprintf(sa2_path, "%s", valuePointer);
		} else if(!strcmp("spp-path", key)) {
			sprintf(spp_path, "%s", valuePointer);
		} else if(!strcmp("music-path", key)) {
			sprintf(music_path, "%s", valuePointer);
		} else if(!strcmp("tcg-path", key)) {
			sprintf(tcg_path, "%s", valuePointer);
		} else if(!strcmp("sadx-iso", key)) {
			sprintf(sadx_iso, "%s", valuePointer);
		} else if(!strcmp("dl-chao", key)) {
			sprintf(chaoDL, "%s", valuePointer);
		} else if(!strcmp("--verbose", key)) {
			verbose = true;
		} else if(!strcmp("--shutupOmochao", key)) {
			shutupOmochao = true;
		} else if(!strcmp("--noNetwork", key)) {
			networkEnabled = false;
		} else if(!strcmp("--showIP", key)) {
			showIP = true;
		} else if(!strcmp("--randomMusic", key)) {
			randMusic = true;
		}
	/*	} else if(!strcmp("--rings", key)) {
				rings = true;
		} else if(!strcmp("--chao", key)) {
				chao = true;
		} else if(!strcmp("--egg", key)) {
				egg = true;
		} else if(!strcmp("--fruit", key)) {
				fruit = true;
		}*/
		//break;
	}
}
#endif

char fullpath_fe8[256] = { 0 };
char fullpath_fe7[256] = { 0 };

#if WANT_FE
//FE7 & FE8
void setOption(char* key, char* valuePointer){
	bool isString = valuePointer[0] == '"';
	//char value = 0;
	
	if(isString) {
		char* p = valuePointer++;
		while(*++p != '"');
		*p = 0;
	} //else
		//value = atoi(valuePointer);
	
	unsigned int i = 0;
	for(i=0; i<2; i++){
		if(!strcmp("--fe8-path", key)) {
			snprintf(fullpath_fe8, 256, valuePointer);
			//sprintf(full_path, "disk:/%s", "Fire Emblem - The Sacred Stones.sav");
		} else if(!strcmp("--fe7-path", key)) {
			snprintf(fullpath_fe7, 256, valuePointer);
		}
		//break;
	}
}
#endif

void handleConfigPair(char* kv){
	char* vs = kv;
	while(*vs != ' ' && *vs != '\t' && *vs != ':' && *vs != '=')
			++vs;
	*(vs++) = 0;
	while(*vs == ' ' || *vs == '\t' || *vs == ':' || *vs == '=')
			++vs;

	setOption(kv, vs);
}

#if WANT_SFX
void SonicRidersRing(int rings)
{
	u8 i;
	int voice = ASND_GetFirstUnusedVoice();
	int dirL = 1, dirR = 0;
	if(rings < 19)
		ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRidersRing_pcm,SRidersRing_pcm_size,150,150,NULL);
	else {
		for(i = 0; i < 40; ++i) {
			dirL ^= 1; dirR ^= 1;
			ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRidersRing_pcm,SRidersRing_pcm_size,150*dirL,150*dirR,NULL);
			usleep(30000);
		}
	}
}

void PaperMarioCoin(int rings)
{
	u8 i;
	int voice = ASND_GetFirstUnusedVoice();
	if(rings < 20)
		ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)PMarioCoin_pcm,PMarioCoin_pcm_size,175,175,NULL);
	else {
		for(i = 0; i < 30; ++i) {
			ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)PMarioCoin_pcm,PMarioCoin_pcm_size,175,175,NULL);
			usleep(40000);
		}
	}
}

void ZeldaRupee(int rings)
{
	int voice = ASND_GetFirstUnusedVoice();
	if(rings > 49)
		ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)WWRupeeRed_pcm,WWRupeeRed_pcm_size,175,175,NULL);
	else
		ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)WWRupee_pcm,WWRupee_pcm_size,175,175,NULL);
}
#endif

#if WANT_CHAO
typedef struct _ChaoSave
{
	//u32 checksum;
	union
	{
		u8 data[0x10040];
		struct
		{
			u8 gamecube_hdr[0x3080];
			u8 checksum_dat[0xCA64];
			u64 checksum_off;
			char unknown[0x554];
		} ATTRIBUTE_PACKED;
	};
} ChaoSave;

typedef struct _ChaoSaveDX
{
	//u32 checksum;
	union
	{
		u8 data[0x10040];
		struct
		{
			u8 gamecube_hdr[0x3080];
			u8 checksum_dat[0xC818];
			u64 checksum_off;
			char unknown[0x7A0];
		} ATTRIBUTE_PACKED;
	};
} ChaoSaveDX;

typedef struct _ChaoSaveAGB
{
	//char identify[4];
	u8 game_main[0xA000];
	union
	{
		u8 data[0x6000];
		struct
		{
			u16 checksum1;
			u16 unk1;
			u32 count1;
			u8 tcg_1[0x1000-8];
			u16 checksum2;
			u16 unk2;
			u32 count2;
			u8 tcg_2[0x1000-8];
			u16 checksum3;
			u16 unk3;
			u32 count3;
			u8 tcg_3[0x1000-8];
			u16 checksum4;
			u16 unk4;
			u32 count4;
			u8 tcg_4[0x1000-8];
			u16 checksum5;
			u16 unk5;
			u32 count5;
			u8 tcg_5[0x1000-8];
			u16 checksum6;
			u16 unk6;
			u32 count6;
			u8 tcg_6[0x1000-8];
		} ATTRIBUTE_PACKED;
	};
} ChaoSaveAGB;

typedef struct _ChaoData
{
	union
	{
		u8 data[0x800];
		struct
		{
			u8 chao_exists;
			u8 egg;
			u8 fruit1;
			u8 fruit2;
			u8 fruit3;
			u8 fruit4;
			u8 fruit5;
			u8 fruit6;
			u8 fruit7;
			u8 fruit8;
			u16 unk1;
			u32 rings;
			u16 unk2;
			u8 name[7];
			u8 chunk1[0x68];
			u8 garden;
			u8 chunk2[0x77E];
		} ATTRIBUTE_PACKED;
	};
} ChaoData;

typedef struct _SA2BData
{
	u8 gc_data[0x2880];
	u32 checksum;
	union
	{
		u8 data[0x379B];
		struct
		{
			u32 chunk1[0xB];
			u32 rings;
			u8 unk[0x378C];
		} ATTRIBUTE_PACKED;
	};
} SA2BData;

typedef struct _SADXData
{
	u8 gc_data[0x1480];
	u16 pad;
	u16 checksum;
	union
	{
		u8 data[0x0BBC];
		struct
		{
			u8 chk1[0x04D8];
			u32 rings;
			u8 chk2[0x06E0];
		} ATTRIBUTE_PACKED;
	};
} SADXData;

typedef struct _RIDERSData
{
	u8 gc_data[0x1480];
	u16 checksum;
	union
	{
		u8 data[0x9BE];
		struct
		{
			u8 chk1[0x706];
			u32 rings;
			u8 chk2[0x2B4];
		} ATTRIBUTE_PACKED;
	};
} RIDERSData;

typedef struct _TTYDData
{
	u8 gc_data[0x2040]; //sizeof = 11 blocks 
	union
	{
		u8 data[0x3FF8];
		struct
		{
			u8 chk1[0x1458];
			u16 coins;
			u8 chk2[0x2B9E];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
	s32 checksum_inv;
	u8 unk_chk[0xC000];
	union
	{
		u8 data2[0x3FF8];
		struct
		{
			u8 chk3[0x1458];
			u16 coins2;
			u8 chk4[0x2B9E];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum2;
	s32 checksum2_inv;
} TTYDData;

typedef struct _WWData
{
	u8 gc_data[0x2040];
	u64 unk0;
	union
	{
		u8 data[0x768];
		struct
		{
			u32 unk1;
			u16 rupee;
			u8 chk1[0x762];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
	s32 checksum_inv;
	u8 unk_chk[0x1890];
	union
	{
		u8 data2[0x768];
		struct
		{
			u32 unk2;
			u16 rupee2;
			u8 chk2[0x762];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum2;
	s32 checksum2_inv;
	u8 rest[0x1888]; //pad to finish block 3
} WWData;

typedef struct _DXTCGData
{
	u8 hdr_data[0xAF];
	u8 region;
	u8 pad1[0xD];
	u8 lang1; //set but unknown purpose
	u8 unk1[6];
	u16 lang2; //set but unknown purpose
	u8 pad2[9];
	u8 lang3; //actual language code 0:JP, 1:EN, 2:DE, 3:FR, 4:ES
	u32 lang4; //0x8CE17886:EN, 0xCE49D6B3:ES, 0x58BFF086:DE, 0x8A637033:FR
	u8 pad[0x11C];
	u8 chao[0x800];
	u8 end_pad[0x0345D8];
} DXTCGData;

typedef struct _ZGData
{
	u32 checksum; //CRC32
	union
	{
		u8 data[0x3FFC];
		struct
		{
			u8 chk1[0xC];
			u32 rings; //limit is 999,999
			u8 chk2[0x3FEC];
		} ATTRIBUTE_PACKED;
	};
} ZGData;

typedef struct _GALAXYData
{
	u32 checksum;
	union
	{
		u8 data[0xBDFC];
		struct
		{
			u8 chk1[0x150];
			u16 starbits; //little-endian
			u8 chk2[0xBCAA];
		} ATTRIBUTE_PACKED;
	};
} GALAXYData;

typedef struct _GAL2Data
{
	u32 checksum;
	union
	{
		u8 data[0x309C];
		struct
		{
			u8 chk1[0xA5];
			u16 starbits_game; //not aligned
			//u8 starbits_game2;
			u16 coins; //not aligned
			//u8 coins2;
			u8 chk2[0x2FB0];
			u16 starbits; //not aligned, banktoad
			u8 chk3[0x41];
		} ATTRIBUTE_PACKED;
	};
} GAL2Data;

typedef struct _EPICData
{
	u32 checksum; //CRC32
	union
	{
		u8 data[0xFFFC];
		struct
		{
			u8 chk1[0x144];
			u32 etickets_1; //savefile display
			u8 chk2[0x4508];
			u32 etickets_2; //actual count, up to 1000
			u8 chk3[0xB9A8];
		} ATTRIBUTE_PACKED;
	};
} EPICData;

typedef struct _YGOData
{
	//u32 checksum; //this game doesn't believe in checksums
	union
	{
		u8 data[0x01DA00];
		struct
		{
			u8 chk1[0x1614];
			u32 dp; //up to 99,999
			u8 chk2[0x01C3E8];
		} ATTRIBUTE_PACKED;
	};
} YGOData;

typedef struct _SSBBData
{
	union
	{
		u8 data[0xC67C];
		struct
		{
			u8 chk1[0x2D0];
			u32 coins; //up to 9999, divide by 10 to get real value
			u8 chk2[0x12DC];
			u8 goldenHammers; //win a golden hammer by sending 5000 or more coins, up to 5 can be stored
			u8 chk3[0xB0CB];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
} SSBBData;

typedef struct _SKYData
{
	u8 unk[0x20];
	union
	{
		u8 data[0x53BC];
		struct
		{
			u8 chk1[0x0A5E];
			u16 rupee; //up to 9900
			u8 chk2[0x25A0];
			u16 gratitude;
			u8 chk3[0x23BA];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum; //CRC32
} SKYData;

typedef struct _NIGHTSData
{
	u32 checksum; //just a byte sum, but order is unk
	union
	{
		u8 data[0x23BC];
		struct
		{
			u8 chk1[0x90];
			u32 bluechips_will;
			u32 pad;
			u32 bluechips_helen;
			u8 chk3[0x2320];
		} ATTRIBUTE_PACKED;
	};
} NIGHTSData;

typedef struct _RODEAData
{
	union
	{
		u8 data[0x4000];
		struct
		{
			u8 chk1[0x0460];
			u32 gravitons; //dioga gravidon!
			u8 chk2[0x3B9C];
		} ATTRIBUTE_PACKED;
	};
	//u32 checksum; //no checksums woo!
} RODEAData;

typedef struct _XENOData
{
	u8 chk1[0x011F5E];
	u16 crc;
	union
	{
		u8 data[0x012120];
		struct
		{
			u8 chk2[0x0120E8];
			u32 gold;
			u8 chk3[0x34];
		} ATTRIBUTE_PACKED;
	};
	u8 chk4[0x3F80];
} XENOData;

typedef struct _ALTTPData
{
	u8 chk1[0x011660];
	u16 rupee;
	u16 rupee_inst;
	//u8 chk2[0x04391C]; //rest, waste of space
} ALTTPData;

typedef struct _ToGData
{
	u8 chk1[0x4F88];
	u32 gald;
	u8 chk2[0xB1F4];
} ToGData;

typedef struct _FFCCData
{
	u8 chk1[0x0D38];
	u32 gil;
	u8 chk2[0x26A4];
	u16 save_cnt;
	u8 chk3[0x2BA];
	u8 name[0x12];
//	u8 chk4[0x4952];
//	u8 backup[0x8000];
} FFCCData;

typedef struct _MURAData
{
	union
	{
		u8 data[0x91C0];
		struct
		{
			u32 npsd;
			u32 unk; //always 10?
			u32 crc; //must be 0 during calculation
			u32 last_file; //latest file saved 0-4
			u8 file1_1[0xA8];
			u32 yen_kisu; //kisuke
			u8 file1_2[0xFC];
			u32 yen_momo; //momohime
			u8 file1_3[0x1CE4];
			u8 chk[0x7320]; //other saves
		} ATTRIBUTE_PACKED;
	};
} MURAData;

typedef struct _ARCData
{
	u8 chk1[0xFF4];
	u32 rico;
	u8 chk2[0x88D0];
	u32 rico_2;
	//rest
} ARCData;

typedef struct _ToS2Data
{
	u32 checksum1;
	u32 checksum2;
	union
	{
		u32 data[0xCABE];
		struct
		{
			u64 cpu_ticks;
			u32 unk1;
			u32 gald;
			u8 chk1[0x1C];
			char name1[0x14];
			u8 chk2[0x240];
			u32 gald_game;
			u8 chk3[0x032874];
		} ATTRIBUTE_PACKED;
	};
} ToS2Data;

typedef struct _MSData
{
	u32 checksum;
	u8 chk1[0x40];
	char name[0x1A]; //actually wide, unsure about length
	u8 chk2[0x7A12];
	u32 star_tokens; //at 0x7A70
} MSData;

#if 1
typedef struct _OKData
{
	union
	{
		//u32 data[0xB2C0/4];
		u32 data[0x100/4];
		struct
		{
			u64 place;
			u32 checksum;
			u32 checksum2;
			u8 chunka[0xE0];
			u32 yen;
			u8 chk2[0xC]; //save time by only working with first 0x100 bytes
		//	u8 chk2[0xB1CC];
		} ATTRIBUTE_PACKED;
	};
} OKData;
#endif

#if 0
typedef struct _OKData
{
	//HD version
	union
	{
		u32 data[0x2B6B00/4];
		struct
		{
			u64 place;
			u32 checksum2;
			u32 checksum1;
			u8 chunka[0xE0];
			u32 yen;
			u8 chk2[0x2B6A0C];
		} ATTRIBUTE_PACKED;
	};
	//u8 chk3[0x143FC0]; //rest
} OKData;
#endif

typedef struct _CBData
{
	union
	{
		u8 data[0x7064];
		struct
		{
			u8 chk1[0xC];
			u32 crc32_hdr;
			u8 chk2[0x67C8];
			u32 gil;
			u8 chk3[0x888];
		} ATTRIBUTE_PACKED;
	};
	u32 crc32;
	//u8 pad[0x18];
} CBData;

typedef struct _ACData
{
	u8 ignr[0x1140];
	u32 CRC32_1;
	union
	{
		u8 data[0x759C];
		struct
		{
			u8 chk1[0x10];
			u32 bells;
			u32 pad1;
			u32 abd; //for display
			u8 chk2[0x7580];
		} ATTRIBUTE_PACKED;
	};
	u8 ignr2[0x1120];
	u32 CRC32_2;
	union
	{
		u8 data2[0x759C];
		struct
		{
			u8 chk3[0x10];
			u32 bells2;
			u8 chk4[0x7588];
		} ATTRIBUTE_PACKED;
	};
	u8 ignr3[0x1120];
	u32 CRC32_3;
	union
	{
		u8 data3[0x759C];
		struct
		{
			u8 chk5[0x10];
			u32 bells3;
			u8 chk6[0x7588];
		} ATTRIBUTE_PACKED;
	};
	u8 ignr4[0x1120];
	u32 CRC32_4;
	union
	{
		u8 data4[0x759C];
		struct
		{
			u8 chk7[0x10];
			u32 bells4;
			u8 chk8[0x7588];
		} ATTRIBUTE_PACKED;
	};
	u8 ignr5[0x4685C];
	char town[0x10];
	u8 unk[6];
	char name[0x10];
} ACData;

typedef struct _UNData
{
	u32 checksum; //subtract byte increments
	union
	{
		//u8 data[0x7FFC]; //for full calculation
		u8 data[0x2D34];
		struct
		{
			u8 chk1[0x2D32]; //0x2D7B == 01 = WereClaw unlocked
			u16 lives;
		//	u8 chk2[0x52C8]; //rest
		} ATTRIBUTE_PACKED;
	};
} UNData;

typedef struct _SPYROData
{
	u8 gciData[0x40];
	u32 checksum; //CRC32
	u8 pad[0x17C];
	union
	{
		u8 data[0x27E80];
		struct
		{
			u8 chk1[0x5FF8];
			u32 gems; //current gem count
			u32 gemsTotal; //assume that's what it is
			u8 chk2[0x21E80]; //rest
		} ATTRIBUTE_PACKED;
	};
/*	union //this is correct but increases the size, so only slot 1 is used
	{
		u8 data2[0xB258];
		struct
		{
			u8 chk3[0x2034];
			u32 gems2; //current gem count
			u32 gemsTotal2; //assume that's what it is
			u8 chk4[0x9220]; //rest
		} ATTRIBUTE_PACKED;
	};
	union
	{
		u8 data3[0xB258];
		struct
		{
			u8 chk5[0x2034];
			u32 gems3; //current gem count
			u32 gemsTotal3; //assume that's what it is
			u8 chk6[0x9220]; //rest
		} ATTRIBUTE_PACKED;
	};*/
} SPYROData;

typedef struct _SSARData
{
	u32 checksum;
	u32 unk; //always 19?
	union
	{
		u8 data[0x1FCA8];
		struct
		{
			u32 slot1_checksum;
			u32 lifetime_miles;
			char name[0x12];
			u8 chk1[0x3A22];
			u32 milesTotal; //??? 1,769,902
			u32 miles;
			u32 unk1;
			u8 chk2[0x1C260];
		} ATTRIBUTE_PACKED;
	};
	union
	{
		u8 data2[0x1FCA8];
		struct
		{
			u32 slot2_checksum;
			u32 lifetime_miles2;
			char name2[0x12];
			u8 chk3[0x3A22];
			u32 milesTotal2;
			u32 miles2;
			u32 unk2;
			u8 chk4[0x1C260];
		} ATTRIBUTE_PACKED;
	};
	union
	{
		u8 data3[0x1FCA8];
		struct
		{
			u32 slot3_checksum;
			u32 lifetime_miles3;
			char name3[0x12];
			u8 chk5[0x3A22];
			u32 milesTotal3;
			u32 miles3;
			u32 unk3;
			u8 chk6[0x1C260];
		} ATTRIBUTE_PACKED;
	};
	union
	{
		u8 data4[0x1FCA8];
		struct
		{
			u32 slot4_checksum;
			u32 lifetime_miles4;
			char name4[0x12];
			u8 chk7[0x3A22];
			u32 milesTotal4;
			u32 miles4;
			u32 unk4;
			u8 chk8[0x1C260];
		} ATTRIBUTE_PACKED;
	};
} SSARData;

typedef struct _VIRTUAData
{
	u8 header[0x4040];
	u8 checksum; //just subtract ring amount
	union
	{
		//u8 data[0x1FBC]; //full data
		u8 data[0xDF];
		struct
		{
			u8 chk1[0xDB];
			u32 data_chips; //game displays around 2,000,000,000
		//	u8 rest[0x9F20]; //rest
		} ATTRIBUTE_PACKED;
	};
	u8 rest[0x1F20];
} VIRTUAData;

typedef struct _ZATCHData
{
	u8 header[0x2080];
	u32 checksum;
	union
	{
		u32 data[0x1FBC/4]; //first 0xAB8 bytes used for calc checksum
		struct
		{
			u8 chk1[0x6EC];
			u32 points; //game only displays 0x270F (9999)
			u8 rest[0x18CC]; //rest
		} ATTRIBUTE_PACKED;
	};
} ZATCHData;

typedef struct _YUJO2Data
{
	u8 header[0x2080];
	u32 checksum;
	union
	{
		u32 data[0x1FBC/4]; //first 0x1090 bytes used for calc checksum
		struct
		{
			u8 chk1[0xCC4];
			u32 points; //game only displays 0x270F (9999)
			u8 rest[0x3C8]; //rest
		} ATTRIBUTE_PACKED;
	};
} YUJO2Data;

typedef struct _YUJOData
{
	u8 header[0x2080];
	u32 checksum;
	union
	{
		u32 data[0x1FBC/4]; //first 0xD42 bytes used for calc checksum
		struct
		{
			u8 chk1[0x80C];
			u32 points; //game only displays 0x270F (9999)
			u8 rest[0x17AC]; //rest
		} ATTRIBUTE_PACKED;
	};
} YUJOData;

typedef struct _COLOSData
{
	u8 header[0x40];
	u8 mc_dat[0x6000-4];
	u32 points;
	union
	{
		u8 data[0x5A000];
		struct
		{
			u32 un1;
			u32 count1;
			u8 chk1[0x1E000 - 8];
			u32 un2;
			u32 count2;
			u8 chk2[0x1E000 - 8];
			u32 un3;
			u32 count3;
			u8 chk3[0x1E000 - 8];
		} ATTRIBUTE_PACKED;
	};
} COLOSData;

typedef struct _XDData
{
	u8 header[0x40];
	u8 mc_dat[0x6000-4];
	u32 points; //???
	union
	{
		u16 data[0x50000/2];
		struct
		{
			u32 un1;
			u32 count1;
			u8 chk1[0x28000 - 8];
			u32 un2;
			u32 count2;
			u8 chk2[0x28000 - 8];
		} ATTRIBUTE_PACKED;
	};
} XDData;

typedef struct _PBRData
{
	union
	{
		u8 data[0x380000];
		struct
		{
			u8 chnk[0x380000];
		/*	u32 un1;
			u32 count1;
			u8 chk1[0x28000 - 8];
			u32 un2;
			u32 count2;
			u8 chk2[0x28000 - 8]; */
		} ATTRIBUTE_PACKED;
	};
} PBRData;

typedef struct _TALESData
{
	u8 header[0x40];
	u8 save_inf[0x40]; //slot number, Lv, playtime, all in ascii
	u8 mc_dat[0x2000];
	u32 checksum1;
	u32 checksum2;
	union
	{
		u32 data[0x3FB8/4];
		struct
		{
			u8 un1[0xC];
			u32 gald_disp; //current money display
			u8 un2[0x1A8];
			u32 gald; //current money
			u8 un3[0x3DFC];
		} ATTRIBUTE_PACKED;
	};
} TALESData;

typedef struct _OOTData
{
	u8 header[0x20];
	union
	{
		u16 data[0x1352/2];
		struct
		{
			u8 un1[0x34];
			u16 rupee;
			u8 un2[0x131C];
		} ATTRIBUTE_PACKED;
	};
	u16 checksum;
} OOTData;

typedef struct _OOTfullData
{
	u32 checksum;
	union
	{
		u32 data[0x8000/4];
		struct
		{
			u8 save1[0x1FFC];
			u32 checksum2;
			u8 save2[0x1FFC];
			u32 checksum3;
			u8 save3[0x1FFC];
			u32 checksum4;
			u8 save4[0x1FFC];
		} ATTRIBUTE_PACKED;
	};
} OOTfullData;

static u16 TPbuf[0x2000/2];

typedef struct _TPData
{
	union
	{
		u8 data[0xA94-8];
		struct
		{
			u32 dat1;
			u16 rupee;
			u8 dat2[0x1AE];
			char name[0x11]; //not all chars are used
			char horse[0x11]; //not all chars are used
			u8 dat3[0x8B6];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
	u32 neg_checksum;
} TPData;

typedef struct _NEXTData
{
	u8 other[0x84];
	u32 sonic;
	u32 shadow;
	u32 silver;
} NEXTData;

ChaoSave * sa2b_save = NULL;
ChaoSaveDX * sadx_save = NULL;
ChaoSaveAGB * sa2agb_save = NULL; //all three GBA games with the TCG
ChaoData * chao_data = NULL; //just the 0x800 chao file

SA2BData * sa2b_main_save = NULL;
SADXData * sadx_main_save = NULL;

RIDERSData * riders_main_save = NULL;
TTYDData * ttyd_main_save = NULL;
WWData * ww_main_save = NULL;

ZGData * zg_main_save = NULL;
GALAXYData * gal_main_save = NULL;
GAL2Data * gal2_main_save = NULL;
EPICData * em_main_save = NULL;
YGOData * ygo_main_save = NULL;
SSBBData * ssb_main_save = NULL;
SKYData * ss_main_save = NULL;
NIGHTSData * nts_main_save = NULL;
RODEAData * rod_main_save = NULL;
XENOData * xeno_main_save = NULL;
//ALTTPData * alttp_main_save = NULL;
FFCCData * ffcc_main_save = NULL;
ToGData * graces_main_save = NULL;
MURAData * mura_main_save = NULL;
ARCData * arc_main_save = NULL;
ToS2Data * sympho_main_save = NULL;
MSData * wint_main_save = NULL;
CBData * ffcb_main_save = NULL;
ACData * accf_main_save = NULL;
SPYROData * spyro_main_save = NULL;
SSARData * ssar_main_save = NULL;
OKData * okami_main_save = NULL;
VIRTUAData * virtua_main_save = NULL;
ZATCHData * zatch_main_save = NULL;
YUJO2Data * yujo2_main_save = NULL;
YUJOData * yujo_main_save = NULL;
COLOSData * colos_main_save = NULL;
XDData * xd_main_save = NULL;
PBRData * pbr_main_save = NULL;
TALESData * tales_main_save = NULL;
OOTData * oot_main_save = NULL;
OOTfullData * oot_full_save = NULL;
TPData * tp_slot_save = NULL;
NEXTData * next_main_save = NULL;

UNData * un_main_save = NULL;

/*
typedef struct _FE8Data
{
	union
	{
		u16 data[0xA0];
		struct
		{
			u8 chk1[0x140];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
} FE8Data;
typedef struct _FE7Data
{
	union
	{
		u16 data[0x140];
		struct
		{
			u8 chk1[0x280];
		} ATTRIBUTE_PACKED;
	};
	u16 blank;
	u16 checksum;
} FE7Data;
FE8Data * fe8_main_save = NULL;
FE7Data * fe7_main_save = NULL; */

DXTCGData * dx_tcg_rom = NULL; //Manipulate TCG ROM from SADX

//CRC32 fast
unsigned int crc32b(u8 *data, u32 size) {
   int i, j, k;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
//   crc = 0;
//   while (data[i] != 0) {
   for(k = 0; k < size; ++k) {
      byte = data[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) { // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

unsigned int crc32z(u8 *data, u16 size) {
   int i, j, k;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0;
//   crc = 0;
//   while (data[i] != 0) {
   for(k = 0; k < size; ++k) {
      byte = data[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) { // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

u16 byte_swap16(u16* buffer, unsigned int length)
{
	u16 result = 0;
	int i;
	for (i = 0; i < (length & ~3); i += 2)
		*(u16*)(buffer + i) = __lhbrx(buffer, i);
	return result;
}

/*
u16 byte_swap16(u16* buffer, unsigned int length)
{
	u16 result = 0;
	int i;
	for (i = 0; i < (length & ~3); i += 2)
		*(u16*)(buffer + i) = __lhbrx(buffer, i);
	return result;
}
*/
/*
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}
*/

u16 RIDERSmainChecksum()
{
	int j;
	u16 v3;
	int i;

	v3 = -1;
	for (i = 0; (unsigned)i < (0x9BE); ++i)
	{
		v3 ^= riders_main_save->data[i];
		for (j = 0; j < 8; ++j)
		{
			if ((v3 & 1) == 1)
				v3 = (u16)(((int)v3 >> 1) ^ 0x8408);
			else
				v3 >>= 1;
		}
	}
	return (u16)~v3;
}

uint32_t galaxy_checksum(void *buf, int len)
{
    uint16_t *data = (uint16_t*)buf;
    uint32_t c1 = 0, c2 = 0;
	int i = 0;

    for (i = 0; i < len / sizeof(uint16_t); i++)
    {
        c1 = (c1 + data[i]) & 0xFFFF;
        c2 = (c2 + ~data[i]) & 0xFFFF;
    }

    return (c2 & 0xFFFF) | ((c1 & 0xFFFF) << 16);
}

u16 DXmainChecksum()
{
	int j;
	u16 v3;
	int i;

	v3 = -1;
	for (i = 0; (unsigned)i < (0x570-4); ++i) //-4 to account for checksum bytes
	{
		v3 ^= sadx_main_save->data[i];
		for (j = 0; j < 8; ++j)
		{
			if ((v3 & 1) == 1)
				v3 = (u16)(((int)v3 >> 1) ^ 0x8408);
			else
				v3 >>= 1;
		}
	}
	return (u16)~v3;
}

static unsigned dword_884D98[255+1] = {
            0, 0x0C9073096, 0x920E612C, 0x0A50951BA, 0x0FF6DC419, 0x0CA6AF48F,
            0x9163A535, 0x0A66495A3, 0x0FEDB8832, 0x0CFDCB8A4, 0x94D5E91E,
            0x0A3D2D988, 0x0F9B64C2B, 0x0CCB17CBD, 0x97B82D07, 0x0A0BF1D91,
            0x0FDB71064, 0x0C4B020F2, 0x9FB97148, 0x0A8BE41DE, 0x0F2DAD47D,
            0x0C7DDE4EB, 0x9CD4B551, 0x0ABD385C7, 0x0F36C9856, 0x0C26BA8C0,
            0x9962F97A, 0x0AE65C9EC, 0x0F4015C4F, 0x0C1066CD9, 0x9A0F3D63,
            0x0AD080DF5, 0x0FB6E20C8, 0x0D269105E, 0x896041E4, 0x0BE677172,
            0x0E403E4D1, 0x0D104D447, 0x8A0D85FD, 0x0BD0AB56B, 0x0E5B5A8FA,
            0x0D4B2986C, 0x8FBBC9D6, 0x0B8BCF940, 0x0E2D86CE3, 0x0D7DF5C75,
            0x8CD60DCF, 0x0BBD13D59, 0x0E6D930AC, 0x0DFDE003A, 0x84D75180,
            0x0B3D06116, 0x0E9B4F4B5, 0x0DCB3C423, 0x87BA9599, 0x0B0BDA50F,
            0x0E802B89E, 0x0D9058808, 0x820CD9B2, 0x0B50BE924, 0x0EF6F7C87,
            0x0DA684C11, 0x81611DAB, 0x0B6662D3D, 0x0F6DC4190, 0x0FFDB7106,
            0x0A4D220BC, 0x93D5102A, 0x0C9B18589, 0x0FCB6B51F, 0x0A7BFE4A5,
            0x90B8D433, 0x0C807C9A2, 0x0F900F934, 0x0A209A88E, 0x950E9818,
            0x0CF6A0DBB, 0x0FA6D3D2D, 0x0A1646C97, 0x96635C01, 0x0CB6B51F4,
            0x0F26C6162, 0x0A96530D8, 0x9E62004E, 0x0C40695ED, 0x0F101A57B,
            0x0AA08F4C1, 0x9D0FC457, 0x0C5B0D9C6, 0x0F4B7E950, 0x0AFBEB8EA,
            0x98B9887C, 0x0C2DD1DDF, 0x0F7DA2D49, 0x0ACD37CF3, 0x9BD44C65,
            0x0CDB26158, 0x0E4B551CE, 0x0BFBC0074, 0x88BB30E2, 0x0D2DFA541,
            0x0E7D895D7, 0x0BCD1C46D, 0x8BD6F4FB, 0x0D369E96A, 0x0E26ED9FC,
            0x0B9678846, 0x8E60B8D0, 0x0D4042D73, 0x0E1031DE5, 0x0BA0A4C5F,
            0x8D0D7CC9, 0x0D005713C, 0x0E90241AA, 0x0B20B1010, 0x850C2086,
            0x0DF68B525, 0x0EA6F85B3, 0x0B166D409, 0x8661E49F, 0x0DEDEF90E,
            0x0EFD9C998, 0x0B4D09822, 0x83D7A8B4, 0x0D9B33D17, 0x0ECB40D81,
            0x0B7BD5C3B, 0x80BA6CAD, 0x0EDB88320, 0x0A4BFB3B6, 0x0FFB6E20C,
            0x0C8B1D29A, 0x92D54739, 0x0A7D277AF, 0x0FCDB2615, 0x0CBDC1683,
            0x93630B12, 0x0A2643B84, 0x0F96D6A3E, 0x0CE6A5AA8, 0x940ECF0B,
            0x0A109FF9D, 0x0FA00AE27, 0x0CD079EB1, 0x900F9344, 0x0A908A3D2,
            0x0F201F268, 0x0C506C2FE, 0x9F62575D, 0x0AA6567CB, 0x0F16C3671,
            0x0C66B06E7, 0x9ED41B76, 0x0AFD32BE0, 0x0F4DA7A5A, 0x0C3DD4ACC,
            0x99B9DF6F, 0x0ACBEEFF9, 0x0F7B7BE43, 0x0C0B08ED5, 0x96D6A3E8,
            0x0BFD1937E, 0x0E4D8C2C4, 0x0D3DFF252, 0x89BB67F1, 0x0BCBC5767,
            0x0E7B506DD, 0x0D0B2364B, 0x880D2BDA, 0x0B90A1B4C, 0x0E2034AF6,
            0x0D5047A60, 0x8F60EFC3, 0x0BA67DF55, 0x0E16E8EEF, 0x0D669BE79,
            0x8B61B38C, 0x0B266831A, 0x0E96FD2A0, 0x0DE68E236, 0x840C7795,
            0x0B10B4703, 0x0EA0216B9, 0x0DD05262F, 0x85BA3BBE, 0x0B4BD0B28,
            0x0EFB45A92, 0x0D8B36A04, 0x82D7FFA7, 0x0B7D0CF31, 0x0ECD99E8B,
            0x0DBDEAE1D, 0x9B64C2B0, 0x9263F226, 0x0C96AA39C, 0x0FE6D930A,
            0x0A40906A9, 0x910E363F, 0x0CA076785, 0x0FD005713, 0x0A5BF4A82,
            0x94B87A14, 0x0CFB12BAE, 0x0F8B61B38, 0x0A2D28E9B, 0x97D5BE0D,
            0x0CCDCEFB7, 0x0FBDBDF21, 0x0A6D3D2D4, 0x9FD4E242, 0x0C4DDB3F8,
            0x0F3DA836E, 0x0A9BE16CD, 0x9CB9265B, 0x0C7B077E1, 0x0F0B74777,
            0x0A8085AE6, 0x990F6A70, 0x0C2063BCA, 0x0F5010B5C, 0x0AF659EFF,
            0x9A62AE69, 0x0C16BFFD3, 0x0F66CCF45, 0x0A00AE278, 0x890DD2EE,
            0x0D2048354, 0x0E503B3C2, 0x0BF672661, 0x8A6016F7, 0x0D169474D,
            0x0E66E77DB, 0x0BED16A4A, 0x8FD65ADC, 0x0D4DF0B66, 0x0E3D83BF0,
            0x0B9BCAE53, 0x8CBB9EC5, 0x0D7B2CF7F, 0x0E0B5FFE9, 0x0BDBDF21C,
            0x84BAC28A, 0x0DFB39330, 0x0E8B4A3A6, 0x0B2D03605, 0x87D70693,
            0x0DCDE5729, 0x0EBD967BF, 0x0B3667A2E, 0x82614AB8, 0x0D9681B02,
            0x0EE6F2B94, 0x0B40BBE37, 0x810C8EA1, 0x0DA05DF1B, 0x0ED02EF8D };

static unsigned sub_721B10(int length, unsigned a3, unsigned a4)
{
            unsigned v4; // eax@1
            int v5; // edx@1
            int v6; // ecx@2

            v5 = length;
            v4 = a3;
            if (length > 0)
            {
                v6 = 0;
                do
                {
                    v4 = dword_884D98[sa2b_save->checksum_dat[v6++] ^ (u8)v4] ^ ((unsigned)v4 >> 8);
                    --v5;
                }
                while (v5 > 0);
            }
            return a4 ^ v4;
}

static unsigned sub_721B10_dx(int length, unsigned a3, unsigned a4)
{
            unsigned v4; // eax@1
            int v5; // edx@1
            int v6; // ecx@2

            v5 = length;
            v4 = a3;
            if (length > 0)
            {
                v6 = 0;
                do
                {
                    v4 = dword_884D98[sadx_save->checksum_dat[v6++] ^ (u8)v4] ^ ((unsigned)v4 >> 8);
                    --v5;
                }
                while (v5 > 0);
            }
            return a4 ^ v4;
}

static unsigned sub_7172B0(int length)
{
      return sub_721B10(length, 0x6368616Fu, 0x686F6765u);
}

static unsigned sub_7172B0_dx(int length)
{
      return sub_721B10_dx(length, 0x6368616Fu, 0x686F6765u);
}

/*
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}*/

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

uint64_t flip(double d) {
  union {double d; uint64_t u64;} u;
  u.d = d;
  return u.u64 ^ (-(u.u64 >> 63) | 0x8000000000000000ULL);
}

static void WriteChaoChecksum()
{
			//double rando = fRand(0.0, 0.9);

            int off = 0x24C;
            sa2b_save->checksum_dat[51229 + off] = 0;
            sa2b_save->checksum_dat[51224 + off] = 0;
            sa2b_save->checksum_dat[51231 + off] = 0;
            sa2b_save->checksum_dat[51226 + off] = 0;
            sa2b_save->checksum_dat[51227 + off] = 0;
            sa2b_save->checksum_dat[51225 + off] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
            sa2b_save->checksum_dat[51228 + off] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
            sa2b_save->checksum_dat[51230 + off] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
            unsigned v1 = sub_7172B0(51232 + off);
            sa2b_save->checksum_dat[51229 + off] = (u8)v1;
            sa2b_save->checksum_dat[51224 + off] = (u8)(v1 >> 8);
            sa2b_save->checksum_dat[51231 + off] = (u8)(v1 >> 16);
            sa2b_save->checksum_dat[51226 + off] = (u8)(v1 >> 24);
            sa2b_save->checksum_dat[51227 + off] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);

			//OHHH, I have to generate random doubles multiple times.
			//Still not clear on several things but... it works!!!!!!

//byte)BitConverter.DoubleToInt64Bits(rand.NextDouble() * 0.000030517578125 * 255.9998931884766);
//printf("NEW CHECKSUM: 0x%4X%4X\n", sa2b_save->checksum_dat[51224+off], sa2b_save->checksum_dat[51228+off]);
}

static void WriteChaoChecksumDX()
{
          //double rando = fRand(0.0, 0.9);
           // int off = 0x24C;

        //  int off = 0;
            sadx_save->checksum_dat[51229] = 0;
            sadx_save->checksum_dat[51224] = 0;
            sadx_save->checksum_dat[51231] = 0;
            sadx_save->checksum_dat[51226] = 0;
            sadx_save->checksum_dat[51227] = 0;
            sadx_save->checksum_dat[51225] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
            sadx_save->checksum_dat[51228] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
            sadx_save->checksum_dat[51230] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
            unsigned v1 = sub_7172B0_dx(51232);
            sadx_save->checksum_dat[51229] = (u8)v1;
            sadx_save->checksum_dat[51224] = (u8)(v1 >> 8);
            sadx_save->checksum_dat[51231] = (u8)(v1 >> 16);
            sadx_save->checksum_dat[51226] = (u8)(v1 >> 24);
            sadx_save->checksum_dat[51227] = flip((fRand(0.0,0.9)) * 0.000030517578125 * 255.9998931884766);
}

static unsigned SA2B_MCD = 0x1BFC0; //default values used originally for testing
static unsigned SADX_MCD = 0x0BFC0;

//test 64
u64 ComputeChecksum64(u16* data, int size)
{
    u64 addAcc = 0;
    u64 xorAcc = 0;
	int i = 0;
    
    for (i = 0; i < size; ++i)
    {
        addAcc += data[i];
        xorAcc ^= data[i];
    }
    
    return (u64)((addAcc & 0xFFFFFFFF) + (xorAcc << 32));
}

//FE8 1
#if 0
u32 ComputeChecksum32(const u16* data, int size)
{
    unsigned addAcc = 0;
    unsigned xorAcc = 0;
	int i = 0;
    
    for (i = 0; i < size/2; ++i)
    {
        addAcc += data[i];
        xorAcc ^= data[i];
    }
    
    return (u32)((addAcc & 0xFFFF) + (xorAcc << 16));
}

//FE8 2
u16 ComputeChecksum16(const u16* data, int size)
{
    unsigned addAcc = 0;
    unsigned xorAcc = 0;
	int i = 0;
    
    for (i = 0; i < size/2; ++i)
    {
        addAcc += data[i];
        xorAcc ^= data[i];
    }
    
    return (u16)(addAcc + xorAcc);
}
#endif

static unsigned short crc_table [256] = {

0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b,
0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401,
0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738,
0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96,
0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb,
0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2,
0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827,
0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d,
0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

//!!!!!! This is the Sonic Riders checksum!
//Slightly modified Xenoblade's algo to make the initial val 0xFFFF and invert the result!
u16 crc16XenoRiders(u8 *in, int len)
{
  u16 crc;
  int offset;
  //u16 first_byte_crc;

  crc = 0xFFFF;
  if ( len > 0 )
  {
    for(offset = 0; offset < len; offset++)
        crc =  (crc << 8) ^ crc_table[(crc >> 8) ^ *in++];
  }
  return ~crc;
}

static const u16 crc16[256] =
{
            0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
            0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
            0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
            0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
            0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
            0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
            0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
            0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
            0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
            0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
            0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
            0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
            0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
            0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
            0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
            0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
            0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
            0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
            0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
            0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
            0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
            0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
            0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
            0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
            0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
            0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
            0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
            0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
            0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
            0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
            0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
            0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

u16 crc16Xeno(u8 *data, int len)
{
    u16 chk = 0x0000;
	int i = 0;
    for (i = 0; i < len; ++i)
       chk = (u16)(crc16[(data[i] ^ chk) & 0xFF] ^ chk >> 8);
    return chk;
}
/*
u16 computeCRC16(const u8* data, u16 initial)
{
    u16 chk = initial;
	int i = 0;
    for (i = 0; i < 0x9BE; ++i)
       chk = (u16)(crc16[(data[i] ^ chk) & 0xFF] ^ chk >> 8);
    return chk;
} */
/*
u16 RIDERS_CRC16_CCITT(void)
        {
            u8 top = 0x00;
            u8 bot = 0x00;
			int i = 0;
			u16 x = 0;
            for (i = 0; i<0x9BE; i+=1)
            {
                x = riders_main_save->data[i] ^ top;
                x ^= (x >> 4);
                top = (u8)(bot ^ (x >> 3) ^ (x << 4));
                bot = (u8)(x ^ (x << 5));
            }
            return (u16)(top << 8 | bot);
        }
*/

#include "tails_GBA_bin.h"
#include "knux_GBA_bin.h"
#include "amy_GBA_bin.h"
#include "moon_GBA_bin.h"
//#include "omochao_GBA_bin.h"
#include "ghosty_GBA_bin.h"
#include "base_chao_bin.h"

static int chao_slot = 1;
//4:StationSquare 5:EggCarrier 6:MysticRuins 1:ChaoGarden 2:Hero 3:Dark
static int chao_garden = 1;

// GBA Bonus Pack
static char AGB_MODE_TEXT[7][40] =
{
	"Zelda: A Link to the Past & Four Swords",
	"Fire Emblem",
	"Fire Emblem: The Sacred Stones",
	"Tiny Chao Garden",
	"Pok\x82mon FireRed & LeafGreen",
	"Pok\x82mon Ruby & Sapphire",
	"Pok\x82mon Emerald"
};

static char TCG_LANGUAGE[5][10] =
{
	"Japanese",
	"English",
	"German",
	"French",
	"Spanish"
};

static char CHAO_MODE_TEXT[8][22] =
{
	"Chao",
	"2r",
	"3",
	"4",
	"5",
	"6r",
	"7",
	"8"
};

static char CHAO_GARDEN_TEXT[6][18] =
{
	"Chao Garden",
	"Hero Garden",
	"Dark Garden",
	"Station Square",
	"The Egg Carrier",
	"The Mystic Ruins"
};

static char CHAO_DATA_NAMES[6][20] =
{
	"Tails Chao",
	"Knuckles Chao",
	"Amy Chao",
	"Romantic Moon Chao",
	"OMOCHAO",
	"Ghost Chao"
};

//development order
#if 0
static char SONIC_TITLE_TEXT[6][44] =
{
	"Sonic Adventure 2: Battle",
	"Sonic Adventure DX",
	"Sonic Advance",
	"Sonic Advance 2",
	"Sonic Pinball Party",
	"Sonic Riders"
	//"Pok\x82mon Rumble", //encrypted
	//"Fragile Dreams", //idk
	//"No More Heroes 2: Desperate Struggle" //not convinced
};
#endif

#ifdef HW_RVL
static char SONIC_TITLE_TEXT[6][44] =
{
	"Sonic Adventure 2: Battle",
	"Sonic Adventure DX",
	"Sonic Advance",
	"Sonic Advance 2",
	"Sonic Pinball Party",
	"Sonic Riders"
};
#endif

#ifdef HW_DOL
static uint8_t organizedTitles[18] =
{
	5, 10, 12, 16, 17, 18, 20, 21, 27, 28, 33, 38, 39, 40, 41, 42, 43, 44
};

static char SONIC_TITLE_TEXT[45][44] =
{
	"Sonic Adventure 2: Battle",
	"Sonic Adventure DX",
	"Sonic Advance",
	"Sonic Advance 2",
	"Sonic Pinball Party",
	"Sonic Riders"
	//"Naruto Clash of Ninja 2" // considering
};
#endif

static char ZATCH_TITLES[3][50] =
{
	"Zatch Bell! Mamodo Battles",
	"Konjiki no Gash Bell!! Yujo Tag Battle 2",
	"Konjiki no Gash Bell!! Yujo Tag Battle Full Power"
};

static char OOT_TITLES[5][24] =
{
	"Master Quest",
	"Collector's Edition",
	"Virtual Console",
	"GameCube", //TP
	"Wii"       //TP
};

#ifdef WANT_NET
static char DEVICE_TITLES[3][16] =
{
	"Nintendo DSi",
	"Nintendo 3DS",
	"Android"
	//"PSP"
};
#endif

static char EPISODE_TITLES[3][8] =
{
	"Sonic",
	"Shadow",
	"Silver"
};

static char TCG_FRUIT_NAMES[7][8] =
{
	"Orange",
	"Blue",
	"Pink",
	"Green",
	"Purple",
	"Yellow",
	"Red"
};

static char TCG_EGG_NAMES[6][12] =
{
	"Normal",
	"Garnet",
	"Onyx",
	"Peridot",
	"Topaz",
	"Glass"
	//Moon chao is shown but egg is normal and hatching is invisible with face/wings
};

static char MARKET_FRUIT_NAMES[1][24] =
{
	"Chao Garden Fruit"
};

static char MARKET_EGG_NAMES[5][28] =
{
	"Normal",
    "Yellow",
    "White",
    "Brown",
    "Sky Blue"
};

static char PRICE_FRUIT[4][6] =
{
	"20", //Chao Garden Fruit
    "1000", //Mint Candy
    "20", //SADX Garden Fruit
    "4000" //Mega Mushroom
};

static char PRICE_EGG[5][6] =
{
	"10", //Normal
    "12000", //Garnet
    "20000", //Onyx
    "16000", //Peridot
    "18000" //Topaz
};

void SA2B_AskGarden(void)
{
	chao_garden = 1; // default is Chao Garden
	u32 pressed;
	printf("HERO or DARK  <%s>", CHAO_GARDEN_TEXT[chao_garden-1]);
	do
	{
		VIDEO_WaitVSync();
		get_input(NULL, &pressed);

		if (!pressed)
			continue;

		if (pressed & INPUT_BUTTON_CANCEL)
		{
			printf("\nExiting...\n");
			exit(0);
		}

		if (pressed & INPUT_BUTTON_DOWN)
			chao_garden++;
		else if (pressed & INPUT_BUTTON_UP)
			chao_garden--;
		else if (pressed & INPUT_BUTTON_RIGHT)
			chao_garden += 1;
		else if (pressed & INPUT_BUTTON_LEFT)
			chao_garden -= 1;

		while (chao_garden < 1)
			chao_garden += 1;
		while (chao_garden >= 4)
			chao_garden -= 1;

		printf("\rHERO or DARK  <%s>\x1b[K", CHAO_GARDEN_TEXT[chao_garden-1]);
	} while (!(pressed & INPUT_BUTTON_OK));
	printf("  LET'S ROCK!\n");
}

void SADX_AskGarden(void)
{
	chao_garden = 4; // default is Station Square
	u32 pressed;
	printf("Where is the train headed for?  <%s>", CHAO_GARDEN_TEXT[chao_garden-1]);
	do
	{
		VIDEO_WaitVSync();
		get_input(NULL, &pressed);

		if (!pressed)
			continue;

		if (pressed & INPUT_BUTTON_CANCEL)
		{
			printf("\nExiting...\n");
			exit(0);
		}

		if (pressed & INPUT_BUTTON_DOWN)
			chao_garden++;
		else if (pressed & INPUT_BUTTON_UP)
			chao_garden--;
		else if (pressed & INPUT_BUTTON_RIGHT)
			chao_garden += 1;
		else if (pressed & INPUT_BUTTON_LEFT)
			chao_garden -= 1;

		while (chao_garden < 4)
			chao_garden += 1;
		while (chao_garden >= 7)
			chao_garden -= 1;

		printf("\rWhere is the train headed for?  <%s>\x1b[K", CHAO_GARDEN_TEXT[chao_garden-1]);
	} while (!(pressed & INPUT_BUTTON_OK));
	printf("\n");
}

bool FruitAsk()
{
	bool transfer = false;
	while (1)
	{
		u32 pressed;
		get_input(NULL, &pressed);
		VIDEO_WaitVSync();

		if (pressed & INPUT_BUTTON_OK) {
			printf("OK! ");
			usleep(25000);
			transfer = true;
			break;
		}
		if (pressed & INPUT_BUTTON_CANCEL) {
			printf("No. ");
			usleep(25000);
			break;
		}
		if (pressed & INPUT_BUTTON_START)
		{
			printf("Exiting...\n");
			exit(1);
		}
	}
	return transfer;
}

u8 TCGJewel(u8 texture)
{
	u8 shade = 1;
	switch (texture) {
		case  1: shade = 0x01; break; //gold
	}
	return shade;
}
u8 TCGColor(u8 color)
{
	u8 shade = 0;
	switch (color) {
		case  0: shade = 0x00; break; //normal
	}
	return shade;
}

void missionFailed(void)
{
	printf("\nMISSION FAILED.\n");
	fatUnmount("disk");
	sleep(2);
	VIDEO_SetBlack(true);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	exit(1);
}

void exitDark(int val)
{
	fatUnmount("disk");
	sleep(val);
	VIDEO_SetBlack(true);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	exit(1);
}

//egg savefile discounts
static bool MegaColl = false;
static bool SonicHeroes = false;
static bool ShadowTH = false; //free black egg
static bool SonicRiders = false;
static bool GemsColl = false;
static bool PSOnline = false; //reduce shiny purple egg by 2000
static bool BillyHatcher = false; //free normal egg, halve shiny egg
static bool SMonkeyBall = false;
static bool SMonkeyBall2 = false;
static bool SSBMelee = false;
static bool Pikmin = false; //reduces price of red/blue/yellow by 200
static bool MP4 = false;
static bool MP5 = false;
static bool MP6 = false;
static bool MP7 = false;
static bool FEPOR = false;
static bool ZeldaGC = false;
static bool FourSwordsAdv = false;
static bool MarioSunshine = false; //halves price of topaz
static bool PaperMarioGC = false;
static bool ZatchMB = false; //reduces price of gold by 400
static bool ZatchMF = false; //reduces price of gold by 400
static u8 TrinityLimit = 0; //reduces price of sapphire/topaz/garnet by 1000
static bool Unleash1Up = false;
//Wii titles
static bool SonicZG = false;
static bool SecretRings = false; //halves shiny yellow
static bool BlackKnight = false; //halves price of shiny black
static bool SonicWorldAdv = false;
static bool SonicColors = false;
static bool SonicAllStarsRacing = false;
static bool SSSTennis = false;
static bool NiGHTS = false; //halves price of shiny purple
static bool AMIGO = false;
static bool SonicChaos = false;
static bool SSBBrawl = false;
static bool SSB64 = false;
static bool Galaxy1 = false;
static bool Galaxy2 = false;
static bool SMWorld = false;
static bool ALTTP = false;
static bool AnotherCode = false; //halves price of shiny sky blue
static bool NPCPikmin = false; //adds olimar trophy after sending a certain amount of coins
static bool NPCPikmin2 = false; //same as above
static bool YGODT = false;
static bool EpicMickey = false; //halves price of shiny black
static bool SkywardSword = false;
static bool RODEA = false;
static bool OlympicGames = false;
static bool WinterGames = false;
static bool XENOBLADE = false;
static bool RUMBLE = false;

static u8 SEGA_GAMES = 2; //implied SADX/SA2B
static u8 NINTENDO_GAMES = 0;

#ifdef HW_RVL
static char WIISAVE0[] ATTRIBUTE_ALIGN(32) = "/title/00010001/"; //WiiWare/VC
static char WIISAVE1[] ATTRIBUTE_ALIGN(32) = "/title/00010000/";
static char WIISAVE2[] ATTRIBUTE_ALIGN(32) = "/data/banner.bin";

static char ZGSAVPATH[] ATTRIBUTE_ALIGN(32) = "52533945/data/0_Games.sav";
static char ZGSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52533950/data/0_Games.sav";

static char SMG1SAVPATH[] ATTRIBUTE_ALIGN(32) = "524d4745/data/GameData.bin";
static char SMG1SAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "524d4750/data/GameData.bin";

static char SMG2SAVPATH[] ATTRIBUTE_ALIGN(32) = "53423445/data/GameData.bin";
static char SMG2SAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "53423450/data/GameData.bin";

static char EMSAVPATH[] ATTRIBUTE_ALIGN(32) = "53454d45/data/save0.dat";
static char EMSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "53454d50/data/save0.dat";
static char EMSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "53454d4a/data/save0.dat";

static char YGOSAVPATH[] ATTRIBUTE_ALIGN(32) = "52384445/data/yugioh.dat";
static char YGOSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52384450/data/yugioh.dat";
static char YGOSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5238444a/data/yugioh.dat";

static char SSBSAVPATH[] ATTRIBUTE_ALIGN(32) = "52534245/data/autosv0.bin";
static char SSBSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52534250/data/autosv0.bin";
static char SSBSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5253424a/data/autosv0.bin";

static char SSSAVPATH[] ATTRIBUTE_ALIGN(32) = "534f5545/data/wiiking2.sav";
static char SSSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "534f5550/data/wiiking2.sav";
static char SSSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "534f554a/data/wiiking2.sav";

static char NIGHTSPATH[] ATTRIBUTE_ALIGN(32) = "52374545/data/Slot00/system.sav";
static char NIGHTSPATH_EU[] ATTRIBUTE_ALIGN(32) = "52374550/data/Slot00/system.sav";
static char NIGHTSPATH_JP[] ATTRIBUTE_ALIGN(32) = "5237454a/data/Slot00/system.sav";

static char ROSAVPATH[] ATTRIBUTE_ALIGN(32) = "53524f45/data/rodeasys.dat";
static char ROSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "53524f50/data/rodeasys.dat";
static char ROSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "53524f4a/data/rodeasys.dat";

static char XCSAVPATH[] ATTRIBUTE_ALIGN(32) = "53583445/data/monado01";
static char XCSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "53583450/data/monado01";
static char XCSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5358344a/data/monado01";

static char Z3SAVPATH[] ATTRIBUTE_ALIGN(32) = "4a414445/data/nocopy/qsdata.bin";
static char Z3SAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "4a414450/data/nocopy/qsdata.bin";
static char Z3SAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "4a41444a/data/nocopy/qsdata.bin";
//static char Z3SAVPATH_KR[] ATTRIBUTE_ALIGN(32) = "4a414454/data/nocopy/qsdata.bin";

static char TOGSAVPATH[] ATTRIBUTE_ALIGN(32) = "5354474a/data/save01.bin";

static char FFCCSAVPATH[] ATTRIBUTE_ALIGN(32) = "52464645/data/savedata";
static char FFCCSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52464650/data/savedata";
static char FFCCSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5246464a/data/savedata";

static char MURASAVPATH[] ATTRIBUTE_ALIGN(32) = "52534645/data/save.dat";
static char MURASAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52534650/data/save.dat";
static char MURASAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5253464a/data/save.dat";

static char ARCSAVPATH[] ATTRIBUTE_ALIGN(32) = "52504a45/data/raydata.dat";
static char ARCSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "52504a4a/data/raydata.dat";

static char TOS2SAVPATH[] ATTRIBUTE_ALIGN(32) = "52543445/data/01.dat";
static char TOS2SAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52543450/data/01.dat";
static char TOS2SAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5254344a/data/01.dat";

static char WINTSAVPATH[] ATTRIBUTE_ALIGN(32) = "524f4c45/data/Slot0/gamedata.sav";
static char WINTSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "524f4c50/data/Slot0/gamedata.sav";
static char WINTSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "524f4c4a/data/Slot0/gamedata.sav";
//static char WINTSAVPATH_KR[] ATTRIBUTE_ALIGN(32) = "524f4c54/data/Slot0/gamedata.sav";

static char FFCBSAVPATH[] ATTRIBUTE_ALIGN(32) = "52464345/data/usr0_1";
static char FFCBSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52464350/data/usr0_1";
static char FFCBSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5246434a/data/usr0_1";

static char ACCFSAVPATH[] ATTRIBUTE_ALIGN(32) = "52555545/data/rvforest.dat";
static char ACCFSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52555550/data/rvforest.dat";
static char ACCFSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5255554a/data/rvforest.dat";

static char SSARSAVPATH[] ATTRIBUTE_ALIGN(32) = "52335245/data/savedata.bin";
static char SSARSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52335250/data/savedata.bin";

static char OKSAVPATH[] ATTRIBUTE_ALIGN(32) = "524f5745/data/okami.bin";
static char OKSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "524f5750/data/okami.bin";
static char OKSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "524f574a/data/okami.bin";

static char PBRSAVPATH[] ATTRIBUTE_ALIGN(32) = "52504245/data/GeniusPbr/PbrSaveData";
static char PBRSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52504250/data/GeniusPbr/PbrSaveData";
static char PBRSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5250424a/data/GeniusPbr/PbrSaveData";

static char OOTSAVPATH[] ATTRIBUTE_ALIGN(32) = "4e414345/data/RAM_CZLE";
static char OOTSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "4e414350/data/RAM_CZLP";
static char OOTSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "4e41434a/data/RAM_CZLJ";

static char TPSAVPATH[] ATTRIBUTE_ALIGN(32) = "525a4445/data/zeldaTp.dat";
static char TPSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "525a4450/data/zeldaTp.dat";
static char TPSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "525a444a/data/zeldaTp.dat";
//static char TPSAVPATH_KO[] ATTRIBUTE_ALIGN(32) = "525A444b/data/zeldaTp.dat";

//Unleashed is for an easteregg
static char UNSAVPATH[] ATTRIBUTE_ALIGN(32) = "52535645/data/game_0.sav";
static char UNSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52535650/data/game_0.sav";
static char UNSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5253564a/data/game_0.sav";
static char UNSLOTSAVPATH[] ATTRIBUTE_ALIGN(32) = "52535645/data/game_";
static char UNSLOTSAVPATH_EU[] ATTRIBUTE_ALIGN(32) = "52535650/data/game_";
static char UNSLOTSAVPATH_JP[] ATTRIBUTE_ALIGN(32) = "5253564a/data/game_";


void WiiSaveFind(void)
{
	s32 fd = 0;
	char temp[42] ATTRIBUTE_ALIGN(32) = {0};
	
	sprintf(temp, "%s%s%s", WIISAVE1, "53454d4a", WIISAVE2);
	fd = IOS_Open(temp, IPC_OPEN_READ);
	if(fd > -1) {
		EpicMickey = true;
		++NINTENDO_GAMES; //published, it's theirs
		IOS_Close(fd);
		//printf("Found Japanese Epic Mickey data!\n");
	}
	sprintf(temp, "%s%s%s", WIISAVE1, "53454d45", WIISAVE2);
	fd = IOS_Open(temp, IPC_OPEN_READ);
	if(fd > -1) {
		EpicMickey = true;
		IOS_Close(fd);
	} else {
		sprintf(temp, "%s%s%s", WIISAVE1, "53454d50", WIISAVE2);
		fd = IOS_Open(temp, IPC_OPEN_READ);
		if(fd > -1) {
			EpicMickey = true;
			IOS_Close(fd);
		}
	}
	
	//Another Code R
	sprintf(temp, "%s%s%s", WIISAVE1, "524e4f50", WIISAVE2);
	fd = IOS_Open(temp, IPC_OPEN_READ);
	if(fd > -1) {
		AnotherCode = true;
		++NINTENDO_GAMES;
		IOS_Close(fd);
		//printf("Found Another Code R data!\n");
	} else {
		sprintf(temp, "%s%s%s", WIISAVE1, "524e4f4a", WIISAVE2);
		fd = IOS_Open(temp, IPC_OPEN_READ);
		if(fd > -1) {
			AnotherCode = true;
			++NINTENDO_GAMES;
			IOS_Close(fd);
		}
	}
	
	//Mario & Sonic at the Olympic Winter Games
	sprintf(temp, "%s%s%s", WIISAVE1, "524f4c45", WIISAVE2);
	fd = IOS_Open(temp, IPC_OPEN_READ);
	if(fd > -1) {
		WinterGames = true;
		++NINTENDO_GAMES;
		++SEGA_GAMES;
		IOS_Close(fd);
	} else { //to avoid marking the same game twice
		sprintf(temp, "%s%s%s", WIISAVE1, "524f4c50", WIISAVE2);
		fd = IOS_Open(temp, IPC_OPEN_READ);
		if(fd > -1) {
			WinterGames = true;
			++NINTENDO_GAMES;
			++SEGA_GAMES;
			IOS_Close(fd);
		}
	}
	
	if(verbose)
		printf("SEGA titles: %d\nNintendo titles: %d\n", SEGA_GAMES, NINTENDO_GAMES);
}
#endif

static char SOYOULIEK[9][38] =
{
	"So you like",
	"Oh, so you're into",
	"Sorry, I looked too much, you play",
	"Am I wrong saying you like",
	"I see, you enjoy",
	"You enjoy playing",
	"I wonder why you like",
	"Oh, it's that game",
	"Fancy a little"
};

static char COMMENTS[6][96] =
{
	"Skyward Sword\"?\nThat Fi lady reminds me of something... !\n",
	"Rodea\"?\nMr. Yuji Naka is OK-chao!\n",
	"Olympic Games\"?\nIt's the Final Countdown-chao!\n",
	"Winter Games\"?\nThis is actually the best Mario & Sonic game-chao!\n",
	"Xenoblade\"?\nIt should go without saying; Melia is the best girl-chao!\n",
	"Pok\x82mon Rumble\"?\nThose Toy Pok\x82mon things remind me of someone-chao!\n"
};

void OMOCHAO_SAYS(void)
{
	if(shutupOmochao)
		return;
	
	int msgCount = -1;
	char comment[96][384] = {{0}};
	
	if(SMWorld) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[0]);
	}
	if(ALTTP) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[1]);
	}
	if(SSBBrawl) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[2]);
	}
	if(NiGHTS) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[3]);
	}
	if(SonicColors) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[4]);
	}
	if(Galaxy1 || Galaxy2) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[5]);
	}
	if(SSB64 || SSBMelee) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[6]);
	}
	if(MarioSunshine) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[7]);
	}
	if(ZatchMB || ZatchMF) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[8]);
	}
	if(EpicMickey) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[9]);
	}
	if(SonicHeroes) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[10]);
	}
	if(ShadowTH) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[11]);
	}
	if(SonicRiders) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[12]);
	}
	if(BillyHatcher) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[13]);
	}
	if(PSOnline) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[14]);
	}
	if(SMonkeyBall || SMonkeyBall2) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[15]);
	}
	if(Pikmin) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[16]);
	}
	if(MP4 || MP5 || MP6 || MP7) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[17]);
	}
	if(FEPOR) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[18]);
	}
	if(ZeldaGC) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[19]);
	}
	if(FourSwordsAdv) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[20]);
	}
	if(PaperMarioGC) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[21]);
	}
	if(SonicZG) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[22]);
	}
	if(SecretRings) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[23]);
	}
	if(BlackKnight) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[24]);
	}
	if(SonicWorldAdv) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[25]);
	}
	if(SonicAllStarsRacing) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[26]);
	}
	if(SSSTennis) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[27]);
	}
	if(AMIGO) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[28]);
	}
	if(SonicChaos) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[29]);
	}
	if(AnotherCode) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[30]);
	}
	if(YGODT) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[31]);
	}
	if(TrinityLimit > 2) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[32]);
	}
	if(NINTENDO_GAMES > 8) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[33]);
	}
	if(SEGA_GAMES > 10) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[34]);
	}
	if(SEGA_GAMES > 10 && NINTENDO_GAMES > 10) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[35]);
	}
	if(SkywardSword) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[36]);
	}
	if(RODEA) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[37]);
	}
	if(OlympicGames) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[38]);
	}
	if(WinterGames) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[39]);
	}
	if(XENOBLADE) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[40]);
	}
	if(RUMBLE) {
		++msgCount;
		strcpy(comment[msgCount], COMMENTS[41]);
	}
	
	printf("\nOMOCHAO SAYS:\n");
	if(msgCount > -1) {
		int sass = rand() % (10 + 1 - 0) + 0;
		if(sass < 2)
			printf("I will access your data if you don't mind-chao!\n");
		else if(sass > 8)
			printf("Ha! Ha! I can read you like a BOOK-chao!\n");

		int pickTitle = rand() % (msgCount + 1 - 0) + 0;
		printf("%s \"%s\n", SOYOULIEK[rand() % (14 + 1 - 0) + 0],
			comment[pickTitle]);
	}
}

static bool xmas = false;
static bool halloween = false;
static bool newyear = false;
static bool valentine = false;
static bool fools = false;
static bool sonicBlue = false;
static bool metalHeroes = false;
static bool emeraldDay = false;
static bool sapphRubyDay = false;
static bool shinyNormDay = false;
static bool shinyPurpleDay = false;
static bool shinyBlueEgg = false;
static bool blueEggDay = false;
static bool shinyGreyDay = false;
static bool shinyYellowDay = false;
static bool shinyBlackDay = false;
static bool blackEggDay = false;
static bool goldEggDay = false;

int ProcessOrder(int type, int val)
{
	u16 value = 0;
	return value;
}

uint16_t AGBcalcChecksum(int tcg)
{
    uint16_t checksum = 0xFFFF;
	int i = 0;
	unsigned incre = 0;

    for (i = 0x855; i >= 0; i--) {
        uint8_t upper = checksum >> 8;

        uint8_t nextByte = sa2agb_save->data[tcg+2+incre];
		++incre;

        uint8_t pointer = checksum ^ nextByte;
        checksum = cryptTable[pointer & 0xFF] ^ upper;
    }
//	printf("show me val: %X", sa2agb_save->tcg_1[0x0]);

    return ~checksum; // invert the result
}

uint32_t SA2BmainChecksum()
{
	int i = 0, checksumMain = 0;
	for(i = 0; i < 0x379B; i++)
		checksumMain += sa2b_main_save->data[i];
	
	sa2b_main_save->checksum = checksumMain;
	
	return checksumMain;
}

#ifdef WANT_NET
//#define HTTP_HEADER_TEMPLATE "HTTP/1.1 200 OK\r\n"
const static char http_200[] = "HTTP/1.1 200 OK\r\n";

const static char indexdata[] = "<html> \
                               <head><title>A test page</title></head> \
                               <body> \
                               This small test page has had %d hits. \
                               </body> \
                               </html>";

//const static char http_html_hdr[] = "Content-type: text/html\r\n\r\n";

const static char http_dat_hdr[] = "Content-Length: %d\r\nContent-type: application/octet-stream\r\n\r\n";
//const static char http_dat_hdr[] = "Content-type: application/octet-stream\r\n\r\n";
const static char http_get_index[] = "GET / HTTP/1.1\r\n";
static	lwp_t httd_handle = (lwp_t)NULL;

int sock, csock;

void *httpd (void *arg) {

//	int sock, csock;
	int ret;
	u32	clientlen;
	struct sockaddr_in client;
	struct sockaddr_in server;
	char temp[1026];
	char contentSize[96];
	int fSize = 0;
	//static int hits=0;
	
	clientlen = sizeof(client);

	sock = net_socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

	if (sock == INVALID_SOCKET) {
      printf ("Cannot create a socket!\n");
    } else {

		memset (&server, 0, sizeof (server));
		memset (&client, 0, sizeof (client));

		server.sin_family = AF_INET;
		server.sin_port = htons (80);
		server.sin_addr.s_addr = INADDR_ANY;
		ret = net_bind (sock, (struct sockaddr *) &server, sizeof (server));
		
		if ( ret ) {

			printf("Error %d binding socket!\n", ret);

		} else {

			if ( (ret = net_listen( sock, 5)) ) {

				printf("Error %d listening!\n", ret);

			} else {
			
				while(1) {
	
					csock = net_accept (sock, (struct sockaddr *) &client, &clientlen);

					if ( csock < 0 ) {
					//	printf("Error connecting socket %d!\n", csock);
						while(1);
					}

					if(verbose) {
						printf("Connecting port %d from %s\n", client.sin_port, inet_ntoa(client.sin_addr));
						memset (temp, 0, 1026);
						ret = net_recv (csock, temp, 1024, 0);
						printf("Received %d bytes\n", ret);
					} else {
						memset (temp, 0, 1026);
						net_recv (csock, temp, 1024, 0);
					}
					//means a connection has been made
					//going to assume it worked fine
					if(sendChao)
						printf("Connected to device...");

					if ( !strncmp( temp, http_get_index, strlen(http_get_index) ) ) {
						//++hits;
						if(ret < 300 || (sendChao && android)) { //guessing, this allows dl from browsers
							net_send(csock, http_200, strlen(http_200), 0);
							
							if(!want3DS)
								fSize = 0x800;
							else
								fSize = 64*1024;
							sprintf(contentSize, http_dat_hdr, fSize);
							net_send(csock, contentSize, strlen(contentSize), 0);
							//printf("less than 300.\n");
						}
						//sprintf(temp, indexdata, hits);
						if(!want3DS)
							net_send(csock, chao_data, 0x800, 0);
						else if(sendChao) {
							//entire GBA save sent in two chunks
							net_send(csock, &sa2agb_save->game_main[0], 0xA000, 0);
							if(netDevice == 0) //trick dsi app
								net_send(csock, &sa2agb_save->data[0], 0xA000, 0);
							else
								net_send(csock, &sa2agb_save->data[0], 0x6000, 0); 
							
							sendChao = false;
						}
					}
					net_close (csock);
				}
			}
		}
	}
	return NULL;
}
#endif

typedef struct _LAData
{
	// A SAVE
	u8 chk1[0x70];
	u16 checksum1_1;
	u16 checksum1_2;
	u8 type_1[4];
	u8 chk2[0x18];
	u16 checksum2_1;
	u16 checksum2_2;
	u8 type_2[4];
	u8 chk3[0x18];
	u16 checksum3_1;
	u16 checksum3_2;
	u8 type_3[4];
	u8 chk4[0x4C8];
	union
	{
		u8 data1[0x40];
		struct
		{
			u8 palace1_1; //Set to 0xDD unlocks riddle/palace of 4S
			u8 palace1_2; //set to 02, means golden sword in Four Swords, otherwise 0
			u8 palace1_3;
			u8 palace1_4;
			u8 foursword1_1; //0x11
			u8 foursword1_2; //0xFF means you have green/blue/red keys
			u8 foursword1_3; //triforce medals
			u8 foursword1_4; //for triforce medals > 255
			u8 name1[6];
			u8 stuff1[0x32];
		} ATTRIBUTE_PACKED;
	};
	u8 pad1[0x4C0];
	union
	{
		u8 data2[0x40];
		struct
		{
			u8 palace2_1; //Set to 0xDD unlocks riddle/palace of 4S 
			u8 palace2_2; //set to 02, means golden sword in Four Swords, otherwise 0
			u8 palace2_3;
			u8 palace2_4;
			u8 foursword2_1; //0x11
			u8 foursword2_2; //0xFF means you have green/blue/red keys
			u8 foursword2_3; //triforce medals
			u8 foursword2_4; //for triforce medals > 255
			u8 name2[6];
			u8 stuff2[0x32];
		} ATTRIBUTE_PACKED;
	};
	u8 pad2[0x4C0];
	union
	{
		u8 data3[0x40];
		struct
		{
			u8 palace3_1; //Set to 0xDD unlocks riddle/palace of 4S 
			u8 palace3_2; //set to 02, means golden sword in Four Swords, otherwise 0
			u8 palace3_3;
			u8 palace3_4;
			u8 foursword3_1; //0x11
			u8 foursword3_2; //0xFF means you have green/blue/red keys
			u8 foursword3_3; //triforce medals
			u8 foursword3_4; //for triforce medals > 255
			u8 name3[6];
			u8 stuff3[0x32];
		} ATTRIBUTE_PACKED;
	};
	u8 pad3[0x1040];
} LAData;

typedef struct _ALData
{
	// A SAVE
	union
	{
		u64 data[0x400];
		struct
		{
			u8 fs[0x1000];
			u8 fs2[0x1000];
		} ATTRIBUTE_PACKED;
	};
} ALData;

ALData * alttp_name_save = NULL;
ALData * alttp_main_save = NULL;
LAData * pttla_main_save = NULL;

char nameZelda[8] = {0};
char nameZelda2[8] = {0};
char nameZelda3[8] = {0};

// check
//#include <zlib.h>

// FE
#include "FE8_SpecialContentDE_bin.h"
#include "FE8_SpecialContentFR_bin.h"
#include "FE8_SpecialContentES_bin.h"
#include "FE8_SpecialContentIT_bin.h"
#include "FE8_SpecialContent_bin.h"
#include "FE7_BonusContent_bin.h"

typedef struct _FE7Data
{
	u8 magic[0x6];
	u8 ident; //0x37 = valid FE7
	u8 ignore[0x712D];
	union
	{
		u16 data[0x280/2];
		struct
		{
			u8 entries[0x280];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
} FE7Data;

typedef struct _FE8Data
{
	u8 magic[0x6];
	u8 ident; //0x39 = valid FE8
	u8 pad[0x8];
	u8 lang; //for EUR, 0=English, 1=JP, 2=Deutsch, 4=Francais, 6=Espanol, 8=Italiano
	u8 ignore[0x724C];
	union
	{
		u16 data[0x140/2];
		struct
		{
			u8 entries[0x140];
		} ATTRIBUTE_PACKED;
	};
	u32 checksum;
} FE8Data;

FE7Data * fe7_main_save = NULL;
FE8Data * fe8_main_save = NULL;

// edited to work with the FE7/FE8 'transfer data' checksum
u32 ComputeChecksum32(const u16* data, int size)
{
    unsigned addAcc = 0;
    unsigned xorAcc = 0;

	//swap bytes to little endian
	u16 checksum = 0;
	
	int i = 0;

    for (i = 0; i < size/2; ++i)
    {
        u8 ck1 = data[i] >> 8;
        u8 ck2 = data[i];
        checksum = ck2 << 8 | ck1;

        addAcc += checksum;
        xorAcc ^= checksum;
    }
    
    return (u32)((addAcc) + (xorAcc));
}

u16 ComputeChecksum16(const u16* data, int size)
{
    unsigned addAcc = 0;
    unsigned xorAcc = 0;
	int i = 0;
    
    for (i = 0; i < size/2; ++i)
    {
        addAcc += data[i];
        xorAcc ^= data[i];
    }
    
    return (u16)(addAcc + xorAcc);
}

void getNames(FILE* fp)
{
	ALData * alttp_name_save = (ALData *)memalign(32, sizeof(ALData));
	if(!alttp_name_save) {
		missionFailed();
	}

#if 0
	//open file
	fp=fopen("disk:/AZLE01.EEP", "rb");
	if(fp == NULL) {
		fp = fopen("disk:/AZLP01.EEP", "r+");
		if(fp == NULL)
			missionFailed();
	}
#endif
	fread(alttp_name_save, 1, sizeof(ALData), fp);
	//fclose(fp);

	//	bool shifts = false;
	if(alttp_name_save->fs[0] == 0x54) { //apparently some versions don't shift by 8
	//	shifts = true;
		int h = 0;
		for(h = 0; h<0x400; ++h) {
			alttp_name_save->data[h] = bswap64(alttp_name_save->data[h]);
		}
	}
	int d = 0;
	for(d = 0; d < 8; ++d)
		nameZelda[d] = alttp_name_save->fs[0x588+d];
	for(d = 0; d < 8; ++d)
		nameZelda2[d] = alttp_name_save->fs[0xA88+d];
	for(d = 0; d < 8; ++d)
		nameZelda3[d] = alttp_name_save->fs[0xF88+d];
	
	//check if type is deleted
	if(alttp_name_save->fs[0x74] == 0x44)
		nameZelda[0] = 0;
	if(alttp_name_save->fs[0x94] == 0x44)
		nameZelda2[0] = 0;
	if(alttp_name_save->fs[0xB4] == 0x44)
		nameZelda3[0] = 0;
	
	free(alttp_name_save);
}

static char POKE_NAMES[8][16] =
{
	"Jirachi",
	"Mew",
	"Celebi",
	"Pikachu",
	"Pichu (EGG)",
	"Zigzagoon (EGG)",
	"Skitty (EGG)",
	"Swablu (EGG)"
};

#define QUIZ_LIST 26

u16 numarray[64];

void ShuffleQuiz()
{
//	unsigned get_inf = time(0);
	int low = 1;
	int	high = QUIZ_LIST;
	//if(8 == 1)
		//high = 2;

    int arraylength = high - low + 0; //not sure why, but it's including 0 so the 2 fixes that.
    //numarray[arraylength];

    /* Create array from low and high numbers provided. */
    int i;
    int j = low;
    for (i = 0; i < arraylength; i++)
    {
       numarray[i] = j + i;
     //  printf("%d\t", numarray[i]);
    }

    /* Shuffle array. */
    int temp;
    int randindex;
    for (i = 0; i < arraylength; i++)
    {
        temp = numarray[i];
        randindex = rand() % arraylength;
        //randindex = rand_r(&get_inf) % arraylength;
        if (numarray[i] == numarray[randindex])
        {
            i = i - 1;
        }
        else
        {
            numarray[i] = numarray[randindex];
            numarray[randindex] = temp;
        }
    }
}

static u8 ANSWERS[QUIZ_LIST] = { 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0 };

static char POKE_QUIZ[QUIZ_LIST][128] =
{
	"How many badges does Ash typically need? [A: 6] | [B: 8]",
	"What did Tracey enjoy doing? [A: Sketch] | [B: Cook]",
	"What was Duplica's PKM? [A: Clefairy] | [B: Ditto]",
	"Pikachu & Raichu's seiyuu both voiced another electric-themed duo.",
	"The Sensational Sisters have an affinity for [A: Water] | [B: Plants]",
	"Ash traveled back in time and met a younger version of myself. TRUE or FALSE?",
	"When someone says 'introduction' do they mean [A: Role-call] | [B: PKM Battle]",
	"Which PKM did Ash's mom keep at home: [A: Chansey] | [B: Mr. Mime]",
	"Which PKM did Ash see first: [A: Ho-oh] | [B: Lugia]",
	"Is Mudkip from [A: Gen 2] | [B: Gen 3]",
	"Who turned Ash's friends into dolls [A: Erika] | [B: Sabrina]",
	"Celadon City Gym gives out the [A: Rainbow Badge] | [B: Cascade Badge]",
	"Besides girls, Brock's main interest was to be a [A: Doctor] | [B: Breeder]",
	"How many glass orbs did Melody ask to retrieve? [A: 3] | [B: 4]",
	"How do you wake a sleeping Snorlax? [A: Pok\x82 Flute] | [B: Pok\x82 Harp]",
	"Which PKM did Ash trade on the S.S. Anne? [A: Pidgeotto] | [B: Butterfree]",
	"The GS Ball isn't for catching PKM but to [A: Boost stats] | [B: Call Celebi]",
	"What puzzle game is enjoyed best by trainers? [A: Panel de Pon] | [B: Tetris]",
	"Magikarp can only learn moves by using a TM. TRUE or FALSE?",
	"I host Pok\x82mon Talk alongside DJ [A: Mary] | [B: Milky]",
	"What are apricorns used for? [A: Evolving] | [B: Pok""\x82""balls]",
	"How many Wii Points was Pok\x82mon Rumble? [A: 1000] | [B: 1500]",
	"What title developed Game Freak for SEGA Genesis? [A: Pulseman] | [B: Ristar]",
	"What was at the Lake of Rage? [A: Dark Lugia] | [B: Red Gyarados]",
	"Why did Meowth learn to talk? [A: To join Team Rocket] | [B: For love]",
	"Which PKM game had RTC first [A: Yellow] | [B: Crystal]",
};

static char JOKE_QUIZ[15][128] =
{
	"Did Ash catch more than 50 PKM by the end of Master Quest?",
	"Is a Gyarados more dangerous than a raging Primeape?",
	"How many characters are voiced by Lisa Ortiz? [A: 74] | [B: 76]",
	"Does Ash and Ash's mom share the same voice actor?",
	"Does Ash dress up as a girl to win a gym badge?",
	"How many brothers does Brock have?",
	"How many Taurus did Ash catch?",
	"How many episodes did it take Ash to buy a bicycle for Misty?",
	"Does Jirachi only come out once every 1000 years?",
	"Which season did Tomoko Kawase sing in?",
	"How many PKM are mentioned in the Pok\x82 Rap?",
	"At what hour did Advanced Battle air on 4KidsTV?",
	"What's the coolest PKM and why is it Mewtwo?",
	"How many games were released for the Pok\x82mon mini?",
	"Was Sonic X on after PKM Advanced Challenge?"
};

bool quizPrompt(void)
{
	u32 pressed;
	do
	{
		VIDEO_WaitVSync();
		get_input(NULL, &pressed);

		if (!pressed)
			continue;

		if (pressed & INPUT_BUTTON_START)
		{
			printf("\nExiting...\n");
			exitDark(1);
		}
		if (pressed & INPUT_BUTTON_CANCEL)
		{
			return false;
		}
		if (pressed & INPUT_BUTTON_OK)
		{
			return true;
		}
	} while(1);
}

int pokeQuiz(void)
{
	int res = 0;
	printf("\n");
	printf("Greetings! I am Prof. Oak!");
	
	quizPrompt();
	printf("\r\x1b[K");
	
//	printf("So I take it you've come for a Pok\x82mon, eh?");
	printf("So you've come all this way for a Pok\x82mon, eh?");
	quizPrompt();
	printf("\r\x1b[K");
	
	printf("Very well, but first you must pass a test!");
	quizPrompt();
	printf("\r\x1b[K");
	
	printf("That's right! The Pok\x82 Quiz!");
	quizPrompt();
	printf("\r\x1b[K");
	
	printf("I will ask 10 questions, you must answer at least 5 correctly.");
	quizPrompt();
	printf("\r\x1b[K");
	
	printf("These are YES or NO questions or double choice A or B...");
	quizPrompt();
	printf("\r\x1b[K");
	
	printf("In any case, let's jump right in!");
	quizPrompt();
	printf("\r\x1b[K");
	
	ShuffleQuiz();
	
	int i = 0;
	for(i = 0; i < 9; ++i) {
	//	s8 question = rand() % 7;
		printf("%s", POKE_QUIZ[numarray[i]]);
		bool answer = quizPrompt();
		printf("\r\x1b[K");
		if(answer == (ANSWERS[numarray[i]])) {
			printf("CORRECT!");
#if WANT_SFX
			int voice = ASND_GetFirstUnusedVoice();
			ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)PMarioCoin_pcm,PMarioCoin_pcm_size,175,175,NULL);
#endif
			++res;
		} else {
			printf("WRONG!");
#if WANT_SFX
			int voice = ASND_GetFirstUnusedVoice();
			ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)TCGAsk_pcm,TCGAsk_pcm_size,200,200,NULL);
#endif
		}
		sleep(2);
		printf("\r\x1b[K");
	}
	// final question is impossible to answer
	if(1) {
		s8 question = rand() % 14;
		printf("%s", JOKE_QUIZ[question]);
		quizPrompt();
		printf("\r\x1b[K");
	}
	printf("Heh heh, it's quite all right.");
	quizPrompt();
	printf("\r\x1b[K");
	
	printf("The final answer doesn't matter!");
	quizPrompt();
	printf("\r\x1b[K");
	++res;
	
	return res;
}

void handleAGBData(void)
{
	//File operations
	//FILE *mcd_file;
	
	u32 pressed;
	FILE *agb_file;
	sa2agb_save = memalign(32, sizeof(ChaoSaveAGB));
	if(!sa2agb_save) {
		missionFailed();
	}
	
	bool open_success = false;
	
	//Mode selector
	if(mode == 0) {
		mode = 1;
		printf("MODE:  <%s>", AGB_MODE_TEXT[mode-1]);
		do
		{
			VIDEO_WaitVSync();
			get_input(NULL, &pressed);

			if (!pressed)
				continue;

			if (pressed & INPUT_BUTTON_CANCEL)
			{
				printf("\nExiting...\n");
				exitDark(1);
			}

			if (pressed & INPUT_BUTTON_RIGHT)
				mode += 1;
			else if (pressed & INPUT_BUTTON_LEFT)
				mode -= 1;

			while (mode < 1)
				mode += 1;
			while (mode >= 8)
				mode -= 1;

			printf("\rMODE:  <%s>\x1b[K", AGB_MODE_TEXT[mode-1]);
		} while (!(pressed & INPUT_BUTTON_OK));
		printf("\n");
	}
	
	// Zelda ALTTP AGB
	if(mode == 1) {
		int rare_chao = 0;
		
		// Open root save from GBI
		FILE* alttp_main = NULL;
		int EEPsize = 0;
		alttp_main = fopen("disk:/AZLE01.EEP", "r+");
		if(!alttp_main) {
			alttp_main = fopen("disk:/AZLP01.EEP", "r+");
		/*	if(!alttp_main) {
				printf("Failed to open AZLE & AZLP !");
				exitDark(2);
			} */
		}
		
		// RA path guess
		if(!alttp_main) {
			char tmp_path[256] = {0};
			sprintf(tmp_path, "%sZelda A Link to the Past.srm", hexaeco_path);
			alttp_main = fopen(tmp_path, "r+");
			if(!alttp_main) {
				printf("Failed to open AZLE & AZLP!\n");
				exitDark(2);
			}
		}
		
		// Get size
		fseek(alttp_main, 0, SEEK_END);
		EEPsize = ftell(alttp_main);
		rewind(alttp_main);
		
		if(EEPsize != 0x2000) {
			printf("Wrong size!");
			exitDark(2);
		}
		
		getNames(alttp_main);
		
		rewind(alttp_main);
		
		printf("Select a file:  <%d> ", rare_chao+1);
		do
		{
			VIDEO_WaitVSync();
			get_input(NULL, &pressed);

			if (!pressed)
				continue;

			if (pressed & INPUT_BUTTON_CANCEL)
			{
				printf("\nExiting...\n");
				exitDark(1);
			}

			if (pressed & INPUT_BUTTON_DOWN)
				rare_chao++;
			else if (pressed & INPUT_BUTTON_UP)
				rare_chao--;
			else if (pressed & INPUT_BUTTON_RIGHT)
				rare_chao += 1;
			else if (pressed & INPUT_BUTTON_LEFT)
				rare_chao -= 1;

			while (rare_chao < 0)
				rare_chao += 1;
			while (rare_chao >= 3)
				rare_chao -= 1;

			printf("\rSelect a file:  <%d> \x1b[K", rare_chao+1);
		} while (!(pressed & INPUT_BUTTON_OK));
		
		if(rare_chao == 0)
			printf("%s", nameZelda);
		else if(rare_chao == 1)
			printf("%s", nameZelda2);
		else if(rare_chao == 2)
			printf("%s", nameZelda3);
		
		printf("\n\n");
		VIDEO_WaitVSync();
		
		printf("-Palace of the Four Sword & Riddle Quest-\nContinue?\n");
			while (1)
			{
				u32 pressed;
				get_input(NULL, &pressed);
				VIDEO_WaitVSync();

				if (pressed & INPUT_BUTTON_OK) {
#if WANT_SFX
					int voice = ASND_GetFirstUnusedVoice();
				//	ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)WWRupeeRed_pcm,WWRupeeRed_pcm_size,175,175,NULL);
					ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)ALTTP_pcm,ALTTP_pcm_size,170,170,NULL);
#endif
					break;
				}

				if (pressed & INPUT_BUTTON_CANCEL)
				{
					exitDark(0);
				}
				if (pressed & INPUT_BUTTON_START)
				{
					printf("Exiting...\n");
					exitDark(1);
				}
			}
			
		ALData * alttp_main_save = (ALData *)memalign(32, sizeof(ALData));
		if(!alttp_main_save) {
			missionFailed();
		}
		LAData * pttla_main_save = (LAData *)memalign(32, sizeof(LAData));
		if(!pttla_main_save) {
			missionFailed();
		}
		
		fread(alttp_main_save, 1, sizeof(ALData), alttp_main);
		fseek(alttp_main, 0, SEEK_SET);
		
		if(rare_chao == 0) {
			if(alttp_main_save->fs[0] != 0x54) {
				if(alttp_main_save->fs[0] != 0x41) {
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
					printf("\n");
					printf("This data is not correct!");
					sleep(4);
					missionFailed();
				}
			}
			int sum_1 = 0;
			int sum_2 = 0;
			int pos = 0;
			int k = 4;
			int h = 0;
			bool shifts = false;
			if(alttp_main_save->fs[0] == 0x54) { //apparently some versions don't shift by 8
				shifts = true;
				for(h = 0; h<0x400; ++h) {
					alttp_main_save->data[h] = bswap64(alttp_main_save->data[h]);
				}
			}
			memcpy(pttla_main_save, alttp_main_save, sizeof(LAData));

			while (k > 0) {
				sum_1 += (pttla_main_save->type_1[pos] | (pttla_main_save->type_1[pos+1] << 8)) ^ k;
				pos += 2;
				k   -= 2;
			}
			sum_1 &= 0xFFFF;

			k = 0x40;
			pos = 0;

			//check if you have the stuff already
			if(pttla_main_save->palace1_1 == 0xDD) {
				fclose(alttp_main);
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
				printf("\n");
				printf("You have this data already!");
				sleep(4);
				missionFailed();
			}
			
			if(pttla_main_save->palace1_1 == 0x11)
				pttla_main_save->palace1_1 = 0xDD; //unlocks palace and riddle quest
			//golden sword Four Swords
			if(pttla_main_save->palace1_2 == 0)
				pttla_main_save->palace1_2 = 0x02; //Sets golden sword in file screen
			//three color keys
			pttla_main_save->foursword1_2 = 0xFF;
			//10 medals
			if(pttla_main_save->foursword1_3 < 0x0A && pttla_main_save->foursword1_4 == 0)
				pttla_main_save->foursword1_3 = 0x0A;

			while (k > 0) {
				sum_2 += (pttla_main_save->data1[pos] | (pttla_main_save->data1[pos+1] << 8)) ^ k;
				pos += 2;
				k   -= 2;
			}
			sum_2 &= 0xFFFF;

			int first = (sum_1 + sum_2) & 0xFFFF;
			int result = first << 16;
			int second = ~first & 0xFFFF;
			second += 1;
			result += second;

			pttla_main_save->checksum1_1 = result >> 16;
			pttla_main_save->checksum1_2 = result & 0x0000FFFF;

			pttla_main_save->checksum1_1 = bswap16(pttla_main_save->checksum1_1);
			pttla_main_save->checksum1_2 = bswap16(pttla_main_save->checksum1_2);

			//restore order
			memcpy(alttp_main_save, pttla_main_save, sizeof(ALData));
			h = 0;
			if(shifts) {
				for(h = 0; h<0x400; ++h) {
					alttp_main_save->data[h] = bswap64(alttp_main_save->data[h]);
				}
			}
			fwrite(alttp_main_save, 1, sizeof(ALData), alttp_main);
			fclose(alttp_main);
		}
		else if(rare_chao == 1) {
			if(alttp_main_save->fs[0] != 0x54) {
				if(alttp_main_save->fs[0] != 0x41) {
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
					printf("\n");
					printf("This data is not correct!");
					sleep(4);
					missionFailed();
				}
			}
			int sum_1 = 0;
			int sum_2 = 0;
			int pos = 0;
			int k = 4;
			int h = 0;
			bool shifts = false;
			if(alttp_main_save->fs[0] == 0x54) { //apparently some versions don't shift by 8
				shifts = true;
				for(h = 0; h<0x400; ++h) {
					alttp_main_save->data[h] = bswap64(alttp_main_save->data[h]);
				}
			}
			memcpy(pttla_main_save, alttp_main_save, sizeof(LAData));

			while (k > 0) {
				sum_1 += (pttla_main_save->type_2[pos] | (pttla_main_save->type_2[pos+1] << 8)) ^ k;
				pos += 2;
				k   -= 2;
			}
			sum_1 &= 0xFFFF;

			k = 0x40;
			pos = 0;

			//check if you have the stuff already
			if(pttla_main_save->palace2_1 == 0xDD) {
				fclose(alttp_main);
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
				printf("\n");
				printf("You have this data already!");
				sleep(4);
				missionFailed();
			}
			
			if(pttla_main_save->palace2_1 == 0x11)
				pttla_main_save->palace2_1 = 0xDD; //unlocks palace and riddle quest
			//golden sword Four Swords
			if(pttla_main_save->palace2_2 == 0)
				pttla_main_save->palace2_2 = 0x02; //Sets golden sword in file screen
			//three color keys
			pttla_main_save->foursword2_2 = 0xFF;
			//10 medals
			if(pttla_main_save->foursword2_3 < 0x0A && pttla_main_save->foursword2_4 == 0)
				pttla_main_save->foursword2_3 = 0x0A;

			while (k > 0) {
				sum_2 += (pttla_main_save->data2[pos] | (pttla_main_save->data2[pos+1] << 8)) ^ k;
				pos += 2;
				k   -= 2;
			}
			sum_2 &= 0xFFFF;

			int first = (sum_1 + sum_2) & 0xFFFF;
			int result = first << 16;
			int second = ~first & 0xFFFF;
			second += 1;
			result += second;

			pttla_main_save->checksum2_1 = result >> 16;
			pttla_main_save->checksum2_2 = result & 0x0000FFFF;

			pttla_main_save->checksum2_1 = bswap16(pttla_main_save->checksum2_1);
			pttla_main_save->checksum2_2 = bswap16(pttla_main_save->checksum2_2);

			//restore order
			memcpy(alttp_main_save, pttla_main_save, sizeof(ALData));
			h = 0;
			if(shifts) {
				for(h = 0; h<0x400; ++h) {
					alttp_main_save->data[h] = bswap64(alttp_main_save->data[h]);
				}
			}
			fwrite(alttp_main_save, 1, sizeof(ALData), alttp_main);
			fclose(alttp_main);
		}
		else if(rare_chao == 2) {
			if(alttp_main_save->fs[0] != 0x54) {
				if(alttp_main_save->fs[0] != 0x41) {
#if WANT_SFX
					int voice = ASND_GetFirstUnusedVoice();
					ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
					sleep(3);
#endif
					printf("\n");
					printf("This data is not correct!");
					sleep(4);
					missionFailed();
				}
			}
			int sum_1 = 0;
			int sum_2 = 0;
			int pos = 0;
			int k = 4;
			int h = 0;
			bool shifts = false;
			if(alttp_main_save->fs[0] == 0x54) { //apparently some versions don't shift by 8
				shifts = true;
				for(h = 0; h<0x400; ++h) {
					alttp_main_save->data[h] = bswap64(alttp_main_save->data[h]);
				}
			}
			memcpy(pttla_main_save, alttp_main_save, sizeof(LAData));

			while (k > 0) {
				sum_1 += (pttla_main_save->type_3[pos] | (pttla_main_save->type_3[pos+1] << 8)) ^ k;
				pos += 2;
				k   -= 2;
			}
			sum_1 &= 0xFFFF;

			k = 0x40;
			pos = 0;

			//check if you have the stuff already
			if(pttla_main_save->palace3_1 == 0xDD) {
				fclose(alttp_main);
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
				printf("\n");
				printf("You have this data already!");
				sleep(4);
				missionFailed();
			}
			
			if(pttla_main_save->palace3_1 == 0x11)
				pttla_main_save->palace3_1 = 0xDD; //unlocks palace and riddle quest
			//golden sword Four Swords
			if(pttla_main_save->palace3_2 == 0)
				pttla_main_save->palace3_2 = 0x02; //Sets golden sword in file screen
			//three color keys
			pttla_main_save->foursword3_2 = 0xFF;
			//10 medals
			if(pttla_main_save->foursword3_3 < 0x0A && pttla_main_save->foursword3_4 == 0)
				pttla_main_save->foursword3_3 = 0x0A;

			while (k > 0) {
				sum_2 += (pttla_main_save->data3[pos] | (pttla_main_save->data3[pos+1] << 8)) ^ k;
				pos += 2;
				k   -= 2;
			}
			sum_2 &= 0xFFFF;

			int first = (sum_1 + sum_2) & 0xFFFF;
			int result = first << 16;
			int second = ~first & 0xFFFF;
			second += 1;
			result += second;

			pttla_main_save->checksum3_1 = result >> 16;
			pttla_main_save->checksum3_2 = result & 0x0000FFFF;

			pttla_main_save->checksum3_1 = bswap16(pttla_main_save->checksum3_1);
			pttla_main_save->checksum3_2 = bswap16(pttla_main_save->checksum3_2);

			//restore order
			memcpy(alttp_main_save, pttla_main_save, sizeof(ALData));
			h = 0;
			if(shifts) {
				for(h = 0; h<0x400; ++h) {
					alttp_main_save->data[h] = bswap64(alttp_main_save->data[h]);
				}
			}
			fwrite(alttp_main_save, 1, sizeof(ALData), alttp_main);
			fclose(alttp_main);
		}
	}
	else if(mode == 2) { // FE7
		FILE* fe70_main = NULL;
		int SRMsize = 0;
		fe70_main = fopen("disk:/AE7E01.SRM", "r+");
		if(!fe70_main) {
			fe70_main = fopen("disk:/AE7X01.SRM", "r+");
		/*	if(!fe70_main) {
				printf("Failed to open AE7E & AE7X !");
				exitDark(2);
			} */
		}
		
		// RA path guess
		if(!fe70_main) {
			char tmp_path[256] = {0};
			sprintf(tmp_path, "%sFire Emblem.srm", hexaeco_path);
			fe70_main = fopen(tmp_path, "r+");
			if(!fe70_main) {
				printf("Failed to open AE7E & EE7X!\n");
				exitDark(2);
			}
		}
		
		// Get size
		fseek(fe70_main, 0, SEEK_END);
		SRMsize = ftell(fe70_main);
		rewind(fe70_main);
		
		if(SRMsize != 0x8000) {
			printf("Wrong size!");
			exitDark(2);
		}
		
		// FE7 work
		FE7Data * fe7_main_save = (FE7Data *)memalign(32, sizeof(FE7Data));
		if(!fe7_main_save) {
			missionFailed();
		}
	#if 0
		//check arg for external content file
		if(fe7_bin_path != NULL) {
			FILE *f;
			f=fopen(fe7_bin_path, "r");
			if(f == NULL) {
				fclose(f);
			} else {
				u16 sz = 0;
				fseek(f, 0, SEEK_END);
				sz = ftell(f);
				rewind(f);
				if(sz != 0x284) //not a valid file
					fclose(f);
				else {
					fe_content_dat = (u8 *)memalign(32, 0x284);
					fread(fe_content_dat, 1, 0x284, f);
					fclose(f);
				}
			}
		}
	#endif
			fread(fe7_main_save, 1, sizeof(FE7Data), fe70_main);
			fseek(fe70_main, 0, SEEK_SET);
			
			if(fe7_main_save->ident != 0x37) {
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
				printf("\n");
				printf("Data is not valid!");
				sleep(4);
				missionFailed();
			}
			//already have it
		#if 0
			if(fe7_main_save->checksum == 0x6A160000) {
			//if(fe7_main_save->checksum != 0) {
				// Play SFX
				
				printf("\n");
				printf("St. Elimine has already blessed this data...");
				sleep(4);
				missionFailed();
			}
		#endif
			memcpy(fe7_main_save->data, FE7_BonusContent_bin, 0x284);
			
		//	if(fe_content_dat != NULL)
		//		memcpy(fe7_main_save->data, fe_content_dat, 0x284);
			//free(fe_content_dat);
			
			fe7_main_save->checksum = ComputeChecksum32(&fe7_main_save->data[0], 0x280);
			
			//convert checksum to little endian
			uint8_t ck1 = fe7_main_save->checksum >> 24;
			uint8_t ck2 = fe7_main_save->checksum >> 16;
			uint8_t ck3 = fe7_main_save->checksum >> 8;
			uint8_t ck4 = fe7_main_save->checksum;
			
			fe7_main_save->checksum = ck2 << 8 | ck3 << 16 | ck4 << 24 | ck1;
			
			//if(!noWrite)
				fwrite(fe7_main_save, 1, sizeof(FE7Data), fe70_main);
			fclose(fe70_main);
			printf("\n");
			printf("Data has been blessed by St. Elimine...\n");
			//printf("May it bring you joy and comfort...");
#ifdef WANT_SFX
			int voice = ASND_GetFirstUnusedVoice();
			ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)WWRupeeRed_pcm,WWRupeeRed_pcm_size,175,175,NULL);
#endif
			sleep(3);
	}
	else if(mode == 3) { // FE8
		FILE* fe8_main = NULL;
		int SRMsize = 0;
		fe8_main = fopen("disk:/BE8E01.SRM", "r+");
		if(!fe8_main) {
			fe8_main = fopen("disk:/BE8P01.SRM", "r+");
		/*	if(!fe8_main) {
				printf("Failed to open BE8E & BE8P !\n");
				exitDark(2);
			} */
		}
		
		// RA path guess
		if(!fe8_main) {
			char tmp_path[256] = {0};
			sprintf(tmp_path, "%sFire Emblem The Sacred Stones.srm", hexaeco_path);
			fe8_main = fopen(tmp_path, "r+");
			if(!fe8_main) {
				printf("Failed to open BE8E & BE8P!\n");
				exitDark(2);
			}
		}
		
		// Get size
		fseek(fe8_main, 0, SEEK_END);
		SRMsize = ftell(fe8_main);
		rewind(fe8_main);
		
		if(SRMsize != 0x8000) {
			printf("Wrong size!");
			exitDark(2);
		}
		
		// FE8 work
		FE8Data * fe8_main_save = (FE8Data *)memalign(32, sizeof(FE8Data));
			if(!fe8_main_save) {
				missionFailed();
			}
		#if 0
			//check arg for external content file
			if(fe8_bin_path != NULL) {
				FILE *f;
				f=fopen(fe8_bin_path, "r");
				if(f == NULL) {
					fclose(f);
				} else {
					u16 sz = 0;
					fseek(f, 0, SEEK_END);
					sz = ftell(f);
					rewind(f);
					if(sz != 0x144) //not a valid file
						fclose(f);
					else {
						fe_content_dat = (u8 *)memalign(32, 0x144);
						fread(fe_content_dat, 1, 0x144, f);
						fclose(f);
					}
				}
			}
		#endif
			fread(fe8_main_save, 1, sizeof(FE8Data), fe8_main);
			fseek(fe8_main, 0, SEEK_SET);
			
			if(fe8_main_save->ident != 0x39) {
#if WANT_SFX
				int voice = ASND_GetFirstUnusedVoice();
				ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
				sleep(3);
#endif
				printf("\n");				
				printf("Data is not correct!");				
				sleep(4);
				missionFailed();
			}
			
			//already have it
		#if 0
			if(fe8_main_save->checksum == 0x24F10D00) {
			//if(fe8_main_save->checksum != 0) {
				// Play SFX
				
				printf("\n");
				printf("man, what am I doing");
				sleep(4);
				missionFailed();
			}
		#endif
			memcpy(fe8_main_save->data, FE8_SpecialContent_bin, 0x144);
			
			//2=Deutsch, 4=Francais, 6=Espanol, 8=Italiano
			if(fe8_main_save->lang == 2)
				memcpy(fe8_main_save->data, FE8_SpecialContentDE_bin, 0x144);
			else if(fe8_main_save->lang == 4)
				memcpy(fe8_main_save->data, FE8_SpecialContentFR_bin, 0x144);
			else if(fe8_main_save->lang == 6)
				memcpy(fe8_main_save->data, FE8_SpecialContentES_bin, 0x144);
			else if(fe8_main_save->lang == 8)
				memcpy(fe8_main_save->data, FE8_SpecialContentIT_bin, 0x144);
			
		//	if(fe_content_dat != NULL)
		//		memcpy(fe8_main_save->data, fe_content_dat, 0x144);
			//free(fe_content_dat);
			
			//verify checksum in case someone edits it to include their own item setup
			//calc checksum
			fe8_main_save->checksum = ComputeChecksum32(&fe8_main_save->data[0], 0x140);
			
			//convert checksum to little endian
			uint8_t ck1 = fe8_main_save->checksum >> 24;
			uint8_t ck2 = fe8_main_save->checksum >> 16;
			uint8_t ck3 = fe8_main_save->checksum >> 8;
			uint8_t ck4 = fe8_main_save->checksum;
			
			fe8_main_save->checksum = ck2 << 8 | ck3 << 16 | ck4 << 24 | ck1;
			
			//gimme
		//	printf("HERE GETTO: 0x%X,,", fe8_main_save->checksum);
			
			fwrite(fe8_main_save, 1, sizeof(FE8Data), fe8_main);
			fclose(fe8_main);
			printf("\n");
			printf("Princess L'Arachel sends her regards.");
#ifdef WANT_SFX
			int voice = ASND_GetFirstUnusedVoice();
			ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)WWRupeeRed_pcm,WWRupeeRed_pcm_size,175,175,NULL);
#endif
			sleep(3);
	}
	else if(mode == 4) { // TCG
		agb_file = NULL;
		int FLAsize = 0;
		int gba_game = 0;
		u32 pressed = 0;
		printf("Select a GBA game:  <%s>", SONIC_TITLE_TEXT[2]);
		do
		{
			VIDEO_WaitVSync();
			get_input(NULL, &pressed);

			if (!pressed)
				continue;

			if (pressed & INPUT_BUTTON_CANCEL)
			{
				printf("\nExiting...\n");
				exitDark(1);
			}

			else if (pressed & INPUT_BUTTON_RIGHT)
				gba_game += 1;
			else if (pressed & INPUT_BUTTON_LEFT)
				gba_game -= 1;

			while (gba_game < 0)
				gba_game += 1;
			while (gba_game >= 3)
				gba_game -= 1;

			printf("\rSelect a GBA game:  <%s>\x1b[K", SONIC_TITLE_TEXT[gba_game+2]);
		} while (!(pressed & INPUT_BUTTON_OK));
		printf("\n");
		
		//Load save based on argument path, if it fails try loading from the app folder
		if(gba_game == 0) {
			agb_file = fopen("disk:/ASOE78.FLA", "r+");
			if(agb_file == NULL)
				agb_file = fopen("disk:/ASOP78.FLA", "r+");
			else if(agb_file == NULL)
				agb_file = fopen("disk:/ASOJ8P.FLA", "r+");
			else if(agb_file == NULL)
				agb_file = fopen(sa1_path, "r+");
			else if(agb_file == NULL)
				agb_file = fopen("disk:/apps/chao transfer tool/sa1.sav", "r+");
			
			else if(!agb_file) {
				char tmp_path[256] = {0};
				sprintf(tmp_path, "%sSonic Advance.srm", hexaeco_path);
				agb_file = fopen(tmp_path, "r+");
			}
		} else if(gba_game == 1) {
			agb_file = fopen("disk:/A2NE78.FLA", "r+");
			if(agb_file == NULL)
				agb_file = fopen("disk:/A2NP78.FLA", "r+");
			else if(agb_file == NULL)
				agb_file = fopen("disk:/A2NJ8P.FLA", "r+");
			else if(agb_file == NULL)
				agb_file = fopen(sa2_path, "r+");
			else if(agb_file == NULL)
				agb_file = fopen("disk:/apps/chao transfer tool/sa2.sav", "r+");
			
			else if(!agb_file) {
				char tmp_path[256] = {0};
				sprintf(tmp_path, "%sSonic Advance 2.srm", hexaeco_path);
				agb_file = fopen(tmp_path, "r+");
			}
		} else if(gba_game == 2) {
			agb_file = fopen("disk:/A3VE8P.FLA", "r+");
			if(agb_file == NULL)
				agb_file = fopen("disk:/A3VP8P.FLA", "r+");
			else if(agb_file == NULL)
				agb_file = fopen("disk:/A86J8P.FLA", "r+");
			else if(agb_file == NULL)
				agb_file = fopen(spp_path, "r+");
			else if(agb_file == NULL)
				agb_file = fopen("disk:/apps/chao transfer tool/spp.sav", "r+");
			
			else if(!agb_file) {
				char tmp_path[256] = {0};
				sprintf(tmp_path, "%sSonic Pinball Party.srm", hexaeco_path);
				agb_file = fopen(tmp_path, "r+");
			}
		}
		
		if(agb_file == NULL) {
			printf("FAILED TO OPEN GBA DATA.\n");
#if WANT_SFX
			int voice = ASND_GetFirstUnusedVoice();
			ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)SRLoseRings_pcm,SRLoseRings_pcm_size,175,175,NULL);
			sleep(3);
#endif
			fatUnmount("disk");
			exit(1);
		}
		
		//allocate space for chao
		chao_data = memalign(32, sizeof(ChaoData));
		if(!chao_data) {
			missionFailed();
		}

		// Get size
		fseek(agb_file, 0, SEEK_END);
		FLAsize = ftell(agb_file);
		rewind(agb_file);
		
		if(FLAsize != 0x10000) {
			printf("Wrong size!");
			exitDark(2);
		}
		
		fseek(agb_file, 0, SEEK_SET);
		fread(sa2agb_save, 1, sizeof(ChaoSaveAGB), agb_file);
		
		//it seems that after the crc the 16 bit value is an id
		//for the TCG save, SA1 is 0x1708, SA2/SPP is 0x6519
		//with this data integrity can more or less be confirmed.
		
		//check save id for SA1/SA2/SPP
		bool validGame = false;
		if(!strncmp((const char *)sa2agb_save, "PIRO", 4))
			validGame = true;
		else if(!strncmp((const char *)sa2agb_save, "MGGE", 4))
			validGame = true;
		else if(!strncmp((const char *)sa2agb_save, "SONIC_PB", 8))
			validGame = true;
		
		//printf("Save ID: %s\n", (const char *)sa2agb_save);
		
		if(!validGame) {
			printf("Invalid savedata.\n");
			sleep(2);
			printf("Exiting...");
			exitDark(1);
		}
		
		//Play TCG open sfx
#if WANT_SFX
		int voice = ASND_GetFirstUnusedVoice();
		ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)TCGAsk_pcm,TCGAsk_pcm_size,200,200,NULL);
#endif

		//find latest save
		int idx = 0, tcg_slot = 0;
		u32 cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0, cnt6 = 0;
		cnt1 = ((sa2agb_save->count1>>24)&0xff) | ((sa2agb_save->count1<<8)&0xff0000) |
				((sa2agb_save->count1>>8)&0xff00) | ((sa2agb_save->count1<<24)&0xff000000);
		cnt2 = ((sa2agb_save->count2>>24)&0xff) | ((sa2agb_save->count2<<8)&0xff0000) |
				((sa2agb_save->count2>>8)&0xff00) | ((sa2agb_save->count2<<24)&0xff000000);
		cnt3 = ((sa2agb_save->count3>>24)&0xff) | ((sa2agb_save->count3<<8)&0xff0000) |
				((sa2agb_save->count3>>8)&0xff00) | ((sa2agb_save->count3<<24)&0xff000000);
		cnt4 = ((sa2agb_save->count4>>24)&0xff) | ((sa2agb_save->count4<<8)&0xff0000) |
				((sa2agb_save->count4>>8)&0xff00) | ((sa2agb_save->count4<<24)&0xff000000);
		cnt5 = ((sa2agb_save->count5>>24)&0xff) | ((sa2agb_save->count5<<8)&0xff0000) |
				((sa2agb_save->count5>>8)&0xff00) | ((sa2agb_save->count5<<24)&0xff000000);
		cnt6 = ((sa2agb_save->count6>>24)&0xff) | ((sa2agb_save->count6<<8)&0xff0000) |
				((sa2agb_save->count6>>8)&0xff00) | ((sa2agb_save->count6<<24)&0xff000000);
		
		if(cnt1 > idx) {
			idx = cnt1;
			tcg_slot = 1;
		} if(cnt2 > idx) {
			idx = cnt2;
			tcg_slot = 2;
		} if(cnt3 > idx) {
			idx = cnt3;
			tcg_slot = 3;
		} if(cnt4 > idx) {
			idx = cnt4;
			tcg_slot = 4;
		} if(cnt5 > idx) {
			idx = cnt5;
			tcg_slot = 5;
		} if(cnt6 > idx) {
			idx = cnt6;
			tcg_slot = 6;
		}

		// debug info
	//	printf("Largest: 0x%X, Slot: %d\n", idx, tcg_slot);
		//Let's not increase the counter when saving, it should not give any advantage
		
		//copy chao to chao slot
		memcpy(chao_data, &sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], 0x800);
		
		s8 rare_chao = 0;
		// Select rare chao: Tails/Amy/Knux
		printf("Select your Rare Chao:  <%s>", CHAO_DATA_NAMES[rare_chao]);
		do
		{
			VIDEO_WaitVSync();
			get_input(NULL, &pressed);
			if (!pressed)
				continue;

			if (pressed & INPUT_BUTTON_CANCEL)
			{
				printf("\nExiting...\n");
				exitDark(1);
			}

			if (pressed & INPUT_BUTTON_DOWN)
				rare_chao++;
			else if (pressed & INPUT_BUTTON_UP)
				rare_chao--;
			else if (pressed & INPUT_BUTTON_RIGHT)
				rare_chao += 1;
			else if (pressed & INPUT_BUTTON_LEFT)
				rare_chao -= 1;

			while (rare_chao < 0)
				rare_chao += 1;
			while (rare_chao >= 3)
				rare_chao -= 1;

			printf("\rSelect your Rare Chao:  <%s>\x1b[K", CHAO_DATA_NAMES[rare_chao]);
		} while (!(pressed & INPUT_BUTTON_OK));
		printf("\n");
		
		VIDEO_WaitVSync();
		
		printf("Transferring '%s' to %s...\n", CHAO_DATA_NAMES[rare_chao], SONIC_TITLE_TEXT[gba_game+2]);
		printf("Continue?\n");
		while (1)
		{
			u32 pressed;
			get_input(NULL, &pressed);
			VIDEO_WaitVSync();

			if (pressed & INPUT_BUTTON_OK) {
				break;
			}

			if (pressed & INPUT_BUTTON_CANCEL)
			{
				printf("Exiting...\n");
				exitDark(1);
			}
			if (pressed & INPUT_BUTTON_START)
			{
				printf("Exiting...\n");
				exitDark(1);
			}
		}
		
		// copy chao save to read some data from it
		if(rare_chao == 0)
			memcpy(chao_data, &sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], 0x800);
		else if(rare_chao == 1)
			memcpy(chao_data, &sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], 0x800);
		else if(rare_chao == 2)
			memcpy(chao_data, &sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], 0x800);
		
		bool writeChao = true;
		
		if(chao_data->chao_exists == 0) {
			writeChao = false;
			printf("A Chao exists in this data!\nOverwrite? ");
			while (1)
			{
				u32 pressed;
				get_input(NULL, &pressed);
				VIDEO_WaitVSync();

				if (pressed & INPUT_BUTTON_OK) {
					writeChao = true;
					printf("OK\n");
					break;
				}

				if (pressed & INPUT_BUTTON_CANCEL)
				{
					printf("No\n");
					break;
				}
				if (pressed & INPUT_BUTTON_START)
				{
					printf("Exiting...\n");
					exitDark(1);
				}
			}
		}
		
		if(writeChao) {
			if(rare_chao == 0)
				memcpy(&sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], tails_GBA_bin, 0x800);
			else if(rare_chao == 1)
				memcpy(&sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], knux_GBA_bin, 0x800);
			else if(rare_chao == 2)
				memcpy(&sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], amy_GBA_bin, 0x800);
			else if(rare_chao == 3)
				memcpy(&sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000], ghosty_GBA_bin, 0x4E0);
			
			//The Chao exists
			sa2agb_save->tcg_1[0x50+(tcg_slot*0x1000)-0x1000] = 0;
			
			// SA1 save mirrors ring count to chao data (not just TCG data)
			// Chao data rings overpower the rest, let's add the 1000 rings only if save has less
		#if 1
			if(chao_data->rings > 1000) {
				sa2agb_save->tcg_1[0x5C+(tcg_slot*0x1000)-0x1000] = chao_data->rings;
				sa2agb_save->tcg_1[0x5D+(tcg_slot*0x1000)-0x1000] = chao_data->rings >> 8;
				sa2agb_save->tcg_1[94  +(tcg_slot*0x1000)-0x1000] = chao_data->rings >> 16;
				sa2agb_save->tcg_1[0x5F+(tcg_slot*0x1000)-0x1000] = chao_data->rings >> 24;
			}
		#endif
			
			//In Sonic Advance 2, the header ring count overpowers the chao data ring count.
			sa2agb_save->tcg_1[0x4+(tcg_slot*0x1000)-0x1000] = chao_data->rings;
			sa2agb_save->tcg_1[0x5+(tcg_slot*0x1000)-0x1000] = chao_data->rings >> 8;
			sa2agb_save->tcg_1[0x6+(tcg_slot*0x1000)-0x1000] = chao_data->rings >> 16;
			sa2agb_save->tcg_1[0x7+(tcg_slot*0x1000)-0x1000] = chao_data->rings >> 24;
			
			//calculate checksum
			u16 checksum = AGBcalcChecksum((tcg_slot*0x1000)-0x1000);
			switch (tcg_slot) {
				case 1:
					sa2agb_save->checksum1 = (checksum>>8) | (checksum<<8);
					break;
				case 2:
					sa2agb_save->checksum2 = (checksum>>8) | (checksum<<8);
					break;
				case 3:
					sa2agb_save->checksum3 = (checksum>>8) | (checksum<<8);
					break;
				case 4:
					sa2agb_save->checksum4 = (checksum>>8) | (checksum<<8);
					break;
				case 5:
					sa2agb_save->checksum5 = (checksum>>8) | (checksum<<8);
					break;
				case 6:
					sa2agb_save->checksum6 = (checksum>>8) | (checksum<<8);
					break;
			}
			// debug
		//	printf("CHECKSUM: 0x%02X\n", checksum);
			
			fseek(agb_file, 0, SEEK_SET);
			fwrite(sa2agb_save, 1, sizeof(ChaoSaveAGB), agb_file);
			fclose(agb_file);
			printf("GBA data saved!\n");
#if WANT_SFX
			voice = ASND_GetFirstUnusedVoice();
			ASND_SetVoice(voice,VOICE_MONO_16BIT,32000,0,(u8 *)TCGOK_pcm,TCGOK_pcm_size,200,200,NULL);
#endif
		}
		free(chao_data);
	}
	else if(mode == 5) { // PKM FR/LG
		FGData * frlg_main_save = (FGData *)memalign(32, sizeof(FGData));
		if(!frlg_main_save) {
			missionFailed();
		}
		FILE *fp;
		bool foundRed = false;
		bool foundGreen = false;
		
		// START
			fp=fopen("disk:/BPRE01.FLA", "r+");
			if(fp != NULL) {
				printf("Found FireRed!\n");
				foundRed = true;
			}
			else {
				fp=fopen("disk:/BPRP01.FLA", "r+");
				if(fp != NULL) {
					printf("Found FireRed!\n");
					foundRed = true;
				}
				else {
					char tmp_path[256] = {0};
					sprintf(tmp_path, "%sPokemon FireRed.srm", hexaeco_path);
					fp = fopen(tmp_path, "r+");
					if(fp != NULL) {
						printf("Found FireRed!\n");
						foundRed = true;
					}
				}
			}
			
			if(foundRed)
				fclose(fp);
			
			fp=fopen("disk:/BPGE01.FLA", "r+");
			if(fp != NULL) {
				printf("Found LeafGreen!\n");
				foundGreen = true;
			}
			else {
				fp=fopen("disk:/BPGP01.FLA", "r+");
				if(fp != NULL) {
					printf("Found LeafGreen!\n");
					foundGreen = true;
				}
				else {
					char tmp_path[256] = {0};
					sprintf(tmp_path, "%sPokemon LeafGreen.srm", hexaeco_path);
					fp = fopen(tmp_path, "r+");
					if(fp != NULL) {
						printf("Found FireRed!\n");
						foundGreen = true;
					}
				}
			}
			
			if(!foundRed && !foundGreen) {
				printf("Data not found! Exiting...");
				exitDark(2);
			}
			
			if(foundRed && foundGreen) {
				printf("Found both FireRed & LeafGreen, load FireRed? ");
				do
				{
					VIDEO_WaitVSync();
					get_input(NULL, &pressed);
					if (!pressed)
						continue;
					if (pressed & INPUT_BUTTON_START)
					{
						printf("\nExiting...\n");
						exitDark(1);
					}
					if (pressed & INPUT_BUTTON_OK)
					{
						printf("OK\n");
						fp=fopen("disk:/BPRE01.FLA", "r+");
						if(fp != NULL) {
							//printf("Found FireRed!\n");
						}
						else {
							fp=fopen("disk:/BPRP01.FLA", "r+");
							if(fp != NULL) {
								//printf("Found FireRed!\n");
							}
							else {
								char tmp_path[256] = {0};
								sprintf(tmp_path, "%sPokemon FireRed.srm", hexaeco_path);
								fp = fopen(tmp_path, "r+");
								if(fp != NULL) {
									// yeah this is dumb
								}
							}
						}
						break;
					}
					if (pressed & INPUT_BUTTON_CANCEL)
					{
						printf("No, loading LeafGreen.\n");
						break;
					}
				} while (!(pressed & INPUT_BUTTON_OK));
				printf("\n");
				
				VIDEO_WaitVSync();
			}
			
			if(!foundGreen && foundRed) {
				fp=fopen("disk:/BPRE01.FLA", "r+");
				if(fp != NULL) {
					//printf("Found FireRed!\n");
				}
				else {
					fp=fopen("disk:/BPRP01.FLA", "r+");
					if(fp != NULL) {
						//printf("Found FireRed!\n");
					}
				}
			}
			
			// Select Poke
			s8 pokeSel = 0;
			printf("Select a PKM:  <%s>", POKE_NAMES[pokeSel]);
			do
			{
				VIDEO_WaitVSync();
				get_input(NULL, &pressed);

				if (!pressed)
					continue;

				if (pressed & INPUT_BUTTON_CANCEL)
				{
					printf("\nExiting...\n");
					exitDark(1);
				}

				if (pressed & INPUT_BUTTON_DOWN)
					pokeSel++;
				else if (pressed & INPUT_BUTTON_UP)
					pokeSel--;
				else if (pressed & INPUT_BUTTON_RIGHT)
					pokeSel += 1;
				else if (pressed & INPUT_BUTTON_LEFT)
					pokeSel -= 1;

				while (pokeSel < 0)
					pokeSel += 1;
				while (pokeSel >= 8)
					pokeSel -= 1;

				printf("\rSelect a PKM:  <%s>\x1b[K", POKE_NAMES[pokeSel]);
			} while (!(pressed & INPUT_BUTTON_OK));
			printf("\n");
			
			VIDEO_WaitVSync();
			
			// Poke Quiz
			if(pokeSel > 0) {
				int quizRes = 0;
				while(1) {
					quizRes = pokeQuiz();
					if(quizRes > 4) {
						printf("Congratulations! You passed!\n");
						sleep(1);
						if(quizRes > 9) {
							printf("Unlock the national dex and events?");
							bool answer = quizPrompt();
							if(answer) {
								wantNat = true;
								wantEvents = true;
#if WANT_SFX
								int voice = ASND_GetFirstUnusedVoice();
								ASND_SetVoice(voice,VOICE_MONO_16BIT,22050,0,(u8 *)WWRupeeRed_pcm,WWRupeeRed_pcm_size,175,175,NULL);
#endif
							}
							printf("\r\x1b[K");
						}
						break;
					}
					else {
						printf("OOH! Hmm... try again!\n");
						sleep(2);
						exitDark(2);
					}
				}
			}
			
			// Activate 'mon
			if(pokeSel == 1)
				wantMew = true;
			else if(pokeSel == 2)
				wantCelebi = true;
			else if(pokeSel == 3)
				wantPikachu = true;
			else if(pokeSel == 4)
				wantPichu = true;
			else if(pokeSel == 5)
				wantZigzag = true;
			else if(pokeSel == 6)
				wantSkitty = true;
			else if(pokeSel == 7)
				wantSwablu = true;
			
			fread(frlg_main_save, 1, sizeof(FGData), fp);
			fseek(fp, 0, SEEK_SET);
			
			//not save
			uint32_t ident = (frlg_main_save->dataUnk1[0x1000-8] << 24) | (frlg_main_save->dataUnk1[0x1000-7] << 16)
				| (frlg_main_save->dataUnk1[0x1000-6] << 8) | frlg_main_save->dataUnk1[0x1000-5];
			if(ident != 0x25200108) {
				printf("Data is not valid...\n");
				sleep(2);
				exitDark(1);
			}
			
			//Find current save
			bool saveB = false;
			if(frlg_main_save->dataUnk1[(0x1000*16)-4] > frlg_main_save->dataUnk1[0x1000-4])
				saveB = true;
			
			if(!saveB) {
				//find stuff
				int section0 = 0, section1 = 0, section2 = 0, section4 = 0; //section2 is for national dex
				int i = 0;
				for(i = 1; i<15; ++i) {
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 0)
						section0 = 0+(0x1000*i)-0x1000;
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 1)
						section1 = 0+(0x1000*i)-0x1000;
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 2)
						section2 = 0+(0x1000*i)-0x1000;
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 4)
						section4 = 0+(0x1000*i)-0x1000;
				}
				
				uint32_t gamecode = (frlg_main_save->dataUnk1[section0+0xAC] << 24) | (frlg_main_save->dataUnk1[section0+0xAD] << 16)
					| (frlg_main_save->dataUnk1[section0+0xAE] << 8) | frlg_main_save->dataUnk1[section0+0xAF];
				if(gamecode != 0x01000000) {
					printf("Incorrect game code.\n");
					sleep(2);
					exitDark(1);
				}
				
				//if pokedex is not unlocked
			//	if(frlg_main_save->dataUnk1[section0+0x19] == 01 && frlg_main_save->dataUnk1[section0+0x1A] == 0xDA)
			//		Sys_BackToLoader(); //natinal dex not unlocked
				
				//key item events
				if(wantEvents) {
					u8 slotFound = 0;
					bool auroraFound = false;
					bool mysticFound = false;
					u8 Aurora1 = 0x73;
					u8 Aurora2 = 0x01;
					u8 Mystic1 = 0x72;
					u8 Mystic2 = 0x01;
					u8 slot1 = 0;
					u8 slot2 = 0;
					
					//check if exists
					int k = 0;
					for(k = 0; k<120; k+=4) {
						if(frlg_main_save->dataUnk1[section1+0x3B8+k] == Aurora1 && frlg_main_save->dataUnk1[section1+0x3B9+k] == Aurora2)
							auroraFound = true;
						if(frlg_main_save->dataUnk1[section1+0x3B8+k] == Mystic1 && frlg_main_save->dataUnk1[section1+0x3B9+k] == Mystic2)
							mysticFound = true;
					}
					//Check "shown ticket" event flags, otherwise user can fill key items
					if(frlg_main_save->dataUnk1[section1+0xF3E] == 0x1E) //Aurora ticket shown
						auroraFound = true;
					if(frlg_main_save->dataUnk1[section1+0xF3E] == 0x1D) //Mystic ticket shown
						mysticFound = true;
					if(frlg_main_save->dataUnk1[section1+0xF3E] == 0x1F) { //Aurora+Mystic ticket shown
						auroraFound = true;
						mysticFound = true;
					}
					//aurora = 0x1E
					//mystic = 0x1D
					//both   = 0x1F
					//none   = 0x1C, I hope
					
					//find 2 empty slots
					for(k = 0; k<120; k+=4) {
						if(frlg_main_save->dataUnk1[section1+0x3B8+k] == 0 && frlg_main_save->dataUnk1[section1+0x3B9+k] == 0)
							++slotFound;
						if(slotFound == 1)
							slot1 = k;
						if(slotFound == 2)
							slot2 = k;
						if(slotFound > 1)
							break;
					}
				//	u16 key = ((uint16_t)frlg_main_save->dataUnk1[section0+0xAD] << 8) | frlg_main_save->dataUnk1[section0+0xAC];
					u16 quant = 0x0100; //1 ticket little endian
				//	quant ^= key;
					
					if(slotFound != 0 && !auroraFound) {
						frlg_main_save->dataUnk1[section1+0x3B8+slot1] = Aurora1;
						frlg_main_save->dataUnk1[section1+0x3B9+slot1] = Aurora2;
						frlg_main_save->dataUnk1[section1+0x3BA+slot1] = quant >> 8;
						frlg_main_save->dataUnk1[section1+0x3BB+slot1] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !mysticFound) {
						frlg_main_save->dataUnk1[section1+0x3B8+slot2] = Mystic1;
						frlg_main_save->dataUnk1[section1+0x3B9+slot2] = Mystic2;
						frlg_main_save->dataUnk1[section1+0x3BA+slot2] = quant >> 8;
						frlg_main_save->dataUnk1[section1+0x3BB+slot2] = quant;
						--slotFound;
					}
				}
				
				//unlock national pokedex
				if(wantNat && frlg_main_save->dataUnk1[section0+0x1B] == 0) {
					// Field A
					frlg_main_save->dataUnk1[section0+0x1B] = 0xB9;
					// Field B
					frlg_main_save->dataUnk1[section2+0x68] |= 1 << 0;
					// Field C
					frlg_main_save->dataUnk1[section2+0x11C] = 0x58;
					frlg_main_save->dataUnk1[section2+0x11D] = 0x62;
				}
				
				//Find free slot in party
			//	bool slot1, slot2, slot3, slot4, slot5, slot6 = false;
				int empty_slot = 0;
				if(frlg_main_save->dataUnk1[section1+0x34] == 6) { //not enough space
					printf("You need at least one spot in your party...\n");
					// Play SFX
					
					
					sleep(2);
					exitDark(2);
				} else {
					int i = 0;
					for(i = 0; i < 6; ++i) { //pointless loop
						if(frlg_main_save->dataUnk1[section1+0x38+8] == 0) {
							empty_slot = 1;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+8] == 0) {
							empty_slot = 2;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+8] == 0) {
							empty_slot = 3;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+8] == 0) {
							empty_slot = 4;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 5;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 6;
							break;
						}
					}
				}
				PickJirachi();
				int bito = frlg_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] >> (gen3ID & 7) & 1; //check bit, 384 = JIRACHI
				if(bito == 0) { //Jirachi not seen, assuming that if you have seen it you have it.
					u8 current;
					u8 newValue;
					current = frlg_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] = (u8)newValue;
					
					current = frlg_main_save->dataUnk1[section1+0x05F8+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section1+0x05F8+(gen3ID >> 3)] = (u8)newValue;
					
					current = frlg_main_save->dataUnk1[section4+0x0B98+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section4+0x0B98+(gen3ID >> 3)] = (u8)newValue;
					
					current = frlg_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] = (u8)newValue;
				}
				switch (empty_slot) {
					case 1: memcpy(&frlg_main_save->dataUnk1[section1+0x38], curr, 0x64);
					 break;
					case 2: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64], curr, 0x64);
					 break;
					case 3: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64], curr, 0x64);
					 break;
					case 4: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 5: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 6: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
				}
				++frlg_main_save->dataUnk1[section1+0x34];
				
				//calculate checksums
				int chk = Chksum(&frlg_main_save->dataUnk1[section0], 0xF2C);
				u8 val_1 = chk&0x000000FF;
				u8 val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section0+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section0+0xFF7] = val_2;
				
				chk = Chksum(&frlg_main_save->dataUnk1[section1], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section1+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section1+0xFF7] = val_2;
				
				chk = Chksum(&frlg_main_save->dataUnk1[section2], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section2+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section2+0xFF7] = val_2;
				
				chk = Chksum(&frlg_main_save->dataUnk1[section4], 0xF08);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section4+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section4+0xFF7] = val_2;
			} else {
				//B version
				int section0 = 0, section1 = 0, section2 = 0, section4 = 0;
				for(int i = 16; i<29; ++i) {
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 0)
						section0 = 0+(0x1000*i)-0x1000;
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 1)
						section1 = 0+(0x1000*i)-0x1000;
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 2)
						section2 = 0+(0x1000*i)-0x1000;
					if(frlg_main_save->dataUnk1[0+(0x1000*i)-12] == 4)
						section4 = 0+(0x1000*i)-0x1000;
				}
				uint32_t gamecode = (frlg_main_save->dataUnk1[section0+0xAC] << 24) | (frlg_main_save->dataUnk1[section0+0xAD] << 16)
					| (frlg_main_save->dataUnk1[section0+0xAE] << 8) | frlg_main_save->dataUnk1[section0+0xAF];
				if(gamecode != 0x01000000) {
					printf("Incorrect game code.\n");
					sleep(2);
					exitDark(1);
				}
				
				//if pokedex is not unlocked
			//	if(frlg_main_save->dataUnk1[section0+0x19] == 01 && frlg_main_save->dataUnk1[section0+0x1A] == 0xDA)
			//		Sys_BackToLoader(); //natinal dex not unlocked
				
				//key item events
				if(wantEvents) {
					u8 slotFound = 0;
					bool auroraFound = false;
					bool mysticFound = false;
					u8 Aurora1 = 0x73;
					u8 Aurora2 = 0x01;
					u8 Mystic1 = 0x72;
					u8 Mystic2 = 0x01;
					u8 slot1 = 0;
					u8 slot2 = 0;
					
					//check if exists
					for(int k = 0; k<120; k+=4) {
						if(frlg_main_save->dataUnk1[section1+0x3B8+k] == Aurora1 && frlg_main_save->dataUnk1[section1+0x3B9+k] == Aurora2)
							auroraFound = true;
						if(frlg_main_save->dataUnk1[section1+0x3B8+k] == Mystic1 && frlg_main_save->dataUnk1[section1+0x3B9+k] == Mystic2)
							mysticFound = true;
					}
					//Check "shown ticket" event flags, otherwise user can fill key items
					if(frlg_main_save->dataUnk1[section1+0xF3E] == 0x1E) //Aurora ticket shown
						auroraFound = true;
					if(frlg_main_save->dataUnk1[section1+0xF3E] == 0x1D) //Mystic ticket shown
						mysticFound = true;
					if(frlg_main_save->dataUnk1[section1+0xF3E] == 0x1F) { //Aurora+Mystic ticket shown
						auroraFound = true;
						mysticFound = true;
					}
					//aurora = 0x1E
					//mystic = 0x1D
					//both   = 0x1F
					//none   = 0x1C, I hope
					
					//find 2 empty slots
					for(int k = 0; k<120; k+=4) {
						if(frlg_main_save->dataUnk1[section1+0x3B8+k] == 0 && frlg_main_save->dataUnk1[section1+0x3B9+k] == 0)
							++slotFound;
						if(slotFound == 1)
							slot1 = k;
						if(slotFound == 2)
							slot2 = k;
						if(slotFound > 1)
							break;
					}
				//	u16 key = ((uint16_t)frlg_main_save->dataUnk1[section0+0xAD] << 8) | frlg_main_save->dataUnk1[section0+0xAC];
					u16 quant = 0x0100; //1 ticket little endian
				//	quant ^= key;
					
					if(slotFound != 0 && !auroraFound) {
						frlg_main_save->dataUnk1[section1+0x3B8+slot1] = Aurora1;
						frlg_main_save->dataUnk1[section1+0x3B9+slot1] = Aurora2;
						frlg_main_save->dataUnk1[section1+0x3BA+slot1] = quant >> 8;
						frlg_main_save->dataUnk1[section1+0x3BB+slot1] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !mysticFound) {
						frlg_main_save->dataUnk1[section1+0x3B8+slot2] = Mystic1;
						frlg_main_save->dataUnk1[section1+0x3B9+slot2] = Mystic2;
						frlg_main_save->dataUnk1[section1+0x3BA+slot2] = quant >> 8;
						frlg_main_save->dataUnk1[section1+0x3BB+slot2] = quant;
						--slotFound;
					}
				}
				
				//unlock national pokedex
				if(wantNat && frlg_main_save->dataUnk1[section0+0x1B] == 0) {
					// Field A
					frlg_main_save->dataUnk1[section0+0x1B] = 0xB9;
					// Field B
					frlg_main_save->dataUnk1[section2+0x68] |= 1 << 0;
					// Field C
					frlg_main_save->dataUnk1[section2+0x11C] = 0x58;
					frlg_main_save->dataUnk1[section2+0x11D] = 0x62;
				}
				
				//Find free slot in party
			//	bool slot1, slot2, slot3, slot4, slot5, slot6 = false;
				int empty_slot = 0;
				if(frlg_main_save->dataUnk1[section1+0x34] == 6) { //not enough space
					printf("You need at least one extra space in your party...\n");
					// Play SFX
					
					sleep(2);
					exitDark(1);
				} else {
					int i = 0;
					for(i = 0; i < 6; ++i) { //pointless loop
						if(frlg_main_save->dataUnk1[section1+0x38+8] == 0) {
							empty_slot = 1;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+8] == 0) {
							empty_slot = 2;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+8] == 0) {
							empty_slot = 3;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+8] == 0) {
							empty_slot = 4;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 5;
							break;
						}
						if(frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 6;
							break;
						}
					}
				}
				PickJirachi();
				int bito = frlg_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] >> (gen3ID & 7) & 1; //check bit, 384 = JIRACHI
				if(bito == 0) { //Jirachi not seen, assuming that if you have seen it you have it.
					u8 current;
					u8 newValue;
					current = frlg_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] = (u8)newValue;
					
					current = frlg_main_save->dataUnk1[section1+0x05F8+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section1+0x05F8+(gen3ID >> 3)] = (u8)newValue;
					
					current = frlg_main_save->dataUnk1[section4+0x0B98+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section4+0x0B98+(gen3ID >> 3)] = (u8)newValue;
					
					current = frlg_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					frlg_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] = (u8)newValue;
				}
				switch (empty_slot) {
					case 1: memcpy(&frlg_main_save->dataUnk1[section1+0x38], curr, 0x64);
					 break;
					case 2: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64], curr, 0x64);
					 break;
					case 3: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64], curr, 0x64);
					 break;
					case 4: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 5: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 6: memcpy(&frlg_main_save->dataUnk1[section1+0x38+0x64+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
				}
				++frlg_main_save->dataUnk1[section1+0x34];
				
				//calculate checksums
				int chk = Chksum(&frlg_main_save->dataUnk1[section0], 0xF2C);
				u8 val_1 = chk&0x000000FF;
				u8 val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section0+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section0+0xFF7] = val_2;
				
				chk = Chksum(&frlg_main_save->dataUnk1[section1], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section1+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section1+0xFF7] = val_2;
				
				chk = Chksum(&frlg_main_save->dataUnk1[section2], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section2+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section2+0xFF7] = val_2;
				
				chk = Chksum(&frlg_main_save->dataUnk1[section4], 0xF08);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				frlg_main_save->dataUnk1[section4+0xFF6] = val_1;
				frlg_main_save->dataUnk1[section4+0xFF7] = val_2;
			}
			
			fwrite(frlg_main_save, 1, sizeof(FGData), fp);
			fclose(fp);
		
		// END
	}
	else if(mode == 6) { // PKM RBY SPH
		PKData * poke_main_save = (PKData *)memalign(32, sizeof(PKData));
		if(!poke_main_save) {
			missionFailed();
		}
		
		//START
		bool foundRed = false;
		bool foundGreen = false;
		
		// START
			FILE* fp;
			fp=fopen("disk:/AXVE01.FLA", "r+");
			if(fp != NULL) {
				printf("Found Ruby!\n");
				foundRed = true;
			}
			else {
				fp=fopen("disk:/AXVE20.FLA", "r+");
				if(fp != NULL) {
					printf("Found Ruby!\n");
					foundRed = true;
				}
				else {
					fp=fopen("disk:/AXVP01.FLA", "r+");
					if(fp != NULL) {
						printf("Found Ruby!\n");
						foundRed = true;
					}
					else {
						char tmp_path[256] = {0};
						sprintf(tmp_path, "%sPokemon Ruby.srm", hexaeco_path);
						fp = fopen(tmp_path, "r+");
						if(fp != NULL) {
							printf("Found Ruby!\n");
							foundRed = true;
						}
					}
				}
			}
			
			if(foundRed)
				fclose(fp);
			
			fp=fopen("disk:/AXPE01.FLA", "r+");
			if(fp != NULL) {
				printf("Found Sapphire!\n");
				foundGreen = true;
			}
			else {
				fp=fopen("disk:/AXPE20.FLA", "r+");
				if(fp != NULL) {
					printf("Found Sapphire!\n");
					foundGreen = true;
				}
				else {
					fp=fopen("disk:/AXPP01.FLA", "r+");
					if(fp != NULL) {
						printf("Found Sapphire!\n");
						foundGreen = true;
					}
					else {
						char tmp_path[256] = {0};
						sprintf(tmp_path, "%sPokemon Sapphire.srm", hexaeco_path);
						fp = fopen(tmp_path, "r+");
						if(fp != NULL) {
							printf("Found Sapphire!\n");
							foundGreen = true;
						}
					}
				}
			}
			
			if(!foundRed && !foundGreen) {
				printf("Data not found! Exiting...");
				exitDark(2);
			}
			
			if(foundRed && foundGreen) {
				printf("Found both Ruby & Sapphire, load Ruby? ");
				do
				{
					VIDEO_WaitVSync();
					get_input(NULL, &pressed);
					if (!pressed)
						continue;
					if (pressed & INPUT_BUTTON_START)
					{
						printf("\nExiting...\n");
						exitDark(1);
					}
					if (pressed & INPUT_BUTTON_OK)
					{
						printf("OK\n");
						fp=fopen("disk:/AXVE01.FLA", "r+");
						if(fp != NULL) {
							//printf("Found FireRed!\n");
						}
						else {
							fp=fopen("disk:/AXVE20.FLA", "r+");
							if(fp != NULL) {
								//printf("Found FireRed!\n");
							}
							else {
								fp=fopen("disk:/AXVP20.FLA", "r+");
								if(fp != NULL) {
									//printf("Found FireRed!\n");
								}
								else {
									char tmp_path[256] = {0};
									sprintf(tmp_path, "%sPokemon Ruby.srm", hexaeco_path);
									fp = fopen(tmp_path, "r+");
									if(fp != NULL) {
										// heh, 220 221, whatever it takes
									}
								}
							}
						}
						break;
					}
					if (pressed & INPUT_BUTTON_CANCEL)
					{
						printf("No, loading LeafGreen.\n");
						break;
					}
				} while (!(pressed & INPUT_BUTTON_OK));
				printf("\n");
				
				VIDEO_WaitVSync();
			}
			
			if(!foundGreen && foundRed) {
				fp=fopen("disk:/AXVE01.FLA", "r+");
				if(fp != NULL) {
					//printf("Found FireRed!\n");
				}
				else {
					fp=fopen("disk:/AXVE20.FLA", "r+");
					if(fp != NULL) {
						//printf("Found FireRed!\n");
					}
					else {
						fp=fopen("disk:/AXVP01.FLA", "r+");
						if(fp != NULL) {
							//printf("Found FireRed!\n");
						}
						else {
							char tmp_path[256] = {0};
							sprintf(tmp_path, "%sPokemon Ruby.srm", hexaeco_path);
							fp = fopen(tmp_path, "r+");
							if(fp != NULL) {
								// yeah
							}
						}
					}
				}
			}
			
			// Select Poke
			s8 pokeSel = 0;
			printf("Select a PKM:  <%s>", POKE_NAMES[pokeSel]);
			do
			{
				VIDEO_WaitVSync();
				get_input(NULL, &pressed);

				if (!pressed)
					continue;

				if (pressed & INPUT_BUTTON_CANCEL)
				{
					printf("\nExiting...\n");
					exitDark(1);
				}

				if (pressed & INPUT_BUTTON_DOWN)
					pokeSel++;
				else if (pressed & INPUT_BUTTON_UP)
					pokeSel--;
				else if (pressed & INPUT_BUTTON_RIGHT)
					pokeSel += 1;
				else if (pressed & INPUT_BUTTON_LEFT)
					pokeSel -= 1;

				while (pokeSel < 0)
					pokeSel += 1;
				while (pokeSel >= 8)
					pokeSel -= 1;

				printf("\rSelect a PKM:  <%s>\x1b[K", POKE_NAMES[pokeSel]);
			} while (!(pressed & INPUT_BUTTON_OK));
			printf("\n");
			
			VIDEO_WaitVSync();
			
			// Poke Quiz
			if(pokeSel > 0) {
				int quizRes = 0;
				while(1) {
					quizRes = pokeQuiz();
					if(quizRes > 4) {
						printf("Congratulations! You passed!\n");
						sleep(1);
						if(quizRes > 9) {
							printf("Unlock the national dex and events?");
							bool answer = quizPrompt();
							if(answer) {
								wantNat = true;
								wantEvents = true;
							}
							printf("\r\x1b[K");
						}
						break;
					}
					else {
						printf("OOH! Hmm... try again!\n");
						sleep(2);
						exitDark(2);
					}
				}
			}
			
			// Activate 'mon
			if(pokeSel == 1)
				wantMew = true;
			else if(pokeSel == 2)
				wantCelebi = true;
			else if(pokeSel == 3)
				wantPikachu = true;
			else if(pokeSel == 4)
				wantPichu = true;
			else if(pokeSel == 5)
				wantZigzag = true;
			else if(pokeSel == 6)
				wantSkitty = true;
			else if(pokeSel == 7)
				wantSwablu = true;
			
			fread(poke_main_save, 1, sizeof(PKData), fp); //full: 0x20000
			fseek(fp, 0, SEEK_SET);
			
			//not save
			uint32_t ident = (poke_main_save->dataUnk1[0x1000-8] << 24) | (poke_main_save->dataUnk1[0x1000-7] << 16)
				| (poke_main_save->dataUnk1[0x1000-6] << 8) | poke_main_save->dataUnk1[0x1000-5];
			if(ident != 0x25200108) {
				printf("Data is not valid.");
				sleep(2);
				exitDark(1);
			}
			
			//Find current save
			bool saveB = false;
			if(poke_main_save->dataUnk1[(0x1000*16)-4] > poke_main_save->dataUnk1[0x1000-4])
				saveB = true;
			
			if(!saveB) {
				//find stuff
				int section0 = 0, section1 = 0, section2 = 0, section4 = 0; //section2 is for national dex
				for(int i = 1; i<15; ++i) {
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 0)
						section0 = 0+(0x1000*i)-0x1000;
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 1)
						section1 = 0+(0x1000*i)-0x1000;
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 2)
						section2 = 0+(0x1000*i)-0x1000;
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 4)
						section4 = 0+(0x1000*i)-0x1000;
				}
				
				uint32_t gamecode = (poke_main_save->dataUnk1[section0+0xAC] << 24) | (poke_main_save->dataUnk1[section0+0xAD] << 16)
					| (poke_main_save->dataUnk1[section0+0xAE] << 8) | poke_main_save->dataUnk1[section0+0xAF];
				if(gamecode != 0) {
					printf("Incorrect game code.");
					sleep(2);
					exitDark(1);
				}
				
				//if pokedex is not unlocked
			//	if(poke_main_save->dataUnk1[section0+0x19] == 01 && poke_main_save->dataUnk1[section0+0x1A] == 0xDA)
			//		Sys_BackToLoader(); //natinal dex not unlocked
				
				//key item events
				if(wantEvents) {
					u8 slotFound = 0;
					bool eonFound = false;
					u8 Eon1 = 0x13;
					u8 Eon2 = 0x01;
					u8 slot1 = 0;
					
					//check if exists
					for(int k = 0; k<80; k+=4) {
						if(poke_main_save->dataUnk1[section1+0x5B0+k] == Eon1 && poke_main_save->dataUnk1[section1+0x5B1+k] == Eon2)
							eonFound = true;
					}
					//Check event flag, otherwise user can fill key items
					if(poke_main_save->dataUnk1[section2+0x3AA] == 0x08) //Eon ticket received
						eonFound = true;
					//none = 0
					
					//find 1 empty slot
					for(int k = 0; k<80; k+=4) {
						if(poke_main_save->dataUnk1[section1+0x5B0+k] == 0 && poke_main_save->dataUnk1[section1+0x5B1+k] == 0)
							++slotFound;
						if(slotFound == 1)
							slot1 = k;
						if(slotFound > 0)
							break;
					}
					u16 quant = 0x0100; //1 ticket little endian
					
					if(slotFound != 0 && !eonFound) {
						poke_main_save->dataUnk1[section1+0x5B0+slot1] = Eon1;
						poke_main_save->dataUnk1[section1+0x5B1+slot1] = Eon2;
						poke_main_save->dataUnk1[section1+0x5B2+slot1] = quant >> 8;
						poke_main_save->dataUnk1[section1+0x5B3+slot1] = quant;
						--slotFound;
					}
				}
				
				//unlock national pokedex
				if(wantNat && poke_main_save->dataUnk1[section0+0x19] == 0) {
					// Field A
					poke_main_save->dataUnk1[section0+0x19] = 0x01;
					poke_main_save->dataUnk1[section0+0x1A] = 0xDA;
					// Field B
					poke_main_save->dataUnk1[section2+0x3A6] |= 1 << 6;
					// Field C
					poke_main_save->dataUnk1[section2+0x44C] = 0x02;
					poke_main_save->dataUnk1[section2+0x44D] = 0x03;
				}
				
				//Find free slot in party
			//	bool slot1, slot2, slot3, slot4, slot5, slot6 = false;
				int empty_slot = 0;
				if(poke_main_save->dataUnk1[section1+0x234] == 6) { //not enough space
					printf("You need at least one free spot in the party.");
					//Play SFX
					
					sleep(2);
					exitDark(1);
				} else {
					int i = 0;
					for(i = 0; i < 6; ++i) { //pointless loop
						if(poke_main_save->dataUnk1[section1+0x238+8] == 0) {
							empty_slot = 1;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+8] == 0) {
							empty_slot = 2;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+8] == 0) {
							empty_slot = 3;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+8] == 0) {
							empty_slot = 4;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 5;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 6;
							break;
						}
					}
				}
				PickJirachi();
				int bito = poke_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] >> (gen3ID & 7) & 1; //check bit, 384 = JIRACHI
				if(bito == 0) { //Jirachi not seen, assuming that if you have seen it you have it.
					u8 current;
					u8 newValue;
					current = poke_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] = (u8)newValue;
					
					current = poke_main_save->dataUnk1[section1+0x0938+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section1+0x0938+(gen3ID >> 3)] = (u8)newValue;
					
					current = poke_main_save->dataUnk1[section4+0x0C0C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section4+0x0C0C+(gen3ID >> 3)] = (u8)newValue;
					
					current = poke_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] = (u8)newValue;
				}
				switch (empty_slot) {
					case 1: memcpy(&poke_main_save->dataUnk1[section1+0x238], curr, 0x64);
					 break;
					case 2: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64], curr, 0x64);
					 break;
					case 3: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64], curr, 0x64);
					 break;
					case 4: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 5: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 6: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
				}
				++poke_main_save->dataUnk1[section1+0x234];
				
				//calculate checksums
				int chk = Chksum(&poke_main_save->dataUnk1[section0], 0xF2C);
				u8 val_1 = chk&0x000000FF;
				u8 val_2 = (chk&0x0000FF00)>>8;
			//	uint16_t sum = val_2 | (val_1 << 8); //swapped
				poke_main_save->dataUnk1[section0+0xFF6] = val_1;
				poke_main_save->dataUnk1[section0+0xFF7] = val_2;
				
				chk = Chksum(&poke_main_save->dataUnk1[section1], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
			//	sum = val_2 | (val_1 << 8); //swapped
				poke_main_save->dataUnk1[section1+0xFF6] = val_1;
				poke_main_save->dataUnk1[section1+0xFF7] = val_2;
				
				chk = Chksum(&poke_main_save->dataUnk1[section2], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				poke_main_save->dataUnk1[section2+0xFF6] = val_1;
				poke_main_save->dataUnk1[section2+0xFF7] = val_2;
				
				chk = Chksum(&poke_main_save->dataUnk1[section4], 0xF08);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
			//	sum = val_2 | (val_1 << 8); //swapped
				poke_main_save->dataUnk1[section4+0xFF6] = val_1;
				poke_main_save->dataUnk1[section4+0xFF7] = val_2;
			} else {
				//B version
				int section0 = 0, section1 = 0, section2 = 0, section4 = 0;
				for(int i = 16; i<29; ++i) {
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 0)
						section0 = 0+(0x1000*i)-0x1000;
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 1)
						section1 = 0+(0x1000*i)-0x1000;
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 2)
						section2 = 0+(0x1000*i)-0x1000;
					if(poke_main_save->dataUnk1[0+(0x1000*i)-12] == 4)
						section4 = 0+(0x1000*i)-0x1000;
				}
				uint32_t gamecode = (poke_main_save->dataUnk1[section0+0xAC] << 24) | (poke_main_save->dataUnk1[section0+0xAD] << 16)
					| (poke_main_save->dataUnk1[section0+0xAE] << 8) | poke_main_save->dataUnk1[section0+0xAF];
				if(gamecode != 0) {
					printf("Incorrect game code.");
					sleep(1);
					exitDark(2);
				}
				
				//if pokedex is not unlocked
			//	if(poke_main_save->dataUnk1[section0+0x19] == 01 && poke_main_save->dataUnk1[section0+0x1A] == 0xDA)
			//		Sys_BackToLoader(); //natinal dex not unlocked
				
				//key item events
				if(wantEvents) {
					u8 slotFound = 0;
					bool eonFound = false;
					u8 Eon1 = 0x13;
					u8 Eon2 = 0x01;
					u8 slot1 = 0;
					
					//check if exists
					for(int k = 0; k<80; k+=4) {
						if(poke_main_save->dataUnk1[section1+0x5B0+k] == Eon1 && poke_main_save->dataUnk1[section1+0x5B1+k] == Eon2)
							eonFound = true;
					}
					//Check event flag, otherwise user can fill key items
					if(poke_main_save->dataUnk1[section2+0x3AA] == 0x08) //Eon ticket received
						eonFound = true;
					//none = 0
					
					//find 1 empty slot
					for(int k = 0; k<80; k+=4) {
						if(poke_main_save->dataUnk1[section1+0x5B0+k] == 0 && poke_main_save->dataUnk1[section1+0x5B1+k] == 0)
							++slotFound;
						if(slotFound == 1)
							slot1 = k;
						if(slotFound > 0)
							break;
					}
					u16 quant = 0x0100; //1 ticket little endian
					
					if(slotFound != 0 && !eonFound) {
						poke_main_save->dataUnk1[section1+0x5B0+slot1] = Eon1;
						poke_main_save->dataUnk1[section1+0x5B1+slot1] = Eon2;
						poke_main_save->dataUnk1[section1+0x5B2+slot1] = quant >> 8;
						poke_main_save->dataUnk1[section1+0x5B3+slot1] = quant;
						--slotFound;
					}
				}
				
				//unlock national pokedex
				if(wantNat && poke_main_save->dataUnk1[section0+0x19] == 0) {
					// Field A
					poke_main_save->dataUnk1[section0+0x19] = 0x01;
					poke_main_save->dataUnk1[section0+0x1A] = 0xDA;
					// Field B
					poke_main_save->dataUnk1[section2+0x3A6] |= 1 << 6;
					// Field C
					poke_main_save->dataUnk1[section2+0x44C] = 0x02;
					poke_main_save->dataUnk1[section2+0x44D] = 0x03;
				}
				
				//Find free slot in party
			//	bool slot1, slot2, slot3, slot4, slot5, slot6 = false;
				int empty_slot = 0;
				if(poke_main_save->dataUnk1[section1+0x234] == 6) { //not enough space
					printf("You need at least one spot in your party...\n");
					//Play SFX
					
					sleep(2);
					exitDark(1);
				} else {
					int i = 0;
					for(i = 0; i < 6; ++i) { //pointless loop
						if(poke_main_save->dataUnk1[section1+0x238+8] == 0) {
							empty_slot = 1;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+8] == 0) {
							empty_slot = 2;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+8] == 0) {
							empty_slot = 3;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+8] == 0) {
							empty_slot = 4;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 5;
							break;
						}
						if(poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 6;
							break;
						}
					}
				}
				PickJirachi();
				int bito = poke_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] >> (gen3ID & 7) & 1; //check bit, 384 = JIRACHI
				if(bito == 0) { //Jirachi not seen, assuming that if you have seen it you have it.
					u8 current;
					u8 newValue;
					current = poke_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] = (u8)newValue;
					
					current = poke_main_save->dataUnk1[section1+0x0938+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section1+0x0938+(gen3ID >> 3)] = (u8)newValue;
					
					current = poke_main_save->dataUnk1[section4+0x0C0C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section4+0x0C0C+(gen3ID >> 3)] = (u8)newValue;
					
					current = poke_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					poke_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] = (u8)newValue;
				}
				switch (empty_slot) {
					case 1: memcpy(&poke_main_save->dataUnk1[section1+0x238], curr, 0x64);
					 break;
					case 2: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64], curr, 0x64);
					 break;
					case 3: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64], curr, 0x64);
					 break;
					case 4: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 5: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 6: memcpy(&poke_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
				}
				++poke_main_save->dataUnk1[section1+0x234];
				
				//calculate checksums
				int chk = Chksum(&poke_main_save->dataUnk1[section0], 0xF2C);
				u8 val_1 = chk&0x000000FF;
				u8 val_2 = (chk&0x0000FF00)>>8;
				poke_main_save->dataUnk1[section0+0xFF6] = val_1;
				poke_main_save->dataUnk1[section0+0xFF7] = val_2;
				
				chk = Chksum(&poke_main_save->dataUnk1[section1], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				poke_main_save->dataUnk1[section1+0xFF6] = val_1;
				poke_main_save->dataUnk1[section1+0xFF7] = val_2;
				
				chk = Chksum(&poke_main_save->dataUnk1[section2], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				poke_main_save->dataUnk1[section2+0xFF6] = val_1;
				poke_main_save->dataUnk1[section2+0xFF7] = val_2;
				
				chk = Chksum(&poke_main_save->dataUnk1[section4], 0xF08);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				poke_main_save->dataUnk1[section4+0xFF6] = val_1;
				poke_main_save->dataUnk1[section4+0xFF7] = val_2;
			}
			
			fwrite(poke_main_save, 1, sizeof(PKData), fp);
			fclose(fp);
		//END
	}
	else if(mode == 7) { // PKM Emerald
		EMData * emerald_main_save = (EMData *)memalign(32, sizeof(EMData));
		if(!emerald_main_save) {
			missionFailed();
		}
		
		FILE *fp;
		fp=fopen("disk:/BPEE01.FLA", "r+");
		if(fp != NULL) {
			printf("Found Emerald!\n");
		}
		else {
			fp=fopen("disk:/BPEP01.FLA", "r+");
			if(fp != NULL) {
				printf("Found Emerald!\n");
			}
		}
		
		// RA path guess
		if(!fp) {
			char tmp_path[256] = {0};
			sprintf(tmp_path, "%sPokemon Emerald.srm", hexaeco_path);
			fp = fopen(tmp_path, "r+");
		}
		
		if(fp == NULL) {
			printf("BPEE or BPEP not found!\n");
			sleep(2);
			exitDark(1);
		}
		
		// Select Poke
			s8 pokeSel = 0;
			printf("Select a PKM:  <%s>", POKE_NAMES[pokeSel]);
			do
			{
				VIDEO_WaitVSync();
				get_input(NULL, &pressed);

				if (!pressed)
					continue;

				if (pressed & INPUT_BUTTON_CANCEL)
				{
					printf("\nExiting...\n");
					exitDark(1);
				}

				if (pressed & INPUT_BUTTON_DOWN)
					pokeSel++;
				else if (pressed & INPUT_BUTTON_UP)
					pokeSel--;
				else if (pressed & INPUT_BUTTON_RIGHT)
					pokeSel += 1;
				else if (pressed & INPUT_BUTTON_LEFT)
					pokeSel -= 1;

				while (pokeSel < 0)
					pokeSel += 1;
				while (pokeSel >= 8)
					pokeSel -= 1;

				printf("\rSelect a PKM:  <%s>\x1b[K", POKE_NAMES[pokeSel]);
			} while (!(pressed & INPUT_BUTTON_OK));
			printf("\n");
			
			VIDEO_WaitVSync();
			
			// Poke Quiz
			if(pokeSel > 0) {
				int quizRes = 0;
				while(1) {
					quizRes = pokeQuiz();
					if(quizRes > 4) {
						printf("Congratulations! You passed!\n");
						sleep(1);
						if(quizRes > 9) {
							printf("Unlock the national dex and events?");
							bool answer = quizPrompt();
							if(answer) {
								wantNat = true;
								wantEvents = true;
							}
							printf("\r\x1b[K");
						}
						break;
					}
					else {
						printf("OOH! Hmm... try again!\n");
						sleep(2);
						exitDark(2);
					}
				}
			}
			
			// Activate 'mon
			if(pokeSel == 1)
				wantMew = true;
			else if(pokeSel == 2)
				wantCelebi = true;
			else if(pokeSel == 3)
				wantPikachu = true;
			else if(pokeSel == 4)
				wantPichu = true;
			else if(pokeSel == 5)
				wantZigzag = true;
			else if(pokeSel == 6)
				wantSkitty = true;
			else if(pokeSel == 7)
				wantSwablu = true;
		
		//START
			fread(emerald_main_save, 1, sizeof(EMData), fp); //full: 0x20000
			fseek(fp, 0, SEEK_SET);
			
			//not save
			uint32_t ident = (emerald_main_save->dataUnk1[0x1000-8] << 24) | (emerald_main_save->dataUnk1[0x1000-7] << 16)
				| (emerald_main_save->dataUnk1[0x1000-6] << 8) | emerald_main_save->dataUnk1[0x1000-5];
			if(ident != 0x25200108) {
				printf("This data is not correct.");
				sleep(2);
				exitDark(1);
			}
			
			//Find current save
			bool saveB = false;
			//if(emerald_main_save->index_B01 > emerald_main_save->index_A01)
			//wait, this is practically just guessing, better check every entry
			if(emerald_main_save->dataUnk1[(0x1000*16)-4] > emerald_main_save->dataUnk1[0x1000-4])
				saveB = true;
			
			if(!saveB) {
				//find stuff
				int section0 = 0, section1 = 0, section2 = 0, section4 = 0;
				for(int i = 1; i<15; ++i) {
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 0)
						section0 = 0+(0x1000*i)-0x1000;
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 1)
						section1 = 0+(0x1000*i)-0x1000;
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 2)
						section2 = 0+(0x1000*i)-0x1000;
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 4)
						section4 = 0+(0x1000*i)-0x1000;
				}
				
				uint32_t gamecode = (emerald_main_save->dataUnk1[section0+0xAC] << 24) | (emerald_main_save->dataUnk1[section0+0xAD] << 16)
					| (emerald_main_save->dataUnk1[section0+0xAE] << 8) | emerald_main_save->dataUnk1[section0+0xAF];
				if(gamecode == 1 || gamecode == 0) {
					printf("Incorrect game code.");
					sleep(1);
					exitDark(2);
				}
				//if pokedex is not unlocked
				
				//key item events
				if(wantEvents) {
					u8 slotFound = 0;
					bool auroraFound = false;
					bool eonFound = false;
					bool mysticFound = false;
					bool oldSeaMapFound = false;
					u8 Aurora1 = 0x73;
					u8 Aurora2 = 0x01;
					u8 Eon1 = 0x13;
					u8 Eon2 = 0x01;
					u8 Mystic1 = 0x72;
					u8 Mystic2 = 0x01;
					u8 OldSeaMap1 = 0x78;
					u8 OldSeaMap2 = 0x01;
					u8 slot1 = 0;
					u8 slot2 = 0;
					u8 slot3 = 0;
					u8 slot4 = 0;
					
					//check if exists
					for(int k = 0; k<120; k+=4) {
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == Aurora1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == Aurora2)
							auroraFound = true;
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == Eon1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == Eon2)
							eonFound = true;
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == Mystic1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == Mystic2)
							mysticFound = true;
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == OldSeaMap1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == OldSeaMap2)
							oldSeaMapFound = true;
					}
					//Check "shown ticket" event flags, otherwise user can fill key items
					if(emerald_main_save->dataUnk1[section2+0x325] == 0x40) //Eon ticket shown
						eonFound = true; //recycle bool
					if(emerald_main_save->dataUnk1[section2+0x325] == 0x80) //Aurora ticket shown
						auroraFound = true;
					if(emerald_main_save->dataUnk1[section2+0x326] == 0x01) //Old Sea Map shown
						oldSeaMapFound = true;
					if(emerald_main_save->dataUnk1[section2+0x32B] == 0x08) //Mystic ticket shown
						mysticFound = true;
					if(emerald_main_save->dataUnk1[section2+0x325] == 0xC0) { //Eon+Aurora ticket shown
						eonFound = true;
						auroraFound = true;
					}
					
					//find 4 empty slots
					for(int k = 0; k<120; k+=4) {
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == 0 && emerald_main_save->dataUnk1[section1+0x5D9+k] == 0)
							++slotFound;
						if(slotFound == 1)
							slot1 = k;
						if(slotFound == 2)
							slot2 = k;
						if(slotFound == 3)
							slot3 = k;
						if(slotFound == 4)
							slot4 = k;
						if(slotFound > 3)
							break;
					}
					//u16 key = 0x0D00;
					//u16 quantity = 0x0100;
					//quantity ^= key;
					//grab key = key >> 16;
					//Uhh, security key volatile so have to do this
					u16 key = ((uint16_t)emerald_main_save->dataUnk1[section0+0xAD] << 8) | emerald_main_save->dataUnk1[section0+0xAC];
					u16 quant = 0x0100; //1 ticket little endian
					quant ^= key;
					
					if(slotFound != 0 && !auroraFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot1] = Aurora1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot1] = Aurora2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot1] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot1] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !eonFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot2] = Eon1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot2] = Eon2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot2] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot2] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !mysticFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot3] = Mystic1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot3] = Mystic2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot3] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot3] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !oldSeaMapFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot4] = OldSeaMap1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot4] = OldSeaMap2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot4] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot4] = quant;
						--slotFound;
					}
				}
				
				//unlock national pokedex
				if(wantNat && emerald_main_save->dataUnk1[section0+0x19] == 0) {
					// Field A
					emerald_main_save->dataUnk1[section0+0x19] = 0x01;
					emerald_main_save->dataUnk1[section0+0x1A] = 0xDA;
					// Field B
					emerald_main_save->dataUnk1[section2+0x402] |= 1 << 6;
					// Field C
					emerald_main_save->dataUnk1[section2+0x4A8] = 0x02;
					emerald_main_save->dataUnk1[section2+0x4A9] = 0x03;
				}
				
				//Find free slot in party
				int empty_slot = 0;
				if(emerald_main_save->dataUnk1[section1+0x234] == 6) { //not enough space
					printf("Need at least one spot free in party.");
					// Play SFX
					sleep(2);
					exitDark(2);
				} else {
					int i = 0;
					for(i = 0; i < 6; ++i) { //pointless loop
						if(emerald_main_save->dataUnk1[section1+0x238+8] == 0) {
							empty_slot = 1;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+8] == 0) {
							empty_slot = 2;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+8] == 0) {
							empty_slot = 3;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+8] == 0) {
							empty_slot = 4;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 5;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 6;
							break;
						}
					}
				}
				PickJirachi();
				//avoid for eggs?
				int bito = emerald_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] >> (gen3ID & 7) & 1; //check bit, 384 = JIRACHI
				if(bito == 0) { //Jirachi not seen, assuming that if you have seen it you have it.
					u8 current;
					u8 newValue;
					current = emerald_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] = (u8)newValue;
					
					current = emerald_main_save->dataUnk1[section1+0x0988+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section1+0x0988+(gen3ID >> 3)] = (u8)newValue;
					
					current = emerald_main_save->dataUnk1[section4+0x0CA4+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section4+0x0CA4+(gen3ID >> 3)] = (u8)newValue;
					
					current = emerald_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] = (u8)newValue;
				}
				switch (empty_slot) {
					case 1: memcpy(&emerald_main_save->dataUnk1[section1+0x238], curr, 0x64);
					 break;
					case 2: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64], curr, 0x64);
					 break;
					case 3: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64], curr, 0x64);
					 break;
					case 4: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 5: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 6: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
				}
				++emerald_main_save->dataUnk1[section1+0x234];
				
				//calculate checksums
				int chk = Chksum(&emerald_main_save->dataUnk1[section0], 0xF2C);
				u8 val_1 = chk&0x000000FF;
				u8 val_2 = (chk&0x0000FF00)>>8;
			//	uint16_t sum = val_2 | (val_1 << 8); //swapped
				emerald_main_save->dataUnk1[section0+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section0+0xFF7] = val_2;
				
				chk = Chksum(&emerald_main_save->dataUnk1[section1], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
			//	sum = val_2 | (val_1 << 8); //swapped
				emerald_main_save->dataUnk1[section1+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section1+0xFF7] = val_2;
				
				chk = Chksum(&emerald_main_save->dataUnk1[section2], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				emerald_main_save->dataUnk1[section2+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section2+0xFF7] = val_2;
				
				chk = Chksum(&emerald_main_save->dataUnk1[section4], 0xF08);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
			//	sum = val_2 | (val_1 << 8); //swapped
				emerald_main_save->dataUnk1[section4+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section4+0xFF7] = val_2;
			} else {
				//B version
				int section0 = 0, section1 = 0, section2 = 0, section4 = 0;
				//int section0 = 0, section1 = 0, section4 = 0;
				for(int i = 16; i<29; ++i) {
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 0)
						section0 = 0+(0x1000*i)-0x1000;
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 1)
						section1 = 0+(0x1000*i)-0x1000;
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 2)
						section2 = 0+(0x1000*i)-0x1000;
					if(emerald_main_save->dataUnk1[0+(0x1000*i)-12] == 4)
						section4 = 0+(0x1000*i)-0x1000;
				}
				uint32_t gamecode = (emerald_main_save->dataUnk1[section0+0xAC] << 24) | (emerald_main_save->dataUnk1[section0+0xAD] << 16)
					| (emerald_main_save->dataUnk1[section0+0xAE] << 8) | emerald_main_save->dataUnk1[section0+0xAF];
				if(gamecode == 1 || gamecode == 0) {
					printf("Incorrect game code.");
					exitDark(2);
				}
				//if pokedex is not unlocked
				
				//key item events
				if(wantEvents) {
					u8 slotFound = 0;
					bool auroraFound = false;
					bool eonFound = false;
					bool mysticFound = false;
					bool oldSeaMapFound = false;
					u8 Aurora1 = 0x73;
					u8 Aurora2 = 0x01;
					u8 Eon1 = 0x13;
					u8 Eon2 = 0x01;
					u8 Mystic1 = 0x72;
					u8 Mystic2 = 0x01;
					u8 OldSeaMap1 = 0x78;
					u8 OldSeaMap2 = 0x01;
					u8 slot1 = 0;
					u8 slot2 = 0;
					u8 slot3 = 0;
					u8 slot4 = 0;
					
					//check if exists
					for(int k = 0; k<120; k+=4) {
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == Aurora1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == Aurora2)
							auroraFound = true;
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == Eon1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == Eon2)
							eonFound = true;
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == Mystic1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == Mystic2)
							mysticFound = true;
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == OldSeaMap1 && emerald_main_save->dataUnk1[section1+0x5D9+k] == OldSeaMap2)
							oldSeaMapFound = true;
					}
					//Check "shown ticket" event flags, otherwise user can fill key items
					if(emerald_main_save->dataUnk1[section2+0x325] == 0x40) //Eon ticket shown
						eonFound = true; //recycle bool
					if(emerald_main_save->dataUnk1[section2+0x325] == 0x80) //Aurora ticket shown
						auroraFound = true;
					if(emerald_main_save->dataUnk1[section2+0x326] == 0x01) //Old Sea Map shown
						oldSeaMapFound = true;
					if(emerald_main_save->dataUnk1[section2+0x32B] == 0x08) //Mystic ticket shown
						mysticFound = true;
					if(emerald_main_save->dataUnk1[section2+0x325] == 0xC0) { //Eon+Aurora ticket shown
						eonFound = true;
						auroraFound = true;
					}
					
					//find 4 empty slots
					for(int k = 0; k<120; k+=4) {
						if(emerald_main_save->dataUnk1[section1+0x5D8+k] == 0 && emerald_main_save->dataUnk1[section1+0x5D9+k] == 0)
							++slotFound;
						if(slotFound == 1)
							slot1 = k;
						if(slotFound == 2)
							slot2 = k;
						if(slotFound == 3)
							slot3 = k;
						if(slotFound == 4)
							slot4 = k;
						if(slotFound > 3)
							break;
					}
					//u16 key = 0x0D00;
					//u16 quantity = 0x0100;
					//quantity ^= key;
					//grab key = key >> 16;
					//Uhh, security key volatile so have to do this
					u16 key = ((uint16_t)emerald_main_save->dataUnk1[section0+0xAD] << 8) | emerald_main_save->dataUnk1[section0+0xAC];
					u16 quant = 0x0100; //1 ticket little endian
					quant ^= key;
					
					if(slotFound != 0 && !auroraFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot1] = Aurora1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot1] = Aurora2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot1] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot1] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !eonFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot2] = Eon1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot2] = Eon2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot2] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot2] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !mysticFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot3] = Mystic1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot3] = Mystic2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot3] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot3] = quant;
						--slotFound;
					}
					if(slotFound != 0 && !oldSeaMapFound) {
						emerald_main_save->dataUnk1[section1+0x5D8+slot4] = OldSeaMap1;
						emerald_main_save->dataUnk1[section1+0x5D9+slot4] = OldSeaMap2;
						emerald_main_save->dataUnk1[section1+0x5DA+slot4] = quant >> 8;
						emerald_main_save->dataUnk1[section1+0x5DB+slot4] = quant;
						--slotFound;
					}
				}
				
				//unlock national pokedex
				if(wantNat && emerald_main_save->dataUnk1[section0+0x19] == 0) {
					// Field A
					emerald_main_save->dataUnk1[section0+0x19] = 0x01;
					emerald_main_save->dataUnk1[section0+0x1A] = 0xDA;
					// Field B
					emerald_main_save->dataUnk1[section2+0x402] |= 1 << 6;
					// Field C
					emerald_main_save->dataUnk1[section2+0x4A8] = 0x02;
					emerald_main_save->dataUnk1[section2+0x4A9] = 0x03;
				}
				
				//Find free slot in party
			//	bool slot1, slot2, slot3, slot4, slot5, slot6 = false;
				int empty_slot = 0;
				if(emerald_main_save->dataUnk1[section1+0x234] == 6) { //not enough space
					printf("Need at least one spot free in party.");
					//Play SFX
					sleep(2);
					exitDark(1);
				} else {
					int i = 0;
					for(i = 0; i < 6; ++i) { //pointless loop
						if(emerald_main_save->dataUnk1[section1+0x238+8] == 0) {
							empty_slot = 1;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+8] == 0) {
							empty_slot = 2;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+8] == 0) {
							empty_slot = 3;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+8] == 0) {
							empty_slot = 4;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 5;
							break;
						}
						if(emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64+8] == 0) {
							empty_slot = 6;
							break;
						}
					}
				}
				PickJirachi();
				int bito = emerald_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] >> (gen3ID & 7) & 1; //check bit, 384 = JIRACHI
				if(bito == 0) { //Jirachi not seen, assuming that if you have seen it you have it.
					u8 current;
					u8 newValue;
					current = emerald_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section0+0x5C+(gen3ID >> 3)] = (u8)newValue;
					
					current = emerald_main_save->dataUnk1[section1+0x0988+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section1+0x0988+(gen3ID >> 3)] = (u8)newValue;
					
					current = emerald_main_save->dataUnk1[section4+0x0CA4+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section4+0x0CA4+(gen3ID >> 3)] = (u8)newValue;
					
					current = emerald_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] & ~(1 << (gen3ID & 7));
					newValue = current | (1 << (gen3ID & 7));
					emerald_main_save->dataUnk1[section0+0x28+(gen3ID >> 3)] = (u8)newValue;
				}
				switch (empty_slot) {
					case 1: memcpy(&emerald_main_save->dataUnk1[section1+0x238], curr, 0x64);
					 break;
					case 2: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64], curr, 0x64);
					 break;
					case 3: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64], curr, 0x64);
					 break;
					case 4: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 5: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
					case 6: memcpy(&emerald_main_save->dataUnk1[section1+0x238+0x64+0x64+0x64+0x64+0x64], curr, 0x64);
					 break;
				}
				++emerald_main_save->dataUnk1[section1+0x234];
				
				//calculate checksums
				int chk = Chksum(&emerald_main_save->dataUnk1[section0], 0xF2C);
				u8 val_1 = chk&0x000000FF;
				u8 val_2 = (chk&0x0000FF00)>>8;
			//	uint16_t sum = val_2 | (val_1 << 8); //swapped
				emerald_main_save->dataUnk1[section0+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section0+0xFF7] = val_2;
				
				chk = Chksum(&emerald_main_save->dataUnk1[section1], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
			//	sum = val_2 | (val_1 << 8); //swapped
				emerald_main_save->dataUnk1[section1+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section1+0xFF7] = val_2;
				
				chk = Chksum(&emerald_main_save->dataUnk1[section2], 0xF80);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
				emerald_main_save->dataUnk1[section2+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section2+0xFF7] = val_2;
				
				chk = Chksum(&emerald_main_save->dataUnk1[section4], 0xF08);
				val_1 = chk&0x000000FF;
				val_2 = (chk&0x0000FF00)>>8;
			//	sum = val_2 | (val_1 << 8); //swapped
				emerald_main_save->dataUnk1[section4+0xFF6] = val_1;
				emerald_main_save->dataUnk1[section4+0xFF7] = val_2;
			}
			
			fwrite(emerald_main_save, 1, sizeof(EMData), fp);
			fclose(fp);
		//END
	}
}
#endif

#define ALIGN32(x) (((x) + 31) & ~31)

#if 0
void byte_swap(u32* buffer, unsigned int length)
{
	int i;
	//return;
	//if (rom_byte_swap == BYTE_SWAP_HALF) { // aka little endian (40123780) vs (80371240)
	for (i = 0; i < (length & ~3); i += 4)
		*(u32*)(buffer + i) = __lwbrx(buffer, i);
}
#endif

//#if WANT_FE
//#include "FE8_SpecialContent_bin.h"
//#include "FE7_BonusContent_bin.h"
//#endif
//u32 swapper
//#include "test_bin.h"
//u8* FE_dat;

//FE8 special data
void handleFE8Data(void)
{
//	char full_path[256];
//	int data_fd;
//	u8 *lowmem = (u8*)0x80000000;
//	struct stat st;
	FILE *fe8_file;

//	sprintf(full_path, "disk:/%s", "Fire Emblem - The Sacred Stones.sav");
	
	//FE_dat = memalign(32, ALIGN32(sizeof(u32)));
	//FE_dat = malloc(0x144);
	//FE_dat2 = malloc(0x140);
	//FE_dat = memalign(32, 0x144);
	int FE_type = 0;
	char FE_title[4];
	
//	iso_file = fopen("disk:/FE8.bin", "rb");
//	fread(FE_dat, 1, 0x144, iso_file);
//	fclose(iso_file);

	fe8_file = fopen(fullpath_fe8, "r+");
	if(fe8_file == NULL) {
	//	printf("FAILED TO OPEN.\n");
		//need to add, like an exit or something
	//	exit(1);
		goto finish;
	}
	fseek(fe8_file, 4, SEEK_SET);
	fread(FE_title, 1, 4, fe8_file);
	if(FE_title[0] == 0x46 && FE_title[1] == 0x45 && FE_title[2] == 0x39) {
		FE_type = 8; //9, but we all know it's 8
		printf("Fire Emblem: The Sacred Stones\n\n");
	} else if(FE_title[0] == 0x46 && FE_title[1] == 0x45 && FE_title[2] == 0x37) {
		FE_type = 7;
		printf("Fire Emblem: The Blazing Buredo\n");
		fclose(fe8_file);
		printf("Data is wrong. Exiting...\n\n");
		exit(1);
	}
	
	//printf("This is FE%d.\n\n", FE_type);
	//printf("FE Type2: 0x%X%X%X\n", FE_title[0], FE_title[1], FE_title[2]);
	if(FE_type == 8) {
		fseek(fe8_file, 0x725C, SEEK_SET);
		fwrite(FE8_SpecialContent_bin, 1, 0x144, fe8_file);
		//fwrite(FE_dat, 1, 0x144, fe8_file);
	} else if(FE_type == 7) {
		fseek(fe8_file, 0x7134, SEEK_SET);
		fwrite(FE7_BonusContent_bin, 1, 0x284, fe8_file);
	} else {
		exit(1);
	}

//	printf("At addr 0x725C: %d\n\n", FE_dat[0]);
	
	fclose(fe8_file);
	printf("Princess L'Arachel:\n");
	printf("Could it be...?\n");
	printf("The hidden treasures of Rausten!\n\n\n");
	
	//printf("Press A to continue.\n");
	
	while (1)
	{
		u32 pressed;
		get_input(NULL, &pressed);
		VIDEO_WaitVSync();

		if (pressed & INPUT_BUTTON_OK)
			break;

		if (pressed & INPUT_BUTTON_CANCEL)
		{
			printf("Exiting...\n");
			exit(1);
		}
		if (pressed & INPUT_BUTTON_START)
		{
			printf("Exiting...\n");
			exit(1);
		}
	}

finish:
	return;
}

void handleFE7Data(void)
{
//	struct stat st;
	FILE *fe7_file;
	
	//FE_dat = memalign(32, 0x284);
	int FE_type = 0;
	char FE_title2[4];
	
	fe7_file = fopen(fullpath_fe7, "r+");
	if(fe7_file == NULL) {
		//printf("FAILED TO OPEN.\n");
		//printf("Exiting...\n");
		//exit(1);
		goto finish;
	}
	fseek(fe7_file, 4, SEEK_SET);
	fread(FE_title2, 1, 4, fe7_file);
	if(FE_title2[0] == 0x46 && FE_title2[1] == 0x45 && FE_title2[2] == 0x39) {
		FE_type = 8; //9, but we all know it's 8
		printf("Fire Emblem: The Sacred Stones...?\n\n");
		fclose(fe7_file);
		printf("Data is wrong. Exiting...\n\n");
		exit(1);
	} else if(FE_title2[0] == 0x46 && FE_title2[1] == 0x45 && FE_title2[2] == 0x37) {
		FE_type = 7;
		printf("Fire Emblem: The Blazing Blade\n\n");
	}
	
	//printf("This is FE%d.\n\n", FE_type);
	if(FE_type == 8) {
		fseek(fe7_file, 0x725C, SEEK_SET);
		fwrite(FE8_SpecialContent_bin, 1, 0x144, fe7_file);
	} else if(FE_type == 7) {
		fseek(fe7_file, 0x7134, SEEK_SET);
		fwrite(FE7_BonusContent_bin, 1, 0x284, fe7_file);
	} else {
		exit(1);
	}
	
	fclose(fe7_file);
	printf("St. Elimine:\n");
	printf("I bestow upon you these, my sacred gifts.\n");
	printf("May they bring you joy and comfort.\n\n\n");
	
	while (1)
	{
		u32 pressed;
		get_input(NULL, &pressed);
		VIDEO_WaitVSync();

		if (pressed & INPUT_BUTTON_OK)
			break;

		if (pressed & INPUT_BUTTON_CANCEL)
		{
			printf("Exiting...\n");
			exit(1);
		}
		if (pressed & INPUT_BUTTON_START)
		{
			printf("Exiting...\n");
			exit(1);
		}
	}

finish:
	return;
}
#if 0
static u32 test_bin[126596] = {
    0xDE, 0xAD, 0xBE, 0xFF, 0x00, 0x00, 0xC8, 0x42, 0x00, 0x00, 0xC8, 0x42, 0x00, 0x01, 0x1D, 0x64,
#endif

//Yu-Gi-Oh! Duel Transer - deck format swapper
#if 0
void byte_swap(u16* buffer, unsigned int length)
{
	int i;
	for (i = 0; i < (length & ~3); i += 2)
		*(u16*)(buffer + i) = __lhbrx(buffer, i);
}
#endif

//u32
/*
void byte_swap(const u8* buffer, unsigned length)
{
	int i;
	for (i = 0; i < (length); i += 4)
		*(u32*)(buffer + i) = __lwbrx(buffer, i);
}*/

void EasterEgg_Day(void)
{
	time_t rtm;
	struct tm *tinfo;
	time(&rtm);
	tinfo = localtime(&rtm);
	
	if(verbose)
		printf("Current Time/Date: %s\n", asctime(tinfo));
	
	if(tinfo->tm_mon == (12-1) && tinfo->tm_mday == 25) {
		printf("It's Christmas, woo! ");
		printf("Everything costs 25 rings!\n");
		xmas = true;
	} else if(tinfo->tm_mon == (10-1) && tinfo->tm_mday == 31) {
		printf("It's Halloween, boo! ");
		printf("Hm? It seems there's something hidden...\n");
		//Basically I add another chao to Rare Chao mode, a Dark Chaos Chao
		//unsure if I should bother. Otherwise a Ghost-like Chao.
		halloween = true;
	} else if(tinfo->tm_mon == (12-1) && tinfo->tm_mday == 31) {
		printf("It's New Year's Eve! ");
		printf("The Silver Egg is free!\n");
		newyear = true;
	} else if(tinfo->tm_mon == (2-1) && tinfo->tm_mday == 14) {
		printf("Valentine's Day, chocolate time! ");
		printf("Heart Fruit is free!\n");
		valentine = true;
	} else if(tinfo->tm_mon == (4-1) && tinfo->tm_mday == 1) {
		printf("It's April Fools! ");
		printf("Everything is free!\n");
		fools = true;
	} else if(tinfo->tm_mon == (6-1) && tinfo->tm_mday == 23) {
		printf("It's Sonic's birthday! ");
		printf("Anything blue is yours!\n");
		sonicBlue = true;
	} else if(tinfo->tm_mon == (12-1) && tinfo->tm_mday == 23) {
		printf("The roar of Perfect Chaos! Sonic Adventure!\n");
	} else if(tinfo->tm_mon == (12-1) && tinfo->tm_mday == 30) {
		printf("\"I'll take it from here!\" Sonic Heroes! ");
		printf("Sapphire/Topaz/Ruby eggs are free!\n");
		metalHeroes = true;
	} else if(tinfo->tm_mon == (9-1) && tinfo->tm_mday == 16) {
		printf("The Emerald Egg is free!\n");
		emeraldDay = true;
	} else if(tinfo->tm_mon == (11-1) && tinfo->tm_mday == 21) {
		printf("The Sapphire/Ruby Egg is free!\n");
		sapphRubyDay = true;
	} else if(tinfo->tm_mon == (12-1) && tinfo->tm_mday == 20) {
		printf("Sonic Advance! ");
		printf("The Shiny Normal Egg is free!\n");
		shinyNormDay = true;
	} else if(tinfo->tm_mon == (11-1) && tinfo->tm_mday == 15) {
		printf("Sonic Rush! ");
		printf("The Shiny Purple Egg is free!\n");
		shinyPurpleDay = true;
	} else if(tinfo->tm_mon == (11-1) && tinfo->tm_mday == 14) {
		printf("\"Elise!\" Sonic The Hedgehog! ");
		printf("The Shiny Blue Egg is free!\n");
		shinyBlueEgg = true;
	} else if(tinfo->tm_mon == (11-1) && tinfo->tm_mday == 18) {
		printf("\"Want some chocolate?\" Sonic Unleashed! ");
		printf("The Blue Egg is free!\n");
		blueEggDay = true;
	} else if(tinfo->tm_mon == (2-1) && tinfo->tm_mday == 21) {
		printf("\"Sonic is building momentum!\" Sonic Riders! ");
		printf("The Shiny Grey Egg is free!\n");
		shinyGreyDay = true;
	} else if(tinfo->tm_mon == (1-1) && tinfo->tm_mday == 8) {
		printf("\"Even without wings...\" Sonic Riders Zero Gravity! ");
		printf("The Shiny Grey Egg is free!\n");
		shinyGreyDay = true;
	} else if(tinfo->tm_mon == (2-1) && tinfo->tm_mday == 20) {
		printf("\"We just have to face it.\" Sonic & the Secret Rings!\n");
		printf("The Shiny Yellow Egg is free!\n");
		shinyYellowDay = true;
	} else if(tinfo->tm_mon == (3-1) && tinfo->tm_mday == 3) {
		printf("\"My stories only end when I stop running!\" Sonic & the Black Knight!\n");
		printf("The Shiny Black Egg is free!\n");
		shinyBlackDay = true;
	} else if(tinfo->tm_mon == (11-1) && tinfo->tm_mday == 2) {
		printf("Atop the space colony Ark is... Shadow The Hedgehog! ");
		printf("The Black Egg is free!\n");
		blackEggDay = true;
	} else if(tinfo->tm_mon == (6-1) && tinfo->tm_mday == 7) {
		printf("No more Tiny Chao Garden... Sonic Advance 3! ");
		printf("You get nothing.\n");
	} else if(tinfo->tm_mon == (1-1) && tinfo->tm_mday == 10) {
		printf("\"Iku zo, Gash!\" Konjiki no Gash!! ");
		printf("The Gold Egg is free!\n");
		//This will trick Omochao into thinking you've played ZB games
		//ZatchMB = true; ZatchMF = true;
		goldEggDay = true;
	} else if(tinfo->tm_mon == (3-1) && tinfo->tm_mday == 5) {
		printf("\"BAO ZAKERUGA!\" It's... Zatch Bell! ");
		printf("The Gold Egg is free!\n");
		//ZatchMB = true; ZatchMF = true;
		goldEggDay = true;
	} else if(tinfo->tm_mon == (3-1) && tinfo->tm_mday == 9) {
		printf("\"Yo soy criollo sensible de pluma poco conspicua,\n");
		printf("no gasto rimas melifluas en cantarle a la impotencia\n");
		printf("y mi decima sentencia: lo imposible... no es boricua!\"\n\n");
		printf("\"I shall not submit! I shall conquer! I shall rise!\"\n");
		printf("\"Until the very planet is in the loving grip of the Pax Bisonica.\"\n\n");
		goldEggDay = true; blackEggDay = true; shinyBlackDay = true; shinyYellowDay = true;
	}
}

u8* mainMusic = NULL;
char demoSHC[] = "- Chao Transfer Tool - by Diego A. 10/25/21, demo build for SHC2021\n\n";

int main(int argc, char **argv)
{
	GXRModeObj *rmode; //, *vidmode;

//	bool isSD = false;
//  Enable for USB gecko logging
//	CON_EnableGecko(1, 1);

	VIDEO_Init();

	rmode = VIDEO_GetPreferredMode(NULL);

	//rmode = &TVNtsc480Int;
	//rmode = &TVNtsc240Ds;
	
	initialize(rmode);

	// Set console size to avoid overscan issues
	int x = 4, y = 0, w, h;
	w = rmode->fbWidth;
	h = rmode->xfbHeight;
	CON_InitEx(rmode, x, y, w, h);

//	static vu16* const _vigReg = (vu16*)0xCC002030;
	//if(*_vigReg == 0x120E || *_vigReg == 0x1107) {
/*	if(*_vigReg == 0x1107) {
		*_vigReg = 0x1001; //0x1001(30fps), 0x120E(480p60fps) 0x1107(480i60fps)
	//	halve_fps = false;
	}*/

	printf("\n\n\n"); // move past screen border

	srand( time(NULL) );

#ifdef HW_RVL
	//Allow NAND savefile editing
	//IOS_ReloadIOS(249);
	
	//Enable NAND access, for modifying savefiles
//	IOSPATCH_Apply();
#endif

	input_startup();
	
#ifdef HW_DOL
//#define PSOSDLOADID 0x7c6000a6
	//int *psoid = (int *) 0x80001800; // from gcmm
	//void (*PSOReload) () = (void (*)()) 0x80001800;
#endif

#if WANT_CHAO
#if WANT_DEMO
	int z = 0, cntSHC = 0;
	for(z = 0; z < 72; ++z)
		cntSHC += demoSHC[z];
	cntSHC += 120;
	if(cntSHC != 0x150C)
		exit(0);
//	printf("How much size: 0x%X,,\n", cntSHC);
	printf("%s", demoSHC);
#else
	printf("- GBA Bonus Pack -   by %s  2/2/2025\n\n", name);


#endif
#endif
#if WANT_FE
	printf("- Fire Emblem Data Transfer Tool -\n\n");
#endif


	// make sure AHBPROT is disabled
/*	if ((read32(0xCD800038) | read32(0xCD80003C))==0)
	{
		printf("This app requires AHBPROT access!\n");
		exit(0);
		//goto wait_for_exit;
	}*/

	u16 count1 = 0, count2 = 0;
	int k = 0, j = 0;
	for (k=0; k < 8; k++)
		count2 += name[k];
	for (j=2; j < 6; j++)
		count1 += quote[j];
	//printf("Checksum: 0x%X\n\n", count1 + count2);
	programChk = count1 + count2;
	if(programChk != 0x397)
		exit(0);

#ifdef WANT_SFX
	Sound_Init();
	Sound_Play_BGM_Main(so_dtt_ogg, so_dtt_ogg_size);
#endif

	bool skipNet = false;
#ifdef HW_RVL
	printf("Press Y/1 to use SD card, X/2 to use USB drive, or B/HOME to exit. ");
#else
	printf("Press Y to use SD card, X to use SD2SP2, or B to exit. ");
	
	//printf("\nMEM1: %8d / %8d\n", SYSMEM1_SIZE - SYS_GetArena1Size(), SYSMEM1_SIZE);
#endif

	while (1)
	{
		u32 pressed;
		get_input(NULL, &pressed);
		VIDEO_WaitVSync();
		
		if (!skipNet && pressed & INPUT_BUTTON_OK)
		{
			skipNet = true;
#ifdef HW_DOL
			randMusic = true;
#endif
			//printf("Skipped!");
		} else if (skipNet && pressed & INPUT_BUTTON_OK)
		{
			skipNet = false;
			//printf("ON! ");
		}
		
		if (pressed & INPUT_BUTTON_2)
		{
#ifdef HW_RVL
			if (fatMountSimple("disk", &__io_usbstorage)==true) {
				break;
			}
			printf("\nFailed to mount USB drive.\n");
#else
			if (fatMountSimple("disk", &__io_gcsd2)==true)
				break;
			
			printf("\nFailed to mount SD Card from SP2.\n");
#endif
			goto finish;
		}

		if (pressed & INPUT_BUTTON_1)
		{
#ifdef HW_RVL
			if (fatMountSimple("disk", &__io_wiisd)==true) {
				//isSD = true;
				break;
			}
#else
			// GCLoader
			if (fatMountSimple("disk", &__io_gcode)==true) {
				//printf("\nMounted SD Card.\n");
				//sleep(4);
				break;
			}
#endif
			printf("\nFailed to mount SD card.\n");
			goto finish;
		}

		if (pressed & INPUT_BUTTON_CANCEL || pressed & INPUT_BUTTON_START)
		{
			printf("\nExiting...\n");
			sleep(2);
			goto finish;
		}
	}
	//Remove device select text
	printf("\r\x1b[K");

#ifdef HW_DOL
	sprintf(sadx_iso, "disk:/games/Sonic Adventure DX.iso");
	sprintf(tcg_path, "disk:/apps/Chao Transfer Tool/ChaoGC.gba");
#endif

#if 0
		int i;
		for(i=1; i<argc; ++i) {
			handleConfigPair(argv[i]);
		}
//	}
#endif

#ifdef WANT_NET
	//init the network
	if(skipNet) {
		networkEnabled = false;
		//printf("Network skipped.\n");
	}
	if(networkEnabled) {
		s32 ret;
		char localip[16] = {0};
		char gateway[16] = {0};
		char netmask[16] = {0};
		printf("Initializing network... ");

		// Configure the network interface
		//lost my devkit versions...
		//ret = if_config ( localip, netmask, gateway, TRUE, 20);
		ret = if_config ( localip, netmask, gateway, TRUE);
		if (ret>=0) {
			//printf ("IP: %s, GW: %s, MASK %s\n", localip, gateway, netmask);
			printf("\r\x1b[K"); //remove init text
			if(showIP)
				printf ("IP: %s\n", localip);

			char *dot = strrchr(localip,'.');
			strcpy(WiiOct, &dot[1]);
			strcpy(WiiIP, localip);

			LWP_CreateThread(	&httd_handle,	/* thread handle */ 
								httpd,			/* code */ 
								localip,		/* arg pointer for thread */
								NULL,			/* stack base */ 
								16*1024,		/* stack size */
								60				/* thread priority */ );
			//networkEnabled = true;
		} else {
			//printf ("Network configuration failed!\n");
			printf ("\n");
			networkEnabled = false;
		}
	}
#endif

//one-line colors working, colors are too limited to be useful though
//printf("\x1b[33;0mColor TEXTO MY FRIEND!\n\x1b[39;0m");

#if WANT_FE
	printf("Would you like to begin the transfer?\n");
#endif
	printf("Press A to continue.\n\n\n");
	
	while (1)
	{
		u32 pressed;
		get_input(NULL, &pressed);
		VIDEO_WaitVSync();

		if (pressed & INPUT_BUTTON_OK)
			break;

		if (pressed & INPUT_BUTTON_CANCEL)
		{
			printf("Exiting...\n");
			goto finish;
		}
		if (pressed & INPUT_BUTTON_START)
		{
			printf("Exiting...\n");
			goto finish;
		}
	}

#if WANT_CHAO
	// Handle all savefile op
	handleAGBData();
	
	//OMOCHAO_SAYS();
#endif

	// Check free MEM1
	//printf("MEM1: %8d / %8d\n", SYSMEM1_SIZE - SYS_GetArena1Size(), SYSMEM1_SIZE);

	//for byteswap 32bit
	//byte_swap(test_bin, 0x0651D4);
	printf("\nSUCCESS!\n\n");

	// unmount disk to be sure any changes are flushed
	fatUnmount("disk");

//wait_for_exit:
#ifdef HW_RVL
	printf("Press START or HOME to exit.\n");
#else
	printf("Press START to exit.\n");
#endif
	while(1) {
		u32 pressed;
		get_input(NULL, &pressed);
		if (pressed & INPUT_BUTTON_START) {
			//Sound_Stop();
			break;
		}
		VIDEO_WaitVSync();
	}

finish:
	VIDEO_SetBlack(true);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	return 0;
}
