// Jacob Bowlware
// OU ID: 113536730

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

// Define the union semun
union semun
{
    int val;               // Value for SETVAL
    struct semid_ds *buf;  // Buffer for IPC_STAT, IPC_SET
    unsigned short *array; // Array for GETALL, SETALL
    struct seminfo *__buf; // Buffer for seminfo
};

#define SEMKEY ((key_t)400L)
#define NSEMS 1

int sem_id;

struct sembuf P = {0, -1, SEM_UNDO}; // Define P operation
struct sembuf V = {0, 1, SEM_UNDO};  // Define V operation

int POP()
{
    return semop(sem_id, &P, 1);
}

int VOP()
{
    return semop(sem_id, &V, 1);
}

int main()
{
    // Initialize variables
    int shmid;
    int *shared_counter;
    pid_t pids[4];
    union semun semctl_arg;
    semctl_arg.val = 1;

    // Create shared memory
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    shared_counter = (int *)shmat(shmid, NULL, 0);
    *shared_counter = 1; // Initialize counter to 1

    // Create semaphores
    sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
    if (sem_id < 0)
    {
        perror("Error in creating the semaphore");
        exit(1);
    }
    semctl(sem_id, 0, SETVAL, semctl_arg);

    // Fork processes
    for (int i = 0; i < 4; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            // Child process logic
            int target;
            if (i == 0)
                target = 100000; // Process 1's target
            else if (i == 1)
                target = 200000; // Process 2's target
            else if (i == 2)
                target = 300000; // Process 3's target
            else
                target = 500000; // Process 4's target

            int j = 0; // Counter for each process
            while (j < target)
            {
                POP();               // Enter critical section
                (*shared_counter)++; // Increment shared counter
                VOP();               // Exit critical section

                j++; // Increment process counter
            }
            printf("From Process %d: counter = %d.\n", i + 1, *shared_counter - 1);
            exit(0);
        }
    }

    // Parent process waits for children
    for (int i = 0; i < 4; i++)
    {
        waitpid(pids[i], NULL, 0);
        printf("Child with ID %d has just exited.\n", pids[i]);
    }

    // Cleanup
    shmdt(shared_counter);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, NULL);
    printf("\n\n         End of Program.\n");
    return 0;
}
