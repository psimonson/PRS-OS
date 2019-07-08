/*
 * This is my simple implementation of a shell.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

asm(".code16gcc\n");

#include "io.h"
#include "time.h"

/* command structure */
typedef struct __PACKED command {
	char *cmd;
	char *help;
	int (*func)(void);
} command_t;

/* command prototypes here */
int cmd_help(void);
int cmd_hello(void);
int cmd_exit(void);

/* command structure initializer */
static const command_t commands[] = {
	{"help", "This help text.", &cmd_help},
	{"hello", "Say hello to the user.", &cmd_hello},
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
/* Exit command, just exits the shell.
 */
int cmd_exit()
{
	return 0;
}

/* ------------------------------ Shell Functions ------------------------- */

/* My Simple implementation of gets.
 */
int gets(char *s, int size)
{
	char c;
	int i;

	for(i=0; (c = getche()) != -1 && c != '\r'; i++)
		s[i] = c;
	s[i] = '\0';
	return i;
}
/* Simple implementation of string compare.
 */
int strcmp(const char *s, const char *t)
{
	int i;

	for(i=0; s[i] != 0 || t[i] != 0; i++)
		if(s[i] != t[i])
			return s[i]-t[i];
	return 0;
}
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
	print("Bad command.\r\n");
	return 1;
}

