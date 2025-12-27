// The MIT License (MIT)
// 
// Copyright (c) 2023 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 128    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11    // Mav shell currently only supports one argument
                                //seting it to 11 arguments for it accept 10 arguments. execvp reqires it end in NULL so we need 11 just in case all ten are used
static void handle_signal (int sig)
{
  fflush(stdin);  //When a signal is called this is used to flush the stdin buffer
}                 //So that the msh command shows up immediatly and quietly

int main()
{
  struct sigaction act;            //These are used to block the SIGINT and 
  memset (&act, '\0', sizeof(act));//SIGTSTP signals. I make a variable
  act.sa_handler = &handle_signal;//set it up and call sigaction to catch the 
  sigaction(SIGINT , &act, NULL);//signals which calls the function.
  sigaction(SIGTSTP , &act, NULL);
  char history[50][50]; //Where the history is stored
  for(int i = 0; i < 50; i++)
  {
    strcpy(history[i], "\0");  //Sets up the array for history by making it all start with value of NULL
  }

  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );

  while( 1 )
  {
    int hist = 0;  //This variable is used to keep track when the code is rerunning a history command
    int check = 0; //This is used to check if the built in commands were used and if not it will run fork() and exec()
    int total = 0; //This is used check that there is no more than 1 redirect or pipe
    int pipes = 0; //This is to make sure there is a pipe
    int where = 0; //This has the location stored in it's value when found
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];
    if(command_string[0] == '!')
    {
      int num = atoi(&command_string[1]);         //This is used to check if the commands entered starts a !
      if(num > 0 && num <= 50)                    //for rerunning a command in the history
      {                                           //It then converts the char following to a num
        strcpy(command_string, history[num - 1]); //then we change the command_string variable to one in the history array.
        hist = 1;
      }
    }
    for( int i = 0; i < MAX_NUM_ARGUMENTS; i++ )
    {
      token[i] = NULL;
    }

    int   token_count = 0;                                 

    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr = NULL;                                         
                                                           
    char *working_string  = strdup( command_string );                

    // we are going to move the working_string pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *head_ptr = working_string;

    // Tokenize the input strings with whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_string, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    // \TODO Remove this for loop and replace with your shell functionality
    if(token[0] != NULL)
    {
      if(token[0][0] != '!' && hist == 0)
      {
        int c = 0;
        for(int i = 0; i < 50; i++)          //This is used to store in the command typed in by the user into history
        {                                    //It avoids storing blank commands along with !# commands
          if(strcmp(history[i], "\0") == 0)  //It also makes c = 1 to CONFIRM that it has been saved
          {
            strcpy(history[i], command_string);
            i = 50;
            c = 1;
          }
        }
        if(c == 0)
        {
          for(int i = 0; i < 49; i++)
          {                                  //When the command wasn't confirmed to be saved
            strcpy(history[i], history[i+1]);//Then it shifts all the array back by one by deleting the oldest command
          }                                  //After it is all shfited the new command is saved at the most recent
          strcpy(history[49], command_string);
        }
      }
      if(strcasecmp(token[0], "Quit") == 0 || strcasecmp(token[0], "Exit") == 0)
      {
        check = 1;
        for( int i = 0; i < token_count; i++ ) //This is the built in command for Quit and Exit
        {                                      //if it finds quit or exit was typed then
          if( token[i] != NULL )               //it frees the allocated memory
          {                                    //then breaks to get out the while loop
            free( token[i] );
          }
        }
        free( head_ptr );
        break;
      }
      if(strcmp(token[0], "cd") == 0)
      {
        if(chdir(token[1]) == -1)                       //This is the built in command for cd
        {                                               //if cd was seen to be typed in, it then uses chnage dir to change the directory
          printf("Directory %s not found.\n", token[1]);//if change dir fails it prints out that the directory wasn't found
        }
        check = 1;
      }
      if(strcasecmp(token[0], "history") == 0)
      {
        for(int i = 0; i < 50; i++)            //The final built in for history
        {                                      //when history or History is typed then it prints all of the contents in history
          if(strcmp(history[i], "\0"))         //that are not NULL with the number from 1 to a max of 50.
          {
            printf("[%d] %s", i+1, history[i]);
          }
        }
        check = 1;                             //For all these commands if they were executed it changes check to 1 to represent that it has ran
      }
    }
    for(int i = 0; i < token_count; i++ )
    {
      if(token[i] != NULL)
      {
        if(strcmp( token[i], ">" ) == 0 || strcmp( token[i], "<" ) == 0) //This is used to count the amount of | or < > symbols in the command
        {                                                                //entered. For pipes it adds to total along with it
          total++;                                                       //has a different variable to know the exact pipes there are
        }                                                                //along with it's location
        if(strcmp( token[i], "|" ) == 0)
        {
          total++;
          pipes++;
          where = i + 1;
        }
      }
    }
    if(check == 0) //When the built in commands haven't ran then we get ready to fork and push command to exec
    {
      int pfds[2];
      pipe(pfds);
      pid_t pid = fork();
      pid_t pid2; //this is set up just incase for pipes
      if( pid == -1 )
      {
        // When fork() returns -1, an error happened.
        perror("fork failed: ");
        exit( EXIT_FAILURE );
      }
      if(pid == 0) //we are in the child process
      {
        if(total == 1)
        {
          for(int i = 0; i < token_count; i++ )
          {
            if(token[i] != NULL)
            {
              if( strcmp( token[i], ">" ) == 0 )                                   //These are used to prepare if there is a redirection
              {                                                                    //or pipe in the command line. This done by setting up the
                int fd = open( token[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );  //file descriptors and where evrything depending on the symbol
                dup2( fd , 1 );                                                    //this is only ran when there is a < > or | found in the command
                close( fd );
                free(token[i]);
                token[i] = NULL;
              }
              else if( strcmp( token[i], "<" ) == 0 )
              {
                int fd = open( token[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
                dup2( fd , 0 );
                close( fd );
                free(token[i]);
                token[i] = NULL;
              }
              else if( strcmp( token[i], "|" ) == 0 )
              {
                close(STDOUT_FILENO);
                dup(pfds[1]);          //closes the hearing side of the pipe since it is not needed
                close(pfds[0]);
                free(token[i]);
                token[i] = NULL;
              }
            }
          }
        }
        if(total <= 1)
        {
          check = execvp(token[0], token); //When total is either 0 or 1 we then push the whole command into exec to change the process
        }
        else if(total > 1)
        {
          check = -1; //this is done to mimic that exec failed if there was more than one < > | typed in. This is done to return the command not found error
        }
        if(check == -1)
        {
          command_string[strlen(command_string)-1] = '\0'; //if exec failed it prints the command types in with a command not found print message and the child exits.
          printf("%s: Command not found.\n", command_string);
        }
        exit(1);
      }
      else if (pid > 0) //we are in the parent process
      {
        if(pipes == 1)
        {
          pid2 = fork(); //if there is a pipe it forks another child process to run the other side of the pipe
          if( pid2 == -1 )
          {
            // When fork() returns -1, an error happened.
            perror("fork failed: ");
            exit( EXIT_FAILURE );
          }
        }
        if(pid2 == 0 && pipes == 1) //we are in the child process 2. The pipes == 1 is used to make sure it only goes in here when there is a fork and there is a pipe.
        {
          char *t[token_count];
          for(int i = 0; i < token_count; i++)
          {
            if(where < token_count && token[where] != NULL) //in child process 2 it makes string that has the command token after
            {                                               //the | was typed for it to be used to pushed into execvp
              t[i] = token[where];                          //before the exec call it changes the standard input to be recived from what child
            }                                               //process ran. It also closes the writing part since it's not needed
            else
            {
              t[i] = NULL;
            }
            where++;
          }
          close(STDIN_FILENO);
          dup(pfds[0]);
          close(pfds[1]);
          int check = execvp(t[0], t); //The rest is exactly the same as child process 1, by calling exec
          if(check == -1)              //and if failed it prints out an error
          {                            //with the command typed in and then it exits
            command_string[strlen(command_string)-1] = '\0';
            printf("%s: Command not found.\n", command_string);
          }
          exit(1);
        }
        close(pfds[0]);            //The parent closes both sides of the pipe since it doesn't need it
        close(pfds[1]);            //in anyway. AFter that it wait for child process 1 and if there is a pipe
        int status;                //it waits for child procces 2 and then the terminal resets to receive
        waitpid(pid, &status, 0);  //input again.
        if(pipes == 1)
        {
          waitpid(pid2, &status, 0);
        }
      }
    }
    // Cleanup allocated memory
    for( int i = 0; i < MAX_NUM_ARGUMENTS; i++ )
    {
      if( token[i] != NULL )
      {
        free( token[i] );
      }
    }

    free( head_ptr );
  }

  free( command_string );

  return 0;
  // e1234ca2-76f3-90d6-0703ac120004
}
