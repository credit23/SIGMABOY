#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "robot.h"

Task tasks[MAX_TASKS];
int taskCount = 0;

static int countComponents(const int components[10]) {
    int count = 0;
    for (int i = 0; i < 10; i++) if (components[i] != 0) count++;
    return count;
}

void createTask(void) {
    if (taskCount >= MAX_TASKS) {
        printf("Error: Maximum tasks reached!\n");
        return;
    }
    Task newTask;
    newTask.id = taskCount + 1;
    printf("Enter task description: ");
    fgets(newTask.description, sizeof(newTask.description), stdin);
    newTask.description[strcspn(newTask.description, "\n")] = '\0';
    memset(newTask.components, 0, sizeof(newTask.components));
    printf("Enter component IDs (comma-separated, max 10): ");
    char input[256];
    fgets(input, sizeof(input), stdin);
    char *token = strtok(input, ",");
    int idx = 0;
    while (token && idx < 10) {
        long val = strtol(token, NULL, 10);
        if (val > 0) tasks[idx++] = newTask, newTask.components[idx-1] = (int)val;
        token = strtok(NULL, ",");
    }
    newTask.stage = PENDING;
    newTask.robotAssigned = -1;
    newTask.startTime = time(NULL);
    newTask.endTime = 0;
    tasks[taskCount++] = newTask;
    printf("Task created! ID: %d | Components: %d | Status: PENDING\n",
           newTask.id, countComponents(newTask.components));
}

void updateTask(int id) {
    Task *task = getTaskById(id);
    if (!task) {
        printf("Error: Task %d not found!\n", id);
        return;
    }
    int choice;
    do {
        printf("Editing Task %d:\n1) Description\n2) Components\n"
               "3) Status\n4) Mark Completed\n5) Return\nChoose: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
        while(getchar()!='\n');
        switch (choice) {
            case 1:
                printf("Current: %s\nNew description: ", task->description);
                fgets(task->description, sizeof(task->description), stdin);
                task->description[strcspn(task->description, "\n")] = '\0';
                break;
            case 2: {
                printf("Enter new components: ");
                char in[256]; fgets(in, sizeof(in), stdin);
                memset(task->components, 0, sizeof(task->components));
                char *tok = strtok(in, ","); int i=0;
                while (tok && i<10) { task->components[i++] = atoi(tok); tok=strtok(NULL, ","); }
                break;
            }
            case 3: {
                printf("New status (0=PENDING,1=ACTIVE,2=COMPLETED): ");
                int s; scanf("%d", &s); while(getchar()!='\n');
                if (s>=PENDING&&s<=COMPLETED) task->stage = s;
                if (task->stage==COMPLETED) task->endTime = time(NULL);
                break;
            }
            case 4:
                task->stage = COMPLETED;
                task->endTime = time(NULL);
                if (task->robotAssigned != -1) {
                    Robot *r = viewRobotById(task->robotAssigned);
                    if (r) { r->tasksCompleted++; r->status = IDLE; }
                }
                break;
        }
    } while (choice != 5);
}

void deleteTask(int id) {
    int idx=-1;
    for(int i=0;i<taskCount;i++) if(tasks[i].id==id) {idx=i; break;}
    if (idx<0) { printf("Error: Task not found!\n"); return; }
    for(int i=idx;i<taskCount-1;i++) tasks[i]=tasks[i+1];
    taskCount--;
}

Task* getTaskById(int id) {
    for(int i=0;i<taskCount;i++) if(tasks[i].id==id) return &tasks[i];
    return NULL;
}

void listAllTasks(void) {
    if (!taskCount) { printf("No tasks available.\n"); return; }
    for(int i=0;i<taskCount;i++) printTaskDetails(&tasks[i]);
}

void searchTasksByStage(StageStatus status) {
    for(int i=0;i<taskCount;i++) if(tasks[i].stage==status) printTaskDetails(&tasks[i]);
}

void printTaskDetails(const Task *t) {
    if (!t) return;
    char buf[20];
    printf("ID: %d\nDesc: %s\nStatus: %d\n",
           t->id, t->description, t->stage);
    strftime(buf,20,"%Y-%m-%d %H:%M:%S", localtime(&t->startTime));
    printf("Start: %s\n", buf);
    if (t->stage==COMPLETED) { strftime(buf,20,"%Y-%m-%d %H:%M:%S", localtime(&t->endTime)); printf("End: %s\n", buf); }
    printf("Components: ");
    for(int i=0;i<10;i++) if(t->components[i]) printf("%d ", t->components[i]);
    printf("\nRobot: %d\n", t->robotAssigned);
}

void assignTaskToRobot(void) {
    int tid, rid;
    printf("Task ID: "); if(scanf("%d",&tid)!=1){while(getchar()!='\n');return;} while(getchar()!='\n');
    Task *t = getTaskById(tid); if(!t){printf("Task not found.\n");return;}
    printf("Robot ID: "); if(scanf("%d",&rid)!=1){while(getchar()!='\n');return;} while(getchar()!='\n');
    Robot *r = viewRobotById(rid); if(!r){printf("Robot not found.\n");return;}
    t->robotAssigned = rid; r->status = WORKING; t->stage = ACTIVE; t->startTime = time(NULL);
    printf("Assigned Task %d to Robot %d.\n", tid, rid);
}
