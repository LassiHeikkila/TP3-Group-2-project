#include "headers.h"

int parseline(char* line)
{
	int i = strlen(line);
	while (*line < '0' || *line > '9')
	{
		line ++;
	}

	line[i-3] = '\0';
	i = atoi(line);
	return i;
}

int mem_measure()
{
	FILE* mem = fopen("/proc/self/status","r");

	int result = -1;
	char line[128];

	while (fgets(line,128,mem) != NULL)
	{
		if (strncmp(line,"VmHWM:",6) == 0)
		{
			result = parseline(line);
			break;
		}
	}
	fclose(mem);
	return result;
}

timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}
