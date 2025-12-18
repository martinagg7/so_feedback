#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

// EJERCICIO 2

// APARTADO 1 - Definiciones básicas

// 1.1 - Size de los arrays que vamos a usar

#define SIZE 10


// 1.2 - Estructura potencia_t
/*
    - base: Valor base de la potencia.
    - exp:  Exponente de la potencia.
    - potencia: base elevado a exp.
*/
typedef struct {
    int base;
    int exp;
    int potencia;
} potencia_t;


// 1.3 - Puntero que apunta a la dirección de memoria de la estructura potencia_t

typedef potencia_t* potenciaP_t;


// 1.4 - setBaseExp() inicaliza los valores base y exp.
/*
    1- Funcion recibe como argumentos: 1. puntero a estructura potencia_t, 2. valor base, 3. valor exponente
    2- A través del puntero, incializa la base y el exponente de la potencia.   
*/

void setBaseExp(potencia_t *p, int base, int exp) {
    p->base = base;
    p->exp = exp;
    p->potencia = -1;
}


// 1.5 - getPotencia()
/*
    - exp < 0 :  No pueden elevarse a num negativos, devuelve -1
    - exp == 0 : Cualquier número elevado a 0 es 1
    - exp > 0 :  Calcula potencia (con un bucle)

    Devuelve:
    - Resultado de la potencia o -1 en caso de error
*/
int getPotencia(int base, int exp) {

    // 1. Exponente negativo
    if (exp < 0) {
        return -1;
    }

    // 2. Exponente cero
    if (exp == 0) {
        return 1;
    }
    // 3. Caso Normal
    int resultado = 1; // guardar el resultado de las potencias en el bucle

    for (int i = 1; i <= exp; i++) {  // recorremos el bucles actualizando el valor de i hasta i
        resultado = resultado * base; 
    }

    return resultado;
}


// 1.6 - setPotenciaEst()

void setPotenciaEst(potencia_t *p) {
    p->potencia = getPotencia(p->base, p->exp); // actualizar el valor de potencia en la estructura
}

// APARTADO 1 - Definiciones básicas

// 2.1 - initArrayEst() incializa los valores del array
/*
    - base : inicaliza los valores 1
    - exp : inicaliza los valores 0
    - potencia : inicaliza los valores 1
*/

void initArrayEst(potencia_t arr[])
{
    for (int i = 0; i < SIZE; i++)
    {
        arr[i].base = i + 1;
        arr[i].exp = 0;
        arr[i].potencia = 1;
    }
}


// 2.2 - printArrayEst() Imprime los valores base,exp y potencia de cada elemento del array

void printArrayEst(potencia_t arr[])
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("arr[%d]: base: %d exp: %d potencia %d\n",
               i,
               arr[i].base,
               arr[i].exp,
               arr[i].potencia);
    }
}

// 2.3 - calcuPotHeb() mediante las hebras calcula la potencia de cada elemento del array

void *calcuPotHeb(void *arg)
{
    // convertir un puntero cualquiera a un puntero de tipo potencia_t
    potencia_t *p = (potencia_t *) arg; 

    // calcular la potencia y actualizar el valor en la estructura
    p->potencia = getPotencia(p->base, p->exp);

    return NULL;
}


// APARTADO 3 - Funcion Main

int main()
{
    // 3.1 Definir arr1
    potencia_t arr1[SIZE];

    // 3.2 Inicializar el arr1 (base=1,exp=0,potencia=1)
    initArrayEst(arr1);

    // 3.3 Imprimir arr1
    printf("Arr1 inicializado:\n");
    printArrayEst(arr1);
    printf("\n");

    // 3.4 Modificar base y exp de cada elemento del arr1
    /*
        base = arr[i]
        exp = 2
    */
    for (int i = 0; i < SIZE; i++)
    {
        setBaseExp(&arr1[i], i, 2);
    }

    // 3.5 Mostrar el contenido de arr1 tras actualizar base y exp
    printf("Arr1 tras actualizar base y exp:\n");
    printArrayEst(arr1);
    printf("\n");

    // 3.6 Crear hijo de Size con fork() y hacer que hijo calcule la potencia de una de las posiciones del arr1

    for (int i = 0; i < SIZE; i++)
    {
        // Hacemos un fork de cada elemento del array
        /*
            - Proceso padre (pid > 0) : espera hijo calcule potencia y recoge resultado
            - Proceso hijo  (pid = 0): calcula potencia y devuelve resultado con exit()
        */
        pid_t pid = fork(); 

        if (pid == 0)
        {
            // Proceso hijo
            int resultado = getPotencia(arr1[i].base, arr1[i].exp);
            exit(resultado); // devuelve el resultado al proceso padre
        }
        else
        {
            // Proceso padre espera 
            int status;
            // obliga al padre a esperar a que el hijo termine 
            waitpid(pid, &status, 0); 

            // Si status (1) indica que el hijo terminó correctamente
            if (WIFEXITED(status))
            {
                arr1[i].potencia = WEXITSTATUS(status); // actualiza el valor de potencia en el arr1
            }
        }
    }

    // 3.7 Mostrar el contenido de arr1 tras calcular las potencias 
    printf("Arr1 tras calcular las potencias:\n");
    printArrayEst(arr1);
    printf("\n");

    // 3.8 Definir arr2
    potencia_t arr2[SIZE];

    // 3.9 Inicializar arr2 (base=1,exp=0,potencia=1)
    printf("Arr2 inicializado:\n");
    initArrayEst(arr2);

    // 3.10 Imprimir arr2
    printf("Arr2 inicializado:\n");
    printArrayEst(arr2);
    printf("\n");

    // 3.11 Modificar base y exp de cada elemento del arr2
    for (int i = 0; i < SIZE; i++)
    {
        setBaseExp(&arr2[i], i, 2);
    }
    // 3.12 Mostrar el contenido de arr2 tras actualizar base y exp
    printf("Arr2 tras actualizar base y exp:\n");
    printArrayEst(arr2);
    printf("\n");

    // 3.13 Crear hebras para calcular la potencia de cada elemento del arr2

    pthread_t thread[SIZE];  // identificadores de las hebras

    for (int i = 0; i < SIZE; i++)
    {
        pthread_create(&thread[i], NULL, calcuPotHeb, &arr2[i]); // para cada elemento arr2, crear una hebra que calcule la potencia
    }

    for (int i = 0; i < SIZE; i++)
    {
        pthread_join(thread[i], NULL); // se bloquea el hilo principal hasta que las hebras terminen
    }

    // 3.14 Mostrar el contenido de arr2 tras calcular las potencias
    printf("Arr2 tras calcular las potencias:\n");
    printArrayEst(arr2);
    printf("\n");

    return 0;
}    


