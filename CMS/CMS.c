
// This file will handle all the functions such as OPEN, SAVE, SHOWALL, INSERT, QUERY, UPDATE and DELETE

// Include libraries
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "CMS.h"

// Global Variables
char buffer[256];
int id;
char name[100];
char program[100];
double marks;
char User_Ans[4];

// Headers for output
void header_output() {
	printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
	printf("ID          Name                  Programme                  Mark\n");
}
//-------------------------------------------------------------------//
// OPEN Function
int Open_File(const char *file, RecordPtr* head) {
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
					if(*head == NULL) {
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
		fprintf(fh_output, "Authors: Cyrus Del Carmen\n\n");
		fprintf(fh_output, "Table: StudentRecords\n");
		fprintf(fh_output, "%s\t%s\t\t\t%s\t\t%s\n","ID", "Name", "Programme", "Mark");

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
			printf("%-10d  %-20s  %-25s  %.1f\n",
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
	printf("A new record with ID=%d is successfully inserted.\n", New_ID);

}
int check_ID(RecordPtr head, int New_ID) {
	RecordPtr current = head;
	while (current != NULL) {
		if (current->id == New_ID) {
			printf("CMS: The record with ID=%d already exists.\n", New_ID);
			return 1;
		}
		current = current->next;
	}
	printf("CMS: The record with ID=%d does not exist.\n", New_ID);
	return -1;
}
//-------------------------------------------------------------------//
// QUERY Function
void Search_id(RecordPtr head, int Student_ID) {
	RecordPtr found = Search_Record(head, Student_ID);
	if (found != NULL) {
		printf("CMS: The record with ID=%d is found in the data table.\n", Student_ID);
		printf("ID        Name                Programme               Mark\n");
		printf("%-8d  %-18s  %-22s  %.1f\n",
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
// UPDATE Function
void Update_New(RecordPtr *head, const char* args) {
	// Case 1: Update marks
	if (sscanf(args, "ID=%d Mark=%lf", &id, &marks) == 2) {
		*head = Update_Node_Marks(*head, id, marks);
		return;
	}

	// Case 2: Update programme
	else if (sscanf(args, "ID=%d Programme=\"%99[^\"]\"", &id, program) == 2) {
		*head = Update_Node_Program(*head, id, program);
		return;
	}

	// Case 3: Invalid format
	else {
		printf("CMS: Invalid UPDATE format. Use: UPDATE ID=<id> Mark=<marks> or Programme=\"<programme>\"\n");
	}

}
//-------------------------------------------------------------------//
// DELETE Function
void Delete_Record(RecordPtr *head, int Student_ID) {
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




int Count_Nodes(RecordPtr head) {
	int count = 0;
	RecordPtr current = head;

	while (current != NULL) {
		count++;
		current = current->next;
	}

	return count;
}
