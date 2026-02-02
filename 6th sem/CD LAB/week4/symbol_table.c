#include "la.h"

#define HASH_TABLE_SIZE 100

// ============ SYMBOL TABLE STRUCTURE ============

struct node {
    char lexeme_name[50];
    char type[20];
    int size;
    char return_type[20];
    struct node *next;
};

typedef struct {
    struct node *table[HASH_TABLE_SIZE];
    char tableName[50];
    int count;
} SymbolTable;

// ============ HASH FUNCTION ============

int hash(const char *str) {
    unsigned long hashValue = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hashValue = hashValue * 31 + str[i];
    }
    return hashValue % HASH_TABLE_SIZE;
}

// ============ INITIALIZE SYMBOL TABLE ============

void initSymbolTable(SymbolTable *st, const char *name) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        st->table[i] = NULL;
    }
    strcpy(st->tableName, name);
    st->count = 0;
}

// ============ SEARCH FUNCTION ============

struct node* search(SymbolTable *st, const char *lexeme_name) {
    int index = hash(lexeme_name);
    struct node *current = st->table[index];
    
    printf("\n[SEARCH] Looking for '%s' at hash index %d...", lexeme_name, index);
    
    while (current != NULL) {
        if (strcmp(current->lexeme_name, lexeme_name) == 0) {
            printf(" FOUND!");
            return current;
        }
        current = current->next;
    }
    
    printf(" NOT FOUND!");
    return NULL;
}

// ============ INSERT FUNCTION ============

bool insert(SymbolTable *st, const char *lexeme_name, const char *type, 
            int size, const char *return_type) {
    
    // Check if already exists
    if (search(st, lexeme_name) != NULL) {
        printf("\n[INSERT] '%s' already exists!", lexeme_name);
        return false;
    }
    
    int index = hash(lexeme_name);
    
    // Create new node
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    if (newNode == NULL) {
        printf("\n[ERROR] Memory allocation failed!");
        return false;
    }
    
    // Fill node data
    strcpy(newNode->lexeme_name, lexeme_name);
    strcpy(newNode->type, type);
    newNode->size = size;
    strcpy(newNode->return_type, return_type);
    newNode->next = st->table[index];
    
    st->table[index] = newNode;
    st->count++;
    
    printf("\n[INSERT] '%s' inserted at hash %d (Type: %s, Size: %d, Return: %s)", 
           lexeme_name, index, type, size, return_type);
    
    return true;
}

// ============ DISPLAY SYMBOL TABLE ============

void displaySymbolTable(SymbolTable *st) {
    printf("\n\n========== %s ==========\n", st->tableName);
    printf("%-5s %-20s %-15s %-10s %-15s\n", 
           "Hash", "Lexeme Name", "Type", "Size", "Return Type");
    printf("------------------------------------------------------------------------\n");
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        struct node *current = st->table[i];
        while (current != NULL) {
            printf("%-5d %-20s %-15s %-10d %-15s\n", 
                   i, current->lexeme_name, current->type, 
                   current->size, current->return_type);
            current = current->next;
        }
    }
    
    printf("\nTotal Symbols: %d\n", st->count);
}

// ============ SAVE TO FILE ============

void saveSymbolTableToFile(SymbolTable *st, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("\n[ERROR] Cannot create file %s\n", filename);
        return;
    }
    
    fprintf(fp, "========== %s ==========\n", st->tableName);
    fprintf(fp, "%-5s %-20s %-15s %-10s %-15s\n", 
            "Hash", "Lexeme Name", "Type", "Size", "Return Type");
    fprintf(fp, "------------------------------------------------------------------------\n");
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        struct node *current = st->table[i];
        while (current != NULL) {
            fprintf(fp, "%-5d %-20s %-15s %-10d %-15s\n", 
                    i, current->lexeme_name, current->type, 
                    current->size, current->return_type);
            current = current->next;
        }
    }
    
    fprintf(fp, "\nTotal Symbols: %d\n", st->count);
    fclose(fp);
}

// ============ FREE SYMBOL TABLE ============

void freeSymbolTable(SymbolTable *st) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        struct node *current = st->table[i];
        while (current != NULL) {
            struct node *temp = current;
            current = current->next;
            free(temp);
        }
        st->table[i] = NULL;
    }
    st->count = 0;
}

// ============ GET SIZE FOR DATA TYPE ============

int getSizeForType(const char *dataType) {
    if (strcmp(dataType, "int") == 0) return 4;
    if (strcmp(dataType, "float") == 0) return 4;
    if (strcmp(dataType, "double") == 0) return 8;
    if (strcmp(dataType, "char") == 0) return 1;
    if (strcmp(dataType, "long") == 0) return 8;
    if (strcmp(dataType, "short") == 0) return 2;
    return 0;
}

// ============ MAIN PROGRAM ============

int main() {
    char fileName[100];
    printf("Enter filename: ");
    scanf("%s", fileName);
    
    // Preprocess source
    char *source = preprocessSource(fileName);
    if (!source) {
        printf("\n[ERROR] Failed to read file!\n");
        return 1;
    }
    
    // Initialize symbol table
    SymbolTable localTable;
    initSymbolTable(&localTable, "LOCAL SYMBOL TABLE");
    
    // Initialize lexer
    Lexer lexer;
    initLexer(&lexer, source);
    
    Token tok;
    char currentDataType[20];
    char currentFunction[50];
    bool inFunction = false;
    int braceCount = 0;
    
    currentDataType[0] = '\0';
    currentFunction[0] = '\0';
    
    // Process all tokens
    do {
        tok = getNextToken(&lexer);
        
        if (strcmp(tok.lexeme, "EOF") != 0) {
            // Track when entering a function
            if (isIdentifier(tok.lexeme) && !inFunction) {
                // Could be a function name
                strcpy(currentFunction, tok.lexeme);
            }
            
            if (strcmp(tok.lexeme, "(") == 0 && currentFunction[0] != '\0') {
                inFunction = true;
            }
            
            if (strcmp(tok.lexeme, "{") == 0) {
                braceCount++;
            }
            
            if (strcmp(tok.lexeme, "}") == 0) {
                braceCount--;
                if (braceCount == 0) {
                    inFunction = false;
                    currentFunction[0] = '\0';
                }
            }
            
            // Track data types
            if (isKeyword(tok.lexeme)) {
                if (strcmp(tok.lexeme, "int") == 0 || 
                    strcmp(tok.lexeme, "float") == 0 ||
                    strcmp(tok.lexeme, "char") == 0 ||
                    strcmp(tok.lexeme, "double") == 0 ||
                    strcmp(tok.lexeme, "long") == 0 ||
                    strcmp(tok.lexeme, "short") == 0) {
                    strcpy(currentDataType, tok.lexeme);
                }
            } else if (isIdentifier(tok.lexeme) && inFunction && braceCount > 0 && currentDataType[0] != '\0') {
                // This is a local variable inside a function
                int size = getSizeForType(currentDataType);
                insert(&localTable, tok.lexeme, "variable", size, currentDataType);
            }
        }
        
    } while (strcmp(tok.lexeme, "EOF") != 0);
    
    // Display symbol table
    displaySymbolTable(&localTable);
    saveSymbolTableToFile(&localTable, "local_table.txt");
    
    // Cleanup
    freeSymbolTable(&localTable);
    free(source);
    
    return 0;
}