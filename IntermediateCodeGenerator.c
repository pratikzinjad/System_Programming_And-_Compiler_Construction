#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    int value;  // Value for numbers
    char op;    // Operator for token (+, -, *, /, etc.)
    char var;   // Variable name (e.g., a, b, t1, t2)
    int type;   // Token type: NUM, OPERATOR, or IDENTIFIER
} Token;

const char *input;  // Input string for parsing
Token currentToken;  // Current token
int tempCount = 1;  // To generate unique temporary variable names

#define NUM 1
#define OPERATOR 2
#define IDENTIFIER 3
#define END 4

void getNextToken();
void eat(int type);
int factor();
int term();
int expr();
void generateIntermediateCode(char *operation, char *operand1, char *operand2, char *result);
void initialize();

void getNextToken() {
    while (*input != '\0' && isspace(*input)) {
        input++;  // Skip whitespace
    }

    if (*input == '\0') {
        currentToken.type = END;
    } else if (*input == '+') {
        currentToken.type = OPERATOR;
        currentToken.op = '+';
        input++;
    } else if (*input == '-') {
        currentToken.type = OPERATOR;
        currentToken.op = '-';
        input++;
    } else if (*input == '*') {
        currentToken.type = OPERATOR;
        currentToken.op = '*';
        input++;
    } else if (*input == '/') {
        currentToken.type = OPERATOR;
        currentToken.op = '/';
        input++;
    } else if (*input == '(') {
        currentToken.type = OPERATOR;
        currentToken.op = '(';
        input++;
    } else if (*input == ')') {
        currentToken.type = OPERATOR;
        currentToken.op = ')';
        input++;
    } else if (isdigit(*input)) {
        currentToken.type = NUM;
        currentToken.value = 0;
        while (isdigit(*input)) {
            currentToken.value = currentToken.value * 10 + (*input - '0');
            input++;
        }
    } else if (isalpha(*input)) {
        currentToken.type = IDENTIFIER;
        currentToken.var = *input;
        input++;
    } else {
        currentToken.type = END;  // End of input or invalid character
    }
}

void eat(int type) {
    if (currentToken.type == type) {
        getNextToken();
    } else {
        printf("Error: Unexpected token\n");
        exit(1);
    }
}

void generateIntermediateCode(char *operation, char *operand1, char *operand2, char *result) {
    if (operation[0] != '\0') {
        printf("%s = %s %s %s\n", result, operand1, operation, operand2);
    } else {
        printf("%s = %s\n", result, operand1); 
    }
}

void initialize() {
    tempCount = 1;  // Reset tempCount for every new expression evaluation
}

int factor() {
    int result = 0;
    char tempVar[10];

    if (currentToken.type == NUM) {
        result = currentToken.value;
        sprintf(tempVar, "%d", result);
        char tempResult[10];
        sprintf(tempResult, "t%d", tempCount++);  // Use a new temporary variable
        generateIntermediateCode("", tempVar, "", tempResult);  // No operation needed for numbers
        eat(NUM);
    } else if (currentToken.type == IDENTIFIER) {
        result = 0;  // We'll handle the identifiers as operands
        char tempVar[2];
        tempVar[0] = currentToken.var;
        tempVar[1] = '\0';
        char tempResult[10];
        sprintf(tempResult, "t%d", tempCount++);  // New temporary variable
        generateIntermediateCode("", tempVar, "", tempResult);  // Variable as intermediate code
        eat(IDENTIFIER);
    } else if (currentToken.type == OPERATOR && currentToken.op == '(') {
        eat(OPERATOR);  // eat '('
        result = expr();  // Recursively parse the expression inside parentheses
        eat(OPERATOR);  // eat ')'
    } else {
        printf("Error: Unexpected token in factor\n");
        exit(1);
    }

    return result;
}

int term() {
    int result = factor();
    char tempVar1[10], tempVar2[10]; 
    sprintf(tempVar1, "t%d", tempCount - 1); // Use the previous temporary variable

    while (currentToken.type == OPERATOR && (currentToken.op == '*' || currentToken.op == '/')) {
        char op = currentToken.op;
        eat(OPERATOR); 
        int right = factor();
        sprintf(tempVar2, "t%d", tempCount - 1); // Use the previous temporary variable

        if (op == '*') {
            sprintf(tempVar1, "t%d", tempCount++);
            generateIntermediateCode("*", tempVar1, tempVar2, tempVar1); 
        } else if (op == '/') {
            sprintf(tempVar1, "t%d", tempCount++);
            generateIntermediateCode("/", tempVar1, tempVar2, tempVar1); 
        }
    }
    return result;
}

int expr() {
    int result = term();
    char tempVar1[10], tempVar2[10]; 
    sprintf(tempVar1, "t%d", tempCount - 1); // Use the previous temporary variable

    while (currentToken.type == OPERATOR && (currentToken.op == '+' || currentToken.op == '-')) {
        char op = currentToken.op;
        eat(OPERATOR);
        int right = term();
        sprintf(tempVar2, "t%d", tempCount - 1); // Use the previous temporary variable

        if (op == '+') {
            sprintf(tempVar1, "t%d", tempCount++);
            generateIntermediateCode("+", tempVar1, tempVar2, tempVar1); 
        } else if (op == '-') {
            sprintf(tempVar1, "t%d", tempCount++);
            generateIntermediateCode("-", tempVar1, tempVar2, tempVar1); 
        }
    }
    return result;
}

int main() {
    const char *expression = "a + b * c";  // Example input
    input = expression;
    
    initialize();
    getNextToken();  // Get the first token
    expr();  // Parse the entire expression
    return 0;
}
