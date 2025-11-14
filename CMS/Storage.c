
// This file will handle all the database functions and allocate memory

// Include libraries
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CMS.h"
//-----------------------------------------------------------------------//
// Load Records
RecordPtr load_Record(int id, const char* name, const char* program, double marks) {
    RecordPtr newNode = malloc(sizeof(Student_records));
    newNode->id = id;
    // creates a deep copy of the node if not done the output will be garbage
    newNode->name = malloc(strlen(name) + 1);
    strcpy(newNode->name, name);
    newNode->program = malloc(strlen(program) + 1);
    strcpy(newNode->program, program);
    // --------------------------------------------------------//
    newNode->marks = marks;
    newNode->next = NULL;
    return newNode;
}
//-----------------------------------------------------------------------//
// Clear the list
void Clear_List(RecordPtr head) {
    RecordPtr temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->name);
        free(temp->program);
        free(temp);
    }
}
//-----------------------------------------------------------------------//
// Search For student ID
RecordPtr Search_Record(RecordPtr head, int *Student_ID) {
    RecordPtr Current = head;

    while (Current != NULL){
        if (Current->id == Student_ID) {
            return Current;
        }
        Current = Current->next;
    }
    return NULL;
}
//-----------------------------------------------------------------------//