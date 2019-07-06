/*
 * This is my simple implementation of a shell.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#include "io.h"
#include "time.h"

typedef struct __PACKED command {
	char *cmd;
	char *help;
	int (*func)(void);
} command_t;

/* command prototypes here */
int cmd_hello(void);
int cmd_exit(void);

static const command_t commands[] = {
	{"hello", "Say hello to the user.", &cmd_hello},
	{"exit", "Exit the shell.", &cmd_exit}
};

int command_count()
{
	return sizeof(commands)/sizeof(command_t);
}

int cmd_hello()
{
	print("Hello user, welcome to a basic shell.\r\n");
	return 1;
}

int cmd_exit()
{
	return 0;
}

/* ------------------------------ Shell Functions ------------------------- */

int gets(char *s, int size)
{
	char c;
	int i;

	for(i=0; (c = getche()) != -1 && c != '\r'; i++)
		s[i] = c;
	s[i] = '\0';
	return i;
}

int strcmp(const char *s, const char *t)
{
	int i;

	for(i=0; s[i] != 0 || t[i] != 0; i++)
		if(s[i] != t[i])
			return s[i]-t[i];
	return 0;
}

int shell()
{
	char buf[256];
	int running;

	running = 1;
	while(running) {
		int i;

		print("Enter command >> ");
		gets(buf, 255);
		print("\r\n");

		for(i=0; i<command_count(); i++)
			if(strcmp(buf, commands[i].cmd) == 0)
				return commands[i].func();
	}
	return running;
}

