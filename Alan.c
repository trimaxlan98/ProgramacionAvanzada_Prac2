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
sem_t sem_disp_corte;//saber si hay estilistas disponibles
sem_t sem_pintura;
sem_t sem_disp_pintura;//saber si hay estilistas disponibles
sem_t sem_cliente_listo_c;
sem_t sem_cliente_listo_p;

void*Cliente(void*argumento);
void* Empleado_Cortar_Cabello(void*argumento);
void* Empleado_Pintar_Cabello(void* argumento);


void main(void){
    int aux=0;
pthread_t  cliente,empleado_c,empleado_p ;

sem_init(&sem_silla, 0, 5);
sem_init(&sem_corte, 0, 0);
sem_init(&sem_disp_corte, 0, 2);
sem_init(&sem_pintura, 0, 0);
sem_init(&sem_disp_pintura, 0, 2);
sem_init(&sem_cliente_listo_c, 0, 0);
sem_init(&sem_cliente_listo_p, 0, 0);

while (aux < 20) {
    if (pthread_create(&cliente, NULL, Cliente, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
    aux++;
}
aux = 0;
while (aux < 2) {
    if (pthread_create(&empleado_c, NULL, Empleado_Cortar_Cabello, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
    aux++;
}
aux = 0;
while (aux < 2) {
    if (pthread_create(&empleado_p, NULL, Empleado_Pintar_Cabello, NULL)) {
        printf("Problema en la creacion del hilo\n");
        exit(EXIT_FAILURE);
    }
    aux++;
}

if(pthread_join(cliente,NULL)){
       printf("Problema al crear enlace con otro hilo\n");
       exit(EXIT_FAILURE);
}
if(pthread_join(empleado_c,NULL)){
     printf("Problema al crear enlace con otro hilo\n");
     exit(EXIT_FAILURE);
}
if (pthread_join(empleado_p, NULL)) {
    printf("Problema al crear enlace con otro hilo\n");
    exit(EXIT_FAILURE);
}

  sem_destroy(&sem_silla);
  sem_destroy(&sem_corte);
  sem_destroy(&sem_disp_corte);
  sem_destroy(&sem_pintura);
  sem_destroy(&sem_disp_pintura);
  sem_destroy(&sem_cliente_listo_c);
  sem_destroy(&sem_cliente_listo_p);

  exit(EXIT_SUCCESS);
}

void*Cliente(void*argumento){
      int corte=0, pintura=0, manicure=0, pedicura=0;
      do {
          corte = rand() % 2;
          pintura = rand() % 2;
          manicure = rand() % 2;
          pedicura = rand() % 2;
      } while (corte == 0 && pintura == 0 && manicure == 0 && pedicura == 0);
      sem_wait(&sem_silla);//sentarse en la silla
      printf("Resultados: %i, %i, %i, %i\n ", corte, pintura, manicure, pedicura);
      printf("Soy el cliente %li y estoy sentado \n", pthread_self());
      //if(corte==1)
      sem_wait(&sem_disp_corte);
      sem_wait(&sem_disp_pintura);
      sem_post(&sem_silla);
      printf("El cliente %li desocupo la silla  \n", pthread_self());
      sem_post(&sem_cliente_listo_c);
      sem_wait(&sem_corte);
      sem_post(&sem_cliente_listo_p);
      sem_wait(&sem_pintura);
      printf("Soy el cliente %li y termine \n", pthread_self());
      pthread_exit(0);
}
void* Empleado_Cortar_Cabello(void* argumento) {
    while (1) {
        sleep(5);
        sem_wait(&sem_cliente_listo_c);
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy cortando el cabello...\n");
        sleep(2);
        sem_post(&sem_corte);
        sem_post(&sem_disp_corte);
    }
    pthread_exit(0);
}
void* Empleado_Pintar_Cabello(void* argumento) {
    while (1) {
        sleep(5);
        sem_wait(&sem_cliente_listo_p);
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy pintando el cabello...\n");
        sleep(2);
        sem_post(&sem_pintura);
        sem_post(&sem_disp_pintura);
    }
    pthread_exit(0);
}