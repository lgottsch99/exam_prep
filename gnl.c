
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

/*
exam task: https://github.com/ComlanGiovanni/42-Exam-Rank-03/tree/main/get_next_line 
Your program will be compiled with the flag -D BUFFER_SIZE=xx, which has to be used as the buffer size for the read calls in your functions.

fts allowed: read, free and malloc
*/

char	  *get_next_line(int fd)
{
    static char *left;
	char *nextline;
	char *buffer;
	int		bytes_read;

    left = NULL;
	nextline = NULL;
	buffer = NULL;
    //check if anything wrong
    if (fd < 0 || BUFFER_SIZE <= 0)
    {
		//free necessary
        return(NULL);
    }
	printf("fd valid\n");
	
	//malloc buffer 
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	//if (!buffer)
	//	free();
	
	//check if static not empty (or always join? if empty no effect)
		//if not empty, join w buffer

	//read into buffer 
	bytes_read = 0;
	while (bytes_read > 0)
	{
		//check if newline \n in buffer, or returned 0 or -1
		//if yes: stop reading and update static
		//if no: continue reading
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)//error
			free_everything();
		if (bytes_read == 0)//EOF
			break;
		if (scan_nl(buffer))
			break;
	}
	//
	return (nextline);
}


int main ()
{
	int fd;
	char *newline;

	newline = NULL;
	//open file to read
	fd = open("text.txt", O_RDONLY);
	if (fd == -1)
		return -1;
	else
	{
		printf("opened file\n");
		newline = get_next_line(fd);
	}

	printf("new line: %s\n", newline);
	close(fd);
	return (0);
}