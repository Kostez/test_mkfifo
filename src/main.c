#include <general.h>
	
    char* _logfile = "STDERR";
    char* _execute = "";
    int _multiplex = 1;
    int childIsZombie;
    
int main(int argc, char** argv) {
    
    const char* short_options = "l::e::m::";
    const char* modes[] = {"logfile", "execute", "multiplex"};
    
    const struct option long_options[] = {
        {"logfile",optional_argument,NULL,'l'},
        {"execute",required_argument,NULL,'e'},
        {"multiplex",optional_argument,NULL,'m'},
    };
    
    int rez;
    int option_index;
    int colparam = 0;
    
    while ((rez=getopt_long(argc,argv,short_options,
                long_options,&option_index))!=-1){
        switch(rez){
            case 'l': {
                colparam = 1;
                _logfile = optarg;
                break;
            };
            case 'e': {
                colparam = 1;
                _execute = optarg;
                break;
            };
            case 'm': {
                colparam = 1;
                _multiplex = atoi(optarg);
                break;
            };
            default:
                fprintf(stderr, "Unknown param\n");
                exit(0);
	        break;
		}
	}
    
	printf("_logfile: %s\n",_logfile);		
	printf("_execute: %s\n",_execute);		
	printf("_multiplex: %d\n",_multiplex);
    
	if(colparam == 0){
		printf("Use README.MD to run with parameters\n");
		return 0;
	}
	
	if(strcmp("", _execute ) == 0 ){
		printf("Укажите \"command and args\" командой: --multiplex \"command and args\"\n");
		exit(1);	
	}
	
	if(((_multiplex > 1)||(_multiplex < 0))){
		printf("Укажите значение multiplex равное 0 или 1\n");
		exit(1);
	}
	
	runmylab();
    
	return 0;
}

void sighandler_child(int signum, siginfo_t *info, void *f) {
	switch(signum){
		case SIGCHLD:
			printf("%i TERMINATED WITH EXIT CODE: %i\n", info->si_pid, info->si_code);
			childIsZombie = 1;
			break;
		default:
			printf("unknown si_code\n");
			return;	
	}
}



void runmylab(){
    /*основное действие*/
    
    childIsZombie = 0;
	struct sigaction act; 
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = sighandler_child;
	
	int pid, status;
	int pipe0[2];
	int pipe1[2];
	int pipe2[2];
	
	if (pipe(pipe0) == -1) {
		perror("pipe0 err");
		exit(EXIT_FAILURE);
	}
	
	if (pipe(pipe1) == -1) {
		perror("pipe1 err");
		exit(EXIT_FAILURE);
	}
    
	if (pipe(pipe2) == -1) {
		perror("pipe2 err");
		exit(EXIT_FAILURE);
	}
	
	pid=fork();
    
	if(pid == -1) {
		perror("fork err");
		exit(0);
	} else if(pid > 0) {
		printf("PARENT: начало\n");
		printf("жду:\n");
		
		sigaction(SIGCHLD, &act, NULL);
		
		close(pipe0[0]);
		close(pipe1[1]);
		close(pipe2[1]);
		
//		while(pid = wait(&status)>0);		//wait выдает номер (pid) потомка, если не осталось потомком, то выдаст
		printf("status: %d\n", status);
		
		char fromcammand[1024];
//		int l = read(pipe1[0], fromcammand, 1024);
		//printf("In command: %s",fromcammand);
//		write(1, fromcammand, l);
		
		fd_set inputs;
		struct timeval tv;
		int retval;
		
		while(1){
			FD_ZERO(&inputs);
			FD_SET(pipe1[0], &inputs);
			FD_SET(pipe2[0], &inputs);
			FD_SET(0, &inputs);
			printf("Начало while\n");
			int inexit;
			char *inexit_buf;
			
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			printf("Перед select\n");
			retval = select(FD_SETSIZE, &inputs, NULL, NULL, &tv);
			printf("После select\n");
			switch(retval) {
				case 0:
					printf("timeoutn\n");
					break;
				case -1:
					printf("-1 error\n");
					exit(1);
				default:
					printf("Начало default\n");
					if (FD_ISSET(0, &inputs)) {
						printf("0\n");
						inexit = read(0, inexit_buf, 1024);
						write(1, inexit_buf, inexit);
						printf("%d\n", inexit);
						if (strstr(inexit_buf, "exit") != NULL) {
							exit(0);
						}
						write(pipe0[1], inexit_buf, inexit);
					}
					if(FD_ISSET(pipe1[0], &inputs)){
						printf("pipe1[0]\n");
						inexit = read(pipe1[0], inexit_buf, 1024);
						if(inexit == 0){
							exit(0);
						}
						printf("%s\n", inexit_buf);
					}
					if(FD_ISSET(pipe2[0], &inputs)){
						printf("pipe2[0]\n");
						inexit = read(pipe2[0], inexit_buf, 1024);
						if(inexit == 0){
							exit(0);
						}
						printf("%s\n", inexit_buf);
					}
					break;
			}
			if (childIsZombie) {
				exit(1);
			}
		}
		
		printf("PARENT: конец\n");
		
	} else {
		printf("CHILD: начало\n");
		
		dup2(pipe0[0], 0);
		close(pipe0[1]);
		dup2(pipe1[1], 1);
		close(pipe1[0]);
		dup2(pipe2[1], 2);
		close(pipe2[0]);
		
		system(_execute);
		close(pipe0[0]);
		close(pipe1[1]);
		close(pipe2[1]);
		printf("CHILD: конец\n");
	}
}
