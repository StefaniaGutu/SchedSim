#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int nr_processes,  *burst_time, *waiting_time, *turn_around_time;

//functie de citire pentru First_Come_First_Serve() si Shortest_Job_First()
void read()
{
  printf("The number of processes: ");
  scanf("%d", &nr_processes);
  printf("The burst time for processes:\n");
  burst_time = (int*)malloc(nr_processes * sizeof(int));

  for(int i=0; i<nr_processes; i++)
  {
    printf("Burst time for process %d: ", i+1);
    scanf("%d", &burst_time[i]);
  }
}

//functie pentru interschimbare
void swap(int* a, int* b)
{
  int aux = *a;
  *a = *b;
  *b = aux;
}

//partiile pentru quickSort
int partition (int v[], int low, int high)
{
  int pivot = v[high];
  int i = (low - 1);
  for (int j = low; j <= high- 1; j++)
  {
    if (v[j] <= pivot)
    {
      i++;
      swap(&v[i], &v[j]);
    }
  }
  swap(&v[i + 1], &v[high]);
  return (i + 1);
}

// sortare pentru Shortest_Job_First()
void quickSort(int v[], int low, int high)
{
  if (low < high)
  {
    int p = partition(v, low, high);
    quickSort(v, low, p - 1);
    quickSort(v, p + 1, high);
  }
}

//algoritmul de scheduling First Come First Serve ia procesele in ordinea in care sunt date
void First_Come_First_Serve()
{
  read();
  int total_waiting_time = 0;
  int total_turn_around_time = 0;
  waiting_time = (int*)malloc(nr_processes * sizeof(int));
  turn_around_time = (int*)malloc(nr_processes * sizeof(int));
  waiting_time[0] = 0;
  //calculare timp de asteptare
  for(int i=1; i<nr_processes; i++)
  {
    waiting_time[i] = burst_time[i-1] + waiting_time[i-1];
  }
  //calculare timp in care a fost asteptat si rezolvat procesul
  for(int i=0; i<nr_processes; i++)
  {
    turn_around_time[i] = burst_time[i] + waiting_time[i];
  }
  //calculare timpii totali si afisare pentru fiecare proces
  for(int i=0; i<nr_processes; i++)
  {
    total_waiting_time += waiting_time[i];
    total_turn_around_time += turn_around_time[i];
    printf("P%d -> burst time = %d, wating time = %d, turn around time = %d\n", i+1, burst_time[i], waiting_time[i], turn_around_time[i]);
  }

  //afisare timp mediu
  printf("Average waiting time = %f \n",(float)total_waiting_time/nr_processes);
  printf("Average turn around time = %f \n", (float) total_turn_around_time/nr_processes);

  free(burst_time);
  free(waiting_time);
  free(turn_around_time);

}

//algoritmul de scheduling Shortest Job First ia procesele in functie de cel mai scurt timp de procesare
void Shortest_Job_First() // consideram ca toate procesele ajung in acelasi timp si procesorul asteapta sa se termine fiecare proces pana sa se apuce de altul
{
  read();
  quickSort(burst_time, 0, nr_processes-1); //sortare procese in functie de timpul de executie
  int total_waiting_time = 0;
  int total_turn_around_time = 0;
  waiting_time = (int*)malloc(nr_processes * sizeof(int));
  turn_around_time = (int*)malloc(nr_processes * sizeof(int));
  waiting_time[0] = 0;
  //calculare timp de asteptare
  for(int i=1; i<nr_processes; i++)
  {
    waiting_time[i] = burst_time[i-1] + waiting_time[i-1];
  }
  //calculare timp in care a fost asteptat si rezolvat procesul
  for(int i=0; i<nr_processes; i++)
  {
    turn_around_time[i] = burst_time[i] + waiting_time[i];
  }
  //calculare timpii totali si afisare pentru fiecare proces
  for(int i=0; i<nr_processes; i++)
  {
    total_waiting_time += waiting_time[i];
    total_turn_around_time += turn_around_time[i];
    printf("P%d -> burst time = %d, wating time = %d, turn around time = %d\n", i+1, burst_time[i], waiting_time[i], turn_around_time[i]);
  }

  //afisare timp mediu
  printf("Average waiting time = %f \n",(float)total_waiting_time/nr_processes);
  printf("Average turn around time = %f \n", (float) total_turn_around_time/nr_processes);

  free(burst_time);
  free(waiting_time);
  free(turn_around_time);
}

struct process
{
  int process_number, arrival_time, burst_time, priority;
};

//algoritmul de scheduling Priority Scheduling ia procesele in ordinea timpului de sosire, apoi daca au acelasi timp de sosire le ia in functie de prioritate
void Priority_Scheduling()
{
  printf("The number of processes: ");
  scanf("%d", &nr_processes);
  struct process *processes;
  processes = (struct process*)malloc(nr_processes * sizeof(struct process));
  for(int i=0; i<nr_processes; i++)
  {
    printf("Process %d \n", i+1);
    processes[i].process_number = i+1;
    printf("Burst time: ");
    scanf("%d", &processes[i].burst_time);
    printf("Arrival time: ");
    scanf("%d", &processes[i].arrival_time);
    printf("Priority: ");
    scanf("%d", &processes[i].priority);
    printf("\n");
  }
  //sortare in functie de timpul de sosire si de prioritate in caz de egalitate
  for(int i=0; i < nr_processes-1;i++)
    for(int j=i+1; j < nr_processes; j++)
    {
      if(processes[i].arrival_time > processes[j].arrival_time || (processes[i].arrival_time == processes[j].arrival_time && processes[i].priority < processes[j].priority))
      {
          struct process aux = processes[i];
          processes[i] = processes[j];
          processes[j] = aux;
      }
    }

  int* burst_time_total = (int*)malloc(nr_processes*sizeof(int));
  waiting_time = (int*)malloc(nr_processes * sizeof(int));
  turn_around_time = (int*)malloc(nr_processes * sizeof(int));
  waiting_time[0] = 0;
  burst_time_total[0] = processes[0].arrival_time;
  //calculare timp de asteptare
  for (int i=1; i < nr_processes; i++)
  {
    burst_time_total[i] = processes[i-1].burst_time + burst_time_total[i-1];
    waiting_time[i] = burst_time_total[i] - processes[i].arrival_time;
    //in cazul in care a ajuns dupa ce au fost terminate procesele anterioare
    if(waiting_time[i]<0)
    {
      waiting_time[i] = 0;
    }
  }

  //calculare timp in care a fost asteptat si rezolvat procesul
  for(int i=0; i<nr_processes; i++)
  {
    turn_around_time[i] = processes[i].burst_time + waiting_time[i];
  }

  int* start_time;
  int* end_time;
  start_time = (int*)malloc(nr_processes * sizeof(int));
  end_time = (int*)malloc(nr_processes * sizeof(int));
  start_time[0] = processes[0].arrival_time;
  end_time[0] = start_time[0] + turn_around_time[0];

  //calculare timp de inceput si sfarsit
  for (int i=1; i < nr_processes ; i++)
  {
    start_time[i] = end_time[i-1];
    end_time[i] = start_time[i] + turn_around_time[i] - waiting_time[i];
  }

  //calculare timpii totali si afisare pentru fiecare proces
  int total_waiting_time = 0;
  int total_turn_around_time = 0;
  for(int i=0; i<nr_processes; i++)
  {
    total_waiting_time += waiting_time[i];
    total_turn_around_time += turn_around_time[i];
    printf("P%d -> burst time = %d, arrival time = %d, priority = %d\n", i+1, processes[i].burst_time, processes[i].arrival_time, processes[i].priority);
    printf("   -> waiting time = %d, turn around time = %d\n", waiting_time[i], turn_around_time[i]);
    printf("   -> start time = %d, end time = %d\n", start_time[i], end_time[i]);
  }
  //afisare timp mediu
  printf("Average waiting time = %f \n",(float)total_waiting_time/nr_processes);
  printf("Average turn around time = %f \n", (float) total_turn_around_time/nr_processes);

  free(burst_time);
  free(waiting_time);
  free(turn_around_time);
  free(start_time);
  free(end_time);
  free(burst_time_total);
  free(processes);
}

//functie care verifica daca a ajuns un nou proces
void new_process_arrival(int spent_time, int *arrival_time, int nr_processes, int index_max,int *queue){
    if(spent_time <= arrival_time[nr_processes-1]) //verificam daca mai sunt procese care trebuie sa ajunga
    {
      bool newArrival = false;
      for(int j = (index_max+1); j < nr_processes; j++)
      {
        if(arrival_time[j] <= spent_time) //verificam daca timpul de arrival este mai mic decat timpul care a trecut
        {
          if(index_max < j) //actualizam indexul maxim cu indexul ultimului proces care a ajuns si care a ajuns mai tarziu decat ultimul proces existent in coada
          {
            index_max = j;
            newArrival = true;
          }
        }
      }
      //daca avem un nou proces, cautam primul index la care nu se afla niciun proces in coada
      if(newArrival)
      {
        int zeroIndex;
        for(int i = 0; i < nr_processes; i++)
        {
          if(queue[i] == 0)
          {
            zeroIndex = i;
            break;
          }
        }
        queue[zeroIndex] = index_max + 1;// adaugam in coada noul proces la indexul gasit anterior
      }
    }
}
void Round_Robin_Scheduling()
{
    int time_quantum, spent_time = 0, index_max = 0;
    int *arrival_time, *queue, *aux_burst_time;
    bool *completed_process;
    arrival_time = (int*)malloc(nr_processes * sizeof(int));
    burst_time = (int*)malloc(nr_processes * sizeof(int));
    waiting_time = (int*)malloc(nr_processes * sizeof(int));
    turn_around_time = (int*)malloc(nr_processes * sizeof(int));
    queue = (int*)malloc(nr_processes * sizeof(int));
    aux_burst_time = (int*)malloc(nr_processes * sizeof(int));
    completed_process = (bool*)malloc(nr_processes * sizeof(bool));

    printf("The number of processes: ");
    scanf("%d", &nr_processes);
    printf("The burst time and arrival time for processes:\n");
    for(int i=0; i<nr_processes; i++)
    {
      printf("Burst time for process %d: ", i+1);
      scanf("%d", &burst_time[i]);
      aux_burst_time[i] = burst_time[i];
      printf("Arrival time for process %d: ", i+1);
      scanf("%d", &arrival_time[i]);
    }

    printf("Time quantum: ");
    scanf("%d", &time_quantum);

    // ordonare procese in functie de arrival time
    for(int i = 0; i < nr_processes-1; i++)
      for(int j = i+1; j < nr_processes; j++)
        if(arrival_time[i] > arrival_time[j])
        {
          int aux = arrival_time[i];
          arrival_time[i] = arrival_time[j];
          arrival_time[j] = aux;

          aux = burst_time[i];
          burst_time[i] = burst_time[j];
          burst_time[j] = aux;

          aux = aux_burst_time[i];
          aux_burst_time[i] = aux_burst_time[j];
          aux_burst_time[j] = aux;
        }

    // initializare coada si lista de procese ca fiind nefinalizate
    for(int i = 0; i < nr_processes; i++)
    {
      completed_process[i] = false;
      queue[i] = 0;
    }
    while(spent_time < arrival_time[0]) //incrementam timpul pana cand ajunge primul proces
      spent_time++;
    queue[0] = 1; //initializam cu primul proces dupa ordonare

    while(true)
    { //verificare daca toate procesele sunt finalizate
      bool all_completed = true;
      for(int i = 0; i < nr_processes; i++)
      {
        if(aux_burst_time[i] != 0)
          {
            all_completed = false;
            break;
          }
      }
      if(all_completed)//daca sunt finalizate toate, iesim din while si calculam timpul cerut
        break;

      for(int i = 0; (i < nr_processes) && (queue[i] != 0); i++)
      {
        int contor = 0;
        //actualizam burst_time-ul ramas din procesul curent, timpul care a trecut si contorul la fiecare secunda din quantum
        while((contor < time_quantum) && (aux_burst_time[queue[0]-1] > 0))
        {
          aux_burst_time[queue[0]-1] -= 1;
          spent_time += 1;
          contor++;

          //verificam daca a ajuns un nou proces
          new_process_arrival(spent_time, arrival_time, nr_processes, index_max, queue);
        }
        //daca un proces este finalizat atunci il marcam ca fiind finalizat si retinem timpul de finalizare
        if((aux_burst_time[queue[0]-1] == 0) && (completed_process[queue[0]-1] == false))
        {
          turn_around_time[queue[0]-1] = spent_time;
          completed_process[queue[0]-1] = true;
        }

        //verificare daca CPU este inactiv
        bool wait_for_arrival = true;
        if(queue[nr_processes-1] == 0)
        {
          for(int i = 0; i < nr_processes && queue[i] != 0; i++)
          {
            if(completed_process[queue[i]-1] == false)
            {
              wait_for_arrival = false;
            }
          }
        }
        else
          wait_for_arrival = false;

        //asteptam sosirea celorlalte procese
        if(wait_for_arrival)
        {
          spent_time++;
          new_process_arrival(spent_time, arrival_time, nr_processes, index_max, queue);
        }

        //mutarea proceselor cu o pozitie la stanga in coada
        for(int i = 0; (i < nr_processes-1) && (queue[i+1] != 0) ; i++)
        {
          int temp = queue[i];
          queue[i] = queue[i+1];
          queue[i+1] = temp;
        }
      }
    }
    //calculare timp de asteptare si timpul in care a fost asteptat si rezolvat procesul
    for(int i = 0; i < nr_processes; i++)
    {
      turn_around_time[i] = turn_around_time[i] - arrival_time[i];
      waiting_time[i] = turn_around_time[i] - burst_time[i];
    }

    int total_waiting_time = 0;
    int total_turn_around_time = 0;
    //calculare timpii totali si afisare pentru fiecare proces
    for(int i=0; i<nr_processes; i++)
    {
      total_waiting_time += waiting_time[i];
      total_turn_around_time += turn_around_time[i];
      printf("P%d -> burst time = %d, arrival time = %d\n", i+1, burst_time[i], arrival_time[i]);
      printf("   -> waiting time = %d, turn around time = %d\n", waiting_time[i], turn_around_time[i]);
    }
    //afisare timp mediu
    printf("Average waiting time = %f \n",(float)total_waiting_time/nr_processes);
    printf("Average turn around time = %f \n", (float) total_turn_around_time/nr_processes);

    free(burst_time);
    free(waiting_time);
    free(arrival_time);
    free(turn_around_time);
    free(aux_burst_time);
    free(completed_process);
    free(queue);
}
//meniu
void meniu()
{
    printf("1: First Come First Serve\n");
    printf("2: Shortest Job First\n");
    printf("3: Priority Scheduling\n");
    printf("4: Round Robin Scheduling\n");
    int optiune;
    printf("Option: ");
    scanf("%d", &optiune);
    switch(optiune)
    {
            case 1:
                First_Come_First_Serve();
                break;
            case 2:
                Shortest_Job_First();
                break;
            case 3:
                Priority_Scheduling();
                break;
            case 4:
                Round_Robin_Scheduling();
                break;
            default:
                printf("Invalid option \n");
                break;
    }
}

int main()
{
    meniu();
    return 0;
}
