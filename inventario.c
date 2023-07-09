#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "inventario.h"

void pausar() {
    printf("Pulsa ENTER para continuar...");
    getchar();
    getchar();
    system(LIMPIAR_PANTALLA);
}

void limpiarPantalla() {
    system(LIMPIAR_PANTALLA);
}

int esFechaValida(char *fecha) {
    int dia, mes, anio;
    if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) != 3) {
        return 0;
    }

    if (anio < 1900 || anio > 2099) {
        return 0;
    }

    if (mes < 1 || mes > 12) {
        return 0;
    }

    int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (anio % 400 == 0 || (anio % 100 != 0 && anio % 4 == 0)) {
        dias_mes[1] = 29;
    }

    if (dia < 1 || dia > dias_mes[mes - 1]) {
        return 0;
    }

    // Obtener la fecha actual del sistema
    time_t tiempo_actual = time(NULL);
    struct tm *fecha_actual = localtime(&tiempo_actual);
    int dia_actual = fecha_actual->tm_mday;
    int mes_actual = fecha_actual->tm_mon + 1;
    int anio_actual = fecha_actual->tm_year + 1900;

    if (anio < anio_actual) {
        return 0;
    } else if (anio == anio_actual) {
        if (mes < mes_actual) {
            return 0;
        } else if (mes == mes_actual) {
            if (dia <= dia_actual) {
                return 0;
            }
        }
    }

    return 1;
}
void agregarMedicina(char *nombre, int cantidad, char *fecha_vencimiento) {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    FILE *temporal = fopen("temporal.txt", "w");
    if (temporal == NULL) {
        printf("Error al crear el archivo temporal\n");
        fclose(archivo);
        return;
    }

    int encontrado = 0;
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        char nombre_medicina[50];
        int cantidad_medicina;
        char fecha_vencimiento_medicina[11];

        sscanf(linea, "%s %d %s", nombre_medicina, &cantidad_medicina, fecha_vencimiento_medicina);

        if (strcmp(nombre_medicina, nombre) == 0 && strcmp(fecha_vencimiento_medicina, fecha_vencimiento) == 0) {
            fprintf(temporal, "%s\t%d\t%s\n", nombre_medicina, cantidad_medicina + cantidad, fecha_vencimiento_medicina);
            encontrado = 1;
        } else {
            fprintf(temporal, "%s", linea);
        }
    }

    if (!encontrado) {
        fprintf(temporal, "%s\t%d\t%s\n", nombre, cantidad, fecha_vencimiento);
    }

    fclose(archivo);
    fclose(temporal);

    remove(ARCHIVO_INVENTARIO);
    rename("temporal.txt", ARCHIVO_INVENTARIO);
}

void actualizarInventario(int indice, int cantidad) {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    FILE *temporal = fopen("temporal.txt", "w");
    if (temporal == NULL) {
        printf("Error al crear el archivo temporal\n");
        fclose(archivo);
        return;
    }

    int contador = 1;
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        char nombre_medicina[50];
        int cantidad_medicina;
        char fecha_vencimiento[11];

        sscanf(linea, "%s %d %s", nombre_medicina, &cantidad_medicina, fecha_vencimiento);

        if (contador == indice) {
            fprintf(temporal, "%s\t%d\t%s\n", nombre_medicina, cantidad_medicina + cantidad, fecha_vencimiento);
        } else {
            fprintf(temporal, "%s", linea);
        }
        contador++;
    }

    fclose(archivo);
    fclose(temporal);

    remove(ARCHIVO_INVENTARIO);
    rename("temporal.txt", ARCHIVO_INVENTARIO);
}

void registrarDespacho(char *nombre, int cantidad) {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    FILE *temporal = fopen("temporal.txt", "w");
    if (temporal == NULL) {
        printf("Error al crear el archivo temporal\n");
        fclose(archivo);
        return;
    }

    int encontrado = 0;
    int cantidad_total = 0;
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        char nombre_medicina[50];
        int cantidad_medicina;
        char fecha_vencimiento[11];

        sscanf(linea, "%s %d %s", nombre_medicina, &cantidad_medicina, fecha_vencimiento);

        if (strcmp(nombre_medicina, nombre) == 0) {
            cantidad_total += cantidad_medicina;
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("No se encontró la medicina en el inventario\n");
        fclose(archivo);
        fclose(temporal);
        remove("temporal.txt");
        return;
    }

    if (cantidad_total < cantidad) {
        printf("No hay suficiente cantidad en el inventario para despachar\n");
        fclose(archivo);
        fclose(temporal);
        remove("temporal.txt");
        return;
    }

    rewind(archivo);
    while (fgets(linea, 100, archivo) != NULL) {
        char nombre_medicina[50];
        int cantidad_medicina;
        char fecha_vencimiento[11];

        sscanf(linea, "%s %d %s", nombre_medicina, &cantidad_medicina, fecha_vencimiento);

        if (strcmp(nombre_medicina, nombre) == 0 && cantidad > 0) {
            if (cantidad >= cantidad_medicina) {
                cantidad -= cantidad_medicina;
                cantidad_medicina = 0;
            } else {
                cantidad_medicina -= cantidad;
                cantidad = 0;
            }
            fprintf(temporal, "%s\t%d\t%s\n", nombre_medicina, cantidad_medicina, fecha_vencimiento);
        } else {
            fprintf(temporal, "%s", linea);
        }
    }

    fclose(archivo);
    fclose(temporal);

    remove(ARCHIVO_INVENTARIO);
    rename("temporal.txt", ARCHIVO_INVENTARIO);
}

void generarAlertas() {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    printf("Alertas:\n");
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        char nombre_medicina[50];
        int cantidad_medicina;
        char fecha_vencimiento[11];

        sscanf(linea, "%s %d %s", nombre_medicina, &cantidad_medicina, fecha_vencimiento);

        if (cantidad_medicina < NIVEL_MINIMO) {
            printf("La medicina %s tiene una cantidad por debajo del nivel mínimo\n", nombre_medicina);
        }
    }

    fclose(archivo);
}

int esBisiesto(int anio) {
    if (anio % 400 == 0) {
        return 1;
    } else if (anio % 100 == 0) {
        return 0;
    } else if (anio % 4 == 0) {
        return 1;
    } else {
        return 0;
    }
}

int diasEntreFechas(int dia1, int mes1, int anio1, int dia2, int mes2, int anio2) {
    int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dia1 > dias_mes[mes1 - 1]) {
        dia1 = dias_mes[mes1 - 1];
    }

    if (dia2 > dias_mes[mes2 - 1]) {
        dia2 = dias_mes[mes2 - 1];
    }

    long int n1 = anio1 * 365 + dia1;
    for (int i = 0; i < mes1 - 1; i++) {
        n1 += dias_mes[i];
    }
    n1 += esBisiesto(anio1) && mes1 > 2;

    long int n2 = anio2 * 365 + dia2;
    for (int i = 0; i < mes2 - 1; i++) {
        n2 += dias_mes[i];
    }
    n2 += esBisiesto(anio2) && mes2 > 2;

    return abs(n2 - n1);
}

void generarAlertasCaducidad() {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int dia_actual = tm.tm_mday;
    int mes_actual = tm.tm_mon + 1;
    int anio_actual = tm.tm_year + 1900;

    printf("Alertas de caducidad:\n");
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        char nombre_medicina[50];
        int cantidad_medicina;
        char fecha_vencimiento[11];

        sscanf(linea, "%s %d %s", nombre_medicina, &cantidad_medicina, fecha_vencimiento);

        int dia_vencimiento, mes_vencimiento, anio_vencimiento;
        sscanf(fecha_vencimiento, "%d/%d/%d", &dia_vencimiento, &mes_vencimiento, &anio_vencimiento);

        int dias_restantes = diasEntreFechas(dia_actual, mes_actual, anio_actual, dia_vencimiento, mes_vencimiento, anio_vencimiento);
        if (dias_restantes <= DIAS_PARA_CADUCAR) {
            printf("La medicina %s está a punto de caducar (%d días restantes)\n", nombre_medicina, dias_restantes);
        }
    }

    fclose(archivo);
}

void mostrarInventario() {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    printf("Inventario:\n");
    int contador = 1;
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        printf("%d. %s", contador, linea);
        contador++;
    }

    fclose(archivo);
}

int contarProductos() {
    FILE *archivo = fopen(ARCHIVO_INVENTARIO, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int contador = 0;
    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        contador++;
    }

    fclose(archivo);
    return contador;
}

