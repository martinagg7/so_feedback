#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


/*  APARTADO 1
- hebval_t: Estructura con id y val (.
 - N y NHP: Numero de elementos en el array y número total de hebras productoras.
 - array: Array de hebval_t para almacenar los datos producidos.
 */
#define N 10   
#define NHP 35 

typedef struct {
    int id;
    int val;
} hebval_t;

hebval_t array[N];
int posicion = 0;  // Puntero para insertar en el array
int contador = 0;  // Numero total de hebras terminado

sem_t sem_prod, sem_cons;
pthread_mutex_t mutex;

/* APARTADO 2
- Recibe un puntero a hebval_t y lo copia en el array.
- Usa puntero posicion para insertar.
- Si array llena posicio=n
    1) Resetea posicion a 0
    2) Despierta a la hebra consumidora para que imprima
    3) Bloquea a la productora
    

*/
void* hebraProductora(void* arg) {
    hebval_t* dato = (hebval_t*) arg;

    sem_wait(&sem_prod);        // Si wait>0 no bloquea, si wait=0 bloquea a la hebra productora
    pthread_mutex_lock(&mutex); // Evitar condiciones de carrera: Dos productoras no peudan escribir al mismo tiempo

    // Apartado 2.1
    array[posicion] = *dato;
    posicion++;
    contador++;

    // Apartado 2.2: Si el array está lleno
    if (posicion == N) {
        posicion = 0;               
        sem_post(&sem_cons);        // Despierta consumidora para imprimir
    } else {
        sem_post(&sem_prod);        // Deja pasar al siguiente productor
    }

    pthread_mutex_unlock(&mutex); // Libera acceso
    return NULL;
}

/* APARTADO 3
- Recibe el aviso de la productora para imprimir contendo
- Imprime el contenido del array
- Si aun faltan hebra contador< numero hebras productoras, siguen produdiendo las productoras.
*/
void* hebraConsumidora(void* arg) {
    while (contador < NHP) {
        sem_wait(&sem_cons); // Espera a que la productura llene el array

        // Imprimir id,valor
        printf("Contenido del array: [ ");
        for (int i = 0; i < N; i++) {
            printf("[id %d, val %d] ", array[i].id, array[i].val);
        }
        printf("]\n\n");

        // Volver avisar para continuar produciendo
        if (contador < NHP) {
            sem_post(&sem_prod);
        }
    }
    return NULL;
}

/* APARTADO 4
- Creamos NHP hebras productoras y una hebra consumidora.
- Inicializamos 
    1) Sem_prod con contador=1 para que sea el primero en empezar a producir
    2) Sem_cons con contador=0
    3) Mutex con valor inicial 1.

*/
int main() {
    // Apartado 4.1
    pthread_t h_productores[NHP], h_consumidora;
    hebval_t datos[NHP];

    // Inicialización de semáforos y mutex
    sem_init(&sem_prod, 0, 1); 
    sem_init(&sem_cons, 0, 0); 
    pthread_mutex_init(&mutex, NULL);

    //A partado 4.2: Inicializar el array con -1
    for (int i = 0; i < N; i++) {
        array[i].id = -1;
        array[i].val = -1;
    }

    // Apartado 4.3 : lanzar hebras productoras
    for (int i = 0; i < NHP; i++) {
        datos[i].id = i;
        datos[i].val = i;
        pthread_create(&h_productores[i], NULL, hebraProductora, &datos[i]);
    }

    // Apartado 4.4: lanzar hebra consumidora
    pthread_create(&h_consumidora, NULL, hebraConsumidora, NULL);

    // Apartado 4.5: evitar hebras huerfanas.
    for (int i = 0; i < NHP; i++) {
        pthread_join(h_productores[i], NULL);
    }
    pthread_join(h_consumidora, NULL);


    sem_destroy(&sem_prod);
    sem_destroy(&sem_cons);
    pthread_mutex_destroy(&mutex);

    return 0;
}