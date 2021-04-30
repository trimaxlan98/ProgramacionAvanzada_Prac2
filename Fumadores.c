#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

sem_t sem_tabaco;
sem_t sem_papel;
sem_t sem_cerillo;
sem_t sem_agente;

void*fumador1(void*argumento);
void*fumador2(void*argumento);
void*fumador3(void*argumento);
void*agente(void*argumento);

int papel=0,tabaco=1,cerillo=2;

void main(void){
pthread_t  agent, fum1_c, fum2_p, fum3_t;

sem_init(&sem_cerillo,0,0);
sem_init(&sem_tabaco,0,0);
sem_init(&sem_papel,0,0);
sem_init(&sem_agente,0,1);

if(pthread_create(&fum1_c,NULL,fumador1,NULL)){
     printf("Problema en la creacion del hilo\n");
     exit(EXIT_FAILURE);
}

if(pthread_create(&fum2_p,NULL,fumador2,NULL)){
printf("Problema en la creacion del hilo\n");
exit(EXIT_FAILURE);
}

if(pthread_create(&fum3_t,NULL,fumador3,NULL)){
      printf("Problema en la creacion del hilo\n");
      exit(EXIT_FAILURE);
}
if(pthread_create(&agent,NULL,agente,NULL)){
     printf("Problema en la creacion del hilo\n");
     exit(EXIT_FAILURE);
}

printf("Proceso papa %li\n",pthread_self());

if(pthread_join(fum1_c,NULL)){
       printf("Problema al crear enlace con otro hilo\n");
       exit(EXIT_FAILURE);
}
if(pthread_join(fum2_p,NULL)){
     printf("Problema al crear enlace con otro hilo\n");
     exit(EXIT_FAILURE);
}
if(pthread_join(fum3_t,NULL)){
     printf("Problema al crear enlace con otro hilo\n");
     exit(EXIT_FAILURE);
}
if(pthread_join(agent,NULL)){
    printf("Problema al crear enlace con otro hilo\n");
    exit(EXIT_FAILURE);
}
  sem_destroy(&sem_cerillo);
  sem_destroy(&sem_papel);
  sem_destroy(&sem_tabaco);
  sem_destroy(&sem_agente);
  exit(EXIT_SUCCESS);
}

void*fumador1(void*argumento){
     while(1){
               sem_wait(&sem_cerillo);
               printf("Soy el fumador %li y tengo el cerillo\n",pthread_self());
               sleep(2);
               sem_post(&sem_agente);
      }
      pthread_exit(0);
}


void* fumador2(void* argumento){
   while(1){
                sem_wait(&sem_papel);
                printf("Soy el fumador %li y tengo el papel\n",pthread_self());
                sleep(2);
                sem_post(&sem_agente);
   }
    pthread_exit(0);
}
void* fumador3(void* argumento){
        while(1){
                  sem_wait(&sem_tabaco);
                  printf("Soy el fumador %li y tengo el tabaco\n",pthread_self());
                  sleep(2);
                  sem_post(&sem_agente);
       } 
       pthread_exit(0);
}

void* agente(void* argumento){
      int ing1=0,ing2=0;
     
      while(1){
              sem_wait(&sem_agente);     
              srand(time(NULL));   //  cambia de semilla aleatoria   en caso de que siempre salgan los mismos ingredientes
              do{
                           ing1=rand()%3;
                           ing2=rand()%3;
                }while(ing1==ing2);

                printf("El ingrediente 1 es: %i\n",ing1);
                printf("El ingrediente 2 es: %i\n",ing2);

                if((ing1==0 && ing2==1)||(ing1==0 && ing2==1)){
                             sem_post(&sem_cerillo);
                             //sem_wait(&sem_agente);                             // quitar esta línea       mañana explico porque
                }
                else if((ing1==1 && ing2==2)||(ing1==2 && ing2==1)){
                              sem_post(&sem_papel);
                              //sem_wait(&sem_agente);                         // quitar esta línea       mañana explico porque
               }
               else if((ing1==0 && ing2==2)||(ing1==2 && ing2==0)){
                             sem_post(&sem_tabaco);
                             //sem_wait(&sem_agente);                       // quitar esta línea       mañana explico porque
             }
   }
   pthread_exit(0);
}