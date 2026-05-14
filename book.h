#ifndef BOOK_H
#define BOOK_H

#define MAX_TITLE 100
#define MAX_NAME 50

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author_firstname[MAX_NAME];
    char author_lastname[MAX_NAME];
    int year;
    int copies;
    int available_copies;
} Book;

typedef struct BookNode {
    Book book;
    struct BookNode* left;
    struct BookNode* right;
} BookNode;

// Prototypes
BookNode* create_book_node(Book book);
BookNode* insert_book(BookNode* root, Book book);
BookNode* search_book_by_id(BookNode* root, int id);
BookNode* search_book_by_title(BookNode* root, const char* title);
void display_all_books(BookNode* root);
void display_book(Book* book);
void display_books_alphabetical(BookNode* root);
int is_book_available(BookNode* root, int id);
int count_books(BookNode* root);
void free_book_tree(BookNode* root);

#endif