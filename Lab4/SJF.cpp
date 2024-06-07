#include <stdio.h>
#include <stdlib.h>
#define N 10010
#define INF 1e9

struct process {
   char name[15];
   int id;
   float arr, brust, finish, wait_time, respones_time, turnround_time;
};
int num_process;

void swap(struct process *a, struct process *b) {
   struct process tmp = *a;
   *a = *b;
   *b = tmp;
}

void sort_SJF(struct process P[]) {
   for (int i = 0; i < num_process - 1; i++)
       for (int j = i + 1; j < num_process; j++)
           if (P[j].arr < P[i].arr) swap(&P[j], &P[i]);
           else if (P[i].arr == P[j].arr && P[j].brust < P[i].brust) swap(&P[j], &P[i]);
}

void output(struct process P[]) {
   // sort increasing id
   for (int i = 0; i < num_process - 1; i++)
       for (int j = i + 1; j < num_process; j++)
           if (P[j].id < P[i].id) swap(&P[i], &P[j]);

   printf("Process Name   Response Time   Waiting Time   Turnaround Time\n");

   float avg_waiting_time = 0;
   float avg_turnaround = 0;
   float avg_respone_time = 0;
   for (int i = 0; i < num_process; i++) {
       printf("|%-14s|%-15d|%-15d|%d\n", P[i].name, (int) P[i].respones_time, (int) P[i].wait_time,
              (int) P[i].turnround_time);

       avg_waiting_time += P[i].wait_time;
       avg_turnaround += P[i].turnround_time;
       avg_respone_time += P[i].respones_time;
   }

   avg_waiting_time /= num_process;
   avg_turnaround /= num_process;
   avg_respone_time /= num_process;

   printf("==========\n");
   printf("Average waiting time : %.2f\n", avg_waiting_time);
   printf("Average turn around time: %.2f\n", avg_turnaround);
   printf("Average respone time: %.2f\n", avg_respone_time);
}


void SJF(struct process P[]) {
   int *done = (int *) malloc(num_process * sizeof(int));
   for (int i = 0; i < num_process; i++) done[i] = 0;

   float time_current = P[0].arr + P[0].brust;
   P[0].finish = time_current;
   P[0].turnround_time = P[0].finish - P[0].arr;
   P[0].wait_time = P[0].turnround_time - P[0].brust;
   P[0].respones_time = P[0].wait_time;

   done[0] = 1;

   for (int i = 1; i < num_process; i++) {
       float shortestBrust = INF;
       int idx = -1;

       for (int j = 0; j < num_process; j++) {
           if (!done[j] && time_current >= P[j].arr && shortestBrust > P[j].brust) {
               shortestBrust = P[j].brust;
               idx = j;
           }
       }

       if (idx == -1) {
           float shortestarr = INF;
           for (int j = 0; j < num_process; j++)
               if (!done[j] && shortestarr > P[j].arr && P[j].arr > time_current) {
                   shortestarr = P[j].arr;
                   idx = j;
               }
           if (idx == -1) break;
           time_current = shortestarr;
       }

       time_current += P[idx].brust;
       P[idx].finish = time_current;
       P[idx].turnround_time = P[idx].finish - P[idx].arr;
       P[idx].wait_time = P[idx].turnround_time - P[idx].brust;
       P[idx].respones_time = P[idx].wait_time;
       done[idx] = 1;
   }
}

int main() {
   struct process P[N];
   printf("Enter number of process: ");
   scanf("%d", &num_process);
   for (int i = 0; i < num_process; i++) {
       printf("==========\n");
       printf("Enter name of process %d: ", (i + 1));
       scanf("%s", &P[i].name);
       printf("Enter arrival time %d: ", (i + 1));
       scanf("%f", &P[i].arr);
       printf("Enter burst time %d: ", (i + 1));
       scanf("%f", &P[i].brust);
       P[i].finish = P[i].turnround_time = P[i].wait_time = P[i].respones_time = 0;
       P[i].id = i;
   }
   printf("==========\n");
   sort_SJF(P);

   SJF(P);
   output(P);
   return 0;
}
