#include "tsh.h"
#include <sstream>
#include "string.h"
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
	std::stringstream ss;
	ss.str(cmd);

	char readchar;
	std::string readword;
	int i = 0;

	while(ss.get(readchar)){

		if(readchar == ' ' || readchar == '\0' || readchar == '\n'){
			char *readstr = new char[readword.length() + 1];
			strcpy(readstr, readword.c_str());
			cmdTokens[i++] = readstr;
			readword.clear();
			continue;
		}
		else{
			readword += readchar;
			if(ss.peek() == EOF){
				char *readstr = new char[readword.length() + 1];
				strcpy(readstr, readword.c_str());
				cmdTokens[i++] = readstr;
				readword.clear();
			}
		}
	}
  // TODO: tokenize the command string into arguments
}

void simple_shell::exec_command(char **argv)
{
  if(int pid = fork())
  {
  	int status;
  	waitpid(pid, &status, 0);
  	//parent
  }
  else {
  	 printf("Exec %s", argv[0]);
  	 execvp(argv[0], argv[1] ? argv : argv[0]);
  	//child
  }
}

bool simple_shell::isQuit(char *cmd){
  // TODO: check for the command "quit" that terminates the shell
	return strcmp(cmd, (char *)"quit") == 0;
}
