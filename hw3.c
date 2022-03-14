#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
FILE *fp;
/*
Name: pranay
BlazerId: 
Project #: Homework 3
To compile: gcc hw3.c
To run: ./<name of executable> <commands and arguments> <directory>
ex: ./executable -f txt -S -s 100 -t f ./projects/
*/
typedef struct
{
    int S_flag;           // is the S flag provided?
    int s_flag;           // is the s flag provided?
    int f_flag;           // is the f flag provided?
    float e_flag;
    char *ecmd;
    float E_flag;
    char *Ecmd;
    int t_flag;           // is the t flag provided?
    int fileSize;         // s flag value
    char filterTerm[300]; // f flag value
    char fileType[2];     // t flag value
} FlagArgs;
    FlagArgs flagArgs = {
        .S_flag = 0,
        .s_flag = 0,
        .f_flag = 0,
        .t_flag = 0};
    char line[100];        // init some memory for the line that will be printed
// type definition of the function pointer. It's void because it won't return anything
typedef void FileHandler(char *filePath, char *dirfile, FlagArgs flagArgs, int nestingCount);
int  print_list(char *,int nestingCount,char type);
// the function that will be used for this assignment
void myPrinterFunction(char *filePath, char *dirfile, FlagArgs flagArgs, int nestingCount)
{
    struct stat buf;       // buffer for data about file
    lstat(filePath, &buf); // very important that you pass the file path, not just file name
    strcpy(line, "");      // verify a clean start
    strcat(line, dirfile); // init the line with the file name
    if (flagArgs.S_flag) // S case
    {
        char strsize[10];                          // allocate memory for the string format of the size
        sprintf(strsize, " %d", (int)buf.st_size); // assign the size to the allocated string
        strcat(line, strsize);                     // concatenate the line and the size
    }
    if (flagArgs.s_flag) // s case
    {
        if (flagArgs.fileSize > (int)buf.st_size) // if the file size is less than the expected
        {
            strcpy(line, ""); // clear the line print
        }
    }
    if (flagArgs.f_flag) // f case
    {
        if (strstr(dirfile, flagArgs.filterTerm) == NULL) // if the filter does not appear in the file
        {
            strcpy(line, ""); // clear the line print
        }
    }
    if (flagArgs.t_flag) // t case
    {
        if (strcmp(flagArgs.fileType, "f") == 0) // if the provided t flag is "f"
        {
            if (S_ISDIR(buf.st_mode) != 0) // if the file is a dir
            {
                strcpy(line, ""); // clear the line print
            }
        }
        if (strcmp(flagArgs.fileType, "d") == 0) // if the provided t flag is "d"
        {
            if (S_ISREG(buf.st_mode) != 0) // if the file is a regular file
            {
                strcpy(line, ""); // clear the line print
            }
        }
    }
  print_list(filePath,nestingCount,(S_ISDIR(buf.st_mode)?0:1)); /*Iam not passing the directory*/
}
void readFileHierarchy(char *dirname, int nestingCount, FileHandler *fileHandlerFunction, FlagArgs flagArgs)
{
    struct dirent *dirent;
    DIR *parentDir = opendir(dirname); // open the dir
    if (parentDir == NULL)             // check if there's issues with opening the dir
    {
        printf("Error opening directory '%s'\n", dirname);
        exit(-1);
    }
    while ((dirent = readdir(parentDir)) != NULL)
    {
        if (strcmp((*dirent).d_name, "..") != 0 &&
            strcmp((*dirent).d_name, ".") != 0) // ignore . and ..
        {
            char pathToFile[300];                                                  
// init variable of the path to the current file
            sprintf(pathToFile, "%s/%s", dirname, ((*dirent).d_name));             
// set above variable to be the path
            fileHandlerFunction(pathToFile, (*dirent).d_name, flagArgs, nestingCount); // function pointer call
            if ((*dirent).d_type == DT_DIR)                                        
// if the file is a dir
            {
                nestingCount++;                                                    
// increase nesting before going in
                readFileHierarchy(pathToFile, nestingCount, fileHandlerFunction, flagArgs); // reccursive call
                nestingCount--;                                                    
// decrease nesting once we're back
            }
        }
    }
    closedir(parentDir); // make sure to close the dir
}
	typedef struct str{
	   char cmd[10];;
	   char *argv[10];
	}STR;
	char first[10];
	STR str={"/bin/",NULL};
int   print_list(char *ss,int nestingCount,char type)
{
	static char entery;
//	printf("t=%d\n",type);
    if (strcmp(line, "") != 0) // check to prevent printing empty lines
    {
        int i = 0;
        for (i = 0; i <= nestingCount; i++) // tab printer
        {
            printf("\t"); // print a tab for every nesting
        }
        printf("%s\n", line); // print the line after the tabs
    }

    if(flagArgs.E_flag && type ==1) //type 1 is regular file
    {
    	if(fp != NULL)
		fprintf(fp,"%s\n",ss);
    }
    if(flagArgs.e_flag && type ==1) //type 1 is regular file
    {
	//printf("ss=%s \n",ss);
	if(entery ==0)
	{ 
		int i;
	for( i=0;flagArgs.ecmd[i];i++)
	{
		char c= flagArgs.ecmd[i];
	     if(flagArgs.ecmd[i] ==' ')
	          {
			  strncat(first,flagArgs.ecmd,i);
			  str.argv[0]= &flagArgs.ecmd[i+1];
				break;
		  }
	     else
		     //strcat(str.cmd,flagArgs.ecmd[i]);
		     strncat(str.cmd,&c,1);
	}
	entery =1;
	}
	
    
#if 0
	//		  str.argv[1]= ss;
	printf("%s\n",str.cmd);
	printf("%s\n",first);
	printf("%s\n",str.argv[0]);
#endif
	pid_t e_ss_pid;
	e_ss_pid = fork();
		if(e_ss_pid==0)	execl(str.cmd,first,str.argv[0],ss,NULL);
		else{ wait(NULL);return 0;}
	
    }
}
int main(int argc, char **argv)
{
    // init opt :
    int opt = 0;
    // init a flag struct with 0s
    // Parse arguments:
    while ((opt = getopt(argc, argv, "Ss:e:f:E:t:")) != -1)
    {
        switch (opt)
        {
        case 'S':
            flagArgs.S_flag = 1; // set the S_flag to a truthy value
            break;
        case 's':
            flagArgs.s_flag = 1;              // set the s_flag to a truthy value
            flagArgs.fileSize = atoi(optarg); // set fileSize to what was provided
            break;
        case 'E':
            flagArgs.E_flag = 1;               // set the t_flag to a truthy value
              flagArgs.Ecmd =optarg; // set fileType to what was provided
            break;
        case 'f':
            flagArgs.f_flag = 1;                 // set the f_flag to a truthy value
            strcpy(flagArgs.filterTerm, optarg); // set filterTerm to what was provided
            break;
        case 't':
            flagArgs.t_flag = 1;               // set the t_flag to a truthy value
            strcpy(flagArgs.fileType, optarg); // set fileType to what was provided
            break;
        case 'e':
            flagArgs.e_flag = 1;               // set the t_flag to a truthy value
              flagArgs.ecmd =optarg; // set fileType to what was provided
            break;
        }
    }
    if(flagArgs.E_flag ==1)
    {
      fp = fopen("E_file.txt","w");
      if(fp ==NULL)
      {
	      printf("Error in file creation\n");
	      return 0;
      }
      fprintf(fp,"%s\n",flagArgs.Ecmd);
      //fclose(fp);

    }
    if (opendir(argv[argc - 1]) == NULL) // check for if a dir is provided
    {
        char defaultdrive[300];
        getcwd(defaultdrive, 300);    // get the current working directory (if no directory was provided)
        printf("%s\n", defaultdrive); // prints the top-level dir
        readFileHierarchy(defaultdrive, 0, myPrinterFunction, flagArgs);
    }
    else
    readFileHierarchy(argv[argc - 1], 0, myPrinterFunction, flagArgs);
    //if(fp != NULL)
    {
	    printf("##\n");
	 fclose(fp);
	 pid_t p;
	 p = fork();
	 if(p==0)
	 {
		 fp = fopen("E_file.txt","r");
		 if(fp ==NULL)
		 {
			 printf("Error in file creation\n");
			 return 0;
		 }
		 char buff[100000]={0};
		 int i=0;
		 char * str;
		 size_t size;
	for (i =1; getline(&str,&size,fp) >0; i++)
	{
		strcat(buff,str);
	}
	//	printf("##%s\n",buff);
	  char *pr[10000]={NULL};
	  int ll=0;
	  char *tr;
	   tr = buff;
	  char *spl;
	   while((spl = strtok_r(tr," \n",&tr)))
		  pr[ll++]= spl;
	  execvp(pr[0],pr) ;
	  
	 }
	 else
		 wait(NULL);
    }

	
    return 0;
}
