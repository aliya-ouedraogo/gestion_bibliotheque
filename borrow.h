#ifndef BORROW_H
#define BORROW_H

#include <time.h>

#define MAX_NAME 50

typedef struct {
    int book_id;
    int student_id;
    char student_firstname[MAX_NAME];
    char student_lastname[MAX_NAME];
    time_t borrow_date;
    time_t return_date;
    int returned;
} BorrowRecord;

typedef struct BorrowNode {
    BorrowRecord record;
    struct BorrowNode* next;
} BorrowNode;

// Prototypes
BorrowNode* create_borrow_node(BorrowRecord record);
BorrowNode* borrow_book(BorrowNode* head, BorrowRecord record);
int return_book(BorrowNode* head, int book_id, int student_id);
void display_all_borrows(BorrowNode* head);
void display_borrow_record(BorrowRecord* record);
void display_student_borrows(BorrowNode* head, int student_id);
void display_overdue_books(BorrowNode* head, time_t deadline);
void free_borrow_list(BorrowNode* head);

#endif