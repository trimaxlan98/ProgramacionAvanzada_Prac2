/*Rosas Palacios Alan - Enriquez Melendez Jesus - Orozco Segovia Mikael Imanol - Santiago Galvan*/
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
sem_t sem_manicura;
sem_t sem_disp_manicura;//saber si hay estilistas disponibles
sem_t sem_pedicura;
sem_t sem_disp_pedicura;//saber si hay estilistas disponibles
sem_t sem_cliente_listo_c;
sem_t sem_cliente_listo_p;
sem_t sem_cliente_listo_m;
sem_t sem_cliente_listo_pe;

void* Cliente(void*argumento);
void* Empleado_Cortar_Cabello(void*argumento);
void* Empleado_Pintar_Cabello(void* argumento);
void* Empleado_Manicura(void* argumento);
void* Empleado_Pedicura(void* argumento);


void main(void){
    int aux=0;
pthread_t  cliente,empleado_c,empleado_p,empleado_m,empleado_pe;

sem_init(&sem_silla, 0, 5);// Habra 5 sillas
sem_init(&sem_corte, 0, 0);
sem_init(&sem_disp_corte, 0, 2); //habra dos empleados que cortan cabello
sem_init(&sem_pintura, 0, 0);
sem_init(&sem_disp_pintura, 0, 2); //habra dos empleados que pintan cabello
sem_init(&sem_manicura, 0, 0);
sem_init(&sem_disp_manicura, 0, 1); // un empleado que hace manicura
sem_init(&sem_pedicura, 0, 0);
sem_init(&sem_disp_pedicura, 0, 1); //Un empleado hace pedicura
sem_init(&sem_cliente_listo_c, 0, 0);
sem_init(&sem_cliente_listo_p, 0, 0);
sem_init(&sem_cliente_listo_m, 0, 0);
sem_init(&sem_cliente_listo_pe, 0, 0);


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
if (pthread_create(&empleado_m, NULL, Empleado_Manicura, NULL)) {
    printf("Problema en la creacion del hilo\n");
    exit(EXIT_FAILURE);
}
if (pthread_create(&empleado_pe, NULL, Empleado_Pedicura, NULL)) {
    printf("Problema en la creacion del hilo\n");
    exit(EXIT_FAILURE);
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
if (pthread_join(empleado_m, NULL)) {
    printf("Problema al crear enlace con otro hilo\n");
    exit(EXIT_FAILURE);
}
if (pthread_join(empleado_pe, NULL)) {
    printf("Problema al crear enlace con otro hilo\n");
    exit(EXIT_FAILURE);
}
  sem_destroy(&sem_silla);
  sem_destroy(&sem_corte);
  sem_destroy(&sem_disp_corte);
  sem_destroy(&sem_pintura);
  sem_destroy(&sem_disp_pintura);
  sem_destroy(&sem_manicura);
  sem_destroy(&sem_disp_manicura);
  sem_destroy(&sem_pedicura);
  sem_destroy(&sem_disp_pedicura);
  sem_destroy(&sem_cliente_listo_c);
  sem_destroy(&sem_cliente_listo_p);
  sem_destroy(&sem_cliente_listo_m);
  sem_destroy(&sem_cliente_listo_pe);

  exit(EXIT_SUCCESS);
}

void*Cliente(void*argumento){
      int corte=0, pintura=0, manicure=0, pedicura=0;
      sleep(5);
      do { // Si el numero aleatorio es 1 el cliente pedira ese servicio
          corte = rand() % 2;
          pintura = rand() % 2;
          manicure = rand() % 2;
          pedicura = rand() % 2;
      } while (corte == 0 && pintura == 0 && manicure == 0 && pedicura == 0);
      sem_wait(&sem_silla);//sentarse en la silla
      //printf("Resultados del cliente %li: %i, %i, %i, %i\n ",pthread_self(), corte, pintura, manicure, pedicura);
      printf("Soy el cliente %li y estoy sentado \n", pthread_self());
    
      if (corte == 1) {
          sem_wait(&sem_disp_corte); //Esperando disponibilidad del corte
          printf("Soy el cliente %li y quiero un corte\n", pthread_self());
      }
      if (pintura == 1) {
          sem_wait(&sem_disp_pintura); //esperando disponibilidad de la pintura
          printf("Soy el cliente %li y quiero pintar mi cabello\n", pthread_self());
      }
      if (manicure == 1) {
          sem_wait(&sem_disp_manicura); //esperando disponibilidad de la manicura
          printf("Soy el cliente %li y quiero una manicura\n", pthread_self());
      }
      if (pedicura == 1) {
          sem_wait(&sem_disp_pedicura);// esperando disponibilidad de la pedicura
          printf("Soy el cliente %li y quiero una pedicura\n", pthread_self());
      }

      sem_post(&sem_silla);// se levanta de la silla
      printf("El cliente %li desocupo la silla  \n", pthread_self());

      if (corte == 1) {
          sem_post(&sem_cliente_listo_c);//Indicamos que estamos listos para un corte al empleado
          sem_wait(&sem_corte); //esperamos a que se realice el corte
      }
      if (pintura == 1) {
          sem_post(&sem_cliente_listo_p);
          sem_wait(&sem_pintura);
      }
      if (manicure == 1) {
          sem_post(&sem_cliente_listo_m);
          sem_wait(&sem_manicura);
      }
      if (pedicura == 1) {
          sem_post(&sem_cliente_listo_pe);
          sem_wait(&sem_pedicura);
      }


      printf("Soy el cliente %li y termine \n", pthread_self());
      pthread_exit(0);
}
void* Empleado_Cortar_Cabello(void* argumento) {
    while (1) {
        //sleep(5);
        sem_wait(&sem_cliente_listo_c);//Esperamos que el cliente este listo
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy cortando el cabello...\n");
        sleep(2);
        sem_post(&sem_corte);//Indicamos que el corte se realizo
        sem_post(&sem_disp_corte);//Indicamos que hay disponibilidad del corte
    }
    pthread_exit(0);
}
void* Empleado_Pintar_Cabello(void* argumento) {
    while (1) {
        //sleep(5);
        sem_wait(&sem_cliente_listo_p);
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy pintando el cabello...\n");
        sleep(2);
        sem_post(&sem_pintura);
        sem_post(&sem_disp_pintura);
    }
    pthread_exit(0);
}
void* Empleado_Manicura(void* argumento) {
    while (1) {
        //sleep(5);
        sem_wait(&sem_cliente_listo_m);
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy haciendo la manicura...\n");
        sleep(2);
        sem_post(&sem_manicura);
        sem_post(&sem_disp_manicura);
    }
    pthread_exit(0);
}
void* Empleado_Pedicura(void* argumento) {
    while (1) {
        //sleep(5);
        sem_wait(&sem_cliente_listo_pe);
        printf("Soy el Empleado %li \n", pthread_self());
        printf("Estoy haciendo la pedicura...\n");
        sleep(2);
        sem_post(&sem_pedicura);
        sem_post(&sem_disp_pedicura);
    }
    pthread_exit(0);
}