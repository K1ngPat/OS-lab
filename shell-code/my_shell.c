#include  <stdio.h>
#include <signal.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_BACKGROUND_PROCESSES 64
#define MAX_FOREGROUND_PROCESSES 64

pid_t foreground_child_pid = -12;
pid_t background_gpid = -212;

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
	printf("Tokenizing\n");
	kill(-1, SIGKILL);
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void  INThandler(int sig)
{
    
    signal(sig, SIG_IGN);
     

	if(foreground_child_pid != -12)
	{
		kill(foreground_child_pid, SIGKILL);
		
		foreground_child_pid = -12;
	}

	 
	signal(sig, INThandler);

}


int main(int argc, char* argv[]) {

	signal(SIGINT, INThandler);
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	int back_pids[MAX_BACKGROUND_PROCESSES];
	int back_stats[MAX_BACKGROUND_PROCESSES];
	for(int j = 0; j<MAX_BACKGROUND_PROCESSES; j++)
	{
		back_pids[j] = -1;
		back_stats[j] = -12;
	}



	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		//printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);


		int serialexec = 1;

		for(i=0;tokens[i]!=NULL;i++){
			if(strcmp(tokens[i], "&&") == 0)
			{
				serialexec++;
			}
			
		}

		int cmdstarts[serialexec];
		cmdstarts[0] = 0;
		int runner = 1;
		for(i=0;tokens[i]!=NULL;i++){
			if(strcmp(tokens[i], "&&") == 0)
			{
				cmdstarts[runner++] = i+1;
				tokens[i] = NULL;
			}
			
		}


		for(int pkl = 0; pkl < serialexec; pkl++)
		{

		
		// No input case
		if (tokens[0] == NULL && serialexec == 1)
		{
			free(tokens);
			break;
		}
		
   
       //do whatever you want with the commands, here we just print them

		// for(i=0;tokens[i]!=NULL;i++){
		// 	printf("found token %s (remove this debug output later)\n", tokens[i]);
			
		// }

		//	Here we work


		bool bkgd_proc = false;
		for(i=0;tokens[cmdstarts[pkl+i]]!=NULL;i++){
			if((strcmp(tokens[cmdstarts[pkl]+i], "&") == 0) && (tokens[cmdstarts[pkl]+i+1] == NULL))
			{
				bkgd_proc = true;
			}
			
		}
		
		if(strcmp(tokens[cmdstarts[pkl]], "exit") == 0)
		{	
			// exit command

			// clear malloc memory 
			for(i=0;tokens[i]!=NULL;i++)
			{
				free(tokens[i]);
			}
			free(tokens);

			// kill children
			for(int pp = 0; pp<MAX_BACKGROUND_PROCESSES; pp++)
			{
				if(back_pids[pp] != -1)
				{
					kill(back_pids[pp], SIGKILL);
				}
			}

			break;
		}

		


		pid_t childpid = fork();
		int stats = -999;

		
		
    	
		if(childpid == 0)
		{
			// we is child
			//exit(3);
			if(strcmp(tokens[cmdstarts[pkl]], "cd") == 0)
			{
				exit(12);
			}

			if(bkgd_proc)
			{
				for(i=0;tokens[cmdstarts[pkl]+i]!=NULL;i++){
					if(strcmp(tokens[cmdstarts[pkl]+i], "&") == 0)
					{
						tokens[cmdstarts[pkl]+i] = NULL;
					}
					
				}
			}


			int nogg = execvp(tokens[cmdstarts[pkl]], &(tokens[cmdstarts[pkl]]));
			exit(-2);
		}
		else
		{
			// we is daddy

			foreground_child_pid = childpid;



			// TODO: Clear background procs
			for(int pp = 0; pp<MAX_BACKGROUND_PROCESSES; pp++)
			{
				if(back_pids[pp]>0)
				{
					waitpid(back_pids[pp], &(back_stats[pp]), WNOHANG);
				

					if(WIFEXITED(back_stats[pp]))
					{
						printf("Shell: Background process with PID %d finished \n", back_pids[pp]);
						back_pids[pp] = -1;
						back_stats[pp] = -12;
					}

				}
			}


			if(!bkgd_proc)
			{
				
				pid_t confirmchildpid = waitpid(childpid, &stats, 0);

				foreground_child_pid = -12;
			}
			else
			{	
				if(background_gpid < 0)
				{
					background_gpid = childpid;
				}
				int newind;
				for(newind = 0; newind<MAX_BACKGROUND_PROCESSES; newind++)
				{
					if(back_pids[newind] == -1)
					{
						break;
					}
				}
				setpgid(childpid, background_gpid);
				back_pids[newind] = childpid;
				back_stats[newind] = -13;

			}

			//printf("%d", stats);
			if(stats/256 == 12)
			{
				// cd was called
				chdir(tokens[1]);
			}

			// if(strcmp(tokens[0], "cd") == 0)
			// {
			// 	// cd was called
			// 	chdir(tokens[1]);
			// }

			

		}



		// Here we done
		}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);


		

	}
	return 0;
}
