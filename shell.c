/*
 * This is my simple implementation of a shell.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "time.h"
#include "string.h"

/* command structure */
typedef struct __PACKED command {
	char *cmd;
	char *help;
	int __REGPARM (*func)(void);
} command_t;

/* command prototypes here */
int __REGPARM cmd_help(void);
int __REGPARM cmd_hello(void);
int __REGPARM cmd_play(void);
int __REGPARM cmd_playmusic(void);
int __REGPARM cmd_search(void);
int __REGPARM cmd_resetcmos(void);
int __REGPARM cmd_reboot(void);
int __REGPARM cmd_exit(void);

/* command structure initializer */
static const command_t commands[] = {
	{"help", "This help text.", &cmd_help},
	{"hello", "Say hello to the user.", &cmd_hello},
	{"play", "Play a frequency given by the user.", &cmd_play},
	{"play music", "Play some hard coded music.", &cmd_playmusic},
	{"search", "Search for a string in another.", &cmd_search},
	{"reset_CMOS", "Reset the CMOS settings.", &cmd_resetcmos},
	{"reboot", "Reboot the machine (warm reboot).", &cmd_reboot},
	{"exit", "Exit the shell.", &cmd_exit}
};

/* Count number of commands.
 */
int command_count()
{
	return sizeof(commands)/sizeof(command_t);
}
/* Help command, displays the help for my shell.
 */
int __REGPARM cmd_help()
{
	int i;

	print("Commands List [help,hello,exit]\r\n");
	for(i=0; i<command_count(); i++) {
		print(commands[i].cmd);
		print(" - ");
		print(commands[i].help);
		print("\r\n");
	}
	return 1;
}
/* Hello command, says hello to the user.
 */
int __REGPARM cmd_hello()
{
	print("Hello user, welcome to a basic shell.\r\n");
	return 1;
}
/* Play command, plays a given frequency with PC speaker.
 */
int __REGPARM cmd_play()
{
	char buf[256];
	int freq;

	print("Enter a number: ");
	if(gets(buf, sizeof(buf)) <= 0) {
		print("\r\nYou need to enter a string.\r\n");
		return -1;
	}
	print("\r\n");
	freq = atoi(buf);
	if(!freq) {
		print("No number given.\r\n");
		return -1;
	}
	play_sound(freq);
	wait(50000);
	no_sound();
	return 1;
}
/* PlayMusic command, plays a hard coded song.
 */
int __REGPARM cmd_playmusic()
{
	play_sound(10);
	wait(100000);
	no_sound();
	play_sound(20);
	wait(100000);
	no_sound();
	play_sound(30);
	wait(100000);
	no_sound();
	play_sound(40);
	wait(100000);
	no_sound();
	play_sound(50);
	wait(100000);
	no_sound();
	play_sound(60);
	wait(100000);
	no_sound();
	return 1;
}
/* Search command, searchs a string for another string.
 */
int __REGPARM cmd_search()
{
	char buf[256];
	char str[256];
	char *found = 0;
	print("Enter a string: ");
	if(gets(buf, sizeof(buf)) <= 0) {
		print("\r\nYou need to enter a string.");
		return -1;
	}
	print("\r\nEnter search pattern: ");
	if(gets(str, sizeof(str)) <= 0) {
		print("\r\nPlease enter search pattern.\r\n");
		return -1;
	}
	print("\r\nPattern: ");
	print(str);
	if((found = strstr(buf, str)) != 0)
		print(" [Found]\r\n");
	else
		print(" [Not Found]\r\n");
	return 1;
}
/* ResetCMOS command, just resets the CMOS settings.
 */
int __REGPARM cmd_resetcmos()
{
	const unsigned char CHECKSUM_HI = 0x2e;
	const unsigned char CHECKSUM_LO = 0x2f;
	print("Resetting CMOS to defaults...\r\n");
	cmos_invert(CHECKSUM_HI);
	cmos_invert(CHECKSUM_LO);
	print("Done.\r\n");
	return 1;
}
/* Reboot command, just reboots the machine.
 */
int __REGPARM cmd_reboot()
{
	reboot();
	return 0;
}
/* Exit command, just exits the shell.
 */
int __REGPARM cmd_exit()
{
	return 0;
}

/* ------------------------------ Shell Functions ------------------------- */

/* Main function for processing and getting commands.
 */
int __REGPARM shell()
{
	char buf[256];
	int i;

	print("Enter command >> ");
	gets(buf, 255);
	print("\r\n");

	for(i=0; i<command_count(); i++)
		if(strcmp(commands[i].cmd, buf) == 0)
			return commands[i].func();
	print("Bad command.\r\n");
	return 1;
}

