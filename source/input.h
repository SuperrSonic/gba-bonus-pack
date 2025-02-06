
// DPAD
#define INPUT_BUTTON_UP      (1<<0)
#define INPUT_BUTTON_DOWN    (1<<1)
#define INPUT_BUTTON_LEFT    (1<<2)
#define INPUT_BUTTON_RIGHT   (1<<3)

#define INPUT_BUTTON_OK      (1<<4)  // A(wiimote), A(gc), cross(PSX), b(classic)
#define INPUT_BUTTON_CANCEL  (1<<5)  // B(wiimote), B(gc), square(PSX), y(classic)
#define INPUT_BUTTON_1       (1<<6)  // 1(wiimote), Y(gc), triangle(PSX), x(classic)
#define INPUT_BUTTON_2       (1<<7)  // 2(wiimote), X(gc), circle(PSX), a(classic)
#define INPUT_BUTTON_START   (1<<8)  // home(wiimote/classic), start(gc/PSX)

void input_startup(void);
void input_shutdown(void);

/* fetches current button state, returns number of devices successfully read */
unsigned int get_input(unsigned int *pressed, unsigned int *down);