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
                printf("Case l \n%s\n",optarg);
                _logfile = optarg;
                break;
            };
            case 'e': {
                colparam = 1;
                printf("Case 2 \n%s\n",optarg);
                _execute = optarg;
                break;
            };
            case 'm': {
                colparam = 1;
                printf("Case 3 \n%s\n",optarg);
                _multiplex = atoi(optarg);
                break;
            };
            default:
                fprintf(stderr, "Unknown param\n");
                exit(0);
                break;
        }
    }
     if(colparam == 0){
        printf("Use README.MD to run with parameters\n");
        return 0;
    }
    
    printf("_logfile: %s\n",_logfile);
    printf("_execute: %s\n",_execute);
    printf("_multiplex: %d\n",_multiplex);
    
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
	/*основное дейстие здесь*/
	
}
