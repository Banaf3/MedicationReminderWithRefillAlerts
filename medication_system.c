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
    RefillInfo refill;
} Medication;

typedef struct MedicationNode {
    Medication med;
    struct MedicationNode* next;
} MedicationNode;

#define STACK_SIZE 20
typedef struct {
    Medication items[STACK_SIZE];
    int top;
} MedicationStack;

#define QUEUE_SIZE 20
typedef struct {
    Medication items[QUEUE_SIZE];
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
void displayMedication(Medication med);

// Linked List Functions
void insertMedication(Medication med);
void deleteMedication(int medicationId);
void updateMedication(int medicationId);
void displayMedicationList();

// Stack Functions
void pushMedication(Medication med);
Medication popMedication();
void displayMedicationHistory();
int isStackEmpty();

// Queue Functions
void enqueueMedication(Medication med);
Medication dequeueMedication();
void displayRefillAlerts();
int isQueueEmpty();
int isQueueFull();

// Search and Sort Functions
void searchMedication();
void linearSearch(char* searchName);
void sortMedications();
void bubbleSort(Medication arr[], int n, int sortBy);
void selectionSort(Medication arr[], int n, int sortBy);
void displaySortedMedications(Medication arr[], int n);

int getMedicationCount();
void populateSampleData();

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
    medicationHistory.top = -1;
    refillAlerts.front = 0;
    refillAlerts.rear = -1;
    refillAlerts.count = 0;
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
    Medication med;
    
    printf("\n--- Enter Medication Details ---\n");
    printf("Medication ID (unique number): ");
    while(scanf("%d", &med.medicationId) != 1) {
        printf("Invalid input! Please enter a valid number for Medication ID: ");
        while(getchar() != '\n');
    }
    
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
    
    return med;
}

void displayMedication(Medication med) {
    printf("\n--- Medication Details ---\n");
    printf("ID: %d | Name: %s | Dosage: %s per tablet\n", med.medicationId, med.name, med.dosage);
    printf("Tablets Available: %d | Price per pack: $%.2f\n", med.quantity, med.price);
    printf("Prescription Refills Left: %d | Next Refill Due: %s\n", 
           med.refill.refillsRemaining, med.refill.nextRefillDate);
    printf("---------------------------\n");
}

void insertMedication(Medication med) {
    MedicationNode* newNode = (MedicationNode*)malloc(sizeof(MedicationNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    newNode->med = med;
    newNode->next = medicationList;
    medicationList = newNode;
    
    printf("Medication '%s' added successfully!\n", med.name);
}

void deleteMedication(int medicationId) {
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
    
    while (current != NULL && current->med.medicationId != medicationId) {
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
        displayMedication(current->med);
        current = current->next;
    }
}

void pushMedication(Medication med) {
    if (medicationHistory.top >= STACK_SIZE - 1) {
        printf("Medication history is full!\n");
        return;
    }
    
    medicationHistory.items[++medicationHistory.top] = med;
}

Medication popMedication() {
    if (isStackEmpty()) {
        printf("No medication history available!\n");
        Medication empty = {0};
        return empty;
    }
    
    return medicationHistory.items[medicationHistory.top--];
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
    
    refillAlerts.rear = (refillAlerts.rear + 1) % QUEUE_SIZE;
    refillAlerts.items[refillAlerts.rear] = med;
    refillAlerts.count++;
    
    printf("Refill alert queued for '%s'\n", med.name);
}

Medication dequeueMedication() {
    if (isQueueEmpty()) {
        printf("No refill alerts in queue!\n");
        Medication empty = {0};
        return empty;
    }
    
    Medication med = refillAlerts.items[refillAlerts.front];
    refillAlerts.front = (refillAlerts.front + 1) % QUEUE_SIZE;
    refillAlerts.count--;
    
    return med;
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

void searchMedication() {
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
            displayMedication(current->med);
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
    
    Medication* medArray = (Medication*)malloc(count * sizeof(Medication));
    MedicationNode* current = medicationList;
    int i = 0;
    
    while (current != NULL) {
        medArray[i++] = current->med;
        current = current->next;
    }
    
    printf("\n=== SORTING OPTIONS ===\n");
    printf("1. Sort by Name\n2. Sort by Price\n3. Sort by Quantity\n");
    printf("Enter sorting category (1-3): ");
    
    int sortBy;
    scanf("%d", &sortBy);
    
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

void bubbleSort(Medication arr[], int n, int sortBy) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int shouldSwap = 0;
            
            switch (sortBy) {
                case 1:
                    shouldSwap = strcmp(arr[j].name, arr[j + 1].name) > 0;
                    break;
                case 2:
                    shouldSwap = arr[j].price > arr[j + 1].price;
                    break;
                case 3:
                    shouldSwap = arr[j].quantity > arr[j + 1].quantity;
                    break;
            }
            
            if (shouldSwap) {
                Medication temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

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

void displaySortedMedications(Medication arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Sorted Entry %d ---", i + 1);
        displayMedication(arr[i]);
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
