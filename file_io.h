#ifndef FILE_IO_H
#define FILE_IO_H

#include "book.h"
#include "borrow.h"

// Prototypes
void save_books_to_file(BookNode* root, const char* filename);
BookNode* load_books_from_file(const char* filename);
void save_borrows_to_file(BorrowNode* head, const char* filename);
BorrowNode* load_borrows_from_file(const char* filename);

#endif