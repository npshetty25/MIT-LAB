
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
    "volatile", "while"};

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
    for (int i = 0; i < 32; i++)
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
    
    char *out = malloc(MAX_SOURCE_SIZE);
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
    Token tok;
    memset(&tok, 0, sizeof(Token));
    
    skipWhitespace(lex);
    
    int startRow = lex->row;
    int startCol = lex->col;
    char ch = peek(lex);
    
    // EOF
    if (ch == '\0') {
        strcpy(tok.lexeme, "EOF");
        tok.row = startRow;
        tok.column = startCol;
        return tok;
    }
    
    // Identifier or Keyword
    if (isalpha(ch) || ch == '_') {
        int i = 0;
        while ((isalnum(peek(lex)) || peek(lex) == '_') && i < MAX_TOKEN_LENGTH - 1) {
            tok.lexeme[i++] = advance(lex);
        }
        tok.lexeme[i] = '\0';
        tok.row = startRow;
        tok. column = startCol;
        return tok;
    }
    
    // Number
    if (isdigit(ch)) {
        int i = 0;
        while ((isdigit(peek(lex)) || peek(lex) == '.') && i < MAX_TOKEN_LENGTH - 1) {
            tok.lexeme[i++] = advance(lex);
        }
        tok.lexeme[i] = '\0';
        tok. row = startRow;
        tok.column = startCol;
        return tok;
    }
    
    // String
    if (ch == '"') {
        advance(lex);
        while (peek(lex) != '"' && peek(lex) != '\0') {
            if (peek(lex) == '\\') {
                advance(lex);
                if (peek(lex)) advance(lex);
            } else {
                advance(lex);
            }
        }
        if (peek(lex) == '"') advance(lex);
        strcpy(tok.lexeme, "str");
        tok.row = startRow;
        tok.column = startCol;
        return tok;
    }
    
    // Special Symbols
    if (isSpecialSymbol(ch)) {
        tok.lexeme[0] = advance(lex);
        tok.lexeme[1] = '\0';
        tok.row = startRow;
        tok.column = startCol;
        return tok;
    }
    
    // Operators (including 2-char)
    if (strchr("+-*/%<>=! &|^~", ch)) {
        tok.lexeme[0] = advance(lex);
        tok.lexeme[1] = '\0';
        char next = peek(lex);
        
        if ((tok.lexeme[0] == '+' && next == '+') ||
            (tok.lexeme[0] == '-' && next == '-') ||
            (tok. lexeme[0] == '=' && next == '=') ||
            (tok.lexeme[0] == '!' && next == '=') ||
            (tok. lexeme[0] == '<' && next == '=') ||
            (tok.lexeme[0] == '>' && next == '=') ||
            (tok.lexeme[0] == '&' && next == '&') ||
            (tok.lexeme[0] == '|' && next == '|') ||
            (tok. lexeme[0] == '-' && next == '>') ||
            (tok.lexeme[0] == '<' && next == '<') ||
            (tok.lexeme[0] == '>' && next == '>')) {
            tok.lexeme[1] = advance(lex);
            tok.lexeme[2] = '\0';
        }
        
        tok.row = startRow;
        tok.column = startCol;
        return tok;
    }
    
    // Unknown
    tok.lexeme[0] = advance(lex);
    tok.lexeme[1] = '\0';
    tok.row = startRow;
    tok. column = startCol;
    return tok;
}

// ============ TOKEN PRINTER ============

void printToken(Token tok, int *lastRow, FILE *out) {
    if (*lastRow != -1 && tok.row != *lastRow) {
        fprintf(out, "\n");
        printf("\n");
    }
    *lastRow = tok.row;
    
    char output[100];
    
    if (strcmp(tok.lexeme, "EOF") == 0) {
        sprintf(output, "\n<EOF,%d,%d>\n", tok. row, tok.column);
    } else if (strcmp(tok.lexeme, "str") == 0) {
        sprintf(output, "<str,%d,%d>", tok.row, tok.column);
    } else if (isdigit(tok.lexeme[0])) {
        sprintf(output, "<num,%d,%d>", tok.row, tok.column);
    } else if (isKeyword(tok.lexeme)) {
        sprintf(output, "<%s,%d,%d>", tok.lexeme, tok.row, tok.column);
    } else if (isIdentifier(tok.lexeme)) {
        sprintf(output, "<id,%d,%d>", tok. row, tok.column);
    } else {
        sprintf(output, "<%s,%d,%d>", tok.lexeme, tok.row, tok.column);
    }
    
    fprintf(out, "%s", output);
    printf("%s", output);
    fflush(stdout);  // Force output to display immediately
}

// ============ MAIN ============

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
