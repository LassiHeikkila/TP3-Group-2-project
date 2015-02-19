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
		if (strncmp(line,"VmSize:",7) == 0)
		{
			result = parseline(line);
			break;
		}
	}
	fclose(mem);
	return result;
}
