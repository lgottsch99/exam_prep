#include <unistd.h>
#include <stdio.h>

void	error(void)
{
	write(STDERR_FILENO, "error: fatal\n", 13);
	exit(1);
}

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	builtin_cd(char *argv[], int i)
{
	argv[i] = NULL; //stop at last one
	argv = &argv[1];

	if (chdir(argv) == -1)
	{
		write(STDERR_FILENO, "error: cd: cannot change directory to", )
	}

}

int main (int argc, char *argv[], char *envp[])
{
	(void)argc;
	int 	i;
	
	i = 0;
	if (!argv)
		return (0);

	argv = &argv[1]; //skip ./a.out

	while (argv[i])
	{
		//count until end or ; or |
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		printf("i : %i\n", i);

		//check cd
		if (strcmp(argv[i], "cd"))
			builtin_cd(argv, i);
		//if encounter ; or | fork 
			//execute first part
		
		//else execute


	}
	
	
	
	
	printf("tmp: %s\n", argv[0]);
	if (execve(argv[0], argv, envp) == -1)
	{
		write(STDERR_FILENO, "error: cannot execute ", 22);
		write(STDERR_FILENO, argv[0], ft_strlen(argv[0]));
	}

}