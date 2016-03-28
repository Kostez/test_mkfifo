#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
static volatile sig_atomic_t sflag;

static sigset_t signal_new, signal_old, signal_leer;

static void sigfunc(int sig_nr)
{
  fprintf(stderr, "SIGPIPE вызывает завершение
          программы\n");
  exit(0);
}

void signal_pipe(void)
{
  if(signal(SIGPIPE, sigfunc) == SIG_ERR)
  {
    fprintf(stderr, "Невозможно получить сигнал
            SIGPIPE\n");
    exit(0);
  }

  /*Удаляем все сигналы из множества сигналов*/

  sigemptyset(&signal_leer);
  sigemptyset(&signal_new);
  sigaddset(&signal_new, SIGPIPE);

  /*Устанавливаем signal_new и сохраняем его*/
  /* теперь маской сигналов будет signal_old*/

  if(sigprocmask(SIG_UNBLOCK, &signal_new,
     &signal_old) < 0)
  exit(0);
}

int main()
{
  int r_fifo, w_fifo; /*дескрипторы FIFO*/
  char buffer[]="Текстовая строка для fifo\n";
  char buf[100];
  pid_t pid;
  signal_pipe();
  unlink("/tmp/fifo0001.1");

  /*Создаем FIFO*/

  if((mkfifo("/tmp/fifo0001.1", O_RDWR)) == -1)
  {
    fprintf(stderr, "Невозможно создать fifo\n");
    exit(0);
  }

  pid=fork();
  if(pid == -1)
    { perror("fork"); exit(0);}

  else if(pid > 0) /*Родитель читает из FIFO*/
  {
    if (( r_fifo=open("/tmp/fifo0001.1", O_RDONLY))<0)
      { perror("r_fifo open"); exit(0); }

    /*Ждем окончания потомка*/

    while(wait(NULL)!=pid);

     /*Читаем из FIFO*/

    read(r_fifo, &buf, sizeof(buf));
    printf("%s\n",buf);
    close(r_fifo);
  }

  else /*Потомок записывает в FIFO*/
  {
    if((w_fifo=open("/tmp/fifo0001.1", O_WRONLY))<0)
      { perror("w_fifo open"); exit(0); }
      
    /*Записываем в FIFO*/

    write(w_fifo, buffer, strlen(buffer));
    close(w_fifo); /*EOF*/
    exit(0);
  }
  return 0;
}
