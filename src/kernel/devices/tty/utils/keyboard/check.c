#include <kernel/devices/tty.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/util/casio.h>
#include <kernel/util/atomic.h>
#include <kernel/signal.h>

//FIXME: SIGINT and SIGQUIT is send by the hardware driver ?!
void tty_keyboard_check_signal(struct tty *tty)
{
	extern struct process *process_current;
	struct { uint8_t keycode; int signal; } echapment[3] = {
		{.keycode = KEY_LN, .signal = SIGINT},
		{.keycode = KEY_DIV, .signal = SIGQUIT},
		{.keycode = KEY_0, .signal = SIGTSTP}
	};
	int signal;

	// Check signal sending request
	if (tty->keyboard.mode.ctrl == 0) {
		pthread_mutex_unlock(&tty->keyboard.private.mutex);
		return;
	}

	// Try to find the signal to send
	signal = -1;
	for (int i = 0 ; i < 3 ; ++i) {
		if (echapment[i].keycode != tty->keyboard.saved.signal.keycode)
			continue;
		signal = echapment[i].signal;
		break;
	}
	if (signal == -1) {
		pthread_mutex_unlock(&tty->keyboard.private.mutex);
		return;
	}

	// Remove first saved character
	tty_keyboard_buffer_remove(tty, tty->keyboard.saved.signal.cursor);

	// Send signal
	// TODO: display signal ?
	signal_raise(process_current, signal);
}


//TODO update me
//@note: Call this function ONLY if the device is locked !!
int tty_keyboard_check_special(struct tty *tty, key_t key)
{
	// Check CTRL key (KEY_OPT)
	if (key == KEY_OPTN) {
		tty->keyboard.mode.ctrl = 1;
		return (1);
	}

	// Check MAJ.
	if (key == KEY_ALPHA) {
		tty->keyboard.mode.maj ^= 1;
		return (1);
	}

	// Check Alpha / num mode. (special)
	if (key == KEY_SHIFT) {
		tty->keyboard.mode.special ^= 0x01;
		return (1);
	}

	// Check space key (workaround)
	if (key == KEY_DOT && tty->keyboard.mode.special == 0) {
		tty_keyboard_buffer_insert(tty, ' ');
		return (1);
	}

	// Check DEL key.
	if (key == KEY_DEL){
		tty_keyboard_buffer_remove(tty, tty->keyboard.private.cursor);
		return (1);
	}

	// Check MENU key.
	if (key == KEY_MENU) {
		casio_return_menu(0);
		return (1);
	}

	// Check EXE key.
	if (key == KEY_EXE)
	{
		// Add new line character.
		tty->buffers.input[tty->keyboard.private.cursor_max] = '\n';
		tty->keyboard.private.cursor_max += 1;
		tty->buffers.input[tty->keyboard.private.cursor_max] = '\0';
			
		// indicate that the EXE has been pressed.
		tty->keyboard.mode.enter = 1;
		return (1);
	}

	// Check LEFT key.
	if (key == KEY_LEFT)
	{
		if (tty->keyboard.private.cursor > 0)
			tty->keyboard.private.cursor -= 1;
		return (1);
	}

	// Check RIGHT key.
	if (key == KEY_RIGHT)
	{
		if (tty->keyboard.private.cursor < tty->keyboard.private.cursor_max)
			tty->keyboard.private.cursor += 1;
		return (1);
	}
	return (0);
}
