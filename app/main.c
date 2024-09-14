#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  // Wait for user input
  char input[100];// Buffer to store user input
  char *args[10];// Array to store the command and its arguments

  // Uncomment this block to pass the first stage


  printf("$ ");
  fflush(stdout);

  //get user input 
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
  // Handle missing commands
  if (args[0] != NULL)
  {
    // If the command is not found, print an error
    fprintf(stderr, "%s: command not found\n", args[0]);
  }

  return 0;
}
