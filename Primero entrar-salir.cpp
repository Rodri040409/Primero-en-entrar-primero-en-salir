#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Proceso {
    char nombre;
    int llegada;
    int cpu;
    int instante;
    int t_fin;
    int t_e;
    int t_r;
    double penalizacion;
};

// Funcion de comparacion para qsort
int compararProcesos(const void *a, const void *b) {
    struct Proceso *procesoA = (struct Proceso *)a;
    struct Proceso *procesoB = (struct Proceso *)b;

    // Primero ordenar por tiempo de llegada
    if (procesoA->llegada < procesoB->llegada) {
        return -1;
    } else if (procesoA->llegada > procesoB->llegada) {
        return 1;
    } else {
        // En caso de empate en tiempo de llegada, ordenar alfabeticamente por nombre
        return procesoA->nombre - procesoB->nombre;
    }
}

int main() {
    printf("+------------------------------------------------------+\n");
    printf("|     Primero en entrar, primero en salir              |\n");
    printf("+------------------------------------------------------+\n");

    int num_procesos;
    printf("Ingrese el numero de procesos: ");
    scanf("%d", &num_procesos);

    struct Proceso procesos[num_procesos];

    // Ingresar informacion de los procesos
    for (int i = 0; i < num_procesos; i++) {
        printf("Ingrese la hora de llegada del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].llegada);
        printf("Ingrese la duracion del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].cpu);
        procesos[i].nombre = 'A' + i;  // Asignar nombres A, B, C, ...
    }

    // Ordenar procesos usando qsort con la funcion de comparacion personalizada
    qsort(procesos, num_procesos, sizeof(struct Proceso), compararProcesos);

    // Calcular los valores de la tabla y la penalizacion
    int tiempo_actual = 0;
    double penalizacion_total = 0;

    // Crear un array de cadenas de caracteres para almacenar las filas de valores
    char filas_por_nombre[num_procesos][100];  // Suponemos un máximo de 100 caracteres por fila

    for (int i = 0; i < num_procesos; i++) {
        struct Proceso *p = &procesos[i];
        p->instante = tiempo_actual;
        p->t_fin = p->instante + p->cpu;
        p->t_e = p->instante - p->llegada;
        p->t_r = p->t_e + p->cpu;

        // Evitar division por 0
        if (p->t_e == 0) {
            p->penalizacion = 0;
        } else {
            p->penalizacion = (double)p->t_r / p->t_e;
        }

        // Reemplazar "INF" con 0
        if (isnan(p->penalizacion)) {
            p->penalizacion = 0;
        }

        penalizacion_total += p->penalizacion;
        tiempo_actual = p->t_fin;

        // Crear la fila de valores para este proceso
        snprintf(filas_por_nombre[i], sizeof(filas_por_nombre[i]), "|    %c    |    %2d   |  %2d |    %2d    |  %3d  |  %2d  |  %3d  |     %.8f   |",
                 p->nombre, p->llegada, p->cpu, p->instante, p->t_fin, p->t_e, p->t_r, p->penalizacion);
    }

    // Ordenar las filas de valores por nombre
    for (int i = 0; i < num_procesos; i++) {
        for (int j = i + 1; j < num_procesos; j++) {
            if (strcmp(filas_por_nombre[i], filas_por_nombre[j]) > 0) {
                char temp[100];
                strcpy(temp, filas_por_nombre[i]);
                strcpy(filas_por_nombre[i], filas_por_nombre[j]);
                strcpy(filas_por_nombre[j], temp);
            }
        }
    }

    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");
    printf("| proceso | llegada | cpu | instante | t.fin | t.e  |  t.r  | penalizacion |\n");
    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");

    // Imprimir las filas de valores en orden alfabetico
    for (int i = 0; i < num_procesos; i++) {
        printf("%s\n", filas_por_nombre[i]);
    }
    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");

    // Calcular y mostrar el promedio de penalizacion
    double promedio_penalizacion = penalizacion_total / num_procesos;

    // Reemplazar "INF" en el promedio con 0
    if (isnan(promedio_penalizacion)) {
        promedio_penalizacion = 0;
    }

    printf("Promedio de penalizacion: %.8f\n", promedio_penalizacion);

    return 0;
}


//IG:rodrigoruiz_11
