#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

sem_t sem_silla;
sem_t sem_corte;
sem_t sem_e_disponible;//saber si hay estilistas disponibles
sem_t sem_cliente_listo;

void*Cliente(void*argumento);
void*Empleado(void*argumento);


void main(void){
    int aux=0;
pthread_t  cliente,empleado ;

sem_init(&sem_silla, 0, 5);
sem_init(&sem_corte, 0, 0);
sem_init(&sem_e_disponible, 0, 6);
sem_init(&sem_cliente_listo, 0, 0);

while (aux < 20) {
    if (pthread_create(&cliente, NULL, Cliente, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
    aux++;
}
aux = 0;
while (aux < 6) {
    if (pthread_create(&empleado, NULL, Empleado, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
    aux++;
}

if(pthread_join(cliente,NULL)){
       printf("Problema al crear enlace con otro hilo\n");
       exit(EXIT_FAILURE);
}
if(pthread_join(empleado,NULL)){
     printf("Problema al crear enlace con otro hilo\n");
     exit(EXIT_FAILURE);
}

  sem_destroy(&sem_silla);
  sem_destroy(&sem_corte);
  sem_destroy(&sem_e_disponible);
  sem_destroy(&sem_cliente_listo);

  exit(EXIT_SUCCESS);
}

void*Cliente(void*argumento){
      sem_wait(&sem_silla);//sentarse en la silla
      printf("Soy el cliente %li y estoy sentado \n", pthread_self());
      sem_wait(&sem_e_disponible);
      sem_post(&sem_silla);
      printf("Soy el cliente %li y estoy cortandome el cabello \n", pthread_self());
      sem_post(&sem_cliente_listo);
      sem_wait(&sem_corte);
      pthread_exit(0);
}
void* Empleado(void* argumento) {
    while (1) {
        sleep(10);
        sem_wait(&sem_cliente_listo);
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy cortando el cabello...\n");
        sleep(2);
        sem_post(&sem_corte);
        sem_post(&sem_e_disponible);
    }
    pthread_exit(0);
}