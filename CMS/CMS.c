
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

// Headers for output
void header_output() {
	printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
	printf("ID        Name                Programme               Mark\n");
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
			printf("%-8d  %-18s  %-22s  %.1f\n",
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

//-------------------------------------------------------------------//
// QUERY Function
void Search_id(RecordPtr head, int *Student_ID) {
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
		printf("CMS: The record with ID=%d does not exist..\n", Student_ID);
	}
}
//-------------------------------------------------------------------//
// UPDATE Function
//-------------------------------------------------------------------//
// DELETE Function