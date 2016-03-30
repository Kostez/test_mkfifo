#include <general.h>
	
    char* _logfile = "STDERR";
    char* _execute = "";
    int _multiplex = 1;
    
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

void runmylab(){
    /*основное действие*/
	
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
		
		close(pipe0[0]);
		close(pipe1[1]);
		close(pipe2[1]);
		
		while(pid = wait(&status)>0);		//wait выдает номер (pid) потомка, если не осталось потомком, то выдаст 
		
		char fromcammand[1024];
		int l = read(pipe1[0], fromcammand, 1024);
		//printf("In command: %s",fromcammand);
		write(1, fromcammand, l);
		
		
		fd_set inputs;
		struct timeval tv;
		int retval;
		
		
		
		while(1){
			int inexit;
			char *inexit_buf;
			FD_ZERO(&inputs);
			FD_SET(0, &inputs);
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			retval = select(FD_SETSIZE, &inputs, NULL, NULL, &tv);
			switch(retval) {
				case 0:
					printf("timeoutn");
					break;
				case -1:
					perror("select");
					exit(1);
				default:
					if (FD_ISSET(0, &inputs)) {
						inexit = read(0, inexit_buf, 1);
						if (strcmp(inexit_buf, "exit") == 0) {
							exit(0);
						}
					}
					break;
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
		
		printf("CHILD: конец\n");
	}
}
