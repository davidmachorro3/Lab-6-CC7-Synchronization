#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int num_prints = 0;

int numero_reacciones;
int cantidad_nitrogenos;
int cantidad_oxigenos;
int cantidad_hidrogenos;

int oxigenos_listos = 0;
int hidrogenos_listos = 0;
int nitrogenos_listos = 0;

pthread_mutex_t *lock_global;
pthread_cond_t *hay_suficientes;
pthread_cond_t *esperando_reaccion;
pthread_cond_t *faltan_hidrogenos;
pthread_cond_t *faltan_oxigenos;
pthread_cond_t *faltan_nitrogenos;

time_t t;

struct chemical_thread {
    int ID;
    int type;
};

void *Ni(void *chem_thread)
{
    pthread_mutex_lock(lock_global);
    struct chemical_thread *real_chem_thread = chem_thread;
    while(nitrogenos_listos == 1) {
        pthread_cond_wait(hay_suficientes, lock_global);
    }
    printf("Se crea id%d Nitrogeno\n\n",real_chem_thread->ID);
    nitrogenos_listos++;
    if(hidrogenos_listos == 3 && oxigenos_listos == 2) {
        printf(">>>>>> una molecula de NH3\n\n");
        printf("FUEGO!!!!!!\n\n");
        printf("id%d Nitrogeno\n\n", real_chem_thread->ID);
        num_prints++;
        pthread_cond_broadcast(esperando_reaccion);
    } else {
        while(hidrogenos_listos < 3 || oxigenos_listos < 2) {
            pthread_cond_wait(esperando_reaccion, lock_global);
        }
        printf("id%d Nitrogeno\n\n", real_chem_thread->ID);
        num_prints++;
        if(num_prints == 6) {
            num_prints = 0;
            hidrogenos_listos = 0;
            nitrogenos_listos = 0;
            oxigenos_listos = 0;
            pthread_cond_broadcast(hay_suficientes);
            printf("### Reaccion exitosa! Convertimos un acido en ... agua?\n\n");
        }
    }
    pthread_mutex_unlock(lock_global);
}

void *Ox(void *chem_thread)
{
    pthread_mutex_lock(lock_global);
    struct chemical_thread *real_chem_thread = chem_thread;
    while(oxigenos_listos == 2) {
        pthread_cond_wait(hay_suficientes, lock_global);
    }
    printf("Se crea id%d Oxigeno\n\n",real_chem_thread->ID);
    oxigenos_listos++;
    /*if(oxigenos_listos == 2 && hidrogenos_listos == 3 && nitrogenos_listos == 1) {
        printf(">>>>> una molecula de O2\n\n");
        printf("FUEGO!!!!!\n\n");
        printf("id%d Oxigeno\n\n", real_chem_thread->ID);
        num_prints++;
        pthread_cond_broadcast(esperando_reaccion);
    } else {
        while(oxigenos_listos < 2 || hidrogenos_listos < 3 || nitrogenos_listos < 1) {
            pthread_cond_wait(esperando_reaccion, lock_global);
        }
        printf("id%d Oxigeno\n\n", real_chem_thread->ID);
        num_prints++;
        if(num_prints == 6) {
            num_prints = 0;
            hidrogenos_listos = 0;
            nitrogenos_listos = 0;
            oxigenos_listos = 0;
            printf("### Reaccion exitosa! Convertimos un acido en ... agua?\n\n");
        }
        
    } */
    if(oxigenos_listos == 2) {
        printf(">>>>>> una molecula de O2\n\n");
        if(hidrogenos_listos == 3 && nitrogenos_listos == 1) {
            printf("FUEGO!!!!!!\n\n");
            printf("id%d Oxigeno\n\n", real_chem_thread->ID);
            num_prints++;
            pthread_cond_broadcast(esperando_reaccion);
        } else {
            while(hidrogenos_listos < 3 || nitrogenos_listos < 1) {
                pthread_cond_wait(esperando_reaccion, lock_global);
            }
            printf("id%d Oxigeno\n\n", real_chem_thread->ID);
            num_prints++;
            if(num_prints == 6) {
                num_prints = 0;
                hidrogenos_listos = 0;
                nitrogenos_listos = 0;
                oxigenos_listos = 0;
                pthread_cond_broadcast(hay_suficientes);
                printf("### Reaccion exitosa! Convertimos un acido en ... agua?\n\n");
            }
        }
    } else {
        while(oxigenos_listos < 2 || hidrogenos_listos < 3 || nitrogenos_listos < 1) {
            pthread_cond_wait(esperando_reaccion, lock_global);
        }
        printf("id%d Oxigeno\n\n", real_chem_thread->ID);
        num_prints++;
        if(num_prints == 6) {
            num_prints = 0;
            hidrogenos_listos = 0;
            nitrogenos_listos = 0;
            oxigenos_listos = 0;
            pthread_cond_broadcast(hay_suficientes);
            printf("### Reaccion exitosa! Convertimos un acido en ... agua?\n\n");
        }   
    } 
    
    pthread_mutex_unlock(lock_global);
}

void *Hi(void *chem_thread)
{
    pthread_mutex_lock(lock_global);
    struct chemical_thread *real_chem_thread = chem_thread;
    while(hidrogenos_listos == 3) {
        pthread_cond_wait(hay_suficientes, lock_global);
    }
    printf("Se crea id%d Hidrogeno\n\n",real_chem_thread->ID);
    hidrogenos_listos++;
    if(hidrogenos_listos == 3 && oxigenos_listos == 2 && nitrogenos_listos == 1) {
        printf(">>>>>> una molecula de NH3\n\n");
        printf("FUEGO!!!!!!\n\n");
        printf("id%d Hidrogeno\n\n", real_chem_thread->ID);
        num_prints++;
        pthread_cond_broadcast(esperando_reaccion);
    } else {
        while(hidrogenos_listos < 3 || nitrogenos_listos < 1 || oxigenos_listos < 2) {
            pthread_cond_wait(esperando_reaccion, lock_global);
        }
        printf("id%d Hidrogeno\n\n", real_chem_thread->ID);
        num_prints++;
        if(num_prints == 6) {
            num_prints = 0;
            hidrogenos_listos = 0;
            nitrogenos_listos = 0;
            oxigenos_listos = 0;
            pthread_cond_broadcast(hay_suficientes);
            printf("### Reaccion exitosa! Convertimos un acido en ... agua?\n\n");
        }
    }
    pthread_mutex_unlock(lock_global);
}

void main()
{
    printf("\nNH3 + O2 -> Agua?\n\n");

    printf("¿Cuantas reacciones se crearan?: ");
    scanf("%d", &numero_reacciones);

    printf("\n\n");

    cantidad_hidrogenos = numero_reacciones * 3;
    cantidad_nitrogenos = numero_reacciones;
    cantidad_oxigenos = numero_reacciones * 2;

    printf("Se crearan %d Nitrogeno, %d Hidrogeno y %d Oxigeno\n\n", cantidad_nitrogenos, cantidad_hidrogenos, cantidad_oxigenos);

    int total = cantidad_oxigenos + cantidad_hidrogenos + cantidad_nitrogenos;
    int hidrogenos_creados = 0;
    int oxigenos_creados = 0;
    int nitrogenos_creados = 0;
    int threads_creados = 0;
    
    int numero_aleatorio;
    srand((unsigned) time(&t));

    pthread_t threads[total];

    lock_global = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    hay_suficientes = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    esperando_reaccion = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    faltan_hidrogenos = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    faltan_nitrogenos = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    faltan_oxigenos = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));

    pthread_mutex_init(lock_global, NULL);
    pthread_cond_init(hay_suficientes, NULL);
    pthread_cond_init(esperando_reaccion, NULL);
    pthread_cond_init(faltan_hidrogenos, NULL);
    pthread_cond_init(faltan_nitrogenos, NULL);
    pthread_cond_init(faltan_oxigenos, NULL);

    while(threads_creados < total) 
    {
        numero_aleatorio = rand() % 3;

        switch(numero_aleatorio) {
            case 0:
                if(hidrogenos_creados < cantidad_hidrogenos) {
                    struct chemical_thread *nuevo_thread = (struct chemical_thread *)malloc(sizeof(struct chemical_thread));
                    nuevo_thread->ID = threads_creados;
                    nuevo_thread->type = numero_aleatorio;
                    pthread_create(&threads[threads_creados], NULL, Hi, nuevo_thread);
                    hidrogenos_creados++;
                    threads_creados++;
                    sleep(2);
                }
                break;
            case 1:
                if(nitrogenos_creados < cantidad_nitrogenos) {
                    struct chemical_thread *nuevo_thread = (struct chemical_thread *)malloc(sizeof(struct chemical_thread));
                    nuevo_thread->ID = threads_creados;
                    nuevo_thread->type = numero_aleatorio;
                    pthread_create(&threads[threads_creados], NULL, Ni, nuevo_thread);
                    nitrogenos_creados++;
                    threads_creados++;
                    sleep(2);
                }
                break;
            case 2:
                if(oxigenos_creados < cantidad_oxigenos) {
                    struct chemical_thread *nuevo_thread = (struct chemical_thread *)malloc(sizeof(struct chemical_thread));
                    nuevo_thread->ID = threads_creados;
                    nuevo_thread->type = numero_aleatorio;
                    pthread_create(&threads[threads_creados], NULL, Ox, nuevo_thread);
                    oxigenos_creados++;
                    threads_creados++;
                    sleep(2);
                }
                break;
        }       
    }

    for(int j=0;j < total;j++) 
    {
        pthread_join(threads[j], NULL);
    }

    printf("\nFin\n\n");
}