#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<semaphore.h>
#include<fcntl.h>
#include <pthread.h>

void *Agente (void*arg);
void *Fumador1 (void*arg);
void* Fumador2(void* arg);
void* Fumador3(void* arg);

sem_t cerillo;
sem_t papel;
sem_t tabaco;
sem_t agente;

int PAPEL = 0;
int TABACO = 1;
int CERILLO = 2;

void  main(void)
{
    pthread_t th1,th2,th3,th4;
    /*Inicializar los semaforos */
    sem_init(&agente, 0, 1);
    sem_init(&cerillo, 0, 0);
    sem_init(&papel, 0, 0);
    sem_init(&tabaco, 0, 0);
    

    /*crear los procesos ligeros */
    pthread_create(&th1,NULL,Agente,NULL);
    pthread_create(&th2,NULL, Fumador1 ,NULL);
    pthread_create(&th3, NULL, Fumador2 , NULL);
    pthread_create(&th4, NULL, Fumador3 , NULL);


    /*Esperar su finalización*/
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);
    sem_destroy(&agente);
    sem_destroy(&cerillo);
    sem_destroy(&papel);
    sem_destroy(&tabaco);
    exit(0);
}

void *Agente (void*var){
int ing1 = 0;
int ing2 = 0;

while(1)
{
            /*un hueco menos */
    sem_wait(&agente); //P
    do {
        ing1 = rand() % 3;
        ing2 = rand() % 3;
        printf("\n ing1 : %i\t ing2 : %i \n", ing1, ing2);
    } while (ing1 == ing2); //Necesitamos dos ingredientes diferentes
    printf("\nEl ingrediente 1 es: %i\n", ing1);
    printf("El ingrediente 2 es: %i\n", ing2);
    if ((ing1==0 && ing2 ==1) || (ing2 == 0 && ing1 == 1)) {
        sem_post(&cerillo);//V
        sem_wait(&agente);
    }
    else if ((ing1 == 0 && ing2 == 2) || (ing2 == 0 && ing1 == 2)) {
        sem_post(&tabaco);//V
        sem_wait(&agente);
    }
    else if ((ing1 == 1 && ing2 == 2) || (ing2 == 1 && ing1 == 2)) {
        sem_post(&papel);//V
        sem_wait(&agente);
    }
}
pthread_exit(0);
}

void *Fumador1 (void*var){

while(1)
{
            /*un elemento menos */
    sem_wait(&cerillo); //P
    printf("\nSpy el fumador tengo el cerillo %li\n", pthread_self());
    sleep(2);
    sem_post(&agente);

}
pthread_exit(0);
}
void* Fumador2(void* var) {

    while(1)
    {
        /*un elemento menos */
        sem_wait(&papel); //P
        printf("\nSpy el fumador tengo el papel %li\n", pthread_self());
        sleep(2);
        sem_post(&agente);

    }
    pthread_exit(0);
}
void* Fumador3(void* var) {

    while(1)
    {
        /*un elemento menos */
        sem_wait(&tabaco); //P
        printf("\nSpy el fumador tengo el tabaco %li\n", pthread_self());
        sleep(2);
        sem_post(&agente);

    }
    pthread_exit(0);
}

