#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== STRUCTURE DEFINITIONS =====

typedef struct {
    int refillsRemaining;
    char nextRefillDate[12];
} RefillInfo;

typedef struct {
    int medicationId;
    char name[50];
    char dosage[20];
    int quantity;
    float price;
    RefillInfo refill; // Nested structure (1)
} Medication;

typedef struct MedicationNode {
    Medication med; // Nested structure (2) 
    struct MedicationNode* next;
} MedicationNode;

#define STACK_SIZE 20 // 
typedef struct {
    Medication items[STACK_SIZE]; // Array of Medication structures (Nested structure 3)
    int top;
} MedicationStack;

#define QUEUE_SIZE 20
typedef struct {
    Medication items[QUEUE_SIZE]; // Array of Medication structures (Nested structure 4) 
    int front;
    int rear;
    int count;
} RefillQueue;

// ===== GLOBAL VARIABLES =====
MedicationNode* medicationList = NULL;
MedicationStack medicationHistory;
RefillQueue refillAlerts;

// ===== FUNCTION DECLARATIONS =====
void initializeSystem();
void displayMenu();
int getMenuChoice();
Medication createMedication();
void displayMedication(Medication med);         // Here, a Medication structure is passed by value to the function (Passing 1)

// Linked List Functions
void insertMedication(Medication med);        // Here, a Medication structure is passed by value be inserted in the linked list (Passing 2)
void deleteMedication(int medicationId);
void updateMedication(int medicationId);
void displayMedicationList();

// Stack Functions
void pushMedication(Medication med);        // Here, a Medication structure is passed to be added to the stack (Passing 3)
Medication popMedication();
void displayMedicationHistory();
int isStackEmpty();

// Queue Functions
void enqueueMedication(Medication med);     // Here, a Medication structure is passed to be added to the queue (Passing 4)
Medication dequeueMedication();
void displayRefillAlerts();
int isQueueEmpty();
int isQueueFull();

// Search and Sort Functions
void searchMedication();
void linearSearch(char* searchName);
void sortMedications();
void bubbleSort(Medication arr[], int n, int sortBy);       // Here, an array of Medication structures is passed to be sorted (Passing 5)
void selectionSort(Medication arr[], int n, int sortBy);    // Here, an array of Medication structures is passed to be sorted (Passing 6)
void displaySortedMedications(Medication arr[], int n);     // Here, an array of Medication structures is passed to be displayed (Passing 7)

int getMedicationCount();
void populateSampleData();

// Post-Testing Functions
void cleanupSystem();
int isDuplicateId(int medicationId);

// ===== MAIN FUNCTION =====
int main() {
    initializeSystem();
    populateSampleData();
    
    printf("=== MEDICATION REMINDER SYSTEM WITH REFILL ALERTS ===\n");
    printf("Welcome to the Medication Management System!\n\n");
    
    int choice;
    do {
        displayMenu();
        choice = getMenuChoice();
        
        switch(choice) {
            case 1: {
                printf("\n=== LINKED LIST OPERATIONS ===\n");
                printf("1. Add Medication\n2. Delete Medication\n3. Update Medication\n4. Display All Medications\n");
                printf("Enter choice (1-4): ");
                
                int listChoice;
                scanf("%d", &listChoice);
                
                switch(listChoice) {
                    case 1: {
                        Medication newMed = createMedication();
                        insertMedication(newMed);
                        pushMedication(newMed);
                        break;
                    }
                    case 2: {
                        if (getMedicationCount() == 0) {
                            printf("No medications to delete!\n");
                            break;
                        }
                        printf("Enter Medication ID to delete: ");
                        int id;
                        scanf("%d", &id);
                        deleteMedication(id);
                        break;
                    }
                    case 3: {
                        if (getMedicationCount() == 0) {
                            printf("No medications to update!\n");
                            break;
                        }
                        printf("Enter Medication ID to update: ");
                        int id;
                        scanf("%d", &id);
                        updateMedication(id);
                        break;
                    }
                    case 4:
                        displayMedicationList();
                        break;
                    default:
                        printf("Invalid choice!\n");
                }
                break;
            }
                
            case 2: {
                // Stack Operations (Medication History)
                if (getMedicationCount() == 0) {
                    printf("No medications in the system!\n");
                    break;
                }
                printf("\n=== STACK OPERATIONS (Medication History) ===\n");
                printf("1. View Recent Medication\n2. Remove Recent from History\n3. Display All History\n");
                printf("Enter choice (1-3): ");
                
                int stackChoice;
                scanf("%d", &stackChoice);
                
                switch(stackChoice) {
                    case 1:
                        if (!isStackEmpty()) {
                            printf("Most recent medication added:\n");
                            displayMedication(medicationHistory.items[medicationHistory.top]);
                        } else {
                            printf("No medication history available!\n");
                        }
                        break;
                    case 2:
                        if (!isStackEmpty()) {
                            Medication removed = popMedication();
                            printf("Removed from history: %s\n", removed.name);
                        } else {
                            printf("No medication history to remove!\n");
                        }
                        break;
                    case 3:
                        displayMedicationHistory();
                        break;
                    default:
                        printf("Invalid choice!\n");
                }
                break;
            }
                
            case 3: {
                // Queue Operations (Refill Alerts)
                if (getMedicationCount() == 0) {
                    printf("No medications in the system!\n");
                    break;
                }
                printf("\n=== QUEUE OPERATIONS (Refill Alerts) ===\n");
                printf("1. Add Refill Alert\n2. Process Next Alert\n3. Display All Alerts\n");
                printf("Enter choice (1-3): ");
                
                int queueChoice;
                scanf("%d", &queueChoice);
                
                switch(queueChoice) {
                    case 1: {
                        displayMedicationList();
                        if (getMedicationCount() == 0) {
                            // Already handled above, but double check after display
                            break;
                        }
                        printf("Enter Medication ID to add refill alert: ");
                        int id;
                        scanf("%d", &id);
                        
                        MedicationNode* current = medicationList;
                        int found = 0;
                        while (current != NULL) {
                            if (current->med.medicationId == id) {
                                enqueueMedication(current->med);
                                printf("Refill alert added for %s\n", current->med.name);
                                found = 1;
                                break;
                            }
                            current = current->next;
                        }
                        if (!found) {
                            printf("Medication with ID %d not found!\n", id);
                        }
                        break;
                    }
                    case 2:
                        if (!isQueueEmpty()) {
                            Medication processed = dequeueMedication();
                            printf("Processed refill alert for: %s\n", processed.name);
                        } else {
                            printf("No refill alerts to process!\n");
                        }
                        break;
                    case 3:
                        displayRefillAlerts();
                        break;
                    default:
                        printf("Invalid choice!\n");
                }
                break;
            }
                
            case 4:
                searchMedication();
                break;
                
            case 5:
                sortMedications();
                break;
                
            case 6:
                printf("Thank you for using the Medication Reminder System!\n");
                cleanupSystem(); // Calling the function of "cleanupSystem" to free memory at program termination.
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
        if (choice != 6) {
            printf("\nPress any key to continue...");
            while(getchar() != '\n');
            getchar();
        }
        
    } while (choice != 6);
    
    return 0;
}

// ===== FUNCTION IMPLEMENTATIONS =====

void initializeSystem() {
    medicationList = NULL; 
    medicationHistory.top = -1; // Access and assign to stack structure element (1)
    refillAlerts.front = 0;     // Access and assign to queue structure element (2)
    refillAlerts.rear = -1;     // Access and assign to queue structure element (3)
    refillAlerts.count = 0;     // Access and assign to queue structure element (4) 
}

void displayMenu() {
    printf("\n============================================\n");
    printf("         MEDICATION MANAGEMENT MENU\n");
    printf("============================================\n");
    printf("1. Linked List Operations (Medications)\n");
    printf("2. Stack Operations (Medication History)\n");
    printf("3. Queue Operations (Refill Alerts)\n");
    printf("4. Search Medications\n");
    printf("5. Sort Medications\n");
    printf("6. Exit Program\n");
    printf("============================================\n");
    printf("Enter your choice (1-6): ");
}

int getMenuChoice() {
    int choice;
    while(scanf("%d", &choice) != 1) {
        printf("Invalid input! Please enter a number (1-6): ");
        while(getchar() != '\n');
    }
    return choice;
}

Medication createMedication() {
    // Function body that fills the structure
    Medication med;  // Structure variable creation
    int validID = 0; // Flag to check if the ID is unique
    
    printf("\n--- Enter Medication Details ---\n");

    while (!validID) {
        printf("Medication ID (unique number): ");
        while(scanf("%d", &med.medicationId) != 1) { // Access and assign to structure element (5) 
            printf("Invalid input! Please enter a valid number for Medication ID: ");
            while(getchar() != '\n');
        }
    

    //Check if ID already exists
    if (isDuplicateId(med.medicationId)) {
        printf("Medication ID %d already exists! Please enter a unique ID.\n", med.medicationId);
    } else {
        validID = 1; // Set flag to true if ID is valid and unique
    }
}

    // Proceed to collect other medication details if ID is valid
    printf("Medication Name (e.g., Aspirin, Paracetamol): ");
    scanf(" %[^\n]", med.name);
    
    printf("Dosage per tablet/capsule (e.g., 500mg, 10mg): ");
    scanf(" %[^\n]", med.dosage);
    
    printf("Quantity (how many tablets/capsules you have): ");
    while(scanf("%d", &med.quantity) != 1) {
        printf("Invalid input! Please enter a valid number for quantity: ");
        while(getchar() != '\n');
    }
    
    printf("Price per bottle/pack: $");
    while(scanf("%f", &med.price) != 1) {
        printf("Invalid input! Please enter a valid price: $");
        while(getchar() != '\n');
    }
    
    printf("Refills Remaining (how many times you can refill this prescription): ");
    while(scanf("%d", &med.refill.refillsRemaining) != 1) {
        printf("Invalid input! Please enter a valid number for refills: ");
        while(getchar() != '\n');
    }
    
    printf("Next Refill Date when you need to refill (DD/MM/YYYY): ");
    scanf(" %[^\n]", med.refill.nextRefillDate);
    
    return med; // This function returns a fully populated Medication structure 
} 

void displayMedication(Medication med) {

    // Function body that uses the passed Medication structure to display its details
    printf("\n--- Medication Details ---\n");
    printf("ID: %d | Name: %s | Dosage: %s per tablet\n", med.medicationId, med.name, med.dosage);  // Access and display structure elements (6 + 7 + 8) 
    printf("Tablets Available: %d | Price per pack: $%.2f\n", med.quantity, med.price);             // Access and display structure elements (9 + 10) 
    printf("Prescription Refills Left: %d | Next Refill Due: %s\n", 
           med.refill.refillsRemaining, med.refill.nextRefillDate);                                 // Access and display structure elements (11 + 12) 
    printf("---------------------------\n");
}

void insertMedication(Medication med) {

    // Implements insertion into a linked list through the function 
    MedicationNode* newNode = (MedicationNode*)malloc(sizeof(MedicationNode)); // Structure creation 
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->med = med;             // Assign entire structure to element
    newNode->next = medicationList;  
    medicationList = newNode;
    
    printf("Medication '%s' added successfully!\n", med.name);
}

void deleteMedication(int medicationId) {

    // Implements deletion from a linked list through the function 
    if (medicationList == NULL) {
        printf("No medications to delete!\n");
        return;
    }
    
    // First, display all medications for reference
    printf("\n=== CURRENT MEDICATIONS ===\n");
    MedicationNode* temp = medicationList;
    int count = 1;
    while (temp != NULL) {
        printf("%d. ID: %d - %s (%s)\n", count++, temp->med.medicationId, temp->med.name, temp->med.dosage);
        temp = temp->next;
    }
    printf("\n");
    
    MedicationNode* current = medicationList;
    MedicationNode* previous = NULL;
    
    if (current->med.medicationId == medicationId) {
        medicationList = current->next;
        printf("Medication '%s' deleted successfully!\n", current->med.name);
        free(current);
        return;
    }

    // Search for the medication to delete
    while (current != NULL && current->med.medicationId != medicationId) { // Access nested structure element (14)  
        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Medication with ID %d not found!\n", medicationId);
        return;
    }
    
    previous->next = current->next;
    printf("Medication '%s' deleted successfully!\n", current->med.name);
    free(current);
}

void updateMedication(int medicationId) {

    // Implements update of a medication in the linked list through the function 
    MedicationNode* current = medicationList;
    
    while (current != NULL) {
        if (current->med.medicationId == medicationId) {
            printf("Current medication details:\n");
            displayMedication(current->med);
            
            printf("\nEnter new details:\n");
            current->med = createMedication();
            current->med.medicationId = medicationId;
            
            printf("Medication updated successfully!\n");
            return;
        }
        current = current->next;
    }
    
    printf("Medication with ID %d not found!\n", medicationId);
}

void displayMedicationList() {
    if (medicationList == NULL) {
        printf("No medications in the system!\n");
        return;
    }
    
    printf("\n=== ALL MEDICATIONS ===\n");
    MedicationNode* current = medicationList;
    int count = 1;
    
    while (current != NULL) {
        printf("\n--- Medication %d ---", count++);
        displayMedication(current->med); // Access structure element (13) 
        current = current->next;         // Access structure element (14) + assign to structure element (15) 
    }
}

void pushMedication(Medication med) {
    if (medicationHistory.top >= STACK_SIZE - 1) { // Access structure element (16) 
        printf("Medication history is full!\n");
        return;
    }
    
    medicationHistory.items[++medicationHistory.top] = med; // Access array in structure and assign (17) 
}

Medication popMedication() {
    // Function body that removes and returns a medication from the stack 
    if (isStackEmpty()) {
        printf("No medication history available!\n");
        Medication empty = {0};
        return empty;
    }

    return medicationHistory.items[medicationHistory.top--]; // This function returns a Medication structure from the top of the stack + Access array in structure (18) 
}

void displayMedicationHistory() {
    if (isStackEmpty()) {
        printf("No medication history available!\n");
        return;
    }
    
    printf("\n=== MEDICATION HISTORY (Most Recent First) ===\n");
    for (int i = medicationHistory.top; i >= 0; i--) {
        printf("\n--- History Entry %d ---", medicationHistory.top - i + 1);
        displayMedication(medicationHistory.items[i]);
    }
}

int isStackEmpty() {
    return medicationHistory.top == -1;
}

void enqueueMedication(Medication med) {
    if (isQueueFull()) {
        printf("Refill alert queue is full!\n");
        return;
    }
    
    refillAlerts.rear = (refillAlerts.rear + 1) % QUEUE_SIZE; // Access and modify structure element (19) 
    refillAlerts.items[refillAlerts.rear] = med;             // Access array in structure and assign (20) 
    refillAlerts.count++;                                   // Access and modify structure element (21) 
    
    printf("Refill alert queued for '%s'\n", med.name);
}

Medication dequeueMedication() {
    // Function body that removes and returns a medication from the queue 
    if (isQueueEmpty()) {
        printf("No refill alerts in queue!\n");
        Medication empty = {0};
        return empty;
    }
    
    Medication med = refillAlerts.items[refillAlerts.front];        // Access array in structure (22)
    refillAlerts.front = (refillAlerts.front + 1) % QUEUE_SIZE;    // Access and modify structure element (23)
    refillAlerts.count--;                                         // Access and modify structure element (24) 
    
    return med; // This function returns a Medication structure from the front of the queue  
}

void displayRefillAlerts() {
    if (isQueueEmpty()) {
        printf("No refill alerts in queue!\n");
        return;
    }
    
    printf("\n=== REFILL ALERTS QUEUE ===\n");
    int index = refillAlerts.front;
    for (int i = 0; i < refillAlerts.count; i++) {
        printf("\n--- Alert %d ---", i + 1);
        displayMedication(refillAlerts.items[index]);
        index = (index + 1) % QUEUE_SIZE;
    }
}

int isQueueEmpty() {
    return refillAlerts.count == 0;
}

int isQueueFull() {
    return refillAlerts.count == QUEUE_SIZE;
}

void searchMedication() { // Implements linear sequential search method through the function 
    printf("\n=== MEDICATION SEARCH ===\n");
    printf("Enter medication name to search: ");
    char searchName[50];
    scanf(" %[^\n]", searchName);
    
    linearSearch(searchName);
}

void linearSearch(char* searchName) {
    MedicationNode* current = medicationList;
    int found = 0;
    
    printf("\n=== SEARCH RESULTS ===\n");
    
    while (current != NULL) {
        if (strstr(current->med.name, searchName) != NULL ||
            strstr(searchName, current->med.name) != NULL) {
            displayMedication(current->med); // Access structure element (25) 
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No medications found matching '%s'\n", searchName);
    }
}

void sortMedications() {
    int count = getMedicationCount();
    if (count == 0) {
        printf("No medications to sort!\n");
        return;
    }
    
    Medication* medArray = (Medication*)malloc(count * sizeof(Medication)); // Create an array of Medication structures (26)

    // Populate the array from the linked list
    MedicationNode* current = medicationList;
    int i = 0;
    
    while (current != NULL) {
        medArray[i++] = current->med; // Assign structure to array element (27) 
        current = current->next;
    }
    
    // Provides the user with multiple options for sorting 
    printf("\n=== SORTING OPTIONS ===\n");
    printf("1. Sort by Name\n2. Sort by Price\n3. Sort by Quantity\n");
    printf("Enter sorting category (1-3): ");
    
    int sortBy;
    scanf("%d", &sortBy);
    
    // Provides the user with multiple options for sorting algorithms 
    printf("\nChoose sorting algorithm:\n");
    printf("1. Bubble Sort\n2. Selection Sort\n");
    printf("Enter choice (1-2): ");
    
    int algorithm;
    scanf("%d", &algorithm);
    
    if (algorithm == 1) {
        bubbleSort(medArray, count, sortBy);
        printf("\nMedications sorted using Bubble Sort:\n");
    } else if (algorithm == 2) {
        selectionSort(medArray, count, sortBy);
        printf("\nMedications sorted using Selection Sort:\n");
    } else {
        printf("Invalid algorithm choice!\n");
        free(medArray);
        return;
    }
    
    displaySortedMedications(medArray, count);
    free(medArray);
}

// Bubble Sort and Selection Sort implementations
void bubbleSort(Medication arr[], int n, int sortBy) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int shouldSwap = 0;
            
            switch (sortBy) {
                case 1: // Sort by Name 
                    shouldSwap = strcmp(arr[j].name, arr[j + 1].name) > 0;   // Access structure element (28)
                    break;
                case 2: // Sort by Price
                    shouldSwap = arr[j].price > arr[j + 1].price;           // Access structure element (29) 
                    break;
                case 3: // Sort by Quantity
                    shouldSwap = arr[j].quantity > arr[j + 1].quantity;    // Access structure element (30) 
                    break;
            }
            
            if (shouldSwap) {
                Medication temp = arr[j]; // Structure assignment (31) 
                arr[j] = arr[j + 1]; // Structure assignment (32) 
                arr[j + 1] = temp; // Structure assignment (33) 
            }
        }
    }
}

// Selection Sort implementation 
void selectionSort(Medication arr[], int n, int sortBy) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        
        for (int j = i + 1; j < n; j++) {
            int shouldSelect = 0;
            
            switch (sortBy) {
                case 1:
                    shouldSelect = strcmp(arr[j].name, arr[minIndex].name) < 0;
                    break;
                case 2:
                    shouldSelect = arr[j].price < arr[minIndex].price;
                    break;
                case 3:
                    shouldSelect = arr[j].quantity < arr[minIndex].quantity;
                    break;
            }
            
            if (shouldSelect) {
                minIndex = j;
            }
        }
        
        if (minIndex != i) {
            Medication temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

// After sorting, display the sorted medications list to the user  
void displaySortedMedications(Medication arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Sorted Entry %d ---", i + 1);
        displayMedication(arr[i]); // Access structure element (34) 
    }
}

int getMedicationCount() {
    int count = 0;
    MedicationNode* current = medicationList;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void populateSampleData() {
    // Start with empty system - no pre-loaded data
    printf("Medication system initialized - ready to add medications!\n");
}

// Freeing allocated memory at program termination
void cleanupSystem() {
    MedicationNode* current = medicationList;
    MedicationNode* next;
    while (current != NULL) {
        next = current->next; // Access structure element (36) 
        free(current);
        current = next;
    }
    medicationList = NULL;
    printf("System Cleanup complete. All Memory Freed.\n");
}

// Adding a function to prevent duplicate medication IDs
int isDuplicateId(int medicationId) {
    MedicationNode* current = medicationList;
    while (current != NULL) {
        if (current->med.medicationId == medicationId) { // Access nested structure element (35) 
            return 1; // ID already exists
        }
        current = current->next;
    }
    return 0; // ID is unique 
}