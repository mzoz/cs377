#include "tsh.h"

using namespace std;

int main(){
	char cmd[81];
	char *cmdTokens[25];
	simple_shell *shell = new simple_shell();
	
	while (true){
		cout << "tsh> ";
		fgets(cmd, sizeof(cmd), stdin);
		if (cmd[0] == '\n') continue;
		shell->parse_command(cmd, cmdTokens);
		if (shell->isQuit(*cmdTokens)){
			exit(0);
		}
		else{
			shell->exec_command(cmdTokens);
		}

	}
	exit(1);
}