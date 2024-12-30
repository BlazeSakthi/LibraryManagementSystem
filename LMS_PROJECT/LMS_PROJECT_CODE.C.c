#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_CATEGORIES 10
#define MAX_BORROW_DAYS 30

// Structure to store book information
struct Book {
    int id;
    char title[50];
    char author[50];
    char category[30];
    int available; // 1 for available, 0 for issued
    int issuedByUserId; // User ID who borrowed the book
    int daysLeft; // Days left for returning the book
};

// Structure to store user information
struct User {
    int id;
    char username[50];
    char password[50];
    char phoneNumber[20]; // User's phone number
};

// Structure for managing book categories
struct Category {
    int id;
    char name[30];
};

// Global arrays for storing books, users, and categories
struct Book books[MAX_BOOKS];
struct User users[MAX_USERS];
struct Category categories[MAX_CATEGORIES];
int bookCount = 0, userCount = 0, categoryCount = 0;

// Function prototypes
void addCategory();
void viewBooks();
void addBook();
void issueBook(int userId);
void returnBook(int userId);
void userLogin();
void adminLogin();
void createUser();
int loginUser(int isAdmin);
int searchBookByTitle(char title[]);
int searchBookByAuthor(char author[]);
void adminViewIssuedBooks();
void viewIssuedBooksByUser(int userId);

// Main function
int main() {
    int choice;
    printf("Welcome to the Library Management System!\n");

    while(1) {
        printf("\n1. User Login\n2. Admin Login\n3. Register\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                userLogin();
                break;
            case 2:
                adminLogin();
                break;
            case 3:
                createUser();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Function to handle user login
void userLogin() {
    if (userCount == 0) {
        printf("No users registered. Please register first.\n");
        return;
    }

    printf("User Login\n");
    int userId = loginUser(0);

    if (userId != -1) {
        printf("Welcome to the library, %s!\n", users[userId - 1].username);
        int choice;
        while(1) {
            printf("\n1. View Books\n2. Issue Book\n3. Return Book\n4. View Issued Books\n5. Logout\nEnter your choice: ");
            scanf("%d", &choice);

            switch(choice) {
                case 1:
                    viewBooks();
                    break;
                case 2:
                    issueBook(userId);
                    break;
                case 3:
                    returnBook(userId);
                    break;
                case 4:
                    viewIssuedBooksByUser(userId);
                    break;
                case 5:
                    return;
                default:
                    printf("Invalid choice! Try again.\n");
            }
        }
    }
}

// Function to handle admin login
void adminLogin() {
    printf("Admin Login\n");
    int adminId = loginUser(1);

    if (adminId != -1) {
        printf("Welcome Admin!\n");
        int choice;
        while(1) {
            printf("\n1. Add Category\n2. Add Book\n3. View Books\n4. View Issued Books\n5. Logout\nEnter your choice: ");
            scanf("%d", &choice);

            switch(choice) {
                case 1:
                    addCategory();
                    break;
                case 2:
                    addBook();
                    break;
                case 3:
                    viewBooks();
                    break;
                case 4:
                    adminViewIssuedBooks();
                    break;
                case 5:
                    return;
                default:
                    printf("Invalid choice! Try again.\n");
            }
        }
    }
}

// Function to register a new user
void createUser() {
    if(userCount < MAX_USERS) {
        printf("Enter username: ");
        scanf("%s", users[userCount].username);
        printf("Enter password: ");
        scanf("%s", users[userCount].password);
        printf("Enter phone number: ");
        scanf("%s", users[userCount].phoneNumber);
        users[userCount].id = userCount + 1;
        userCount++;
        printf("Thank you for registering! You can now login.\n");
    } else {
        printf("User limit reached! Unable to register.\n");
    }
}

// Function to login either user or admin
int loginUser(int isAdmin) {
    char username[50], password[50];
    int i;

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for(i = 0; i < (isAdmin ? 1 : userCount); i++) {
        if (strcmp(isAdmin ? "admin" : users[i].username, username) == 0 && 
            strcmp(isAdmin ? "admin" : users[i].password, password) == 0) {
            return isAdmin ? 0 : users[i].id;
        }
    }
    printf("Invalid credentials! Try again.\n");
    return -1;
}

// Function to add a book category
void addCategory() {
    if(categoryCount < MAX_CATEGORIES) {
        printf("Enter category name: ");
        scanf("%s", categories[categoryCount].name);
        categories[categoryCount].id = categoryCount + 1;
        categoryCount++;
        printf("Category added successfully!\n");
    } else {
        printf("Category limit reached!\n");
    }
}

// Function to view all books
void viewBooks() {
    printf("\nBook Catalog:\n");
    if (bookCount == 0) {
        printf("No books available in the catalog.\n");
    } else {
        for(int i = 0; i < bookCount; i++) {
            printf("%d. Title: %s | Author: %s | Category: %s | %s\n", 
                books[i].id, books[i].title, books[i].author, books[i].category, 
                books[i].available ? "Available" : "Issued");
            if (!books[i].available) {
                printf("    Issued to User ID: %d, Days Left: %d, Phone: %s\n", 
                        books[i].issuedByUserId, books[i].daysLeft, users[books[i].issuedByUserId - 1].phoneNumber);
            }
        }
    }
}

// Function to add a new book
void addBook() {
    if(bookCount < MAX_BOOKS) {
        printf("Enter book title: ");
        scanf(" %[^\n]%*c", books[bookCount].title);
        printf("Enter book author: ");
        scanf(" %[^\n]%*c", books[bookCount].author);
        printf("Enter book category: ");
        scanf(" %[^\n]%*c", books[bookCount].category);

        books[bookCount].id = bookCount + 1;
        books[bookCount].available = 1; // Available by default
        books[bookCount].issuedByUserId = 0; // Not issued
        books[bookCount].daysLeft = 0; // No borrowing yet
        bookCount++;
        printf("Book added successfully!\n");
    } else {
        printf("Book limit reached!\n");
    }
}

// Function to issue a book
void issueBook(int userId) {
    int bookId;
    printf("Enter book ID to issue: ");
    scanf("%d", &bookId);

    if(bookId > 0 && bookId <= bookCount && books[bookId-1].available) {
        books[bookId-1].available = 0; // Mark book as issued
        books[bookId-1].issuedByUserId = userId; // Mark user who borrowed
        books[bookId-1].daysLeft = MAX_BORROW_DAYS; // Set the borrow period
        printf("Thank you for issuing the book! Please return it within %d days.\n", MAX_BORROW_DAYS);
    } else {
        printf("Book is not available for issue.\n");
    }
}

// Function to return a book
void returnBook(int userId) {
    int bookId;
    printf("Enter book ID to return: ");
    scanf("%d", &bookId);

    if(bookId > 0 && bookId <= bookCount && !books[bookId-1].available && books[bookId-1].issuedByUserId == userId) {
        books[bookId-1].available = 1; // Mark book as available
        books[bookId-1].issuedByUserId = 0; // Clear user who borrowed it
        books[bookId-1].daysLeft = 0; // Clear days left
        printf("Book returned successfully!\n");
    } else {
        printf("Invalid book ID or the book was not issued to you.\n");
    }
}

// Function for admin to view all issued books
void adminViewIssuedBooks() {
    printf("\nIssued Books Information:\n");
    for(int i = 0; i < bookCount; i++) {
        if (!books[i].available) {
            printf("Book ID: %d | Title: %s | Issued to User ID: %d | Days Left: %d | User Phone: %s\n",
                   books[i].id, books[i].title, books[i].issuedByUserId, books[i].daysLeft,
                   users[books[i].issuedByUserId - 1].phoneNumber);
        }
    }
}

// Function to view books issued by a user
void viewIssuedBooksByUser(int userId) {
    printf("\nIssued Books by You:\n");
    int found = 0;
    for(int i = 0; i < bookCount; i++) {
        if (!books[i].available && books[i].issuedByUserId == userId) {
            printf("Book ID: %d | Title: %s | Days Left: %d\n", books[i].id, books[i].title, books[i].daysLeft);
            found = 1;
        }
    }
    if (!found) {
        printf("No books issued yet.\n");
    }
}
