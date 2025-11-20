// This File will handle the user input and deal with any weird user inputs

// Include Libraries
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "CMS.h"

// create a set of constant commands
typedef enum {
	CMD_OPEN,
	CMD_SAVE,
	CMD_HELP,
	CMD_SHOWALL,
	CMD_SHOWSUMMARY,
	CMD_INSERT,
	CMD_QUERY,
	CMD_UPDATE,
	CMD_DELETE,
	CMD_EXIT,
	CMD_UNKNOWN
} CommandType;

// map the commmands to an input string
static CommandType getCommandType(const char* command) {
	if (strcmp(command, "open") == 0) {
		return CMD_OPEN;
	}
	else if (strcmp(command, "save") == 0) {
		return CMD_SAVE;
	}
	else if (strcmp(command, "help") == 0) {
		return CMD_HELP;
	}
	else if (strcmp(command, "showall") == 0) {
		return CMD_SHOWALL;
	}
	else if (strcmp(command, "showsummary") == 0) {
		return CMD_SHOWSUMMARY;
	}
	else if (strcmp(command, "insert") == 0) {
		return CMD_INSERT;
	}
	else if (strcmp(command, "query") == 0) {
		return CMD_QUERY;
	}
	else if (strcmp(command, "update") == 0) {
		return CMD_UPDATE;
	}
	else if (strcmp(command, "delete") == 0) {
		return CMD_DELETE;
	}
	else if (strcmp(command, "exit") == 0) {
		return CMD_EXIT;
	}
	return CMD_UNKNOWN;
}


// User name validation
static int Valid_User_Id(const char* id) {
	if (id[0] == '\0') return 0;  // reject empty string

	for (int i = 0; id[i] != '\0'; i++) {
		if (id[i] == ' ') {
			return 0;  // reject spaces
		}
	}
	return 1;  // everything else is allowed
}

static void decleration() {
	printf("Declaration\n"
		"SIT’s policy on copying does not allow the students to copy source code as well as assessment solutions\n"
		"from another person AI or other places. It is the students’ responsibility to guarantee that their\n"
		"assessment solutions are their own work. Meanwhile, the students must also ensure that their work is\n"
		"not accessible by others. Where such plagiarism is detected, both of the assessments involved will\n"
		"receive ZERO mark.\n\n"
		"We hereby declare that:\n"
		"- We fully understand and agree to the abovementioned plagiarism policy.\n"
		"- We did not copy any code from others or from other places.\n"
		"- We did not share our codes with others or upload to any other places for public access and will do that in the future.\n"
		"- We agree that our project will receive Zero mark if there is any plagiarism detected.\n"
		"- We agree that we will not disclose any information or material of the group project to others or upload to any other places for public access.\n"
		"- We agree that we did not copy any code directly from AI generated sources.\n"
		"Declared by: P14-5\n"
		"Team members:\n"
		"1. Nasyirah Binte Mohd Shariff (2503665)\n"
		"2. Siew Wee Kiam Eugene (2502012)\n"
		"3. Del Carmen Cyrus Aldwin Haboc (2501172)\n"
		"4. Lim Jun Wei (2501117)\n"
		"5. Muhamad Akid Qusyairi Bin Muhamad Riduan (2500882)\n\n"
		"Date: 23 November 2025.\n\n"
	);
}

int main() {

	// Variables
	char command_str[20];
	char User_ID[20];
	char file_name[20] = "P14_5-CMS.txt";
	char terminal_Name[] = "CMS";
	int program_running = 1;
	int Student_ID;
	char User_Input[256];
	char args[236] = ""; // var for string of commands
	char Student_Name[100];
	char New_Program[100];
	double New_Marks;
	RecordPtr head = NULL;
	int total;

	decleration();
	printf("Welcome to the Class Management System (CMS)\n");

	// loop to check for valid user name
	while (1) {
		printf("Please enter username:\n");
		fgets(User_ID, sizeof(User_ID), stdin); // reads the whole line including spaces
		User_ID[strcspn(User_ID, "\n")] = '\0'; // Remove trailing newline if present
		if (Valid_User_Id(User_ID)) {
			break; // valid username, exit loop
		}
		else {
			printf("Invalid username. Please try again.\n");
		}
	}

	// Program loop
	while (program_running) {
		// input command
		printf("%s: Please type a Command (Type help to list commands)\n%s: ", terminal_Name, User_ID);
		fgets(User_Input, sizeof(User_Input), stdin); // reads the whole line including spaces
		User_Input[strcspn(User_Input, "\n")] = '\0'; // Remove trailing newline if present

		//Split into command and arguments
		args[0] = '\0';
		sscanf(User_Input, "%s %[^\n]", command_str, args);

		// converts the string input to all lower letters
		for (int i = 0; command_str[i] != '\0'; i++) {
			command_str[i] = tolower((unsigned char)command_str[i]);
		}

		switch (getCommandType(command_str)) {
		case CMD_OPEN:
			// Checks if the list is empty if it is not empty clear the list
			if (head != NULL) {
				Clear_List(head);
				head = NULL;
			}
			if (Open_File(file_name, &head) == 1) {
				printf("%s: The database file %s is successfully opened.\n", terminal_Name, file_name);
			}
			else {
				printf("%s: The file was not successfully opened.\n", terminal_Name);
			}
			break;
		case CMD_SAVE:
			if (Save_File(file_name, head) == 1) {
				printf("%s: The database file %s is successfully saved.\n", terminal_Name, file_name);
			}
			else {
				printf("%s: The database file %s was not saved.\n", terminal_Name, file_name);
			}
			break;
		case CMD_SHOWALL:
			if (args[0] == '\0') {
				Show_All(head);
			}
			if (strcmp(args, "SORT BY ID") == 0) {
				Sort_ID(head);
			}
			else if (strcmp(args, "SORT BY MARKS") == 0) {
				Sort_Marks(head);
			}
			break;
		case CMD_UPDATE:
			Update_New(&head, args);
			break;
		case CMD_INSERT:
			if (Parse_Insert_Args(args, &Student_ID, Student_Name, sizeof(Student_Name), New_Program, sizeof(New_Program), &New_Marks)) {
				Insert_Data(&head, Student_ID, Student_Name, New_Program, New_Marks);
			}
			// checks if only ID was typed
			else if (sscanf(args, "ID=%d", &Student_ID) == 1) {
				if (check_ID(head, Student_ID)) {
					printf("CMS: The record with ID=%d already exists.\n", Student_ID);
					printf("CMS: Please try again. Use: INSERT ID=<id> Name=<name> Programme=<programme> Marks=<marks>\n");
				}
				else {
					printf("CMS: The record with ID=%d does not exist.\n", Student_ID);
					printf("CMS: Please try again. Use: INSERT ID=<id> Name=<name> Programme=<programme> Marks=<marks>\n");
				}
			}
			else {
				printf("CMS: Invalid INSERT format. Use: INSERT ID=<id> Name=<name> Programme=<programme> Marks=<marks>\n");
			}
			break;
		case CMD_QUERY:
			if (sscanf(args, "ID=%d", &Student_ID) == 1) {
				Search_id(head, Student_ID);
			}
			else {
				printf("CMS: Invalid QUERY format. Use: QUERY ID=<id>\n");
			}
			break;
		case CMD_SHOWSUMMARY:
			Show_Summary(head);
			break;
		case CMD_DELETE:
			if (sscanf(args, "ID=%d", &Student_ID) == 1) {
				Delete_Record(&head, Student_ID);
			}
			else {
				printf("CMS: Invalid DELETE format. Use: DELETE ID=<id>\n");
			}
			break;
		case CMD_HELP:
			printf("Options:\n"
				" 1)  OPEN        : Opens File.\n"
				" 2)  SAVE        : Saves File.\n"
				" 3)  HELP        : Opens the help menu.\n"
				" 4)  SHOWALL     : Displays all records and able to SORT by ID or Marks.\n"
				" 5)  SHOWSUMMARY : Displays summary of records.\n"
				" 6)  INSERT      : Adds a new record.\n"
				" 7)  QUERY       : Searches for a record.\n"
				" 8)  UPDATE      : Modifies an existing record.\n"
				" 9)  DELETE      : Removes a record.\n"
				" 10) EXIT        : Exits the program.\n");
			break;
		case CMD_EXIT:
			printf("Exiting CMS.....\n");
			Clear_List(head);
			head = NULL;
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