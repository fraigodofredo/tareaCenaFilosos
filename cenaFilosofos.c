#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define FILOSOFOS 5
#define COMIDAS 3

typedef struct {
    int id;
    sem_t* tenedores;
    sem_t* sala;
} Datos;

void* filosofo(void* arg);

int main() {

    pthread_t hilos[FILOSOFOS];
    sem_t tenedores[FILOSOFOS];
    sem_t sala;
    Datos datos[FILOSOFOS];
    for (int i = 0; i < FILOSOFOS; i++) {
        sem_init(&tenedores[i], 0, 1);
    }

    sem_init(&sala, 0, FILOSOFOS - 1);
    for (int i = 0; i < FILOSOFOS; i++) {

        datos[i].id = i;
        datos[i].tenedores = tenedores;
        datos[i].sala = &sala;

        pthread_create(&hilos[i], NULL, filosofo, &datos[i]);
    }

    for (int i = 0; i < FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    for (int i = 0; i < FILOSOFOS; i++) {
        sem_destroy(&tenedores[i]);
    }
    sem_destroy(&sala);
    return 0;
}

void* filosofo(void* arg) {

    Datos* datos = (Datos*)arg;
    int id = datos->id;
    int izquierda = id;
    int derecha = (id + 1) % FILOSOFOS;

    for (int i = 1; i <= COMIDAS; i++) {

        printf("Filosofo %d esta pensando\n", id + 1);
        sleep(1);
        sem_wait(datos->sala);
        sem_wait(&datos->tenedores[izquierda]);
        sem_wait(&datos->tenedores[derecha]);
        printf("Filosofo %d esta comiendo (%d)\n", id + 1, i);
        sleep(2);
        sem_post(&datos->tenedores[derecha]);
        sem_post(&datos->tenedores[izquierda]);
        sem_post(datos->sala);
    }

    printf("Filosofo %d termino de comer\n", id + 1);

    return NULL;
}