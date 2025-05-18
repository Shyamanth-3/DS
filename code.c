#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data {
    char name[50];
    struct data* next;
} node;

typedef struct Book {
    int ISBN;
    char title[100];
    char author[50];
    int copiesAvailable;
    node* borrowers;
    struct Book* left;
    struct Book* right;
} Book;

// User-defined functions
Book* createBook(int , char* , char* , int );
Book* insertBook(Book* , int , char* , char* , int );
Book* searchBook(Book* , int );
void addBorrower(Book* , char* );
void removeBorrower(Book* , char* );
void borrowBook(Book* , int , char* );
void returnBook(Book* , int , char* );
void displayBorrowers(node* );
void displayBooks(Book* );

//Main Funtion
int main() {
    Book* root = NULL;
    int choice;
    int ISBN;
    char title[100], author[50], borrowerName[50];
    int copies;
    while (1) {
        printf("\n----- Library Management System -----\n");
        printf("1. Insert Book\n");
        printf("2. Search Book\n");
        printf("3. Borrow Book\n");
        printf("4. Return Book\n");
        printf("5. Display All Books\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                printf("Enter ISBN (integer): ");
                scanf("%d", &ISBN);
                getchar();

                printf("Enter Title: ");
                fgets(title, 100, stdin);

                printf("Enter Author: ");
                fgets(author, 50, stdin);

                printf("Enter number of copies: ");
                scanf("%d", &copies);
                getchar();

                root = insertBook(root, ISBN, title, author, copies);
                printf("Book inserted successfully.\n");
                break;

            case 2:
                printf("Enter ISBN to search: ");
                scanf("%d", &ISBN);
                getchar();

                Book* found = searchBook(root, ISBN);
                if (found) {
                    printf("Book Found:\n");
                    printf("ISBN: %d\nTitle: %s\nAuthor: %s\nCopies Available: %d\n", 
                        found->ISBN, found->title, found->author, found->copiesAvailable);
                    displayBorrowers(found->borrowers);
                } else {
                    printf("Book not found.\n");
                }
                break;

            case 3:
                printf("Enter ISBN to borrow: ");
                scanf("%d", &ISBN);
                getchar();

                printf("Enter Borrower Name: ");
                fgets(borrowerName, 50, stdin);

                borrowBook(root, ISBN, borrowerName);
                break;

            case 4:
                printf("Enter ISBN to return: ");
                scanf("%d", &ISBN);
                getchar();

                printf("Enter Borrower Name: ");
                fgets(borrowerName, 50, stdin);
                returnBook(root, ISBN, borrowerName);
                break;

            case 5:
                printf("\nAll Books in Library:\n");
                displayBooks(root);
                break;

            case 6:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

//Create Book
Book* createBook(int ISBN, char* title, char* author, int copies) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->ISBN = ISBN;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->copiesAvailable = copies;
    newBook->borrowers = NULL;
    newBook->left = newBook->right = NULL;
    return newBook;
}

//Insert Book
Book* insertBook(Book* root, int ISBN, char* title, char* author, int copies) {
    if (root == NULL)
        return createBook(ISBN, title, author, copies);

    if (ISBN < root->ISBN)
        root->left = insertBook(root->left, ISBN, title, author, copies);
    else if (ISBN > root->ISBN)
        root->right = insertBook(root->right, ISBN, title, author, copies);
    else
        printf("Book with ISBN %d already exists!\n", ISBN);

    return root;
}

//Search Book
Book* searchBook(Book* root, int ISBN) {
    if (root == NULL) return NULL;

    if (ISBN == root->ISBN)
        return root;
    else if (ISBN < root->ISBN)
        return searchBook(root->left, ISBN);
    else
        return searchBook(root->right, ISBN);
}

//Add Borrower
void addBorrower(Book* book, char* borrowerName) {
    node* newnode = (node*)malloc(sizeof(node));
    strcpy(newnode->name, borrowerName);
    newnode->next = book->borrowers;
    book->borrowers = newnode;
}

//Remove Borrower
void removeBorrower(Book* book, char* borrowerName) {
    node *curr = book->borrowers, *prev = NULL;
    while (curr != NULL) {
        if (strcmp(curr->name, borrowerName) == 0) {
            if (prev == NULL)
                book->borrowers = curr->next;
            else
                prev->next = curr->next;
            free(curr);
            printf("Borrower %s removed.\n", borrowerName);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Borrower %s not found.\n", borrowerName);
}

//Borrow Book
void borrowBook(Book* root, int ISBN, char* borrowerName) {
    Book* book = searchBook(root, ISBN);
    if (book == NULL) {
        printf("Book with ISBN %d not found.\n", ISBN);
        return;
    }
    if (book->copiesAvailable > 0) {
        book->copiesAvailable--;
        addBorrower(book, borrowerName);
        printf("%s successfully borrowed '%s'.\n", borrowerName, book->title);
    } else {
        printf("No copies available for '%s'.\n", book->title);
    }
}

//Return Book
void returnBook(Book* root, int ISBN, char* borrowerName) {
    Book* book = searchBook(root, ISBN);
    if (book == NULL) {
        printf("Book with ISBN %d not found.\n", ISBN);
        return;
    }
    removeBorrower(book, borrowerName);
    book->copiesAvailable++;
    printf("%s returned '%s'.\n", borrowerName, book->title);
}

//Display Borrowers
void displayBorrowers(node* head) {
    if (head == NULL) {
        printf("    No borrowers.\n");
        return;
    }
    printf("    Borrowers:\n");
    node* temp = head;
    while (temp != NULL) {
        printf("      - %s\n", temp->name);
        temp = temp->next;
    }
}

//Display All Books
void displayBooks(Book* root) {
    if (root == NULL) return;
    displayBooks(root->left);
    printf("ISBN: %d\n", root->ISBN);
    printf("Title: %s\n", root->title);
    printf("Author: %s\n", root->author);
    printf("Copies Available: %d\n", root->copiesAvailable);
    displayBorrowers(root->borrowers);
    printf("---------------------------\n");
    displayBooks(root->right);
}
