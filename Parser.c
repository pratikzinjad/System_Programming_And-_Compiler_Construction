#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum { NUM, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, END } TokenType;

typedef struct {
    TokenType type;
    int value;  // used for NUM
} Token;

const char *input;  // input string for parsing
Token currentToken;

// Function declarations
Token getNextToken();
void eat(TokenType type);
int factor();
int term();
int expr();

// Lexical analyzer (returns next token from the input)
Token getNextToken() {
    while (*input != '\0' && isspace(*input)) {
        input++;  // Skip whitespace
    }

    Token token;
    if (*input == '\0') {
        token.type = END;
    } else if (*input == '+') {
        token.type = PLUS;
        input++;
    } else if (*input == '-') {
        token.type = MINUS;
        input++;
    } else if (*input == '*') {
        token.type = MUL;
        input++;
    } else if (*input == '/') {
        token.type = DIV;
        input++;
    } else if (*input == '(') {
        token.type = LPAREN;
        input++;
    } else if (*input == ')') {
        token.type = RPAREN;
        input++;
    } else if (isdigit(*input)) {
        token.type = NUM;
        token.value = 0;
        while (isdigit(*input)) {
            token.value = token.value * 10 + (*input - '0');
            input++;
        }
    } else {
        token.type = END;  // End of input or invalid character
    }
    return token;
}

// Function to consume a token of the expected type
void eat(TokenType type) {
    if (currentToken.type == type) {
        currentToken = getNextToken();
    } else {
        printf("Error: Unexpected token\n");
        exit(1);
    }
}

// Parser function for parsing factors (numbers and parentheses)
int factor() {
    int result = 0;
    if (currentToken.type == NUM) {
        result = currentToken.value;
        eat(NUM);
    } else if (currentToken.type == LPAREN) {
        eat(LPAREN);
        result = expr();  // Recursively parse the expression inside parentheses
        eat(RPAREN);
    } else {
        printf("Error: Unexpected token in factor\n");
        exit(1);
    }
    return result;
}

// Parser function for parsing terms (factor * factor or factor / factor)
int term() {
    int result = factor();
    while (currentToken.type == MUL || currentToken.type == DIV) {
        TokenType op = currentToken.type;
        if (op == MUL) {
            eat(MUL);
            result *= factor();
        } else if (op == DIV) {
            eat(DIV);
            result /= factor();
        }
    }
    return result;
}

// Parser function for parsing expressions (term + term or term - term)
int expr() {
    int result = term();
    while (currentToken.type == PLUS || currentToken.type == MINUS) {
        TokenType op = currentToken.type;
        if (op == PLUS) {
            eat(PLUS);
            result += term();
        } else if (op == MINUS) {
            eat(MINUS);
            result -= term();
        }
    }
    return result;
}

int main() {
    const char *expression = "3 + 5 * (10 - 4)";  // Example input
    input = expression;
    
    currentToken = getNextToken();  // Get the first token
    int result = expr();  // Parse the entire expression
    printf("Result: %d\n", result);
    return 0;
}
