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
	int (*func)(void);
} command_t;

/* command prototypes here */
int cmd_help(void);
int cmd_hello(void);
int cmd_search(void);
int cmd_exit(void);

/* command structure initializer */
static const command_t commands[] = {
	{"help", "This help text.", &cmd_help},
	{"hello", "Say hello to the user.", &cmd_hello},
	{"search", "Search for a string in another.", &cmd_search},
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
		if(strcmp(commands[i].cmd, buf) == 0)
			return commands[i].func();
	print("Bad command.\r\n");
	return 1;
}

