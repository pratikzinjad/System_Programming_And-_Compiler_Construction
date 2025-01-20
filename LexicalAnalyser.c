#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

// Function declarations
void getToken(FILE *fp);
int isKeyword(const char *word);
int isOperator(char ch);
int isDelimiter(char ch);

// List of keywords
char *keywords[] = {"int", "float", "if", "else", "while", "return"};
int numKeywords = 6;

int main() {
    FILE *fp;
    fp = fopen("source.c", "r");  // Open the C source file for lexing

    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("Lexical Analysis:\n\n");
    getToken(fp); // Start tokenization
    fclose(fp);   // Close file after reading

    return 0;
}

// Function to get and print tokens
void getToken(FILE *fp) {
    char currentChar;
    char token[MAX_TOKEN_LENGTH];
    int i = 0;

    while ((currentChar = fgetc(fp)) != EOF) {
        // Skip whitespace characters
        if (isspace(currentChar)) {
            continue;
        }

        // Check for identifier or keyword
        if (isalpha(currentChar) || currentChar ==  ‘_’) {
            token[i++] = currentChar;
            while (isalnum((currentChar = fgetc(fp))) || currentChar == '_') {
                token[i++] = currentChar;
            }
            token[i] = '\0';
            ungetc(currentChar, fp); // Put back the non-alphanumeric character
            if (isKeyword(token)) {
                printf("Keyword: %s\n", token);
            } else {
                printf("Identifier: %s\n", token);
            }
            i = 0;  // Reset token index
        }

        // Check for numbers
        else if (isdigit(currentChar)) {
            token[i++] = currentChar;
            while (isdigit((currentChar = fgetc(fp)))) {
                token[i++] = currentChar;
            }
            token[i] = '\0';
            ungetc(currentChar, fp); // Put back the non-digit character
            printf("Number: %s\n", token);
            i = 0;
        }

        // Check for operators (+, -, *, /)
        else if (isOperator(currentChar)) {
            printf("Operator: %c\n", currentChar);
        }

        // Check for delimiters (;, ., ,)
        else if (isDelimiter(currentChar)) {
            printf("Delimiter: %c\n", currentChar);
        }

        // Handle unknown characters
        else {
            printf("Unknown character: %c\n", currentChar);
        }
    }
}

// Check if the word is a keyword
int isKeyword(const char *word) {
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;  // It's a keyword
        }
    }
    return 0;  // It's not a keyword
}

// Check if the character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch ==’=’);
}

// Check if the character is a delimiter
int isDelimiter(char ch) {
    return (ch == ';' || ch == ',' || ch == '.' || ch == '(' || ch == ')');
}
