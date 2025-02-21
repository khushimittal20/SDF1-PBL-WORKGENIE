#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#include<string.h>
#define MaxEntry 1000
void Add(FILE *fptr){
    char entry[MaxEntry];
    char timestamp[50];
    printf("Enter the timestamp for the entry (YYYY-MM-DD HH:MM:SS): ");
    fgets(timestamp, sizeof(timestamp),stdin);
    timestamp[strcspn(timestamp, "\n")]='\0';   
    printf("Enter your journal entry (upto %d characters):\n",MaxEntry-1);
    fgets(entry, sizeof(entry),stdin);
    entry[strcspn(entry, "\n")]='\0';
    fprintf(fptr,"Timestamp:%s\n",timestamp);
    fprintf(fptr,"Entry:%s\n",entry);
    printf("Entry added.\n");
}
void View(FILE *fptr){
    char ch;
    fseek(fptr,0,SEEK_SET);
    printf("All journal Entries:\n");
    while((ch=fgetc(fptr))!=EOF){
        putchar(ch);
    }
}
void Delete(FILE *fptr) { 
    FILE *tempFile = fopen("temp_journal.txt", "w+");
    if (tempFile == NULL) {
        printf("Error opening temporary file.\n");
        return;
    }

    char line[MaxEntry];
    int entryCount = 0;
    int entryToDelete;
    int currentEntry = 0;
    char timestamp[50];
    char entry[MaxEntry];
    int totalEntries = 0;

    fseek(fptr, 0, SEEK_SET);
    while (fgets(line, sizeof(line), fptr)) {
        if (entryCount % 2 == 0) {
            totalEntries++;
        }
        entryCount++;
    }

    printf("Enter the entry number you want to delete (1 to %d):\n", totalEntries);
    if (scanf("%d", &entryToDelete) != 1 || entryToDelete < 1 || entryToDelete > totalEntries) {
        printf("Invalid entry number. Please try again.\n");
        getchar();
        fclose(tempFile);
        return;
    }
    getchar();

    fseek(fptr, 0, SEEK_SET);

    entryCount = 0;
    currentEntry = 1;

    while (fgets(line, sizeof(line), fptr)) {
        if (entryCount % 2 == 0) {
            snprintf(timestamp, sizeof(timestamp), "%s", line);
        } else {
            snprintf(entry, sizeof(entry), "%s", line);

            if (currentEntry == entryToDelete) {
                printf("Deleting entry: %s %s\n", timestamp, entry);
            } else {
                fprintf(tempFile, "%s", timestamp);
                fprintf(tempFile, "%s", entry);
            }
            currentEntry++;
        }
        entryCount++;
    }

    fclose(fptr);
    fclose(tempFile);

    if (remove("journal.txt") == 0) {
        if (rename("temp_journal.txt", "journal.txt") == 0) {
            printf("Entry deleted successfully.\n");
        } else {
            printf("Error renaming the temporary file.\n");
            rename("journal.txt", "temp_journal.txt");
        }
    } else {
        printf("Error deleting the original journal file.\n");
    }

    fptr = fopen("journal.txt", "a+");
    if (fptr == NULL) {
        printf("Error reopening the journal file.\n");
    }
}
void Search(FILE *fptr, const char *keyword) {
    char line[MaxEntry];
    int entryCount = 0;
    int found = 0;
    char timestamp[50];
    char entry[MaxEntry];

    fseek(fptr, 0, SEEK_SET);

    while (fgets(line, sizeof(line), fptr)) {
        if (entryCount % 2 == 0) {
            snprintf(timestamp, sizeof(timestamp), "%s", line);
        } else {
            snprintf(entry, sizeof(entry), "%s", line);
            if (strstr(timestamp, keyword) != NULL || strstr(entry, keyword) != NULL) {
                printf("Found matching entry:\n");
                printf("%s", timestamp);
                printf("%s", entry);
                printf("\n");
                found = 1;
            }
        }
        entryCount++;
    }

    if (!found) {
        printf("No entries found containing the keyword: %s\n", keyword);
    }
}
#define maxTask 100
#define TaskLength 256
#define DateLength 11

typedef struct {
    char task[TaskLength];
    int Completed;
    char priority[10];
    char dueDate[DateLength];
} Task;

Task tasks[maxTask];
int taskCount = 0;
void addTask(char *task, char *priority, char *dueDate) { 
    if (taskCount < maxTask) {
        task[strcspn(task, "\n")] = '\0';  
        priority[strcspn(priority, "\n")] = '\0'; 
        dueDate[strcspn(dueDate, "\n")] = '\0';  
        
        strncpy(tasks[taskCount].task, task, TaskLength - 1); 
        tasks[taskCount].task[TaskLength - 1] = '\0'; 
        tasks[taskCount].Completed = 0;
        
        strncpy(tasks[taskCount].priority, priority, 10);  
        tasks[taskCount].priority[9] = '\0';  
        
        strncpy(tasks[taskCount].dueDate, dueDate, DateLength);  
        tasks[taskCount].dueDate[DateLength - 1] = '\0';  

        taskCount++;
        printf("Task added: %s\n", task);
    } else {
        printf("Task limit reached. Cannot add more tasks.\n");
    }
}


/*void addTask(const char *task, const char *priority, const char *dueDate) {
    if (taskCount < maxTask) {
        strncpy(tasks[taskCount].task, task, TaskLength);
        tasks[taskCount].Completed = 0;
        strncpy(tasks[taskCount].priority, priority, 10);
        strncpy(tasks[taskCount].dueDate, dueDate, DateLength);
        taskCount++;
        printf("Task added: %s\n", task);
    } else {
        printf("Task limit reached. Cannot add more tasks.\n");
    }
}*/

void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    for (int i = 0; i < taskCount; i++) {
        printf("%d. [%s] %s (Priority: %s, Due: %s)\n", i + 1, tasks[i].Completed ? "X" : " ", tasks[i].task, tasks[i].priority, tasks[i].dueDate);
    }
}

void markTaskAsCompleted(int index) {
    if (index >= 0 && index < taskCount) {
        tasks[index].Completed = 1;
        printf("Task marked as completed: %s\n", tasks[index].task);
    } else {
        printf("Invalid task number.\n");
    }
}

void deleteTask(int index) {
    if (index >= 0 && index < taskCount) {
        for (int i = index; i < taskCount - 1; i++) {
            tasks[i] = tasks[i + 1];
        }
        taskCount--;
        printf("Task deleted.\n");
    } else {
        printf("Invalid task number.\n");
    }
}

void printStatistics() {
    int completedTasks = 0;
    int pendingTasks = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].Completed)
            completedTasks++;
        else
            pendingTasks++;
    }
    printf("Total tasks: %d\n", taskCount);
    printf("Completed tasks: %d\n", completedTasks);
    printf("Pending tasks: %d\n", pendingTasks);
}

int comparePriority(const void *a, const void *b) {
    return strcmp(((Task*)a)->priority, ((Task*)b)->priority);
}

void sortTasksByPriority() {
    qsort(tasks, taskCount, sizeof(Task), comparePriority);
    printf("Tasks sorted by priority.\n");
}

void todoMenu() {
    printf("\nTo-Do List Menu:\n");
    printf("1. Add Task\n");
    printf("2. View Tasks\n");
    printf("3. Mark Task as Completed\n");
    printf("4. Delete Task\n");
    printf("5. View Task Statistics\n");
    printf("6. Sort Tasks by Priority\n");
    printf("7. Exit\n");
    printf("Choose an option: ");
}
#define _USE_MATH_DEFINES
#define PI 3.14159265358979323846

void showMenu() {
    printf("Scientific Calculator\n");
    printf("=======================\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Sine (sin)\n");
    printf("6. Cosine (cos)\n");
    printf("7. Tangent (tan)\n");
    printf("8. Logarithm (log base 10)\n");
    printf("9. Natural Logarithm (ln)\n");
    printf("10. Exponentiation (x^y)\n");
    printf("11. Square Root\n");
    printf("12. Exit\n");
}

void calc() {
    int choice;
    double num1, num2, result;

    while (1) {
        showMenu();
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter two numbers: ");
                scanf("%lf %lf", &num1, &num2);
                result = num1 + num2;
                printf("Result: %.2lf\n", result);
                break;

            case 2:
                printf("Enter two numbers: ");
                scanf("%lf %lf", &num1, &num2);
                result = num1 - num2;
                printf("Result: %.2lf\n", result);
                break;

            case 3:
                printf("Enter two numbers: ");
                scanf("%lf %lf", &num1, &num2);
                result = num1 * num2;
                printf("Result: %.2lf\n", result);
                break;

            case 4:
                printf("Enter two numbers: ");
                scanf("%lf %lf", &num1, &num2);
                if (num2 != 0) {
                    result = num1 / num2;
                    printf("Result: %.2lf\n", result);
                } else {
                    printf("Error: Division by zero!\n");
                }
                break;

            case 5:
                printf("Enter angle in degrees: ");
                scanf("%lf", &num1);
                result = sin(num1 * (PI / 180.0));
                printf("Sine: %.2lf\n", result);
                break;

            case 6:
                printf("Enter angle in degrees: ");
                scanf("%lf", &num1);
                result = cos(num1 * (PI / 180.0));
                printf("Cosine: %.2lf\n", result);
                break;

            case 7:
                printf("Enter angle in degrees: ");
                scanf("%lf", &num1);
                result = tan(num1 * (PI / 180.0));
                printf("Tangent: %.2lf\n", result);
                break;

            case 8:
                printf("Enter a number: ");
                scanf("%lf", &num1);
                if (num1 > 0) {
                    result = log10(num1);
                    printf("Result: %.2lf\n", result);
                } else {
                    printf("Error: Logarithm undefined for non-positive values!\n");
                }
                break;

            case 9:
                printf("Enter a number: ");
                scanf("%lf", &num1);
                if (num1 > 0) {
                    result = log(num1);
                    printf("Result: %.2lf\n", result);
                } else {
                    printf("Error: Natural logarithm undefined for non-positive values!\n");
                }
                break;

            case 10:
                printf("Enter base and exponent: ");
                scanf("%lf %lf", &num1, &num2);
                result = pow(num1, num2);
                printf("Result: %.2lf\n", result);
                break;

            case 11:
                printf("Enter a number: ");
                scanf("%lf", &num1);
                if (num1 >= 0) {
                    result = sqrt(num1);
                    printf("Result: %.2lf\n", result);
                } else {
                    printf("Error: Square root undefined for negative values!\n");
                }
                break;

            case 12:
                printf("Exiting the calculator. Goodbye!\n");
                return;
                
            default:
                printf("Invalid option! Please try again.\n");
        }
        printf("\n");
    }
}
#define MAX_CHALLENGES 5
#define MAX_QUOTES 5

typedef struct {
    int health;
    int productivity;
    int mindfulness;
} UserProgress;

void showChallenge(int category) {
    const char *healthChallenges[] = {
        "Walk 10,000 steps today.",
        "Drink 8 glasses of water.",
        "Sleep for at least 8 hours.",
        "Eat 5 servings of vegetables today.",
        "Take a 30-minute walk outside."
    };

    const char *productivityChallenges[] = {
        "Complete your most important task today.",
        "Set and achieve 3 goals for today.",
        "Organize your workspace.",
        "Declutter your digital files.",
        "Write down your to-do list for the week."
    };

    const char *mindfulnessChallenges[] = {
        "Meditate for 10 minutes.",
        "Practice deep breathing exercises.",
        "Journal your thoughts for 10 minutes.",
        "Take a break to clear your mind.",
        "Try a new relaxation technique."
    };

    const char *quotes[] = {
        "The best time to start is now!",
        "Don't stop when you're tired, stop when you're done.",
        "Small progress is still progress.",
        "Believe in yourself and all that you are.",
        "Success doesn't come from what you do occasionally, it comes from what you do consistently."
    };

    int randomChallengeIndex = rand() % MAX_CHALLENGES;
    int randomQuoteIndex = rand() % MAX_QUOTES;

    switch (category) {
        case 1:
            printf("\nToday's Health Challenge: %s\n", healthChallenges[randomChallengeIndex]);
            break;
        case 2:
            printf("\nToday's Productivity Challenge: %s\n", productivityChallenges[randomChallengeIndex]);
            break;
        case 3:
            printf("\nToday's Mindfulness Challenge: %s\n", mindfulnessChallenges[randomChallengeIndex]);
            break;
        default:
            printf("\nInvalid category.\n");
            return;
    }
    
    printf("\nInspirational Quote: %s\n", quotes[randomQuoteIndex]);
}

void trackProgress(UserProgress *progress) {
    int choice;
    printf("\nDid you complete today's challenge? (1 = Yes, 0 = No): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nGreat job! You completed today's challenge!\n");
        if (progress->health < MAX_CHALLENGES) progress->health++;
        if (progress->productivity < MAX_CHALLENGES) progress->productivity++;
        if (progress->mindfulness < MAX_CHALLENGES) progress->mindfulness++;
    } else {
        printf("\nNo worries! You can try again tomorrow. Keep going!\n");
    }
}

void showProgress(UserProgress *progress) {
    printf("\nYour Progress:\n");
    printf("Health Challenges Completed: %d/%d\n", progress->health, MAX_CHALLENGES);
    printf("Productivity Challenges Completed: %d/%d\n", progress->productivity, MAX_CHALLENGES);
    printf("Mindfulness Challenges Completed: %d/%d\n", progress->mindfulness, MAX_CHALLENGES);

    if (progress->health == MAX_CHALLENGES) {
        printf("\n You've earned the 'Healthy Habits' badge!\n");
    }
    if (progress->productivity == MAX_CHALLENGES) {
        printf("\n You've earned the 'Productivity Master' badge!\n");
    }
    if (progress->mindfulness == MAX_CHALLENGES) {
        printf("\n You've earned the 'Mindfulness Guru' badge!\n");
    }
}

int dailyMenu() {
    int choice;
    printf("\nWelcome to the Daily Challenge System!\n");
    printf("Please select a category:\n");
    printf("1. Health\n");
    printf("2. Productivity\n");
    printf("3. Mindfulness\n");
    printf("4. Show Progress\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

void journalMenu() {
    printf("\nJournal Menu:\n");
    printf("1. Add New Entry\n");
    printf("2. View Entries\n");
    printf("3. Delete Entries\n");
    printf("4. Search entries\n");
    printf("5. Exit.\n");
    printf("Choose an option:\n");
}












int main() {
    FILE *journalFile = fopen("journal.txt", "a+");
    if (journalFile == NULL) {
        printf("Error opening journal file.\n");
        return 1;
    }

    UserProgress progress = {0, 0, 0}; 
    int choice;

    while (1) {
       
        printf("Hi! This is Work Genie, your personal assistant. \n");
        printf("I can help you with the following:\n");
        printf("\nMain Menu:\n");
        printf("1. Journal\n");
        printf("2. To-Do List\n");
        printf("3. Scientific Calculator\n");
        printf("4. Daily Challenges\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();  

        switch (choice) {
            case 1: {
                int journalChoice;
                while (1) {
                    printf("\nJournal Menu:\n");
                    printf("1. Add New Entry\n");
                    printf("2. View Entries\n");
                    printf("3. Delete Entries\n");
                    printf("4. Search Entries\n");
                    printf("5. Go Back to Main Menu\n");
                    printf("Choose an option: ");
                    scanf("%d", &journalChoice);
                    getchar(); 

                    switch (journalChoice) {
                        case 1:
                            Add(journalFile);
                            break;
                        case 2:
                            View(journalFile);
                            break;
                        case 3:
                            Delete(journalFile);
                            break;
                        case 4: {
                            char keyword[MaxEntry];
                            printf("Enter keyword to search: ");
                            fgets(keyword, sizeof(keyword), stdin);
                            keyword[strcspn(keyword, "\n")] = 0;
                            Search(journalFile, keyword);
                            break;
                        }
                        case 5:
                            printf("Returning to Main Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }

                    if (journalChoice == 5) break;  
                }
                break;
            }

            case 2: {
                int todoChoice;
                while (1) {
                    printf("\nTo-Do List Menu:\n");
                    printf("1. Add Task\n");
                    printf("2. View Tasks\n");
                    printf("3. Mark Task as Completed\n");
                    printf("4. Delete Task\n");
                    printf("5. View Task Statistics\n");
                    printf("6. Sort Tasks by Priority\n");
                    printf("7. Go Back to Main Menu\n");
                    printf("Choose an option: ");
                    scanf("%d", &todoChoice);
                    getchar(); 

                    switch (todoChoice) {
                        case 1: {
                            char task[TaskLength], priority[10], dueDate[DateLength];
                            printf("Enter task: ");
                            fgets(task, sizeof(task), stdin);
                            task[strcspn(task, "\n")] = 0;
                            printf("Enter priority: ");
                            fgets(priority, sizeof(priority), stdin);
                            priority[strcspn(priority, "\n")] = 0;
                            printf("Enter due date (YYYY-MM-DD): ");
                            fgets(dueDate, sizeof(dueDate), stdin);
                            dueDate[strcspn(dueDate, "\n")] = 0;
                            addTask(task, priority, dueDate);
                            break;
                        }
                        case 2:
                            viewTasks();
                            break;
                        case 3: {
                            int taskNum;
                            printf("Enter task number to mark as completed: ");
                            scanf("%d", &taskNum);
                            markTaskAsCompleted(taskNum - 1);
                            break;
                        }
                        case 4: {
                            int taskNum;
                            printf("Enter task number to delete: ");
                            scanf("%d", &taskNum);
                            deleteTask(taskNum - 1);
                            break;
                        }
                        case 5:
                            printStatistics();
                            break;
                        case 6:
                            sortTasksByPriority();
                            break;
                        case 7:
                            printf("Returning to Main Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }

                    if (todoChoice == 7) break;  
                }
                break;
            }

            case 3: {
                calc();
                break;
            }

            case 4: {
                int challengeChoice;
                while (1) {
                    printf("\nDaily Challenges Menu:\n");
                    printf("1. Health Challenges\n");
                    printf("2. Productivity Challenges\n");
                    printf("3. Mindfulness Challenges\n");
                    printf("4. Show Progress\n");
                    printf("5. Exit to Main Menu\n");
                    printf("Choose a challenge category: ");
                    scanf("%d", &challengeChoice);
                    getchar(); 

                    switch (challengeChoice) {
                        case 1:
                            showChallenge(1);
                            trackProgress(&progress);
                            break;
                        case 2:
                            showChallenge(2);
                            trackProgress(&progress);
                            break;
                        case 3:
                            showChallenge(3);
                            trackProgress(&progress);
                            break;
                        case 4:
                            showProgress(&progress);
                            break;
                        case 5:
                            printf("Returning to Main Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }

                    if (challengeChoice == 5) break;  
                }
                break;
            }

            case 5:
                printf("Exiting the program. Goodbye!\n");
                fclose(journalFile); 
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
  