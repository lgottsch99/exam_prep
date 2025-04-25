#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

//valgrind --track-fds=yes --trace-children=yes ./a.out 



void	putstr_fd(int fd, char *str)
{
	int i;
	i = 0;

	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

void	do_cd(char *argv[], int i)
{
	(void)argv;
	if (i != 2)
		putstr_fd(STDERR_FILENO, "error: cd: bad arguments\n");
	else
	{
		if (chdir(argv[1]) != 0)
		{
			putstr_fd(STDERR_FILENO, "error: cd: cannot change directory to ");
			putstr_fd(STDERR_FILENO, argv[1]);
		}
	}
}

void	execute(char *argv[], int i, char *envp[], int tmp) //in chils always
{
	//printf("in execute\n");

	argv[i] = NULL; //overwrite to stop at end in case of ; or |
	dup2(tmp, STDIN_FILENO); //mak tmp be input 
	close(tmp);

	if (execve(argv[0], argv, envp) == -1)
	{
		putstr_fd(STDERR_FILENO, "error: cannot execute ");
		putstr_fd(STDERR_FILENO, argv[0]);
		write(STDERR_FILENO, "\n", 1);
		exit(1);//exit process on error
	}
}

int main(int argc, char *argv[], char *envp[])
{
	int i;
	int pid;
	int fd[2]; //0 read end (out), 1 write end (in)
	int tmp_fd; //to keep track of fd to chain pipes together

	i = 0;
	if (argc < 2)
		return (0);

	tmp_fd = dup(STDIN_FILENO); 
	while (argv[i] && argv[i + 1]) //loop thru args
	{
		//set argv start to [i + 1]
		argv = &argv[i + 1]; //skip ./microshell or | or ;
		i = 0;
		//printf("new start argv: %s\n", *argv);

		//count until end or | or ; (to get range of args)
		while(argv[i] && strcmp("|", argv[i]) != 0 && strcmp(";", argv[i]) != 0)
			i++;
		//printf("after loop, i: %i\n", i);


		//check if cd (no fork or pipe, directly execute)
		if (strcmp("cd", argv[0]) == 0)
		{
			//printf("cd detected\n");
			do_cd(argv, i);
		}


		//else check if ; or argv[i] is last (fork and execute parsed args in parent, in child wait)
		else if (argv[i] == NULL || strcmp(";", argv[i]) == 0)
		{
			//printf("; or END  detected\n");

			pid = fork();
			if (pid == 0)
			{
				//printf("in child\n");
				execute(argv, i, envp, tmp_fd);
			}
			else
			{ 	/*Parent is done w tmp fd at this point. 
				If tmp_fd points to the read-end of a pipe, and you don't close it in the parent, 
				the pipe may not send EOF to the child — causing the child to hang waiting for input.
				*/
				close(tmp_fd);

				//wait for child to finish 

				//!!!!ONLY WAITS FOR ONE CHILD
				// if (waitpid(pid, NULL, 0) == -1)
				// {
				// 	putstr_fd(STDERR_FILENO, "error: fatal\n");
				// 	exit(1);
				// }

				//BETTER: 
				while(waitpid(-1, NULL, WUNTRACED) != -1) //-1, waits for any child
					;										//-> spinlock like: calls waitpid again until all childs finished

				tmp_fd = dup(STDIN_FILENO); //restore to default stdin

			}
		}


		//else check if | (pipe AND fork, redirect ends of pipe, exec in child, close fds in parent)
		else if (strcmp("|", argv[i]) == 0)
		{
			//printf("PIPE  | detected\n");

			pipe(fd); //create pipe
			pid = fork(); //clone

			if (pid == 0)
			{//child

				//redirect STDOUT to pipe write in end
				dup2(fd[1], STDOUT_FILENO);
				//close all not needed pipe ends
				close(fd[0]);
				close(fd[1]);
				execute(argv, i, envp, tmp_fd);

			}
			else //(no waiting bc pipe ends need to run at same time)
			{
				//close all not needed pipe ends
				close(fd[1]);
				close(tmp_fd);

				//save read out end to redirect next exec
				tmp_fd = fd[0];
			}
		}

	}
	close(tmp_fd);

	return (0);
}