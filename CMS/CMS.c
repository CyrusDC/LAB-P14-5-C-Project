// This file will handle all the functions such as OPEN, SAVE, SHOWALL, INSERT, QUERY, UPDATE and DELETE

// Include libraries
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CMS.h"

// Global Variables
char buffer[256];
int id;
char name[100];
char program[100];
double marks;
char User_Ans[4];
int Output_Mark = 0;
int Output_Program = 0;

// Headers for output
void header_output() {
	// Use fixed width columns: ID (8), Name (20), Programme (25), Mark (5 with 1 decimal)
	printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
	printf("%-8s  %-20s  %-25s  %5s\n", "ID", "Name", "Programme", "Mark");
}
//-------------------------------------------------------------------//
// OPEN Function
int Open_File(const char* file, RecordPtr* head) {
	FILE* fh_output;
	fh_output = fopen(file, "r");
	if (fh_output == NULL) {
		return -1; // failed
	}
	else {
		while (fgets(buffer, sizeof(buffer), fh_output)) {
			// Skip header lines and look for the colunm headers
			if (strncmp(buffer, "Database", 8) == 0) continue;
			if (strncmp(buffer, "Authors", 7) == 0) continue;
			if (strncmp(buffer, "Table", 5) == 0) continue;
			if (strncmp(buffer, "ID", 2) == 0) continue;
			if (strlen(buffer) < 2) continue; // skip blank lines

			// Parse record line: ID, Name, Programme, Mark
			if (sscanf(buffer, "%d %[^\t] %[^\t] %lf", &id, name, program, &marks) == 4) {
				RecordPtr node = load_Record(id, name, program, marks);
				if (node) {
					if (*head == NULL) {
						*head = node; // Assigns node as the head
					}
					else { // if head is not empty loop through the list until you reach the tail and insert the node
						RecordPtr current = *head;
						while (current->next != NULL) {
							current = current->next;
						}
						current->next = node;
					}
				}
			}
		}

		fclose(fh_output);
		return 1; // success
	}
}
//-------------------------------------------------------------------//
// SAVE Function
int Save_File(const char* file, RecordPtr head) {
	FILE* fh_output;
	fh_output = fopen(file, "w");
	if (fh_output == NULL) {
		return -1; // failed
	}
	else {
		fprintf(fh_output, "Database: Student-Records-CMS\n");
		fprintf(fh_output, "Authors: Cyrus Del Carmen,\n"
							"Nasyirah Binte Mohd Shariff,\n"
							"Siew Wee Kiam Eugene,\n"
							"Lim Jun Wei,\n"
							"Muhamad Akid Qusyairi Bin Muhamad Riduan\n\n"
		);
		fprintf(fh_output, "Table: StudentRecords\n");
		fprintf(fh_output, "%s\t%s\t\t\t%s\t\t%s\n", "ID", "Name", "Programme", "Mark");

		RecordPtr current = head;
		while (current != NULL) {
			fprintf(fh_output, "%d\t%s\t\t%s\t%.1f\n",
				current->id,
				current->name,
				current->program,
				current->marks);
			current = current->next;
		}

		fclose(fh_output);
		return 1; // success

	}
}
//-------------------------------------------------------------------//
// SHOWALL Function
void Show_All(RecordPtr head) {
	RecordPtr current = head;
	if (current == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
		header_output();
		while (current != NULL) {
			// Print using fixed column widths
			printf("%-8d  %-20s  %-25s  %5.1f\n",
				current->id,
				current->name,
				current->program,
				current->marks);
			current = current->next;  // move to next node
		}
	}
}
//-------------------------------------------------------------------//
// INSERT Function
void Insert_Data(RecordPtr* head, int New_ID, const char* Student_Name, const char* New_Program, double New_Marks) {
	if (check_ID(*head, New_ID) == 1) {
		printf("CMS: Insert failed. Duplicate ID=%d.\n", New_ID);
		return;
	}
	*head = Insert_Tail(*head, New_ID, Student_Name, New_Program, New_Marks);
	printf("CMS: A new record with ID=%d is successfully inserted.\n", New_ID);

}
int check_ID(RecordPtr head, int New_ID) {
	RecordPtr current = head;
	while (current != NULL) {
		if (current->id == New_ID) {
			return 1; // found
		}
		current = current->next;
	}
	return 0; // not found
}
//-------------------------------------------------------------------//
// QUERY Function
void Search_id(RecordPtr head, int Student_ID) {
	RecordPtr found = Search_Record(head, Student_ID);
	if (found != NULL) {
		printf("CMS: The record with ID=%d is found in the data table.\n", Student_ID);
		printf("%-8s  %-20s  %-25s  %5s\n", "ID", "Name", "Programme", "Mark");
		printf("%-8d  %-20s  %-25s  %5.1f\n",
			found->id,
			found->name,
			found->program,
			found->marks);
	}
	else {
		printf("CMS: The record with ID=%d does not exist.\n", Student_ID);
	}
}
//-------------------------------------------------------------------//
// UPDATE helper: parse update args
static int Parse_Update_Args(const char* args, int* Output_ID, int* Output_Mark, double* New_Mark, int* Output_Program, char* New_Program, size_t Program_size) {
	const char* Start_Str;
	const char* End_Str = NULL;
	*Output_Mark = 0;
	*Output_Program = 0;

	// Must have input ID
	Start_Str = strstr(args, "ID=");
	if (!Start_Str || sscanf(Start_Str, "ID=%d", Output_ID) != 1) {
		return 0;
	}

	// Marks are optional
	Start_Str = strstr(args, "Mark=");
	if (Start_Str) {
		if (sscanf(Start_Str, "Mark=%lf", New_Mark) == 1) {
			*Output_Mark = 1;
		}
	}

	// Look for Programme input. can accept quoted or unquoted value
	Start_Str = strstr(args, "Programme=");
	if (Start_Str) {
		Start_Str += strlen("Programme=");
		// skip leading spaces
		while (*Start_Str && isspace((unsigned char)*Start_Str)) {
			Start_Str++;
		}
		size_t len = 0;
		// continue to read until the next field or end of the string
		End_Str = strstr(Start_Str, "Mark=");
		len = End_Str ? (size_t)(End_Str - Start_Str) : strlen(Start_Str);

		// trim trailing spaces and copy into new program
		while (len > 0 && isspace((unsigned char)Start_Str[len - 1])) {
			len--;
		}
		if (len >= Program_size) len = Program_size - 1;
		if (len > 0) {
			memcpy(New_Program, Start_Str, len);
		}
		New_Program[len] = '\0';
		*Output_Program = 1;
	}

	// At least one of mark or programme must be present for a valid update
	if (!*Output_Mark && !*Output_Program) return 0;

	return 1;
}

// UPDATE Function
void Update_New(RecordPtr* head, const char* args) {

	if (!Parse_Update_Args(args, &id, &Output_Mark, &marks, &Output_Program, program, sizeof(program))) {
		printf("CMS: Invalid UPDATE format. Use: UPDATE ID=<id> Mark=<marks> or Programme=<programme>\n");
		return;
	}

	// If programme requested, update it first
	if (Output_Program) {
		*head = Update_Node_Program(*head, id, program);
	}

	// If mark requested, update it
	if (Output_Mark) {
		*head = Update_Node_Marks(*head, id, marks);
	}
}
//-------------------------------------------------------------------//
// DELETE Function
void Delete_Record(RecordPtr* head, int Student_ID) {
	// Use Search_Record to check existence
	RecordPtr found = Search_Record(*head, Student_ID);
	if (found == NULL) {
		printf("CMS: The record with ID=%d does not exist.\n", Student_ID);
		return;
	}
	printf("CMS: Are you sure you want to delete record with ID=%d? Type Y to confirm or N to cancel: ", Student_ID);
	fgets(User_Ans, sizeof(User_Ans), stdin);
	User_Ans[strcspn(User_Ans, "\n")] = '\0'; // Remove trailing newline if present
	if (strcmp(User_Ans, "Y") == 0 || strcmp(User_Ans, "y") == 0) {
		*head = Delete_Node(*head, Student_ID);  // call delete node function
	}
	else if (strcmp(User_Ans, "N") == 0 || strcmp(User_Ans, "n") == 0) {
		printf("CMS: The deletion is cancelled.\n");
	}
	else {
		printf("CMS: Invalid input. Deletion cancelled.\n");
	}
}
//-------------------------------------------------------------------//

