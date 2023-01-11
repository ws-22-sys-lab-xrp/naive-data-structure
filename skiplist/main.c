#include <stdio.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_LEVEL 16

// Structure for a skip list node
typedef struct Node
{
    int key;
    int value;
    int level;
    struct Node *next[MAX_LEVEL];
    struct Node *prev[MAX_LEVEL];
} Node;

// Structure for the skip list
typedef struct SkipList
{
    Node *head;
    Node *tail;
    int max_level;
    int size;
} SkipList;

// Function prototypes
SkipList *create_skiplist();
void insert_node(SkipList *list, int key, int value);
void delete_node(SkipList *list, int key);
void print_skiplist(SkipList *list);
void save_skiplist(SkipList *list, char *filename);
SkipList *load_skiplist(char *filename);

// Create a new skip list
SkipList *create_skiplist()
{
    SkipList *list = malloc(sizeof(SkipList));
    list->head = malloc(sizeof(Node));
    list->tail = malloc(sizeof(Node));
    list->max_level = 0;
    list->size = 0;

    for (int i = 0; i < MAX_LEVEL; i++)
    {
        list->head->next[i] = list->tail;
        list->tail->prev[i] = list->head;
    }

    return list;
}

// Insert a new node into the skip list
void insert_node(SkipList *list, int key, int value)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->level = 0;

    Node *curr = list->head;
    for (int i = list->max_level; i >= 0; i--)
    {
        while (curr->next[i] != list->tail && curr->next[i]->key < key)
        {
            curr = curr->next[i];
        }
        new_node->next[i] = curr->next[i];
        new_node->prev[i] = curr;
        curr->next[i]->prev[i] = new_node;
        curr->next[i] = new_node;
    }

    list->size++;
}
void delete_node(SkipList *list, int key)
{
    Node *curr = list->head;
    Node *update[MAX_LEVEL + 1];
    memset(update, 0, sizeof(Node *) * (MAX_LEVEL + 1));

    for (int i = list->max_level; i >= 0; i--)
    {
        while (curr->next[i] != list->tail && curr->next[i]->key < key)
        {
            curr = curr->next[i];
        }
        update[i] = curr;
    }

    curr = curr->next[0];
    if (curr != list->tail && curr->key == key)
    {
        for (int i = 0; i <= list->max_level; i++)
        {
            if (update[i]->next[i] != curr)
            {
                break;
            }
            update[i]->next[i] = curr->next[i];
        }
        free(curr);

        // Update the max level
        while (list->max_level > 0 && list->head->next[list->max_level] == list->tail)
        {
            list->max_level--;
        }
    }
}
// Function to search for a specific node in the skip list
Node *search_node(SkipList *list, int key)
{
    Node *curr = list->head;
    for (int i = list->max_level; i >= 0; i--)
    {
        while (curr->next[i] != list->tail && curr->next[i]->key < key)
        {
            curr = curr->next[i];
        }
        if (curr->next[i]->key == key)
        {
            // Key found, return the node
            return curr->next[i];
        }
    }

    // Key not found
    return NULL;
}

// Function to save the skip list to a file
void save_skiplist(SkipList *list, char *filename)
{
    // Open the file for writing
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(1);
    }

    // Write the skip list structure to the file
    pwrite(fd, list, sizeof(SkipList), 0);

    // Write the nodes to the file
    Node *curr = list->head;
    for (int i = 0; i <= list->max_level; i++)
    {
        pwrite(fd, curr->next[i], sizeof(Node),
               sizeof(SkipList) + i * sizeof(Node));
        curr = curr->next[i];
    }

    close(fd);
}

// Function to load the skip list from a file
SkipList *load_skiplist(char *filename)
{
    // Open the file for reading
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(1);
    }

    // Read the skip list structure from the file
    SkipList *list = malloc(sizeof(SkipList));
    pread(fd, list, sizeof(SkipList), 0);

    // Read the nodes from the file
    Node *curr = list->head;
    for (int i = 0; i <= list->max_level; i++)
    {
        pread(fd, curr->next[i], sizeof(Node),
              sizeof(SkipList) + i * sizeof(Node));
        curr = curr->next[i];
    }

    close(fd);
    return list;
}

void print_skiplist(SkipList *list)
{
    Node *curr = list->head->next[0];
    while (curr != list->tail)
    {
        printf("Key: %d, Value: %d\n", curr->key, curr->value);
        curr = curr->next[0];
    }
}

// Function to read the elements of the skip list from a file
void read_skiplist(char *filename)
{
    // Open the file for reading
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    // Read the skip list structure from the file
    SkipList *list = malloc(sizeof(SkipList));
    fread(list, sizeof(SkipList), 1, fp);

    // Read the nodes from the file and print their keys and values
    for (int i = 0; i < list->size; i++)
    {
        Node *node = malloc(sizeof(Node));
        fread(node, sizeof(Node), 1, fp);
        printf("Key: %d, Value: %d\n", node->key, node->value);
    }

    fclose(fp);
}

// Function to search for a specific element in the skip list
int search_skiplist_fread(char *filename, int key)
{
    // Open the file for reading
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    // Read the skip list structure from the file
    SkipList *list = malloc(sizeof(SkipList));
    fread(list, sizeof(SkipList), 1, fp);

    // Search for the key in the skip list
    Node *curr = list->head;
    for (int i = list->max_level; i >= 0; i--)
    {
        // Read the next node from the file
        fread(curr->next[i], sizeof(Node), 1, fp);
        while (curr->next[i] != list->tail && curr->next[i]->key < key)
        {
            // Move to the next node
            curr = curr->next[i];
            // Read the next node from the file
            fread(curr->next[i], sizeof(Node), 1, fp);
        }
        if (curr->next[i]->key == key)
        {
            // Key found, return the value
            int value = curr->next[i]->value;
            fclose(fp);
            return value;
        }
    }

    // Key not found
    fclose(fp);
    return -1;
}

int main()
{
    int num = 1000;
    int result;
    char *filename = "skiplist.dat";
    // Create and populate the skip list
    SkipList *list = create_skiplist();

    for (int i = 0; i < num; i++)
    {
        insert_node(list, i, 10 * i);
    }

    delete_node(list, 2);
    // result = search_node(list, 1)->value;
    // printf("Searching Result is %d\n", result);

    result = search_node(list, 3)->value;
    printf("Searching Result is %d\n", result);

    // Save the skip list to a file
    save_skiplist(list, "skiplist.dat");

    // Load the skip list from the file
    SkipList *loaded_list = load_skiplist(filename);
    result = search_node(list, 3)->value;
    printf("Latest Searching Result is %d\n", result);

    // Print the loaded skip list
    // print_skiplist(loaded_list);

    printf("Everything is prepared!\n\n");
    printf("Starting Directly Read from Disk...\n");

    for (int key = 0; key <= 100; key++)
    {
        result = search_skiplist_fread(filename, key);
        printf("Disk Searching Result is %d\n", result);
    }

    return 0;
}