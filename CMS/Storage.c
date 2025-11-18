
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

    newNode->marks = marks;
    newNode->next = NULL;
    return newNode;
}
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
RecordPtr Search_Record(RecordPtr head, int Student_ID) {
    RecordPtr Current = head;
    while (Current != NULL) {
        if (Current->id == Student_ID) {
            return Current;
        }
        Current = Current->next;
    }
    return NULL;
}
//-----------------------------------------------------------------------//
// Insert a new node at the tail of the list
RecordPtr Insert_Tail(RecordPtr head, int New_ID, const char* Student_Name, const char* New_Program, double New_Marks) {
    RecordPtr newNode = load_Record(New_ID, Student_Name, New_Program, New_Marks);
    if (head == NULL) {
        return newNode; // If the list is empty, return the new node as the head
    }
    RecordPtr current = head;
    while (current->next != NULL) {
        current = current->next; // Traverse to the last node
    }
    current->next = newNode; // Link the new node at the end
    return head; // Return the head of the list
}
//-----------------------------------------------------------------------//
// Update a record based on ID number
// Update marks of a student
RecordPtr Update_Node_Marks(RecordPtr head, int ID, double New_Marks) {
    RecordPtr Current = head;
    while (Current != NULL) {
        if (Current->id == ID) {
            Current->marks = New_Marks;
            printf("CMS: The record with ID=%d is successfully updated.\n", ID);
            return head;
        }
        Current = Current->next;
    }
    printf("CMS: The record with ID=%d does not exist.\n", ID);
    return head;
}
// Update program of a student
RecordPtr Update_Node_Program(RecordPtr head, int ID, const char* New_Program) {
    RecordPtr Current = head;
    while (Current != NULL) {
        if (Current->id == ID) {
            free(Current->program);
            Current->program = malloc(strlen(New_Program) + 1);
            strcpy(Current->program, New_Program);  // copy contents
            printf("CMS: The record with ID=%d is successfully updated.\n", ID);
            return head;
        }
        Current = Current->next;
    }
    printf("CMS: The record with ID=%d does not exist.\n", ID);
    return head;
}
//-----------------------------------------------------------------------//
// Delete Record based on student ID
RecordPtr Delete_Node(RecordPtr head, int Target_ID) {
    RecordPtr current = head;
    RecordPtr prev = NULL;
    while (current != NULL) {
        if (current->id == Target_ID) {
            // If deleting the head node
            if (prev == NULL) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }

            // Free memory for strings and node
            free(current->name);
            free(current->program);
            free(current);
            printf("CMS: Record with ID=%d deleted successfully.\n", Target_ID);
            return head;
        }
        prev = current;
        current = current->next;
    }

}
//-----------------------------------------------------------------------//
