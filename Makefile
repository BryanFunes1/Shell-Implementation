msh: msh.c
	gcc msh.c -o msh -g -Wall -Werror

clean:
	rm ./msh
