#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKEN_LENGTH 256
#define MAX_SOURCE_SIZE 100000

typedef struct {
    char lexeme[MAX_TOKEN_LENGTH];
    int row, column;
} Token;

typedef struct {
    const char *source;
    int pos, row, col, length;
} Lexer;

const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", 
    "default", "do", "double", "else", "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return", "short", "signed", "sizeof", 
    "static", "struct", "switch", "typedef", "union", "unsigned", "void", 
    "volatile", "while", "if", "else", "printf"};

// ============ TOKEN IDENTIFICATION FUNCTIONS ============

bool isArithmeticOp(const char *s) {
    return (strcmp(s, "+") == 0 || strcmp(s, "-") == 0 || strcmp(s, "*") == 0 || 
            strcmp(s, "/") == 0 || strcmp(s, "%") == 0 || strcmp(s, "++") == 0 || 
            strcmp(s, "--") == 0);
}

bool isRelationalOp(const char *s) {
    return (strcmp(s, "<") == 0 || strcmp(s, ">") == 0 || strcmp(s, "<=") == 0 || 
            strcmp(s, ">=") == 0 || strcmp(s, "==") == 0 || strcmp(s, "!=") == 0);
}

bool isLogicalOp(const char *s) {
    return (strcmp(s, "&&") == 0 || strcmp(s, "||") == 0 || strcmp(s, "!") == 0);
}

bool isKeyword(const char *s) {
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++)
        if (strcmp(s, keywords[i]) == 0) return true;
    return false;
}

bool isSpecialSymbol(char c) {
    return strchr("{}[]();,.", c) != NULL;
}

bool isIdentifier(const char *s) {
    if (!s || !s[0]) return false;
    if (! isalpha(s[0]) && s[0] != '_') return false;
    for (int i = 1; s[i]; i++)
        if (!isalnum(s[i]) && s[i] != '_') return false;
    return ! isKeyword(s);
}

// ============ PREPROCESSOR ============

char* preprocessSource(const char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("Error: Cannot open file '%s'\n", fileName);
        printf("Make sure the file exists in the current directory.\n");
        return NULL;
    }
    
    char *out = (char *)malloc(MAX_SOURCE_SIZE);
    if (!out) {
        printf("Error: Memory allocation failed\n");
        fclose(f);
        return NULL;
    }
    
    int idx = 0;
    char ch, next;
    bool singleComment = false, multiComment = false, inStr = false, inPreproc = false;
    
    while ((ch = fgetc(f)) != EOF && idx < MAX_SOURCE_SIZE - 1) {
        // String handling
        if (ch == '"' && !singleComment && !multiComment && !inPreproc) {
            out[idx++] = ch;
            inStr = !inStr;
            continue;
        }
        
        if (inStr) {
            out[idx++] = ch;
            if (ch == '\\') {
                next = fgetc(f);
                if (next != EOF) out[idx++] = next;
            }
            continue;
        }
        
        // Preprocessor
        if (ch == '#' && !singleComment && ! multiComment) {
            inPreproc = true;
            continue;
        }
        
        if (inPreproc) {
            if (ch == '\n') {
                inPreproc = false;
                out[idx++] = ch;
            }
            continue;
        }
        
        // Comments
        if (ch == '/' && !multiComment) {
            next = fgetc(f);
            if (next == '/') {
                singleComment = true;
                continue;
            }
            if (next == '*') {
                multiComment = true;
                continue;
            }
            out[idx++] = ch;
            if (next != EOF) ungetc(next, f);
            continue;
        }
        
        if (singleComment && ch == '\n') {
            singleComment = false;
            out[idx++] = ch;
            continue;
        }
        
        if (multiComment) {
            if (ch == '*') {
                next = fgetc(f);
                if (next == '/') {
                    multiComment = false;
                    continue;
                }
                if (next != EOF) ungetc(next, f);
            }
            if (ch == '\n') out[idx++] = ch;
            continue;
        }
        
        if (! singleComment && !multiComment && !inPreproc) {
            out[idx++] = ch;
        }
    }
    
    out[idx] = '\0';
    fclose(f);
    
    // Debug:  Check if preprocessed content is empty
    if (idx == 0) {
        printf("Warning:  File is empty or all content was removed during preprocessing\n");
    }
    
    return out;
}

// ============ LEXER ============

void initLexer(Lexer *lex, const char *src) {
    lex->source = src;
    lex->pos = 0;
    lex->row = 1;
    lex->col = 1;
    lex->length = strlen(src);
}

char peek(Lexer *lex) {
    return lex->pos < lex->length ? lex->source[lex->pos] : '\0';
}

char advance(Lexer *lex) {
    if (lex->pos >= lex->length) return '\0';
    char ch = lex->source[lex->pos++];
    if (ch == '\n') {
        lex->row++;
        lex->col = 1;
    } else {
        lex->col++;
    }
    return ch;
}

void skipWhitespace(Lexer *lex) {
    while (isspace(peek(lex))) advance(lex);
}

Token getNextToken(Lexer *lex) {
    // Token structure to store the extracted token
    Token tok;

    // Initialize all fields of token to zero
    memset(&tok, 0, sizeof(Token));
    
    // Skip spaces, tabs, and newlines before tokenizing
    skipWhitespace(lex);
    
    // Save the starting position of the token
    int startRow = lex->row;
    int startCol = lex->col;

    // Look at the current character without consuming it
    char ch = peek(lex);
    
    // ---------- END OF FILE ----------
    // If no more characters are left in source
    if (ch == '\0') {
        // Mark token as EOF
        strcpy(tok.lexeme, "EOF");

        // Store position of EOF
        tok.row = startRow;
        tok.column = startCol;

        // Return EOF token
        return tok;
    }
    
    // ---------- IDENTIFIER OR KEYWORD ----------
    // If token starts with letter or underscore
    if (isalpha(ch) || ch == '_') {
        int i = 0;

        // Read characters while they are valid identifier characters
        while ((isalnum(peek(lex)) || peek(lex) == '_') &&
               i < MAX_TOKEN_LENGTH - 1) {

            // Consume character and store in lexeme
            tok.lexeme[i++] = advance(lex);
        }

        // Null-terminate the identifier
        tok.lexeme[i] = '\0';

        // Store starting position of token
        tok.row = startRow;
        tok.column = startCol;

        // Return identifier or keyword token
        return tok;
    }
    
    // ---------- NUMBER ----------
    // If token starts with a digit
    if (isdigit(ch)) {
        int i = 0;

        // Read digits (and decimal point for float numbers)
        while ((isdigit(peek(lex)) || peek(lex) == '.') &&
               i < MAX_TOKEN_LENGTH - 1) {

            // Consume character and add to lexeme
            tok.lexeme[i++] = advance(lex);
        }

        // Null-terminate the number token
        tok.lexeme[i] = '\0';

        // Store token position
        tok.row = startRow;
        tok.column = startCol;

        // Return numeric token
        return tok;
    }
    
    // ---------- STRING LITERAL ----------
    // If token starts with double quote
    if (ch == '"') {

        // Skip opening quote
        advance(lex);

        // Read until closing quote or end of file
        while (peek(lex) != '"' && peek(lex) != '\0') {

            // Handle escaped characters inside string
            if (peek(lex) == '\\') {
                advance(lex);        // Skip escape character
                if (peek(lex))
                    advance(lex);    // Skip escaped character
            } else {
                advance(lex);        // Normal character
            }
        }

        // Skip closing quote
        if (peek(lex) == '"')
            advance(lex);

        // Store generic string token name
        strcpy(tok.lexeme, "str");

        // Store token position
        tok.row = startRow;
        tok.column = startCol;

        // Return string token
        return tok;
    }
    
    // ---------- SPECIAL SYMBOLS ----------
    // If character is one of {}[]();,.
    if (isSpecialSymbol(ch)) {

        // Store the symbol as token
        tok.lexeme[0] = advance(lex);
        tok.lexeme[1] = '\0';

        // Store token position
        tok.row = startRow;
        tok.column = startCol;

        // Return special symbol token
        return tok;
    }
    
    // ---------- OPERATORS ----------
    // If character is an operator symbol
    if (strchr("+-*/%<>=! &|^~", ch)) {

        // Store first operator character
        tok.lexeme[0] = advance(lex);
        tok.lexeme[1] = '\0';

        // Peek next character to check for 2-character operator
        char next = peek(lex);
        
        // Check for multi-character operators
        if ((tok.lexeme[0] == '+' && next == '+') ||
            (tok.lexeme[0] == '-' && next == '-') ||
            (tok.lexeme[0] == '=' && next == '=') ||
            (tok.lexeme[0] == '!' && next == '=') ||
            (tok.lexeme[0] == '<' && next == '=') ||
            (tok.lexeme[0] == '>' && next == '=') ||
            (tok.lexeme[0] == '&' && next == '&') ||
            (tok.lexeme[0] == '|' && next == '|') ||
            (tok.lexeme[0] == '-' && next == '>') ||
            (tok.lexeme[0] == '<' && next == '<') ||
            (tok.lexeme[0] == '>' && next == '>')) {

            // Append second character of operator
            tok.lexeme[1] = advance(lex);
            tok.lexeme[2] = '\0';
        }

        // Store operator position
        tok.row = startRow;
        tok.column = startCol;

        // Return operator token
        return tok;
    }
    
    // ---------- UNKNOWN CHARACTER ----------
    // If character does not match any known pattern
    tok.lexeme[0] = advance(lex);
    tok.lexeme[1] = '\0';

    // Store token position
    tok.row = startRow;
    tok.column = startCol;

    // Return unknown token
    return tok;
}

// ============ TOKEN PRINTER ============

void printToken(Token tok, int *lastRow, FILE *out) {
    if (*lastRow != -1 && tok.row != *lastRow) {
        fprintf(out, "\n");
        printf("\n");
    }
    *lastRow = tok.row;
    
    char output[300];
    
    if (strcmp(tok.lexeme, "EOF") == 0) {
        sprintf(output, "\n<EOF,%d,%d>\n", tok. row, tok.column);
    } else if (strcmp(tok.lexeme, "str") == 0) {
        sprintf(output, "<str,%d,%d>", tok.row, tok.column);
    } else if (isdigit(tok.lexeme[0])) {
        sprintf(output, "<num,%d,%d>", tok.row, tok.column);
    } else if (isKeyword(tok.lexeme)) {
        snprintf(output, sizeof(output), "<%s,%d,%d>", tok.lexeme, tok.row, tok.column);
    } else if (isIdentifier(tok.lexeme)) {
        sprintf(output, "<id,%d,%d>", tok. row, tok.column);
    } else {
        snprintf(output, sizeof(output), "<%s,%d,%d>", tok.lexeme, tok.row, tok.column);
    }
    
    fprintf(out, "%s", output);
    printf("%s", output);
    fflush(stdout);  // Force output to display immediately
}

// ============ MAIN ============
// Commented out - main() is now in symbol_table.c

/*
int main() {
    char fileName[100];
    printf("Enter filename to open for token generation: ");
    scanf("%s", fileName);
    
    printf("\nAttempting to read file: %s\n", fileName);
    
    char *source = preprocessSource(fileName);
    if (!source) {
        printf("\nError:  Preprocessing failed!\n");
        printf("Please check:\n");
        printf("  1. File '%s' exists in current directory\n", fileName);
        printf("  2. File has read permissions\n");
        printf("  3. File is not empty\n");
        return 1;
    }
        
    printf("File read successfully!  Preprocessed length: %lu bytes\n", strlen(source));
    printf("\nPreprocessing completed successfully!\n");
    printf("\n\n========== TOKEN ANALYSIS ==========\n");
        printf("Tokens saved to 'tokens.txt'\n\n");

    FILE *out = fopen("tokens.txt", "w");
    if (!out) {
        printf("Error: Cannot create tokens.txt\n");
        free(source);
        return 1;
    }
    
    Lexer lexer;
    initLexer(&lexer, source);
    
    Token tok;
    int lastRow = -1;
    int tokenCount = 0;
    do {
        tok = getNextToken(&lexer);
        printToken(tok, &lastRow, out);
        tokenCount++;
    } while (strcmp(tok. lexeme, "EOF") != 0);   
    fclose(out);
    free(source);
    return 0;
}
*/
