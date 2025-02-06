/* This input code was written by tueidj */

#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <stdio.h>
#include <wiiuse/wpad.h>

#include "input.h"

typedef struct input_driver
{
	struct input_driver* (*init)(struct input_driver*);
	void (*shutdown)(struct input_driver* this);
	int (*poll)(struct input_driver* this, unsigned int *pressed);
} input_driver;

static input_driver *g_in;
static const input_driver wpad_interface;
static const input_driver gc_interface;

#ifdef HW_RVL
extern int batt_lvl;
u8 frame_cnt = 0;

/***** BEGIN WIIMOTE *****/

typedef struct wpad_driver
{
	input_driver driver;
	input_driver *next;
	struct wiimote_t** wiimotes;
	int wpad_initted;
	int wpad_connected;
	uint8_t wm_data[WPAD_MAX_WIIMOTES][64];
} wpad_driver;

static wpad_driver *_wpad;

static void wpad_shutdown(input_driver *driver)
{
	wpad_driver *wpad = (wpad_driver*)driver;

	if (wpad==NULL)
		return;

	if (wpad->wpad_initted)
	{
		WPAD_Shutdown();
		wpad->wpad_initted = 0;
	}

	if (wpad->next)
		wpad->next->shutdown(wpad->next);

	wpad->next = NULL;
}
#if 0
static void wm_read_cb(struct wiimote_t *wm, ubyte *data, uword len)
{
	int i;
	if (len != 16 || _wpad==NULL || _wpad->wiimotes==NULL)
		return;

	for (i=0; i < WPAD_MAX_WIIMOTES; i++)
	{
		if (_wpad->wiimotes[i]==wm)
			break;
	}

	if (i==WPAD_MAX_WIIMOTES)
		return; // don't know which wiimote this is
}
#endif
static int wpad_poll(input_driver *driver, unsigned int *pressed)
{
	wpad_driver *wpad = (wpad_driver*)driver;
	int read = 0;
	const WPADData *wd;
	bool chk_bat = false;

	if (wpad && wpad->wpad_initted)
	{
		int i;

		// This stuff isn't used yet
		for (i=0; i < WPAD_MAX_WIIMOTES; i++)
		{
			if (wpad->wpad_connected & (1<<i) && WPAD_Probe(i, NULL)==WPAD_ERR_NO_CONTROLLER)
			{
//				printf("Wiimote %d disconnected\n", i);
				wpad->wpad_connected &= ~(1<<i);
			}
			else if ((wpad->wpad_connected & (1<<i))==0 && WPAD_Probe(i, NULL) == WPAD_ERR_NONE)
			{
				chk_bat = true;
//				printf("Wiimote %d connected (%p)\n", i, wpad->wm_data[i]);
				wpad->wpad_connected |= (1<<i);
//				wiiuse_read_data(wpad->wiimotes[i], wpad->wm_data[i], , sizeof(wpad->wm_data[0]), wm_read_cb);
			}
		}

		if (WPAD_ReadPending(WPAD_CHAN_0, NULL)>0 && (wd = WPAD_Data(WPAD_CHAN_0)) && (wd->data_present&WPAD_DATA_BUTTONS))
		{
			if (wd->btns_d & WPAD_BUTTON_DOWN)
				*pressed |= INPUT_BUTTON_DOWN;
			if (wd->btns_d & WPAD_BUTTON_UP)
				*pressed |= INPUT_BUTTON_UP;
			if (wd->btns_d & WPAD_BUTTON_LEFT)
				*pressed |= INPUT_BUTTON_LEFT;
			if (wd->btns_d & WPAD_BUTTON_RIGHT)
				*pressed |= INPUT_BUTTON_RIGHT;
			if (wd->btns_d & WPAD_BUTTON_A)
				*pressed |= INPUT_BUTTON_OK;
			if (wd->btns_d & WPAD_BUTTON_B)
				*pressed |= INPUT_BUTTON_CANCEL;
			if (wd->btns_d & WPAD_BUTTON_1)
				*pressed |= INPUT_BUTTON_1;
			if (wd->btns_d & WPAD_BUTTON_2)
				*pressed |= INPUT_BUTTON_2;
			if (wd->btns_d & WPAD_BUTTON_HOME)
				*pressed |= INPUT_BUTTON_START;

			if (wd->exp.type == WPAD_EXP_CLASSIC)
			{
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_DOWN)
					*pressed |= INPUT_BUTTON_DOWN;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_UP)
					*pressed |= INPUT_BUTTON_UP;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_LEFT)
					*pressed |= INPUT_BUTTON_LEFT;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_RIGHT)
					*pressed |= INPUT_BUTTON_RIGHT;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_B)
					*pressed |= INPUT_BUTTON_OK;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_Y)
					*pressed |= INPUT_BUTTON_CANCEL;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_X)
					*pressed |= INPUT_BUTTON_1;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_A)
					*pressed |= INPUT_BUTTON_2;
				if (wd->btns_d & WPAD_CLASSIC_BUTTON_HOME)
					*pressed |= INPUT_BUTTON_START;
			}

			read = 1;
		}
	}

	if (wpad->next)
		read += wpad->next->poll(wpad->next, pressed);
	
	if(chk_bat && frame_cnt < 20) {
		batt_lvl = ((WPAD_BatteryLevel(0) * 4) / 100) ; //only first wiimote
		if (batt_lvl > 4) batt_lvl = 4;
		//printf("\nRIGHT NOW: %d\n", batt_lvl);
	}
	++frame_cnt;

	return read;
}

static input_driver* wpad_init(input_driver *in)
{
	wpad_driver *wpad = _wpad;

	if (wpad && wpad->wpad_initted)
		return in;

	if (wpad == NULL)
	{
		wpad = (wpad_driver*)malloc(sizeof(wpad_driver));
		if (wpad==NULL)
			return in;
		_wpad = wpad;
		// get wiimote_t array before WPAD_Init() calls wiiuse_init()
		wpad->wiimotes = wiiuse_init(WPAD_MAX_WIIMOTES, NULL);
		wpad->wpad_connected = 0;
	}
	else if (wpad->wpad_initted)
		return in;


	WPAD_Init();
	wpad->wpad_initted = 1;

	wpad->driver = wpad_interface;
	wpad->next = in;

	return &wpad->driver;
}
#endif
/***** END WIIMOTE *****/

/***** BEGIN GAMECUBE *****/

typedef struct gc_driver
{
	input_driver driver;
	input_driver *next;
} gc_driver;

static int gc_initted;

static int gc_poll(input_driver *driver, unsigned int *pressed)
{
	gc_driver *gc = (gc_driver*)driver;
	int read = 0;

	if (PAD_ScanPads() & 1) // check for controller 0 being connected
	{
		u16 buttons = PAD_ButtonsHeld(PAD_CHAN0);

		if (buttons & PAD_BUTTON_DOWN)
			*pressed |= INPUT_BUTTON_DOWN;
		if (buttons & PAD_BUTTON_UP)
			*pressed |= INPUT_BUTTON_UP;
		if (buttons & PAD_BUTTON_LEFT)
			*pressed |= INPUT_BUTTON_LEFT;
		if (buttons & PAD_BUTTON_RIGHT)
			*pressed |= INPUT_BUTTON_RIGHT;
		if (buttons & PAD_BUTTON_A)
			*pressed |= INPUT_BUTTON_OK;
		if (buttons & PAD_BUTTON_B)
			*pressed |= INPUT_BUTTON_CANCEL;
		if (buttons & PAD_BUTTON_Y)
			*pressed |= INPUT_BUTTON_1;
		if (buttons & PAD_BUTTON_X)
			*pressed |= INPUT_BUTTON_2;
		if (buttons & PAD_BUTTON_START)
			*pressed |= INPUT_BUTTON_START;

		read = 1;
	}

	if (gc->next)
		read += gc->next->poll(gc->next, pressed);

	return read;
}

static void gc_shutdown(input_driver *driver)
{
	gc_driver *gc = (gc_driver*)driver;

	// nothing to do here...

	if (gc->next)
		gc->next->shutdown(gc->next);

	free(gc);
	gc_initted = 0;
}

static input_driver* gc_init(input_driver *in)
{
	gc_driver *gc;

	if (gc_initted || PAD_Init() < 0)
		return in;

	gc = (gc_driver*)malloc(sizeof(gc_driver));
	if (gc==NULL)
		return in;

	gc->driver = gc_interface;
	gc->next = in;
	gc_initted = 1;

	return &gc->driver;
}


/***** END GAMECUBE *****/
#ifdef HW_RVL
static const input_driver wpad_interface = {wpad_init, wpad_shutdown, wpad_poll};
#endif
static const input_driver gc_interface = {gc_init, gc_shutdown, gc_poll};

static const input_driver* g_drivers[] =
{
#ifdef HW_RVL
	&wpad_interface,
#endif
	&gc_interface,
	NULL
};


void input_startup(void)
{
	unsigned int i;

	for (i=0; g_drivers[i]; i++)
		g_in = g_drivers[i]->init(g_in);
}

void input_shutdown(void)
{
	if (g_in)
	{
		g_in->shutdown(g_in);
		g_in = NULL;
	}
}

/* fetches current button state, returns number of devices successfully read */
unsigned int get_input(unsigned int *pressed, unsigned int *down)
{
	static unsigned int last;
	unsigned int buttons = 0;
	unsigned int read = 0;

	if (g_in)
		read = g_in->poll(g_in, &buttons);

	if (pressed)
		*pressed = buttons;
	if (down)
		*down = ~last & buttons;
	last = buttons;
	return read;
}