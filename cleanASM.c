#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
char* trim(char *str)
{
    size_t sz = sizeof(str);
    if(sz == 0)return 0;

    int i = 0 ; 
    while( i < sizeof(str) )
    {
        if( isspace(str[i]) )
            i++;
        else
            break;  
    }

    if( i == sz )
        return 0;
    else
        return str + i;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s filename", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *src = fopen(argv[1], "r");

    char tgtname[128]= {0};
    sprintf(tgtname, "%s%s","clean", argv[1]);

    int fd = open(tgtname, O_RDWR|O_CREAT, S_IRWXU);
    
    if(fd == -1)
    {
        printf("Err: open() %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }

    char *line;
    ssize_t len;
    while(  (len = getline(&line, &len, src )) != -1  ) 
    {
        char *ptr = trim(line);
        if( ptr[0] != '.')
        {   
            int i = 0;
            while( line[i] != '\00' && line[i] != '\0')
            {
                write(fd, &line[i], sizeof(char));
                i++;
            }

            write(STDOUT_FILENO, line, strlen(line)+1);
        }
    }

    fclose(src);

    if(close(fd) < 0)
    {
        printf("Err: close()\n");
        exit(EXIT_FAILURE);
    }
}