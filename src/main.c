#include <general.h>
	
int main(int argc, char** argv) {

    const char* short_options = "l:e::m::";
    const char* modes[] = {"logfile", "execute", "multiplex"};
    
    const struct option long_options[] = {
        {"logfile",required_argument,NULL,'l'},
        {"execute",optional_argument,NULL,'e'},
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
                printf("Case l \n%d",optarg);
                break;
            };
            case 'e': {
                colparam = 1;
                printf("Case e \n");
                params.amount = atoi(optarg);
                break;
            };
            case 'm': {
                colparam = 1;
                printf("Case m \n");
                params.signalname = atoi(optarg);
                break;
            };
            default:
                fprintf(stderr, "Unknown param\n");
                exit(0);
                break;
        }
    }
    return 0;
}
