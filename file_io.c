#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"

void save_books_inorder(BookNode* root, FILE* file) {
    if (root == NULL) return;

    save_books_inorder(root->left, file);
    fprintf(file, "%d|%s|%s|%s|%d|%d|%d\n",
            root->book.id,
            root->book.title,
            root->book.author_firstname,
            root->book.author_lastname,
            root->book.year,
            root->book.copies,
            root->book.available_copies);
    save_books_inorder(root->right, file);
}

void save_books_to_file(BookNode* root, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        return;
    }

    save_books_inorder(root, file);
    fclose(file);
    printf("Livres sauvegardés en français dans %s\n", filename);
}

BookNode* load_books_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    BookNode* root = NULL;
    Book book;

    while(fscanf(file, "%d|%99[^|]|%49[^|]|%49[^|]|%d|%d|%d\n",
                 &book.id,
                 book.title,
                 book.author_firstname,
                 book.author_lastname,
                 &book.year,
                 &book.copies,
                 &book.available_copies) == 7) {
        root = insert_book(root, book);
    }

    fclose(file);
    return root;
}

void save_borrows_to_file(BorrowNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        return;
    }

    BorrowNode* current = head;
    while (current != NULL) {
        fprintf(file, "%d|%d|%s|%s|%ld|%ld|%d\n",
                current->record.book_id,
                current->record.student_id,
                current->record.student_firstname,
                current->record.student_lastname,
                current->record.borrow_date,
                current->record.return_date,
                current->record.returned);
        current = current->next;
    }

    fclose(file);
    printf("Emprunts sauvegardés en français dans %s\n", filename);
}

BorrowNode* load_borrows_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    BorrowNode* head = NULL;
    BorrowRecord record;

    while(fscanf(file, "%d|%d|%49[^|]|%49[^|]|%ld|%ld|%d\n",
                 &record.book_id,
                 &record.student_id,
                 record.student_firstname,
                 record.student_lastname,
                 &record.borrow_date,
                 &record.return_date,
                 &record.returned) == 7) {
        head = borrow_book(head, record);
    }

    fclose(file);
    return head;
}
