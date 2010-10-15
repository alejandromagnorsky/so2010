#include "../include/keyboard.h"

struct KBNamespace Keyboard = {
	updateLeds
};


unsigned char ordinary_map[] = {
		//	0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15
		NDEF, 		ESC, 	'1', 	'2', 	'3', 	'4', 	'5', 	'6', 	'7', 	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
		//	16		17		18		19		20		21		22		23		24		25		26		27		28		29		30		31
			'q', 	'w', 	'e', 	'r', 	't', 	'y', 	'u', 	'i', 	'o', 	'p', 	'[',	']',	'\n',	L_CTRL,	'a',	's',
		//	32		33		34		35		36		37		38		39		40		41		42		43		44		45		46		47
			'd', 	'f', 	'g', 	'h', 	'j', 	'k', 	'l', 	164, 	'\'',	NDEF, 	L_SHIFT,'\\',	'z',	'x',	'c',	'v',
		//	48		49		50		51		52		53		54		55		56		57		58		59		60		61		62		63
			'b', 	'n', 	'm', 	',', 	'.', 	'/', 	R_SHIFT,'*', 	L_ALT, 	' ', 	CAPS, 	F1, 	F2,		F3,		F4,		F5,
		//	64		65		66		67		68		69		70		71		72		73		74		75		76		77		78		79
			F6, 	F7, 	F8, 	F9, 	F10, 	NUM, 	SCROLL, N7, 	N8, 	N9, 	'-', 	N4, 	N5, 	N6,		'+',	N1,
		//	80		81		82		83		84		85		86		87		88		89		90		91		92		93		94		95
			N2, 	N3, 	N0, 	NDOT, 	NDEF, 	NDEF, 	'<', 	F11, 	F12, 	NDEF, 	NDEF, 	NDEF, 	NDEF,	NDEF, 	NDEF,	NDEF,
		//	96		97		98		99		100		101		102		103		104		105		106		107		108		109		110		111
			NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF,	NDEF, 	NDEF, 	NDEF, 	NDEF,
		//	112		113		114		115		116		117		118		119		120		122		123		124		125		126		127		128		
			NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF,	NDEF, 	NDEF, 	NDEF, 	NDEF 
};


unsigned char special_map[] = {
		//	0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15
			NDEF, 	ESC, 	'!', 	'@', 	'#', 	'$', 	'%', 	'^', 	'&', 	'*',	'(',	')',	'_',	'+',	'\b',	'\t',
		//	16		17		18		19		20		21		22		23		24		25		26		27		28		29		30		31
			'Q', 	'W', 	'E', 	'R', 	'T', 	'Y', 	'U', 	'I', 	'O', 	'P', 	'{',	'}',	'\n',	L_CTRL,	'A',	'S',
		//	32		33		34		35		36		37		38		39		40		41		42		43		44		45		46		47
			'D', 	'F', 	'G', 	'H', 	'J', 	'K', 	'L', 	165, 	'\'', 	NDEF, 	L_SHIFT,'|',	'Z',	'X',	'C',	'V',
		//	48		49		50		51		52		53		54		55		56		57		58		59		60		61		62		63
			'B', 	'N', 	'M', 	'<', 	'>', 	'?', 	R_SHIFT,'*', 	L_ALT, 	' ', 	CAPS, 	F1, 	F2,		F3,		F4,		F5,
		//	64		65		66		67		68		69		70		71		72		73		74		75		76		77		78		79
			F6, 	F7, 	F8, 	F9, 	F10, 	NUM, 	SCROLL, N7, 	N8, 	N9, 	'-', 	N4, 	N5, 	N6,		'+',	N1,
		//	80		81		82		83		84		85		86		87		88		89		90		91		92		93		94		95
			N2, 	N3, 	N0, 	NDOT, 	NDEF, 	NDEF, 	'>', 	F11, 	F12, 	NDEF, 	NDEF, 	NDEF, 	NDEF,	NDEF, 	NDEF,	NDEF,
		//	96		97		98		99		100		101		102		103		104		105		106		107		108		109		110		111
			NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF,	NDEF, 	NDEF, 	NDEF, 	NDEF,
		//	112		113		114		115		116		117		118		119		120		122		123		124		125		126		127		128		
			NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF, 	NDEF,	NDEF, 	NDEF, 	NDEF, 	NDEF
};

struct keyDataInput ans = {PRIVATE_KEY, NDEF, 0};

keyEntry scan2ascii(byte scan_code) {

	ans.scan_code = scan_code;

	switch (scan_code) {
	case CAPS:
		ans.type = PRIVATE_KEY;
		ttys[System.atty].input.flags.mayus_status = ttys[System.atty].input.flags.mayus_status == 0 ? 1 : 0;
		updateLeds();
		break;
	case L_SHIFT:
	case R_SHIFT:
	case L_SHIFT_OUT:
	case R_SHIFT_OUT:
		ans.type = PRIVATE_KEY;
		ttys[System.atty].input.flags.shift_status = ttys[System.atty].input.flags.shift_status == 0 ? 1 : 0;
		break;
	case NUM:
		ans.type = PRIVATE_KEY;
		ttys[System.atty].input.flags.num_status = ttys[System.atty].input.flags.num_status == 0 ? 1 : 0;
		updateLeds();
		break;
	case ESCAPED_PREFIX:
		ans.type = PRIVATE_KEY;
		ttys[System.atty].input.flags.escaped = 1;
		break;
	case ACCENT:
		ttys[System.atty].input.flags.accent_status = 1;
		break;
	case L_CTRL:case L_ALT:
		ans.type = PRIVATE_KEY;
		break;
	case F1:case F2:case F3:case F4:case F5:case F6:case F7:case F8:case F9:case F10:case F11:case F12:
		ans.type = SPECIAL_KEY;
		break;
	default:
		if (scan_code & 0x80) {
			ans.type = PRIVATE_KEY;
		} else {
			ans.type = PRINTABLE_KEY;
			if (ttys[System.atty].input.flags.escaped == 1) {
				ans.type = SPECIAL_KEY;
				ans.scan_code += 0x100;
				ttys[System.atty].input.flags.escaped = 0;
				return &ans;
			}
			if (ttys[System.atty].input.flags.shift_status == 1) {
				ans.ascii = special_map[ans.scan_code];
				if ((ans.ascii >= 'A' && ans.ascii <= 'Z') || ans.ascii == 165) {
					if (ttys[System.atty].input.flags.mayus_status == 1) {
						ans.ascii = ordinary_map[ans.scan_code];
					}
				}
				if(ttys[System.atty].input.flags.accent_status == 1){
					ans.ascii = accentuate(ans.ascii);
					ttys[System.atty].input.flags.accent_status = 0;
				}
			} else {
				if (ttys[System.atty].input.flags.mayus_status == 0) {
					ans.ascii = ordinary_map[ans.scan_code];
				} else {
					ans.ascii = special_map[ans.scan_code];
					if (!((ans.ascii >= 'A' && ans.ascii <= 'Z') || ans.ascii == 165))
						ans.ascii = ordinary_map[ans.scan_code];
				}
				if(ttys[System.atty].input.flags.accent_status == 1){
					ans.ascii = accentuate(ans.ascii);
					ttys[System.atty].input.flags.accent_status = 0;
				}
			}
		}
	}
	
	if (isKeyNumPad(ans.scan_code) == 1) {
		if (ttys[System.atty].input.flags.num_status == 1) {
			ans.type = PRINTABLE_KEY;
			ans.ascii = modify_ascii(ans.ascii);
		} else {
			ans.type = PRIVATE_KEY;
			ans.ascii = NDEF;
		}
	}
	return &ans;
	//return ascii;
}

int isKeyNumPad(unsigned char ascii) {
	if (ascii == N0 || ascii == N1 || ascii == N2 || ascii == N3 || ascii == N4
			|| ascii == N5 || ascii == N6 || ascii == N7 || ascii == N8
			|| ascii == N9 || ascii == NDOT)
		return 1;
	return 0;
}

unsigned char accentuate(unsigned char ascii){
	switch(ascii){
		case 'a': return 160;
		case 'e': return 130;
		case 'i': return 161;
		case 'o': return 162;
		case 'u': return 163;
		default: return ascii;
	}
}

unsigned char modify_ascii(unsigned char ascii) {
	switch (ascii) {
	case N0:
		ascii = '0';
		break;
	case N1:
		ascii = '1';
		break;
	case N2:
		ascii = '2';
		break;
	case N3:
		ascii = '3';
		break;
	case N4:
		ascii = '4';
		break;
	case N5:
		ascii = '5';
		break;
	case N6:
		ascii = '6';
		break;
	case N7:
		ascii = '7';
		break;
	case N8:
		ascii = '8';
		break;
	case N9:
		ascii = '9';
		break;
	case NDOT:
		ascii = '.';
		break;
	}
	return ascii;
}

void initializeKeyboard() {
	updateLeds();
}

void updateLeds() {

	unsigned char leds = ttys[System.atty].input.flags.scroll_status | 
		(ttys[System.atty].input.flags.num_status * 2) | 
		(ttys[System.atty].input.flags.mayus_status * 4);

	while ((_inport(0x64) & 2) != 0)
		;
	_outport(0x60, 0xED);

	while ((_inport(0x64) & 2) != 0)
		;
	_outport(0x60, leds);

}

escapedKey(unsigned int scan_code){
	unsigned char ascii;	
	switch(scan_code) {
		case F5:
			printf("F5");
			break;
		case F6:
			printf("F6");
			break;
		case F7:
			printf("F7");
			break;
		case F8:
			printf("F8");
			break;
		case F9:
			printf("F9");
			break;
		case F10:
			printf("F10");
			break;
		case F11:
			printf("F11");
			break;
		case F12:
			printf("F12");
			break;	
		case R_CTRL:
		case L_GUI:
		case R_GUI:
		case ALT_GR:
		case APPS:
		case INSERT:
		case HOME:
		case PG_UP:
		case DEL:
		case END:
		case PG_DN:
		case UP_ARROW:
		case LEFT_ARROW:
		case DOWN_ARROW:
		case RIGHT_ARROW:
			break;
		case KP_DIV:
			ascii = '/';
			System.write(DEVICE_KEYBOARD, &ascii, 1);
			break;
		case KP_EN:break;
	}
}
