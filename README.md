[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/v_hRy-0Q)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=20409466)
## Description
In this assignment you will write your own shell program, Mav shell (msh), similar to 
bourne shell (bash), c-shell (csh), or korn shell (ksh). It will accept commands, fork a child 
process and execute those commands. The shell, like csh or bash, will run and accept 
commands until the user exits the shell

## Requirements

1. Your program will print out a prompt of msh> when it is ready to 
accept input. It must read a line of input and, if the command given is a supported shell 
command, it shall execute the command and display the output of the command.

<img src="Images/prompt.png" alt="main" width="400"/>

2. If the command is not supported your shell shall print the invalid 
command followed by “: Command not found.”

<img src="Images/notfound.png" alt="main" width="400"/>

3. After each command completes, your program shall print the msh>
prompt and accept another line of input.

4. Your shell will exit with status zero if the command is “quit” or “exit”. 

5. If the user types a blank line, your shell will, quietly and with no other 
output, print another prompt and accept a new line of input.

<img src="Images/blank.png" alt="main" width="400"/>

6. Your version of Mav shell shall support up to 10 command line 
parameters in addition to the command. 

7. Your shell shall support and execute any command entered. Any 
command in /bin, /usr/bin/, /usr/local/bin/ and the current working directory 
is to be considered valid for testing.

8. Mav shell shall be implemented using fork(), wait() and one of the 
exec family of functions. Your Mav shell shall not use system(). Use of system() will result in a grade of 0.

9. Your shell shall support the cd command to change directories. Your 
shell must handle cd ..

<img src="Images/cd.png" alt="main" width="400"/>

10. Your shell shall  save the last 50 commands and command line parameters

11. You shall print the history log, excluding blank line entries when the user types history
   
```
msh> history
[1] ls
[2] mkdir foo
[3] cd foo
[4] cd ..
[5] history
``` 

12. The user can re-run any commnd in the history by typing !# where # is the number of the command to rerun.

13. Your shell shall support redirection.  The syntax [process] > [file] tells your shell to redirect the process’s standard output to a file. 

You do not need to support redirection for shell built-in commands. You do not need to support stderr redirection or appending to files (e.g. [process] >> [file]). You can assume that there will always be spaces around special characters < and >.  See popen.c in [Code-Examples](https://github.com/CSE3320-Spring-2025/Code-Examples)

15. Your shell shall support pipes between programs. You can assume there will always be spaces around the special character | . See pipe.c in [Code-Examples](https://github.com/CSE3320-Spring-2025/Code-Examples)

16. Your program shall block the SIGINT and SIGTSTP signals. See sigint.c in [Code-Examples](https://github.com/CSE3320-Spring-2025/Code-Examples)
        
17. Tabs or spaces shall be used to indent the code. Your code must use 
one or the other. All indentation must be consistent.

18. No line of code shall exceed 100 characters. 

19. All code must be well commented. This means descriptive comments 
that tell the intent of the code, not just what the code is executing. 

20. Keep your curly brace placement consistent. If you place curly braces 
on a new line , always place curly braces on a new end. Don’t mix end line brace 
placement with new line brace placement. 
   
21. Remove all extraneous debug output before submission. The only 
output shall be the output of the commands entered or the shell prompt. 
   
## Administrative

This assignment must be coded in C. Any other language will result in 0 points. You 
programs will be compiled and graded on the course GitHub Codespace. Code that does not compile 
with :
        make 
will result in a 0.

Your program, msh.c is to be turned in by pushing your code to main.  Submission time is determined by 
the GitHub system time. You may push your code as often as you wish. Only your 
last submission will be graded. 

There are coding resources and working code you may use on the course GitHub page and in the assignment repository at: [https://github.com/CSE3320-Fall-2025/Shell-Assignment](https://github.com/CSE3320-Fall-2025/Shell-Assignment). You are free to use any of that code in your program if needed. You may use no other outside code.

## Academic Integrity
This assignment must be 100% your own work. No code may be copied from friends, 
previous students, books, web pages, etc. All code submitted is automatically checked 
against a database of previous semester’s graded assignments, current student’s code 
and common web sources. By submitting your code on GitHub you are attesting that 
you have neither given nor received unauthorized assistance on this work. Code that 
is copied from an external source or used as inspiration, excluding the 
course github, will result in a 0 for the assignment and referral to 
the Office of Student Conduct.

