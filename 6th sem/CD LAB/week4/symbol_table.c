#include "la.h"

#define TABLE_SIZE 100

/* ================= SYMBOL TABLE NODE ================= */
struct node {
    char lexeme_name[50];
    char type[20];        // int / char / bool / FUNCTION
    int size;
    char return_type[20]; // only for functions, "-" otherwise
    int order;            // insertion order
    struct node *next;
};

/* ================= SYMBOL TABLE ================= */
typedef struct {
    struct node *table[TABLE_SIZE];
    int count;
    int order_counter;
} SymbolTable;

/* ================= HASH FUNCTION ================= */
int hash(const char *key) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++)
        sum += key[i];
    return sum % TABLE_SIZE;
}

/* ================= SEARCH ================= */
struct node *search(SymbolTable *st, const char *lexeme) {
    int index = hash(lexeme);
    struct node *temp = st->table[index];

    while (temp) {
        if (strcmp(temp->lexeme_name, lexeme) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/* ================= INSERT ================= */
void insert(SymbolTable *st,
            const char *lexeme,
            const char *type,
            int size,
            const char *return_type)
{
    if (search(st, lexeme))
        return;

    int index = hash(lexeme);
    struct node *newNode = (struct node *)malloc(sizeof(struct node));

    strcpy(newNode->lexeme_name, lexeme);
    strcpy(newNode->type, type);
    newNode->size = size;
    strcpy(newNode->return_type, return_type);
    newNode->order = st->order_counter++;

    newNode->next = st->table[index];
    st->table[index] = newNode;
    st->count++;
}

/* ================= INIT ================= */
void initSymbolTable(SymbolTable *st) {
    for (int i = 0; i < TABLE_SIZE; i++)
        st->table[i] = NULL;
    st->count = 0;
    st->order_counter = 0;
}

/* ================= DISPLAY ================= */
void display(SymbolTable *st) {
    printf("\n========== SYMBOL TABLE ==========\n");
    printf("%-5s %-20s %-15s %-10s %-15s\n",
           "Hash", "Lexeme Name", "Type", "Size", "Return Type");
    printf("-----------------------------------------------------------\n");

    // Collect all nodes in an array
    struct node *nodes[1000];
    int nodeCount = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct node *temp = st->table[i];
        while (temp) {
            nodes[nodeCount++] = temp;
            temp = temp->next;
        }
    }
    
    // Sort by insertion order
    for (int i = 0; i < nodeCount - 1; i++) {
        for (int j = 0; j < nodeCount - i - 1; j++) {
            if (nodes[j]->order > nodes[j + 1]->order) {
                struct node *temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
        }
    }
    
    // Display in order
    for (int i = 0; i < nodeCount; i++) {
        printf("%-5d %-20s %-15s %-10d %-15s\n",
               hash(nodes[i]->lexeme_name),
               nodes[i]->lexeme_name,
               nodes[i]->type,
               nodes[i]->size,
               nodes[i]->return_type);
    }
    
    printf("\nTotal Symbols: %d\n", st->count);
}

/* ================= UTILS ================= */
int isTypeKeyword(const char *str) {
    const char *keywords[] = {
        "int", "char", "bool", "float", "double", "void", NULL
    };
    for (int i = 0; keywords[i]; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}

int getSize(const char *type) {
    if (strcmp(type, "int") == 0) return 4;
    if (strcmp(type, "char") == 0) return 1;
    if (strcmp(type, "bool") == 0) return 1;
    if (strcmp(type, "float") == 0) return 4;
    if (strcmp(type, "double") == 0) return 8;
    return 0;
}

/* ================= MAIN ================= */
int main() {
    char fileName[100];
    printf("Enter filename: ");
    scanf("%s", fileName);
    
    // Use preprocessSource from la.h
    char *source = preprocessSource(fileName);
    if (!source) {
        printf("\n[ERROR] Failed to read file!\n");
        return 1;
    }
    
    printf("\n[INFO] File preprocessed successfully\n");

    SymbolTable st;
    initSymbolTable(&st);

    // Use Lexer from la.h to parse tokens
    Lexer lexer;
    initLexer(&lexer, source);
    
    char currentType[20] = "";
    Token tok;
    
    while (1) {
        tok = getNextToken(&lexer);
        
        // End of file
        if (strcmp(tok.lexeme, "EOF") == 0)
            break;
        
        // Check if it's a type keyword
        if (isTypeKeyword(tok.lexeme)) {
            strcpy(currentType, tok.lexeme);
        }
        // Check if it's an identifier and we have a current type
        else if (currentType[0] && isIdentifier(tok.lexeme)) {
            // Get next token to check if it's a function
            Token nextTok = getNextToken(&lexer);
            
            if (strcmp(nextTok.lexeme, "(") == 0) {
                // It's a function
                insert(&st, tok.lexeme, "FUNCTION", 0, currentType);
            } else {
                // It's a variable
                insert(&st, tok.lexeme, currentType, getSize(currentType), "-");
            }
        }
    }

    free(source);
    display(&st);
    return 0;
}
