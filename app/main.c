#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    // Buffer to store user input
    char input[100];
    // Array to store the command and its arguments
    char *args[10];
    // Buffer to store the current working directory
    char cwd[1024];

    while (1) 
    {
        // Get current working directory
        if (getcwd(cwd, sizeof(cwd)) != NULL) // Print current directory followed by the prompt
        {
          printf("%s $ ", cwd);
        }
        else 
        {
          perror("getcwd() error");
          return 1;
        }

        fflush(stdout);

        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) 
        {
            printf("Error reading input.\n");
            return 1;
        }

        // Remove newline character from input (if it exists)
        input[strcspn(input, "\n")] = 0;

        // Split input into arguments
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < 10) 
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the array of arguments

        // Handle "exit" command to quit the shell
        if (args[0] != NULL && strcmp(args[0], "exit") == 0)
        {
            break; // Exit the loop and terminate the shell
        }

        // Handle "cd" command to change directory
        if (args[0] != NULL && strcmp(args[0], "cd") == 0) 
        {
            if (args[1] != NULL) 
            {
                if (chdir(args[1]) != 0) 
                {
                    perror("cd failed");
                }
            } 
            else 
            {
                fprintf(stderr, "cd: missing argument\n");
            }
            continue; // Skip the fork and exec for the "cd" command
        }

        // Check if the command is not empty
        if (args[0] != NULL) 
        {
            // Fork a child process
            pid_t pid = fork();

            if (pid < 0) 
            {
                // Fork failed
                perror("Fork failed");
                exit(1);
            } 
            else if (pid == 0) 
            {
                // In child process: execute the command
                if (execvp(args[0], args) == -1) 
                {
                    perror("Command execution failed");
                    exit(1); // Exit child process after execution
                }
            } 
            else 
            {
                // In parent process: wait for child to finish
                int status;
                waitpid(pid, &status, 0); // Wait for the child process
            }
        }
    }

    return 0;
}
