#include "nav.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robot.h"
#include "task.h"
#include "queue.h"

extern TaskQueue assemblyQueue;
extern Robot robots[MAX_ROBOTS];
extern Task tasks[MAX_TASKS];
extern int robotCount;
extern int taskCount;

void managerobotssubmenu(void) {
    int choice = 0;
    while (1) {
        printf("\n\033[36m=== \033[33m Manage Robots\033[36m ===\033[0m\n");
        printf("\033[36m1.\033[97m   Add New Robot\n");
        printf("\033[36m2.\033[97m   Edit Robot Information\n");
        printf("\033[36m3.\033[97m   Delete Robot\n");
        printf("\033[36m4.\033[97m   View Robot by ID\n");
        printf("\033[36m5.\033[97m   List All Robots\n");
        printf("\033[36m6.\033[97m   Search by Status (0-IDLE,1-WORKING,2-ERROR)\n");
        printf("\033[36m7.\033[97m   Show Work History\n");
        printf("\033[36m8.\033[97m   Reset Robot Status\n");
        printf("\033[36m9.\033[97m   Return to Main Menu\033[0m\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number 1-9.\n");
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                addRobot();
                break;
            case 2: {
                int id;
                printf("Enter Robot ID to edit: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    editRobot(id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 3: {
                int id;
                printf("Enter Robot ID to delete: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    deleteRobot(id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 4: {
                int id;
                printf("Enter Robot ID to view: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    Robot *robot = viewRobotById(id);
                    if (robot) printRobotDetails(robot);
                    else printf("Robot with ID %d not found!\n", id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 5:
                listAllRobots();
                break;
            case 6: {
                int status;
                printf("Enter status (0-IDLE,1-WORKING,2-ERROR): ");
                if (scanf("%d", &status) == 1) {
                    while (getchar() != '\n');
                    searchRobotsByStatus(status);
                } else {
                    printf("Invalid status.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 7: {
                int id;
                printf("Enter Robot ID: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    Robot *r = viewRobotById(id);
                    if (r) showWorkHistory(r);
                    else printf("Robot with ID %d not found!\n", id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 8: {
                int id;
                printf("Enter Robot ID: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    resetRobotStatus(id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 9:
                return;
            default:
                printf("Invalid choice! Please select 1-9.\n");
        }
    }
}

void managetaskssubmenu(void) {
    int choice = 0;
    while (1) {
        printf("\n\033[36m=== \033[33m Manage Tasks\033[36m ===\033[0m\n");
        printf("\033[36m1.\033[97m   Create New Task\n");
        printf("\033[36m2.\033[97m   Update Task\n");
        printf("\033[36m3.\033[97m   Delete Task\n");
        printf("\033[36m4.\033[97m   View Task Details\n");
        printf("\033[36m5.\033[97m   List All Tasks\n");
        printf("\033[36m6.\033[97m   Search by Stage (0-PENDING,1-ACTIVE,2-COMPLETED)\n");
        printf("\033[36m7.\033[97m   Return to Main Menu\033[0m\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number 1-7.\n");
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                createTask();
                break;
            case 2: {
                int id;
                printf("Enter Task ID to update: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    updateTask(id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 3: {
                int id;
                printf("Enter Task ID to delete: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    deleteTask(id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 4: {
                int id;
                printf("Enter Task ID to view: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    Task *t = getTaskById(id);
                    if (t) printTaskDetails(t);
                    else printf("Task with ID %d not found!\n", id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 5:
                listAllTasks();
                break;
            case 6: {
                int stage;
                printf("Enter stage (0=PENDING,1=ACTIVE,2=COMPLETED): ");
                if (scanf("%d", &stage) == 1) {
                    while (getchar() != '\n');
                    searchTasksByStage(stage);
                } else {
                    printf("Invalid stage.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 7:
                return;
            default:
                printf("Invalid choice! Please select 1-7.\n");
        }
    }
}

void controlQueueSubmenu(void) {
    int choice = 0;
    while (1) {
        printf("\n\033[36m=== \033[33m Task Queue Control\033[36m ===\033[0m\n");
        printf("\033[36m1.\033[97m  View Queue\n");
        printf("\033[36m2.\033[97m  Add Task to Queue\n");
        printf("\033[36m3.\033[97m  Process Next Task\n");
        printf("\033[36m4.\033[97m  Clear Queue\n");
        printf("\033[36m5.\033[97m  Return to Main Menu\033[0m\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number 1-5.\n");
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                displayQueue();
                break;
            case 2: {
                int id;
                printf("Enter Task ID to queue: ");
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    Task *t = getTaskById(id);
                    if (t) enqueue(*t);
                    else printf("Task %d not found!\n", id);
                } else {
                    printf("Invalid ID.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 3: {
                Task processed = dequeue();
                if (processed.id != -1) {
                    printf("\nProcessing task %d:\n", processed.id);
                    printTaskDetails(&processed);
                } else {
                    printf("Queue is empty!\n");
                }
                break;
            }
            case 4:
                freeQueue(&assemblyQueue);
                printf("Queue cleared successfully!\n");
                break;
            case 5:
                return;
            default:
                printf("Invalid choice! Please select 1-5.\n");
        }
    }
}