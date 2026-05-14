#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "book.h"

// Crée un nouveau nœud
BookNode* create_book_node(Book book) {
    BookNode* node = (BookNode*)malloc(sizeof(BookNode));
    if (node == NULL) {
        printf("Erreur d'allocation memoire!\n");
        exit(1);
    }
    node->book = book;
    node->left = node->right = NULL;
    return node;
}

// Insère un livre dans l'arbre (FONCTION CRITIQUE)
BookNode* insert_book(BookNode* root, Book book) {
    if (root == NULL) {
        return create_book_node(book);
    }

    if (book.id < root->book.id) {
        root->left = insert_book(root->left, book);
    } else if (book.id > root->book.id) {
        root->right = insert_book(root->right, book);
    }

    return root;
}

// Recherche par ID
BookNode* search_book_by_id(BookNode* root, int id) {
    if (root == NULL || root->book.id == id) {
        return root;
    }

    if (id < root->book.id) {
        return search_book_by_id(root->left, id);
    }

    return search_book_by_id(root->right, id);
}

// Compte les livres
int count_books(BookNode* root) {
    if (root == NULL) return 0;
    return 1 + count_books(root->left) + count_books(root->right);
}



// Affiche tous les livres
void display_all_books(BookNode* root) {
    if (root != NULL) {
        display_all_books(root->left);
        display_book(&root->book);
        display_all_books(root->right);
    }
}

// Affiche un livre
void display_book(Book* book) {
    printf("\n=================================\n");
    printf("Numero: %d\n", book->id);
    printf("Titre: %s\n", book->title);
    printf("Auteur: %s %s\n", book->author_firstname, book->author_lastname);
    printf("Annee: %d\n", book->year);
    printf("Exemplaires: %d/%d disponibles\n", book->available_copies, book->copies);
    printf("=================================\n");
}

// Vérifie disponibilité
int is_book_available(BookNode* root, int id) {
    BookNode* node = search_book_by_id(root, id);
    if (node == NULL) return 0;
    return node->book.available_copies > 0;
}

// Libère la mémoire
void free_book_tree(BookNode* root) {
    if (root != NULL) {
        free_book_tree(root->left);
        free_book_tree(root->right);
        free(root);
    }
}

// Fonctions pour l'ordre alphabétique
void display_books_alphabetical_helper(BookNode* root, Book* books[], int* index) {
    if (root != NULL) {
        display_books_alphabetical_helper(root->left, books, index);
        books[*index] = &root->book;
        (*index)++;
        display_books_alphabetical_helper(root->right, books, index);
    }
}

int compare_books_by_title(const void* a, const void* b) {
    Book* bookA = *(Book**)a;
    Book* bookB = *(Book**)b;
    const char *s1 = bookA->title;
    const char *s2 = bookB->title;
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++; s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

void display_books_alphabetical(BookNode* root) {
    int total = count_books(root);
    if (total == 0) {
        printf("Aucun livre dans la bibliotheque.\n");
        return;
    }
    Book** books = (Book**)malloc(total * sizeof(Book*));
    if (books == NULL) {
        printf("Erreur d'allocation memoire!\n");
        return;
    }
    int index = 0;
    display_books_alphabetical_helper(root, books, &index);

    qsort(books, total, sizeof(Book*), compare_books_by_title);

    printf("\n=== LIVRES PAR ORDRE ALPHABETIQUE ===\n");
    int i;
    for (i = 0; i < total; i++) {
        printf("%d. %s (par %s %s)\n",
               i+1,
               books[i]->title,
               books[i]->author_firstname,
               books[i]->author_lastname);
    }
    free(books);
}

// Recherche d'un livre par titre (insensible à la casse)
BookNode* search_book_by_title(BookNode* root, const char* title) {
    if (root == NULL) return NULL;

    // Comparaison insensible à la casse
    const char *s1 = title;
    const char *s2 = root->book.title;
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) break;
        s1++; s2++;
    }
    if (tolower((unsigned char)*s1) == tolower((unsigned char)*s2)) {
        return root;
    }

    BookNode* left = search_book_by_title(root->left, title);
    if (left) return left;
    return search_book_by_title(root->right, title);
}
