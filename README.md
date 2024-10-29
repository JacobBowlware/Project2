# Shared Memory and Semaphore Project
Jacob Bowlware
OU ID: 113536730

## Compilation
To compile the program, run the following command in the root directory of this project:
```bash
gcc -o shared_memory_semaphore main.c
```

## Execution
To run the program, execute the following command in the root directory of this project:
```bash
./shared_memory_semaphore
```

## Expected Output
The program will output something similar to the following, always finishing with a shared counter of 1100000:
```
From Process 1: counter = 404603.
Child with ID 80088 has just exited.
From Process 2: counter = 709463.
Child with ID 80089 has just exited.
From Process 3: counter = 899475.
Child with ID 80090 has just exited.
From Process 4: counter = 1100000.
Child with ID 80091 has just exited.


        End of Program.
```
