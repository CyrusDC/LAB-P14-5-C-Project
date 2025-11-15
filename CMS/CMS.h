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
RecordPtr load_Record(int id, const char* name, const char* program, double marks); // Open file and load data into linked list
RecordPtr Search_Record(RecordPtr head, int Student_Id); // Looks through the linked list
RecordPtr Insert_Tail(RecordPtr head, int New_ID, const char* Student_Name, const char* New_Program, double New_Marks); // Insert a new node at the tail of the list
RecordPtr Delete_Node(RecordPtr head, int Target_ID); // Delete a node based on ID number
RecordPtr Update_Node_Program(RecordPtr head, int ID, const char* New_Program);
RecordPtr Update_Node_Marks(RecordPtr head, int ID, double New_Marks);

//-------------------------------------------------------------------//
// CMS.c Functions
int Open_File(const char* file, RecordPtr* head); // Open File function
void Clear_List(RecordPtr head); // clear the list
void Show_All(RecordPtr head); // Show All function
void Search_id(RecordPtr head, int Student_ID); // Query Function
void Insert_Data(RecordPtr head, int New_ID, const char* Student_Name, const char* New_Program, double New_Marks); // Insert Function
int check_ID(RecordPtr head, int* New_ID);// Check if ID exists
void Delete_Record(RecordPtr head, int* Student_ID); // Delete Function
int Save_File(const char* file, RecordPtr* head);
void Update_New(RecordPtr* head, const char* args);

int Count_Nodes(RecordPtr head);
#endif