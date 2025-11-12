#ifndef CMS_H
#define CMS_H

// Linked list student_records
typedef struct Student_records { 
	int id;
	char* name;
	char* program;
	double marks;
	struct Student_records* next;
} Student_records;

typedef Student_records* RecordPtr;

//--------------------------------------------------------------------//
// Storage.c functions
RecordPtr load_Record(int id, const char* name, const char* program, double marks);



//-------------------------------------------------------------------//
// CMS.c Functions
int Open_File(const char* file, RecordPtr* head); // Open File function
void Show_All(RecordPtr head); // Show All function

#endif