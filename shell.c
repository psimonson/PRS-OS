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
#include "disk.h"

/* external variables */
extern boot_t *_boot_sector;		/* boot sector */
extern drive_params_t _drive_params;	/* disk parameters */

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
int cmd_dump(void);
int cmd_exec(void);
int cmd_ls(void);
int cmd_find(void);
int cmd_format(void);
int cmd_type(void);
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
	{"dump", "Dumps the CMOS data to the screen.", &cmd_dump},
	{"exec", "Execute a given file name.", &cmd_exec},
	{"ls", "List root directory file names.", &cmd_ls},
	{"find", "Search for a file in root directory.", &cmd_find},
	{"format", "Format a floppy diskette.", &cmd_format},
	{"type", "Re-type the phrase you enter.", &cmd_type},
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
int cmd_help()
{
	int i;

	printf(
	"============================================================\r\n"
	"                      .:[COMMANDS]:.\r\n"
	"============================================================\r\n"
	);
	for(i=0; i<command_count(); i++)
		printf("%s - %s\r\n", commands[i].cmd, commands[i].help);
	printf(
	"============================================================\r\n"
	"                        .:[EOL]:.\r\n"
	"============================================================\r\n"
	);
	return 1;
}
/* Hello command, says hello to the user.
 */
int cmd_hello()
{
	printf("Hello user, welcome to a basic shell.\r\n");
	return 1;
}
/* Play command, plays a given frequency with PC speaker.
 */
int cmd_play()
{
	char buf[50];
	int freq;

	printf("Enter a number: ");
	if(gets(buf, sizeof(buf)) <= 0) {
		printf("\r\nYou need to enter a string.\r\n");
		return -1;
	}
	printf("\r\n");
	freq = atoi(buf);
	if(!freq) {
		printf("No number given.\r\n");
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
#if 0
	char buf[50];

	printf("Enter filename: ");
	if(gets(buf, sizeof(buf) <= 0) {
		printf("\r\nYou need to enter a filename.\r\n");
		return -1;
	}
	printf("\r\n");
	/* TODO: Add PC speaker code here to play MOD/S3M files. */
#else
	int i = 3;
	while(i > 0) {
		play_sound(100);
		wait(100000);
		no_sound();
		play_sound(200);
		wait(100000);
		no_sound();
		play_sound(300);
		wait(100000);
		no_sound();
		play_sound(400);
		wait(100000);
		no_sound();
		play_sound(250);
		wait(100000);
		no_sound();
		play_sound(280);
		wait(100000);
		no_sound();
		play_sound(300);
		wait(100000);
		no_sound();
		play_sound(200);
		wait(100000);
		no_sound();
		play_sound(300);
		wait(100000);
		no_sound();
		play_sound(100);
		wait(100000);
		no_sound();
		play_sound(500);
		wait(100000);
		no_sound();
		play_sound(200);
		wait(100000);
		no_sound();
		--i;
	}
#endif
	return 1;
}
/* Search command, searchs a string for another string.
 */
int cmd_search()
{
	char buf[50];
	char str[50];
	char *found = 0;
	printf("Enter a string: ");
	if(gets(buf, sizeof(buf)) <= 0) {
		printf("\r\nYou need to enter a string.");
		return -1;
	}
	printf("\r\nEnter search pattern: ");
	if(gets(str, sizeof(str)) <= 0) {
		printf("\r\nPlease enter search pattern.\r\n");
		return -1;
	}
	printf("\r\nPattern: ");
	if((found = strstr(buf, str)) != 0)
		printf("\"%s\" [Found]\r\n", str);
	else
		printf("\"%s\" [Not Found]\r\n", str);
	return 1;
}
/* ResetCMOS command, just resets the CMOS settings.
 */
int cmd_resetcmos()
{
	char i;
	printf("Resetting CMOS to defaults...\r\n");
	for(i=0; i<128; i++) {
		cmos_write(i, 0xff);
	}
	printf("Done.\r\n");
	return 1;
}
/* Reboot command, just reboots the machine.
 */
int cmd_reboot()
{
	reboot();
	return 0;
}
/* Dump command, dumps the CMOS data to the screen.
 */
int cmd_dump()
{
	extern boot_t *_boot_sector;
	extern void *_FAT_table;
	char buf[32];
	int i;
	printf("Enter [boot|cmos|FAT]: ");
	if(gets(buf, sizeof(buf)) <= 0) {
		printf("You must enter 'boot', 'cmos', or 'FAT'.\r\n");
		return 1;
	}
	printf("\r\n");
	if(!strcmp(buf, "cmos")) {
		printf("=============================================\r\n");
		printf("CMOS Data\r\n"
			"=============================================\r\n");
		for(i=0; i<255; i++) {
			if(i>0 && !(i%15))
				printf("\r\n");
			itoh(cmos_read(i), buf);
			printf("%s ", buf);
		}
		printf("\r\n=============================================\r\n");
		printf("Press any key to continue...\r\n");
		getch();
		printf("=============================================\r\n");
		printf("CMOS Data\r\n"
			"=============================================\r\n");
		for(i=0; i<255; i++) {
			if(i>0 && !(i%44))
				printf("\r\n");
			putch(cmos_read(i));
		}
		printf("\r\n=============================================\r\n");
	} else if(!strcmp(buf, "boot")) {
		int nl;
		printf("=============================================\r\n");
		printf("BOOT Data\r\n"
			"=============================================\r\n");
		for(i=0,nl=0; i<sizeof(boot_t); i++) {
			if(i>0 && !(i%21)) {
				printf("\r\n");
				++nl;
			}
			if(nl>0 && !(nl%20)) {
				printf("Press any key to continue...\r\n");
				getch();
				++nl;
			}
			itoh(((unsigned char*)_boot_sector)[i], buf);
			printf("%s ", buf);
		}
		printf("\r\n=============================================\r\n");
		printf("Press any key to continue...\r\n");
		getch();
		printf("=============================================\r\n");
		printf("BOOT Data\r\n"
			"=============================================\r\n");
		for(i=0,nl=0; i<sizeof(boot_t); i++) {
			if(i>0 && !(i%50)) {
				printf("\r\n");
				++nl;
			}
			if(nl>0 && !(nl%20)) {
				printf("Press any key to continue...\r\n");
				getch();
				++nl;
			}
			putch(((unsigned char*)_boot_sector)[i]);
		}
		printf("\r\n=============================================\r\n");
	} else if(!strcmp(buf, "FAT")) {
		int nl;
		printf("=============================================\r\n");
		printf("FAT12 Data\r\n"
			"=============================================\r\n");
		for(i=0,nl=0; i<BUFSIZ; i++) {
			if(i>0 && !(i%21)) {
				printf("\r\n");
				++nl;
			}
			if(nl>0 && !(nl%20)) {
				printf("Press any key to continue...\r\n");
				getch();
				++nl;
			}
			itoh(((unsigned char*)_FAT_table)[i], buf);
			printf("%s ", buf);
		}
		printf("\r\n=============================================\r\n");
		printf("Press any key to continue...\r\n");
		getch();
		printf("=============================================\r\n");
		printf("FAT12 Data\r\n"
			"=============================================\r\n");
		for(i=0,nl=0; i<BUFSIZ; i++) {
			if(i>0 && !(i%50)) {
				printf("\r\n");
				++nl;
			}
			if(nl>0 && !(nl%20)) {
				printf("Press any key to continue...\r\n");
				getch();
				++nl;
			}
			putch(((unsigned char*)_FAT_table)[i]);
		}
		printf("\r\n=============================================\r\n");
	} else {
		printf("Invalid choice: %s\r\n"
			"Choices are: 'boot', 'cmos', or 'FAT'.\r\n",
			buf);
	}
	return 1;
}
/* Exec command, loads and executes a given filename.
 */
int cmd_exec()
{
#if 0
	char name[256];
	printf("Enter binary filename: ");
	if(gets(name, sizeof(name)) <= 0) {
		printf("\r\nPlease enter a file name.\r\n");
		return -1;
	}
#else
	puts("Not yet implemented!");
#endif
	return 1;
}
/* Ls command, just list out root directory entries.
 */
int cmd_ls()
{
	if(reset_drive(&_drive_params)) {
		get_drive_error(&_drive_params);
		return -1;
	}
	printf("Listing root directory...\r\n");
	list_directory(&_drive_params, _boot_sector);
	return 1;
}
/* Find command, search for a file in the root directory.
 */
int cmd_find()
{
	char buf[50];

	printf("Enter filename: ");
	if(gets(buf, sizeof(buf)) <= 0) {
		printf("\r\nYou must enter a filename.\r\n");
		return -1;
	}
	if(reset_drive(&_drive_params)) {
		get_drive_error(&_drive_params);
		return -1;
	}
	printf("\r\nSearching...\r\n");
	find_file(&_drive_params, _boot_sector, buf);
	return 1;
}
/* Format command, just what it says (format a disk).
 */
int cmd_format()
{
	static unsigned char sector[512];
	unsigned char result = 0;
	unsigned short i, num, last_error = 0;
	drive_params_t p;
	char buf[10];

	memset(sector, 0, sizeof(sector));
	printf("Enter drive index number (0 or 1): ");
	if(gets(buf, sizeof(buf)) <= 0) {
		printf("\r\nYou must enter a drive index number.\r\n");
		return -1;
	}
	num = atoi(buf);
	result = get_drive_params(&p, (num >= 0 && num <= 1 ? num : -1));
	if(result) {
		printf("\r\nCould not get drive parameters.\r\n");
		return -1;
	}
	result = reset_drive(&p);
	if(result) {
		printf("\r\nCould not reset drive.\r\n");
		return -1;
	}
	i = 0;
	printf("\r\nWiping drive number %d.\r\n", p.drive);
	while(!last_error && i < FLP_144_SECT) {
#ifdef DEBUG
		unsigned char c, h, s;
		sector_to_chs(i, &c, &h, &s);
		printf("Wiping [C:%d | H:%d | S:%d]\r\n", c, h, s);
#endif
		result = write_drive(sector, 1, i, &p);
		if(result) {
			last_error = get_drive_status(&p);
			result = 0;
		} else {
			if((i%80) == 0) {
				printf("Wiped: %d%%\r\n",
					(int)((i/(float)FLP_144_SECT)*100));
			}
		}
		i++;
	}
	if(last_error)
		printf("Failure partially wiped.\r\n");
	else
		printf("Drive wiped successfully.\r\n");
	return 1;
}
/* Type command, just types the text you enter with a short delay.
 */
int cmd_type()
{
	char buf[80];
	printf("Enter some text for the typer...\r\n> ");
	if(gets(buf, 80) <= 0) {
		printf("\r\nYou must enter something.\r\n");
		return -1;
	}
	printf("\r\n");
	typerf("%s", buf);
	printf("\r\n");
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
	int i, running;

	running = 1;
	while(running) {
		char found = 0;
		printf("Enter command >> ");
		if(gets(buf, 255) <= 0) {
			printf("\r\nPlease enter some text.\r\n");
			continue;
		}
		printf("\r\n");

		for(i=0; i<command_count(); i++)
			if(strcmp(commands[i].cmd, buf) == 0) {
				running = commands[i].func();
				found = 1;
			}
		if(found)
			continue;
		printf("Bad command entered - \"%s\".\r\n", buf);
	}
	return running;
}

