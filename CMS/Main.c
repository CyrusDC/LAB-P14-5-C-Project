
// This File will handle the user input and deal with any weird user inputs

// Include Libraries
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "CMS.h"


typedef enum {
	CMD_OPEN,
	CMD_SAVE,
	CMD_HELP,
	CMD_SHOWALL,
	CMD_INSERT,
	CMD_QUERY,
	CMD_UPDATE,
	CMD_DELETE,
	CMD_EXIT,
	CMD_UNKNOWN
} CommandType;

static CommandType getCommandType(const char* command) {
	if (strcmp(command, "open") == 0) return CMD_OPEN;
	if (strcmp(command, "save") == 0) return CMD_SAVE;
	if (strcmp(command, "help") == 0) return CMD_HELP;
	if (strcmp(command, "showall") == 0) return CMD_SHOWALL;
	if (strcmp(command, "insert") == 0) return CMD_INSERT;
	if (strcmp(command, "query") == 0) return CMD_QUERY;
	if (strcmp(command, "update") == 0) return CMD_UPDATE;
	if (strcmp(command, "delete") == 0) return CMD_DELETE;
	if (strcmp(command, "exit") == 0) return CMD_EXIT;
	return CMD_UNKNOWN;
}

static int isValidUserId(const char* id) {
	if (id[0] == '\0') return 0;  // reject empty string

	for (int i = 0; id[i] != '\0'; i++) {
		if (id[i] == ' ') {
			return 0;  // reject spaces
		}
	}
	return 1;  // everything else is allowed
}


int main() {
	
	// Variables
	char command_str[20];
	char user_id[20];
	char file_name[20];
	char terminal_Name[] = "CMS";
	int program_running = 1;
	RecordPtr head = NULL;

	printf("Welcome to the Class Management System (CMS)\n");

	// loop to check for valid user name
	while (1) {
		printf("Please enter username:\n");
		scanf("%s", user_id);
		if (isValidUserId(user_id)) {
			break; // valid username, exit loop
		}
		else {
			printf("Invalid username. Please try again.\n");
		}
	}
	
	// Program loop
	while (program_running) {
		// input command
		printf("%s: Please type a Command (Type help to list commands)\n%s: ", terminal_Name, user_id);
		scanf("%s" ,command_str);

		// converts the string input to all lower letters
		for (int i = 0; command_str[i] != '\0'; i++) {
			command_str[i] = tolower((unsigned char)command_str[i]);
		}

		switch (getCommandType(command_str)) {
			case CMD_OPEN:
				printf("%s: Please type file name\n%s: ", terminal_Name,user_id);
				scanf("%s" ,file_name);
				if (Open_File(file_name, &head) == 1) {
					printf("%s: The database file %s is successfully opened.\n", terminal_Name, file_name);
				}
				else {
					printf("%s: The file was not successfully opened.\n", terminal_Name);
				}
				break;
			case CMD_SAVE:
				printf("See you later!\n");
				break;
			case CMD_HELP:
				printf("Options:\n"
					" 1) OPEN    : Opens File\n"
					" 2) SAVE    : Saves File\n"
					" 3) HELP    : Opens the help menu\n"		
					" 4) SHOWALL : Displays all records\n"
					" 5) INSERT  : Adds a new record\n"
					" 6) QUERY   : Searches for a record\n"
					" 7) UPDATE  : Modifies an existing record\n"
					" 8) DELETE  : Removes a record\n"
					" 9) EXIT    : Exits the program\n");
				break;
			case CMD_SHOWALL:
				Show_All(head);
				break;
			case CMD_INSERT:
				printf("Insert\n");
				break;
			case CMD_QUERY:
				printf("Query\n");
				break;
			case CMD_UPDATE:
				printf("update\n");
				break;
			case CMD_DELETE:
				printf("delete\n");
				break;
			case CMD_EXIT:
				printf("Exiting CMS.....\n");
				program_running = 0;
				break;
			case CMD_UNKNOWN:
			default:
				printf("Unknown command. Please Try Again or Type Help\n");
				break;
		}
	}
	return 0;
}