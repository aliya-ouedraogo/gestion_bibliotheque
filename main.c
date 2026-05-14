#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
#ifndef _WIN32
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif
#include "book.h"
#include "borrow.h"
#include "file_io.h"
#include "date_utils.h"

#define MAX_TITLE 100
#define MAX_NAME 50

void display_menu() {
    printf("\n=== SYSTEME DE GESTION DE BIBLIOTHEQUE ===\n");
    printf("1. Ajouter un nouveau livre\n");
    printf("2. Afficher tous les livres\n");
    printf("3. Rechercher un livre par numero\n");
    printf("4. Rechercher un livre par titre\n");
    printf("5. Verifier disponibilite d'un livre\n");
    printf("6. Afficher livres par ordre alphabetique\n");
    printf("7. Emprunter un livre\n");
    printf("8. Afficher tous les emprunts\n");
    printf("9. Retourner un livre\n");
    printf("10. Afficher emprunts d'un etudiant\n");
    printf("11. Afficher livres non retournes a une date\n");
    printf("12. Sauvegarder les donnees\n");
    printf("13. Charger les donnees\n");
    printf("14. Ajouter livres de test\n");
    printf("15. Afficher horloge en temps reel (fr)\n");
    printf("0. Quitter\n");
    printf("==========================================\n");
    printf("Votre choix: ");
}

Book input_book() {
    Book book;

    printf("\n--- AJOUT D'UN NOUVEAU LIVRE ---\n");
    printf("Numero du livre: ");
    scanf("%d", &book.id);
    getchar();

    printf("Titre: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = 0;

    printf("Prenom auteur: ");
    fgets(book.author_firstname, MAX_NAME, stdin);
    book.author_firstname[strcspn(book.author_firstname, "\n")] = 0;

    printf("Nom auteur: ");
    fgets(book.author_lastname, MAX_NAME, stdin);
    book.author_lastname[strcspn(book.author_lastname, "\n")] = 0;

    printf("Annee d'edition: ");
    scanf("%d", &book.year);

    printf("Nombre d'exemplaires: ");
    scanf("%d", &book.copies);

    book.available_copies = book.copies;

    return book;
}

BorrowRecord input_borrow() {
    BorrowRecord record;

    printf("\n--- NOUVEL EMPRUNT ---\n");
    printf("Numero du livre: ");
    scanf("%d", &record.book_id);

    printf("Numero etudiant: ");
    scanf("%d", &record.student_id);
    getchar();

    printf("Prenom etudiant: ");
    fgets(record.student_firstname, MAX_NAME, stdin);
    record.student_firstname[strcspn(record.student_firstname, "\n")] = 0;

    printf("Nom etudiant: ");
    fgets(record.student_lastname, MAX_NAME, stdin);
    record.student_lastname[strcspn(record.student_lastname, "\n")] = 0;

    record.borrow_date = time(NULL);

    int days;
    printf("Duree d'emprunt (en jours): ");
    scanf("%d", &days);

    record.return_date = record.borrow_date + (days * 24 * 3600);
    record.returned = 0;

    return record;
}

void add_test_books(BookNode** root) {
    Book test_books[] = {
        {101, "Algorithmes en C", "Thomas", "Cormen", 2020, 5, 5},
        {102, "Structures de donnees", "Mark", "Allen", 2019, 3, 3},
        {103, "Programmation C", "Brian", "Kernighan", 2018, 4, 4},
        {104, "Systemes d'exploitation", "Abraham", "Silberschatz", 2021, 2, 2},
        {105, "Bases de donnees", "Ramez", "Elmasri", 2017, 6, 6}
    };

    int i;
    for (i = 0; i < 5; i++) {
        *root = insert_book(*root, test_books[i]);
    }

    printf("5 livres de test ajoutes avec succes!\n");
}

/* Portable kbhit implementation for POSIX (Windows uses conio.h _kbhit()) */
#ifndef _WIN32
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

/* Show real-time clock formatted in French until a key is pressed */
void show_realtime_clock(void) {
    const char *locales[] = {"fr_FR.UTF-8", "fr_FR", "French_France.1252", "French", NULL};
    const char **p = locales;
    char buf[128];
    time_t t;
    struct tm tmv;

    /* Try to set a French locale for time formatting */
    while (*p) {
        if (setlocale(LC_TIME, *p)) break;
        p++;
    }

    printf("Appuyez sur une touche pour quitter l'horloge...\n");

    while (1) {
        t = time(NULL);
        /* Use gmtime for UTC (GMT) display; locale still controls month/day names */
        struct tm *res = gmtime(&t);
        if (res) tmv = *res; else memset(&tmv, 0, sizeof(tmv));

        if (strftime(buf, sizeof(buf), "%A %d %B %Y %H:%M:%S", &tmv) == 0) {
            snprintf(buf, sizeof(buf), "Erreur format date");
        }

        printf("\r%s", buf);
        fflush(stdout);

#ifdef _WIN32
        Sleep(1000);
        if (_kbhit()) { int c = _getch(); (void)c; break; }
#else
        sleep(1);
        if (kbhit()) { int c = getchar(); (void)c; break; }
#endif
    }

    printf("\n");
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
    setlocale(LC_ALL, "");
    BookNode* book_tree = NULL;
    BorrowNode* borrow_list = NULL;
    int choice;

    printf("=== SYSTEME DE GESTION DE BIBLIOTHEQUE UNIVERSITAIRE ===\n");
    printf("Chargement des donnees...\n");

    book_tree = load_books_from_file("books.dat");
    borrow_list = load_borrows_from_file("borrows.dat");

    if (book_tree == NULL) {
        printf("Aucune donnee de livres trouvee.\n");
    } else {
        printf("Livres charges: %d\n", count_books(book_tree));
    }

    do {
        display_menu();
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                Book book = input_book();
                book_tree = insert_book(book_tree, book);
                printf("Livre ajoute avec succes!\n");
                break;
            }

            case 2:
                printf("\n=== LISTE DES LIVRES ===\n");
                if (book_tree == NULL) {
                    printf("Aucun livre dans la bibliotheque.\n");
                } else {
                    display_all_books(book_tree);
                }
                break;

            case 3: {
                int id;
                printf("Numero du livre: ");
                scanf("%d", &id);
                BookNode* result = search_book_by_id(book_tree, id);
                if (result != NULL) {
                    display_book(&result->book);
                } else {
                    printf("Livre non trouve!\n");
                }
                break;
            }

            case 4: {
                char title[MAX_TITLE];
                getchar();
                printf("Titre du livre: ");
                fgets(title, MAX_TITLE, stdin);
                title[strcspn(title, "\n")] = 0;
                BookNode* result = search_book_by_title(book_tree, title);
                if (result != NULL) {
                    display_book(&result->book);
                } else {
                    printf("Livre non trouve!\n");
                }
                break;
            }

            case 5: {
                int id;
                printf("Numero du livre: ");
                scanf("%d", &id);
                if (is_book_available(book_tree, id)) {
                    printf("Le livre est disponible!\n");
                } else {
                    printf("Le livre n'est pas disponible!\n");
                }
                break;
            }

            case 6:
                printf("\n=== LIVRES PAR ORDRE ALPHABETIQUE ===\n");
                display_books_alphabetical(book_tree);
                break;

            case 7: {
                if (book_tree == NULL) {
                    printf("Aucun livre dans la bibliotheque!\n");
                    break;
                }

                BorrowRecord record = input_borrow();

                BookNode* book_node = search_book_by_id(book_tree, record.book_id);
                if (book_node == NULL) {
                    printf("Livre non trouve!\n");
                    break;
                }

                if (book_node->book.available_copies <= 0) {
                    printf("Aucun exemplaire disponible!\n");
                    break;
                }

                borrow_list = borrow_book(borrow_list, record);
                book_node->book.available_copies--;
                printf("Emprunt enregistre avec succes!\n");
                break;
            }

            case 8:
                printf("\n=== LISTE DES EMPRUNTS ===\n");
                display_all_borrows(borrow_list);
                break;

            case 9: {
                int book_id, student_id;
                printf("Numero du livre: ");
                scanf("%d", &book_id);
                printf("Numero de l'etudiant: ");
                scanf("%d", &student_id);

                if (return_book(borrow_list, book_id, student_id)) {
                    BookNode* book_node = search_book_by_id(book_tree, book_id);
                    if (book_node != NULL) {
                        book_node->book.available_copies++;
                    }
                    printf("Livre retourne avec succes!\n");
                } else {
                    printf("Emprunt non trouve ou deja retourne!\n");
                }
                break;
            }

            case 10: {
                int student_id;
                printf("Numero de l'etudiant: ");
                scanf("%d", &student_id);
                printf("\n=== EMPRUNTS DE L'ETUDIANT %d ===\n", student_id);
                display_student_borrows(borrow_list, student_id);
                break;
            }

            case 11: {
                int day, month, year;
                printf("Entrez une date (JJ MM AAAA): ");
                scanf("%d %d %d", &day, &month, &year);
                time_t deadline = create_date(day, month, year);
                printf("\n=== LIVRES NON RETOURNES ===\n");
                display_overdue_books(borrow_list, deadline);
                break;
            }

            case 12:
                save_books_to_file(book_tree, "books.dat");
                save_borrows_to_file(borrow_list, "borrows.dat");
                printf("Donnees sauvegardees avec succes!\n");
                break;

            case 13:
                free_book_tree(book_tree);
                free_borrow_list(borrow_list);
                book_tree = load_books_from_file("books.dat");
                borrow_list = load_borrows_from_file("borrows.dat");
                printf("Donnees chargees avec succes!\n");
                if (book_tree != NULL) {
                    printf("Livres charges: %d\n", count_books(book_tree));
                }
                break;

            case 14:
                add_test_books(&book_tree);
                break;

            case 15:
                show_realtime_clock();
                break;

            case 0:
                printf("Sauvegarde automatique...\n");
                save_books_to_file(book_tree, "books.dat");
                save_borrows_to_file(borrow_list, "borrows.dat");
                printf("Au revoir!\n");
                break;

            default:
                printf("Choix invalide! Veuillez reessayer.\n");
        }

        printf("\nAppuyez sur Entree pour continuer...");
        getchar();
        getchar();

    } while(choice != 0);

    free_book_tree(book_tree);
    free_borrow_list(borrow_list);

    return 0;
}
