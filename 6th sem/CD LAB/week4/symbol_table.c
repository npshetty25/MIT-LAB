#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
int isKeyword(const char *str) {
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

/* ================= REMOVE COMMENTS ================= */
char* removeComments(const char *fileName) {
    FILE *fp = fopen(fileName, "r");
    if (!fp) return NULL;
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *content = (char *)malloc(size + 1);
    char *result = (char *)malloc(size + 1);
    fread(content, 1, size, fp);
    content[size] = '\0';
    fclose(fp);
    
    int i = 0, j = 0;
    while (i < size) {
        // String literals - skip completely
        if (content[i] == '"') {
            i++;
            while (i < size) {
                if (content[i] == '"' && (i == 0 || content[i-1] != '\\')) {
                    i++;
                    break;
                }
                i++;
            }
            result[j++] = ' '; // Replace with space
        }
        // Single-line comment
        else if (content[i] == '/' && i+1 < size && content[i+1] == '/') {
            while (i < size && content[i] != '\n') i++;
        }
        // Multi-line comment
        else if (content[i] == '/' && i+1 < size && content[i+1] == '*') {
            i += 2;
            while (i < size - 1 && !(content[i] == '*' && content[i+1] == '/')) i++;
            i += 2;
        }
        // Preprocessor directives
        else if (content[i] == '#') {
            while (i < size && content[i] != '\n') i++;
        }
        else {
            result[j++] = content[i++];
        }
    }
    result[j] = '\0';
    free(content);
    return result;
}

/* ================= MAIN ================= */
int main() {
   char fileName[100];
    printf("Enter filename: ");
    scanf("%s", fileName);
    
    // Remove comments
    char *source = removeComments(fileName);
    if (!source) {
        printf("\n[ERROR] Failed to read file!\n");
        return 1;
    }
    
    printf("\n[INFO] Reading file: %s\n", fileName);

    SymbolTable st;
    initSymbolTable(&st);

    // Parse tokens manually to detect functions
    int i = 0;
    char currentType[20] = "";
    
    while (i < strlen(source)) {
        // Skip whitespace
        while (i < strlen(source) && isspace(source[i])) i++;
        if (i >= strlen(source)) break;
        
        // Read token
        char token[100] = "";
        int ti = 0;
        while (i < strlen(source) && !isspace(source[i]) && 
               source[i] != '(' && source[i] != ')' && 
               source[i] != '{' && source[i] != '}' &&
               source[i] != '[' && source[i] != ']' &&
               source[i] != ',' && source[i] != ';' &&
               source[i] != '*' && source[i] != '=' &&
               source[i] != '+' && source[i] != '-' &&
               source[i] != '/' && source[i] != '%' &&
               source[i] != '<' && source[i] != '>' &&
               source[i] != '!' && source[i] != '&' &&
               source[i] != '|') {
            token[ti++] = source[i++];
        }
        token[ti] = '\0';
        
        if (strlen(token) == 0) {
            // Just skip the delimiter
            if (i < strlen(source) && !isspace(source[i])) i++;
            continue;
        }
        
        // Check if keyword
        if (isKeyword(token)) {
            strcpy(currentType, token);
            // Skip delimiter after keyword
            if (i < strlen(source) && !isspace(source[i])) i++;
        }
        // Check if identifier
        else if (currentType[0] && isalpha(token[0])) {
            // Look ahead to see if this is a function
            int j = i;
            while (j < strlen(source) && isspace(source[j])) j++;
            
            if (j < strlen(source) && source[j] == '(') {
                // It's a function
                insert(&st, token, "FUNCTION", 0, currentType);
            } else {
                // It's a variable
                insert(&st, token, currentType, getSize(currentType), "-");
            }
            // Skip delimiter after identifier
            if (i < strlen(source) && !isspace(source[i])) i++;
        }
        else {
            // Skip delimiter for other cases
            if (i < strlen(source) && !isspace(source[i])) i++;
        }
    }

    free(source);
    display(&st);
    return 0;
}
