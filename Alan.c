#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

sem_t sem_silla;
sem_t sem_silla_E;

void*Cliente(void*argumento);
void*Empleado(void*argumento);


void main(void){
    int aux;
pthread_t  cliente,empleado ;

sem_init(&sem_silla,0,5);

while (aux < 20) {
    if (pthread_create(&cliente, NULL, Cliente, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
}

while (aux < 6) {
    if (pthread_create(&empleado, NULL, Empleado, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
}


printf("Proceso papa %li\n",pthread_self());

if(pthread_join(cliente,NULL)){
       printf("Problema al crear enlace con otro hilo\n");
       exit(EXIT_FAILURE);
}
if(pthread_join(empleado,NULL)){
     printf("Problema al crear enlace con otro hilo\n");
     exit(EXIT_FAILURE);
}

  sem_destroy(&sem_cerillo);

  exit(EXIT_SUCCESS);
}

void*Cliente(void*argumento){
      printf("Soy el cliente %li \n", pthread_self());
      pthread_exit(0);
}
void* Empleado(void* argumento) {
    printf("Soy el Empleado %li \n", pthread_self());
    pthread_exit(0);
}