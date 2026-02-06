// EJERCICIO 1
#include <stdio.h>

// APARTADO 1 - Declarar estructura de arrayLength_t
/*
    - arrInt[10]: Array de 10 enteros positivos
    - arrSize:    Elementos almacenados en array
    - arrAdd:     Suma de los elementos del array
*/

typedef struct {
    int arrInt[10];
    int arrSize;
    int arrAdd;
} arrayLength_t;


// APARTADO 2- Funciones para manejar estructura arrayLength_t
// 2.1-initArray()
/*
    - arrInt[10]: Incializamos todas las posiciones del array a -1
    - arrSize:    Nos indica los elementos =! -1. Se inicializa a 0.
    - arrAdd:     Nos da la suma de estos últimos elementos. Se inicializa a 0.  
*/

int initArray(arrayLength_t *p) {

    // Error:-1
    if (p == NULL) {
        return -1;
    }
    // Bucle incializar todas las posiciones del array a -1
    for (int i = 0; i < 10; i++) {
        p->arrInt[i] = -1;
    }
    // arrSize=arrAdd=0
    p->arrSize = 0;
    p->arrAdd = 0;

    return 0;
}


// Apartado 2.2 - printArr()
/*
    Imprime la estructura con el formato:
    {[v0,..., v9], arrSize, arrAdd}
    -vi: Elementos del array
    -arrSize y arrAdd: Valores de la estructura
*/

void printArr(const arrayLength_t *p) {

    printf("{[");
    for (int i = 0; i < 10; i++) {
        printf("%d", p->arrInt[i]); //Elementos del array
        if (i < 9) {
            printf(", ");
        }
    }
    printf("], %d, %d}\n", p->arrSize, p->arrAdd); //arrSize y arrAdd
}

// Apartado 2.3 - addElement()
/*
    - Inserta un nuevo valor en la primera posición libre,
      que corresponde al índice arrSize.
    - Actualiza arrSize y arrAdd.

    Errores:
        - array lleno (arrSize >= 10)
        - valor negativo
*/

int addElement(arrayLength_t *p, int value) {
    // Array lleno
    if (p->arrSize >= 10) {
        return -1;
    }

    // Valor negativo
    if (value < 0) {
        return -1;
    }

    // Añadir valor en la primera posición libre
    p->arrInt[p->arrSize] = value;

    // Actualizar arrSize y arrAdd
    p->arrSize++;
    p->arrAdd += value;

    return 0; 
}

// APARTADO 3 - Funciones de consulta

// Apartado 3.1 - getArrSize
/*
    Devuelve el número de elementos en el array
*/

int getArrSize(const arrayLength_t *p) {

    if (p == NULL) {
        return -1;
    }

    return p->arrSize;
}

// Apartado 3.2 - getArrAdd
/*
    Devuelve la suma de los elementos en el array

*/

int getArrAdd(const arrayLength_t *p) {

    if (p == NULL) {
        return -1;
    }

    return p->arrAdd;
}

// Apartado 3.3 - getElement
/*
    Devuelve el elemento almacenado en la posición indicada.

    Condiciones:
        - posicion debe estar entre 0 y 9  
        - posicion < arrSize              
        - el valor almacenado debe ser positivo

*/

int getElement(const arrayLength_t *p, int pos) {

    if (p == NULL) {
        return -1;
    }
    if (pos< 0 || pos > 9) {
        return -1;
    }

    // position < arrSize
    if (pos >= p->arrSize) {
        return -1;
    }

    // valor almacenado debe ser positivo
    if (p->arrInt[pos] < 0) {
        return -1;
    }

    return p->arrInt[pos];
}

// APARTADO 4 - setElement
/*
    Sustituye un valor almacenado != -1 por un nuevo valor positivo.
    Condiciones:
        - posicion entre 0 y 9
        - position < arrSize  (debe estar ocupada por valor positivo)
        - el valor a insertar debe ser positivo
*/

int setElement(arrayLength_t *p, int position, int value) {

    // 1. Comprobar límites
    if (position < 0 || position >= 10) {
        return -1;
    }

    // 2. Posición debe estar ocupada
    if (position >= p->arrSize) {
        return -1;
    }

    // 3.Nuevo valor positivo
    if (value < 0) {
        return -1;
    }

    // 4. Actualizar arrAdd
    p->arrAdd = (p->arrAdd - p->arrInt[position]) + value;

    // 5. Sustituir valor
    p->arrInt[position] = value;

    return 0;
}

// APARTADO 5 - resetArr
/*
    Reinicia completamente la estructura dejándola en su estado inicial.
    - Todas las posiciones del array se ponen a -1
    - arrSize = 0
    - arrAdd  = 0
*/
int resetArr(arrayLength_t *p) {
    return initArray(p);
}


// PRUEBA MAIN DE LA ESTRUCTURA Y FUNCIONES
int main() {

    arrayLength_t al1, al2;

    initArray(&al1);
    initArray(&al2);

    //Llenar al1 con múltiplos de 10
    for (int i = 0; i < 10; i++) {
        addElement(&al1, i * 10);
    }
    printArr(&al1);

    // Actualizar posiciones impares c
    int nuevo = 1;
    for (int pos = 1; pos < al1.arrSize; pos += 2) {
        setElement(&al1, pos, nuevo);
        nuevo += 2;
    }
    printArr(&al1);

    // Añadir elementos de posiciones pares de al1 a al2
    for (int pos = 0; pos < al1.arrSize; pos += 2) {
        int valor = getElement(&al1, pos);
        addElement(&al2, valor);
    }

    // Al final añadir valores 0 a 4 en al2
    for (int v = 0; v < 5; v++) {
        addElement(&al2, v);
    }
    printArr(&al2);

    return 0;
}