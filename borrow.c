#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "borrow.h"

BorrowNode* create_borrow_node(BorrowRecord record) {
    BorrowNode* node = (BorrowNode*)malloc(sizeof(BorrowNode));
    if (node == NULL) {
        printf("Erreur d'allocation memoire!\n");
        exit(1);
    }
    node->record = record;
    node->next = NULL;
    return node;
}

BorrowNode* borrow_book(BorrowNode* head, BorrowRecord record) {
    BorrowNode* new_node = create_borrow_node(record);
    new_node->next = head;
    return new_node;
}

void display_borrow_record(BorrowRecord* record) {
    printf("\n=================================\n");
    printf("Livre ID: %d\n", record->book_id);
    printf("Etudiant: %s %s (ID: %d)\n", 
           record->student_firstname, 
           record->student_lastname, 
           record->student_id);
    printf("Date emprunt: %s", ctime(&record->borrow_date));
    printf("Date retour prevue: %s", ctime(&record->return_date));
    printf("Statut: %s\n", record->returned ? "Retourne" : "En cours");
    printf("=================================\n");
}

void display_all_borrows(BorrowNode* head) {
    BorrowNode* current = head;
    int count = 0;
    
    while (current != NULL) {
        display_borrow_record(&current->record);
        current = current->next;
        count++;
    }
    
    if (count == 0) {
        printf("Aucun emprunt enregistre.\n");
    }
}

int return_book(BorrowNode* head, int book_id, int student_id) {
    BorrowNode* current = head;
    while (current != NULL) {
        if (current->record.book_id == book_id && 
            current->record.student_id == student_id && 
            !current->record.returned) {
            current->record.returned = 1;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void display_student_borrows(BorrowNode* head, int student_id) {
    BorrowNode* current = head;
    int found = 0;
    
    while (current != NULL) {
        if (current->record.student_id == student_id) {
            display_borrow_record(&current->record);
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("Aucun emprunt trouve pour cet etudiant.\n");
    }
}

void display_overdue_books(BorrowNode* head, time_t deadline) {
    BorrowNode* current = head;
    int found = 0;
    
    printf("Date de reference: %s", ctime(&deadline));
    
    while (current != NULL) {
        if (!current->record.returned && current->record.return_date < deadline) {
            printf("\nLivre ID %d en retard:\n", current->record.book_id);
            printf("Etudiant: %s %s\n", current->record.student_firstname, current->record.student_lastname);
            printf("Date retour prevue: %s", ctime(&current->record.return_date));
            
            double days_late = difftime(deadline, current->record.return_date) / (24 * 3600);
            printf("Retard: %.1f jours\n", days_late);
            
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("Aucun livre non retourne a cette date.\n");
    }
}

void free_borrow_list(BorrowNode* head) {
    BorrowNode* current = head;
    while (current != NULL) {
        BorrowNode* next = current->next;
        free(current);
        current = next;
    }
}