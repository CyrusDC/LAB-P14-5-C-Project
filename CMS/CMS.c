// This file will handle all the functions such as OPEN, SAVE, SHOW ALL, SUMMARY, INSERT, QUERY, UPDATE and DELETE

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
static void header_output() {
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
	if (head == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return -1; // failed
	}
	else {
		FILE* fh_output;
		fh_output = fopen(file, "w");
		if (fh_output == NULL) {
			printf("CMS: Unable to open file %s.\n", file);
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
// SORTING FUNCTION
// Sort by ID function
void Sort_ID(RecordPtr head) {
	RecordPtr current = head;
	RecordPtr Last = NULL;
	int temp_ID = 0;
	char* temp_Name = NULL;
	char* temp_Program = NULL;
	double temp_Marks = 0.0;
	int swapping = 0;

	if (current == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
		do {
			swapping = 0;
			current = head;
			while (current != NULL && current->next != Last) {
				if (current->id > current->next->id) {
					// swap the ID
					temp_ID = current->id;
					current->id = current->next->id;
					current->next->id = temp_ID;

					// swap the name
					temp_Name = current->name;
					current->name = current->next->name;
					current->next->name = temp_Name;

					// swap the program
					temp_Program = current->program;
					current->program = current->next->program;
					current->next->program = temp_Program;

					// swap the marks
					temp_Marks = current->marks;
					current->marks = current->next->marks;
					current->next->marks = temp_Marks;

					swapping = 1;
				}
				current = current->next;  // move to next node
			}
			Last = current;
		} while (swapping);
		Show_All(head);
	}
}
// Sort by marks function same as above but compares marks instead of ID
void Sort_Marks(RecordPtr head) {
	RecordPtr current = head;
	RecordPtr Last = NULL;
	int temp_ID = 0;
	char* temp_Name = NULL;
	char* temp_Program = NULL;
	double temp_Marks = 0.0;
	int swapping = 0;

	if (current == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
		do {
			swapping = 0;
			current = head;
			while (current != NULL && current->next != Last) {
				if (current->marks > current->next->marks) {
					// swap the ID
					temp_ID = current->id;
					current->id = current->next->id;
					current->next->id = temp_ID;

					// swap the name
					temp_Name = current->name;
					current->name = current->next->name;
					current->next->name = temp_Name;

					// swap the program
					temp_Program = current->program;
					current->program = current->next->program;
					current->next->program = temp_Program;

					// swap the marks
					temp_Marks = current->marks;
					current->marks = current->next->marks;
					current->next->marks = temp_Marks;

					swapping = 1;
				}
				current = current->next;  // move to next node
			}
			Last = current;
		} while (swapping);
		Show_All(head);
	}
}
//-------------------------------------------------------------------//
// Show Summary Function
void Show_Summary(RecordPtr head) {
	RecordPtr current = head;
	if (current == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
		RecordPtr lowest = Lowest_Record(head);
		RecordPtr highest = Highest_Record(head);
		int count = 0;
		double total_marks = 0.0;
		double average_marks = 0.0;
		while (current != NULL) {
			count++;
			total_marks += current->marks;
			current = current->next;  // move to next node
		}
		average_marks = total_marks / count;
		printf("CMS: Summary of Student Records:\n");
		printf("Total number of records: %d\n", count);
		printf("Average marks: %.2f\n", average_marks);
		printf("Highest marks: %s %.2f\n", highest->name,highest->marks);
		printf("Lowest marks: %s %.2f\n", lowest->name, lowest->marks);
	}
}
//-------------------------------------------------------------------//
// INSERT Function
// Function to parse insert command args
int Parse_Insert_Args(const char* args, int* Output_ID, char* Output_Name, size_t Name_Size, char* Output_Program, size_t Program_Size, double* Output_Mark)
{
	const char* start;
	const char* end;
	size_t length;

	// Check for valid id
	start = strstr(args, "ID=");
	if (start == NULL || sscanf(start, "ID=%d", Output_ID) != 1) {
		return 0;   // couldn't find ID or invalid number
	}

	// Check for name
	start = strstr(args, "Name=");
	end = strstr(args, "Programme=");

	if (start == NULL || end == NULL || end <= start) {
		return 0;   // wrong format or missing fields
	}

	start += strlen("Name=");
	length = (size_t)(end - start);

	// trim leading spaces
	while (length > 0 && isspace((unsigned char)*start)) {
		start++;
		length--;
	}

	// trim trailing spaces
	while (length > 0 && isspace((unsigned char)start[length - 1])) {
		length--;
	}

	if (length >= Name_Size) {
		length = Name_Size - 1;
	}

	strncpy(Output_Name, start, length);
	Output_Name[length] = '\0';

	// Reject quoted input for Name
	if (strchr(Output_Name, '"') != NULL || strchr(Output_Name, '\'') != NULL) {
		return 0;   // invalid: quotes not allowed
	}

	// Check for Programme
	start = strstr(args, "Programme=");
	end = strstr(args, "Mark=");

	if (start == NULL || end == NULL || end <= start) {
		return 0;
	}

	start += strlen("Programme=");
	length = (size_t)(end - start);

	// trim leading spaces
	while (length > 0 && isspace((unsigned char)*start)) {
		start++;
		length--;
	}

	// trim trailing spaces
	while (length > 0 && isspace((unsigned char)start[length - 1])) {
		length--;
	}

	if (length >= Program_Size) {
		length = Program_Size - 1;
	}

	strncpy(Output_Program, start, length);
	Output_Program[length] = '\0';

	// Reject quoted input for Programme
	if (strchr(Output_Program, '"') != NULL || strchr(Output_Program, '\'') != NULL) {
		return 0;   // invalid: quotes not allowed
	}


	// Check for marks
	start = strstr(args, "Mark=");
	if (start == NULL || sscanf(start, "Mark=%lf", Output_Mark) != 1) {
		return 0;
	}

	return 1;   // successfully parsed all fields
}

void Insert_Data(RecordPtr* head, int New_ID, const char* Student_Name, const char* New_Program, double New_Marks) {
	if (head == NULL || *head == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
		if (check_ID(*head, New_ID) == 1) {
			printf("CMS: Insert failed. Duplicate ID=%d.\n", New_ID);
			return;
		}
		else {
			*head = Insert_Tail(*head, New_ID, Student_Name, New_Program, New_Marks);
			printf("CMS: A new record with ID=%d is successfully inserted.\n", New_ID);
		}
	}
}
int check_ID(RecordPtr head, int New_ID) {
	RecordPtr current = Search_Record(head, New_ID);
	if (current != NULL) {
		return 1; // found
	}
	return 0; // not found
}
//-------------------------------------------------------------------//
// QUERY Function
void Search_id(RecordPtr head, int Student_ID) {
	if(head == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
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
}
//-------------------------------------------------------------------//
// UPDATE helper: parse update args
static int Parse_Update_Args(const char* args, int* Output_ID, int* Output_Mark, double* New_Mark, int* Output_Program, char* New_Program, size_t Program_size) {
	const char* Start_Str;
	//const char* End_Str = NULL;
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

	// Look for Programme input.
	Start_Str = strstr(args, "Programme=");
	if (Start_Str) {
		Start_Str += strlen("Programme=");
		// skip leading spaces
		while (*Start_Str && isspace((unsigned char)*Start_Str)) {
			Start_Str++;
		}

		// If the value is quoted, skip parsing (we only want unquoted values)
		if (*Start_Str == '"') {
			return -1;// Do not set Output_Program; quoted values are ignored by this parser.
		}
		else {
			// unquoted: read up to next field marker ("Mark=") or end
			const char* End_Str = strstr(Start_Str, "Mark=");
			size_t len = End_Str ? (size_t)(End_Str - Start_Str) : strlen(Start_Str);

			// trim trailing spaces
			while (len > 0 && isspace((unsigned char)Start_Str[len - 1])) len--;

			if (len >= Program_size) len = Program_size - 1;
			if (len > 0) {
				memcpy(New_Program, Start_Str, len);
			}
			New_Program[len] = '\0';
			*Output_Program = 1;
		}
	}

	// At least one of mark or programme must be present for a valid update
	if (!*Output_Mark && !*Output_Program) return 0;

	return 1;
}

// UPDATE Function
void Update_New(RecordPtr* head, const char* args) {

	if(head == NULL || *head == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else {
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
}
//-------------------------------------------------------------------//
// DELETE Function
void Delete_Record(RecordPtr* head, int Student_ID) {
	if (head == NULL || *head == NULL) {
		printf("CMS: No records found. Please Open a file first.\n");
		return;
	}
	else
	{
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
}
//-------------------------------------------------------------------//
