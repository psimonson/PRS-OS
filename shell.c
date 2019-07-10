/*
 * This is my simple implementation of a shell.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "time.h"
#include "string.h"
#include "fat12.h"

/* command structure */
typedef struct command {
	char *cmd;
	char *help;
	int (*func)(void);
} __attribute__((packed)) command_t;

/* command prototypes here */
int cmd_help(void);
int cmd_hello(void);
int cmd_play(void);
int cmd_playmusic(void);
int cmd_search(void);
int cmd_resetcmos(void);
int cmd_reboot(void);
int cmd_exec(void);
int cmd_ls(void);
int cmd_exit(void);

/* command structure initializer */
static const command_t commands[] = {
	{"help", "This help text.", &cmd_help},
	{"hello", "Say hello to the user.", &cmd_hello},
	{"play", "Play a frequency given by the user.", &cmd_play},
	{"play music", "Play some hard coded music.", &cmd_playmusic},
	{"search", "Search for a string in another.", &cmd_search},
	{"reset_CMOS", "Reset the CMOS settings.", &cmd_resetcmos},
	{"reboot", "Reboot the machine (warm reboot).", &cmd_reboot},
	{"exec", "Execute a given file name.", &cmd_exec},
	{"ls", "List root directory file names.", &cmd_ls},
	{"exit", "Exit the shell.", &cmd_exit}
};

/* uncomment these when needed */
/*static unsigned long *_buffer = (unsigned long*)0x02007c00;	* root directory in memory */
/*static entry_t const *_entry;					* directory entries */

/* Count number of commands.
 */
int command_count()
{
	return sizeof(commands)/sizeof(command_t);
}
/* Help command, displays the help for my shell.
 */
int cmd_help()
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
int cmd_hello()
{
	print("Hello user, welcome to a basic shell.\r\n");
	return 1;
}
/* Play command, plays a given frequency with PC speaker.
 */
int cmd_play()
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
int cmd_playmusic()
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
int cmd_search()
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
int cmd_resetcmos()
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
int cmd_reboot()
{
	reboot();
	return 0;
}
/* Exec Command, loads and executes a given filename.
 */
int cmd_exec()
{
	char name[256];
	print("Enter binary filename: ");
	if(gets(name, sizeof(name)) <= 0) {
		print("\r\nPlease enter a file name.\r\n");
		return -1;
	}
	puts("\r\nNot yet implemented!");
	return 1;
}
/* LS Command, just list out root directory entries.
 */
int cmd_ls()
{
/*
 * This code is for calculating stuff in a disk drive.
 * c = Cylinders
 * h = Heads
 * t = Tracks
 * s = Sectors
 ******************************************************

	unsigned char c, h, s;
	unsigned short t;
	unsigned long size;

	c = 1 / (p->numh * p->spt);
	t = 1 % (p->numh * p->spt);
	s = (t % p->spt) + 1;
	h = t / p->spt;

	size = 0;
 *******************************************************
 * Uncomment when needed...
 */

	/* TODO: implement for loop for each entry on disk */

	return 1;
}
/* Exit command, just exits the shell.
 */
int cmd_exit()
{
	return 0;
}

/* ------------------------------ Shell Functions ------------------------- */

/* Main function for processing and getting commands.
 */
int shell()
{
	char buf[256];
	int i;

	print("Enter command >> ");
	gets(buf, 255);
	print("\r\n");

	for(i=0; i<command_count(); i++)
		if(strcmp(buf, commands[i].cmd) == 0)
			return commands[i].func();
	puts("Bad command.");
	return 1;
}

