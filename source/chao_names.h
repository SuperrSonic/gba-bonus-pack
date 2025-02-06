/* Copyright (c) 2022-2025 Diego A.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// Return chao name
u8 GetName (u8 character)
{
	//return character;

	switch (character) {
		case 0x00: return 0;
			break;
		case 0x01: return 0x21; //!
			break;
		case 0x02: return 0x5C; // "\""
			break;
		case 0x03: return 0x23; //#
			break;
		case 0x04: return 0x24; //$
			break;
		case 0x05: return 0x25; //%
			break;
		case 0x06: return 0x26; //&
			break;
		case 0x07: return 0x27; // "\\" Uhhh, what is this? I'll use 0x27 for now
			break;
		case 0x08: return 0x28; //(
			break;
		case 0x09: return 0x29; //)
			break;
		case 0x0A: return 0x2A; //*
			break;
		case 0x0B: return 0x2B; //+
			break;
		case 0x0C: return 0x2C; //,
			break;
		case 0x0D: return 0x2D; //-
			break;
		case 0x0E: return 0x2E; //.
			break;
		case 0x0F: return 0x2F; ///
			break;
		case 0x10: return 0x30; //0
			break;
		case 0x11: return 0x31; //1
			break;
		case 0x12: return 0x32; //2
			break;
		case 0x13: return 0x33; //3
			break;
		case 0x14: return 0x34; //4
			break;
		case 0x15: return 0x35; //5
			break;
		case 0x16: return 0x36; //6
			break;
		case 0x17: return 0x37; //7
			break;
		case 0x18: return 0x38; //8
			break;
		case 0x19: return 0x39; //9
			break;
		case 0x1A: return 0x3A; //:
			break;
		case 0x1B: return 0x3B; //;
			break;
		case 0x1C: return 0x3C; //<
			break;
		case 0x1D: return 0x3D; //=
			break;
		case 0x1E: return 0x3E; //>
			break;
		case 0x1F: return 0x3F; //?
			break;
		case 0x20: return 0x40; //@
			break;
		case 0x21: return 0x41; //A
			break;
		case 0x22: return 0x42; //B
			break;
		case 0x23: return 0x43; //C
			break;
		case 0x24: return 0x44; //D
			break;
		case 0x25: return 0x45; //E
			break;
		case 0x26: return 0x46; //F
			break;
		case 0x27: return 0x47; //G
			break;
		case 0x28: return 0x48; //H
			break;
		case 0x29: return 0x49; //I
			break;
		case 0x2A: return 0x4A; //J
			break;
		case 0x2B: return 0x4B; //K
			break;
		case 0x2C: return 0x4C; //L
			break;
		case 0x2D: return 0x4D; //M
			break;
		case 0x2E: return 0x4E; //N
			break;
		case 0x2F: return 0x4F; //O
			break;
		case 0x30: return 0x50; //P
			break;
		case 0x31: return 0x51; //Q
			break;
		case 0x32: return 0x52; //R
			break;
		case 0x33: return 0x53; //S
			break;
		case 0x34: return 0x54; //T
			break;
		case 0x35: return 0x55; //U
			break;
		case 0x36: return 0x56; //V
			break;
		case 0x37: return 0x57; //W
			break;
		case 0x38: return 0x58; //X
			break;
		case 0x39: return 0x59; //Y
			break;
		case 0x3A: return 0x5A; //Z
			break;
		case 0x3B: return 0x5B; //[
			break;
		case 0x3C: return 0x9D; //¥
			break;
		case 0x3D: return 0x5D; //]
			break;
		case 0x3E: return 0x5E; //^
			break;
		case 0x3F: return 0x5F; //_
			break;
		case 0x40: return 0x91; //‘
			break;
		case 0x41: return 0x61; //a
			break;
		case 0x42: return 0x62; //b
			break;
		case 0x43: return 0x63; //c
			break;
		case 0x44: return 0x64; //d
			break;
		case 0x45: return 0x65; //e
			break;
		case 0x46: return 0x66; //f
			break;
		case 0x47: return 0x67; //g
			break;
		case 0x48: return 0x68; //h
			break;
		case 0x49: return 0x69; //i
			break;
		case 0x4A: return 0x6A; //j
			break;
		case 0x4B: return 0x6B; //k
			break;
		case 0x4C: return 0x6C; //l
			break;
		case 0x4D: return 0x6D; //m
			break;
		case 0x4E: return 0x6E; //n
			break;
		case 0x4F: return 0x6F; //o
			break;
		case 0x50: return 0x70; //p
			break;
		case 0x51: return 0x71; //q
			break;
		case 0x52: return 0x72; //r
			break;
		case 0x53: return 0x73; //s
			break;
		case 0x54: return 0x74; //t
			break;
		case 0x55: return 0x75; //u
			break;
		case 0x56: return 0x76; //v
			break;
		case 0x57: return 0x77; //w
			break;
		case 0x58: return 0x78; //x
			break;
		case 0x59: return 0x79; //y
			break;
		case 0x5A: return 0x7A; //z
			break;
		case 0x5B: return 0x7B; //{
			break;
		case 0x5C: return 0x7C; //|
			break;
		case 0x5D: return 0x7D; //}
			break;
		case 0x5E: return 0x7E; //~
			break;
		case 0x5F: return 0x20; //" " 'space'
			break;
		case 0x60: return 0xC0; //À
			break;
		case 0x61: return 0xC1; //Á
			break;
		case 0x62: return 0xC2; //Â
			break;
		case 0x63: return 0xC3; //Ã
			break;
		case 0x64: return 0x8E; //Ä console
			break;
		case 0x65: return 0x8F; //Å console
			break;
		case 0x66: return 0x92; //Æ console
			break;
		case 0x67: return 0x80; //Ç console
			break;
		case 0x68: return 0xC8; //È
			break;
		case 0x69: return 0x90; //É console
			break;
		case 0x6A: return 0xCA; //Ê
			break;
		case 0x6B: return 0xCB; //Ë
			break;
		case 0x6C: return 0xCC; //Ì
			break;
		case 0x6D: return 0xCD; //Í
			break;
		case 0x6E: return 0xCE; //Î
			break;
		case 0x6F: return 0xCF; //Ï
			break;
		case 0x70: return 0xD0; //Ð
			break;
		case 0x71: return 0xA5; //Ñ console, note: 0xA6 0xA7 are tiny a and o underlined
			break;
		case 0x72: return 0xD2; //Ò
			break;
		case 0x73: return 0xD3; //Ó
			break;
		case 0x74: return 0xD4; //Ô
			break;
		case 0x75: return 0xD5; //Õ
			break;
		case 0x76: return 0x99; //Ö console
			break;
		case 0x77: return 0xA8; //¿ console
			break;
		case 0x78: return 0xD8; //Ø
			break;
		case 0x79: return 0xD9; //Ù
			break;
		case 0x7A: return 0xDA; //Ú
			break;
		case 0x7B: return 0xDB; //Û
			break;
		case 0x7C: return 0x9A; //Ü console, ¢ 0xA2,0x9B cent sign and £ 0xA3 0x9C pound sign isn't used
			break;
		case 0x7D: return 0xDD; //Ý
			break;
		case 0x7E: return 0xDE; //Þ
			break;
		case 0x7F: return 0xDF; //ß
			break;
		case 0x80: return 0x85; //à console
			break;
		case 0x81: return 0xA0; //á console
			break;
		case 0x82: return 0x83; //â console
			break;
		case 0x83: return 0xE3; //ã
			break;
		case 0x84: return 0x84; //ä console
			break;
		case 0x85: return 0x86; //å console
			break;
		case 0x86: return 0x91; //æ console
			break;
		case 0x87: return 0x87; //ç console
			break;
		case 0x88: return 0x8A; //è console
			break;
		case 0x89: return 0x82; //é console
			break;
		case 0x8A: return 0x88; //ê console
			break;
		case 0x8B: return 0x89; //ë console
			break;
		case 0x8C: return 0x8D; //ì console
			break;
		case 0x8D: return 0xA1; //í console
			break;
		case 0x8E: return 0x8C; //î console
			break;
		case 0x8F: return 0x8B; //ï console
			break;
		case 0x90: return 0xF0; //ð
			break;
		case 0x91: return 0xA4; //ñ console
			break;
		case 0x92: return 0x95; //ò console
			break;
		case 0x93: return 0xA2; //ó console
			break;
		case 0x94: return 0x93; //ô console
			break;
		case 0x95: return 0xF5; //õ
			break;
		case 0x96: return 0x94; //ö console
			break;
		case 0x97: return 0xAD; //¡ console  NOTE: 0xAE is << 0xAF is >>
			break;
		case 0x98: return 0xF8; //ø
			break;
		case 0x99: return 0x97; //ù console
			break;
		case 0x9A: return 0xA3; //ú console
			break;
		case 0x9B: return 0x96; //û console
			break;
		case 0x9C: return 0x81; //ü console
			break;
		case 0x9D: return 0xFD; //ý
			break;
		case 0x9E: return 0xFE; //þ
			break;
		case 0x9F: return 0x98; //ÿ console
			break;
		
	/*	case 0xA0: return 0x4A; //J JPN characters start here
			break;
		case 0xF6: return 0x4A; //J end of JPN characters
			break; */

		case 0xF6: return 0x07; //Unicode center dot
			break;
		case 0xF7: return 0x0F; //Star
			break;
		case 0xF8: return 0x9F; //female sign not available, using fancy F sign, perhaps the heart 0x03
			break;
		case 0xF9: return 0x0B; //male sign, 0x9E is Pt sign
			break;
		case 0xFA: return 0x0E; //wrong musical dingbat
			break;
		case 0xFB: return 0x85; //…
			break;
		case 0xFC: return 0xA9; //JPN bracket top-left
			break;
		case 0xFD: return 0xAA; //JPN bracket bottom-right  NOTE: 0xAB = 1/2 0xAC = 1/4
			break;
		case 0xFE: return 0x4A; //JPN character
			break;
		case 0xFF: return 0x20; //alt space, should never hit
			break;
	}
	//JPN characters
	if(character >= 0xA0 && character <= 0xF6)
		return 0x4A;
	return character; // if it fails to find char just use original data
}

//try to fix the invalid characters
u8 SafeName (u8 character)
{
	switch (character) {
	//	case 0x00: return 0x20; //maybe a good idea to keep all file names the same length
		//	break;
		case 0x5C: return 0x5F; // "\"" INVALID
			break;
		case 0x27: return 0x5F; // "\\" INVALID
			break;
		case 0x2A: return 0x5F; //* INVALID
			break;
		case 0x2F: return 0x5F; /// INVALID
			break;
		case 0x3A: return 0x5F; //: INVALID
			break;
		case 0x3C: return 0x5F; //< INVALID
			break;
		case 0x3E: return 0x5F; //> INVALID
			break;
		case 0x3F: return 0x5F; //? INVALID
			break;
		case 0x9D: return 0xA5; //¥ Yen console FIXED
			break;
		case 0x7C: return 0x6C; //| INVALID
			break;
		case 0x8E: return 0xC4; //Ä console FIXED
			break;
		case 0x8F: return 0xC5; //Å console FIXED
			break;
		case 0x92: return 0xC6; //Æ console FIXED
			break;
		case 0x80: return 0xC7; //Ç console FIXED
			break;
		case 0x90: return 0xC9; //É console FIXED
			break;
		case 0xA5: return 0xD1; //Ñ console FIXED
			break;
		case 0x99: return 0xD6; //Ö console FIXED
			break;
		case 0xA8: return 0xBF; //¿ console FIXED
			break;
		case 0x9A: return 0xDC; //Ü console FIXED
			break;
		case 0x85: return 0xE0; //à console FIXED
			break;
		case 0xA0: return 0xE1; //á console FIXED
			break;
		case 0x83: return 0xE2; //â console FIXED
			break;
		case 0x84: return 0xE4; //ä console FIXED
			break;
		case 0x86: return 0xE5; //å console FIXED
			break;
		case 0x91: return 0xE6; //æ console FIXED
			break;
		case 0x87: return 0xE7; //ç console FIXED
			break;
		case 0x8A: return 0xE8; //è console FIXED
			break;
		case 0x82: return 0xE9; //é console FIXED
			break;
		case 0x88: return 0xEA; //ê console FIXED
			break;
		case 0x89: return 0xEB; //ë console FIXED
			break;
		case 0x8D: return 0xEC; //ì console FIXED
			break;
		case 0xA1: return 0xED; //í console FIXED
			break;
		case 0x8C: return 0xEE; //î console FIXED
			break;
		case 0x8B: return 0xEF; //ï console FIXED
			break;
		case 0xA4: return 0xF1; //ñ console FIXED
			break;
		case 0x95: return 0xF2; //ò console FIXED
			break;
		case 0xA2: return 0xF3; //ó console FIXED
			break;
		case 0x93: return 0xF4; //ô console FIXED
			break;
		case 0x94: return 0xF6; //ö console FIXED
			break;
		case 0xAD: return 0xA1; //¡ console FIXED
			break;
		case 0x97: return 0xF9; //ù console FIXED
			break;
		case 0xA3: return 0xFA; //ú console FIXED
			break;
		case 0x96: return 0xFB; //û console FIXED
			break;
		case 0x81: return 0xFC; //ü console FIXED
			break;
		case 0x98: return 0xFF; //ÿ console FIXED
			break;
		case 0x07: return 0x2E; //Unicode center dot FIXED
			break;
		case 0x0F: return 0x5F; //Star FIXED
			break;
		case 0x9F: return 0x46; //female sign FIXED
			break;
		case 0x0B: return 0x4D; //male sign, FIXED
			break;
		case 0x0E: return 0x5F; //wrong musical dingbat FIXED
			break;
		case 0xA9: return 0x5B; //JPN bracket top-left FIXED
			break;
		case 0xAA: return 0x5D; //JPN bracket bottom-right FIXED
			break;
	}
	
	return character;
}
