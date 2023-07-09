#include <stdlib.h>
#include <stdio.h>
#include "inventario.h"

void mostrarMenu() {
    printf("Menú:\n");
    printf("1. Agregar medicina\n");
    printf("2. Actualizar inventario\n");
    printf("3. Registrar despacho\n");
    printf("4. Mostrar medicamentos próximos a terminar\n");
    printf("5. Mostrar medicamentos próximos a caducar\n");
    printf("6. Mostrar inventario\n");
    printf("7. Salir\n");
    printf("Ingrese una opción: ");
}

int main() {
    int opcion;
    char nombre[50];
    int cantidad, indice;
    char fecha_vencimiento[11];

    do {
        limpiarPantalla();
        mostrarMenu();
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                printf("Ingrese el nombre de la medicina: ");
                scanf("%s", nombre);
                do {
                    printf("Ingrese la cantidad: ");
                    scanf("%d", &cantidad);
                    if(cantidad <= 0) {
                        printf("Ingrese un número válido.\n");
                    }
                } while (cantidad <= 0);
                do {
                    printf("Ingrese la fecha de vencimiento (dd/mm/yyyy): ");
                    scanf("%s", fecha_vencimiento);
                    if (!esFechaValida(fecha_vencimiento)) {
                        printf("Fecha de vencimiento inválida o caduca\n");
                    }
                } while (!esFechaValida(fecha_vencimiento));
                agregarMedicina(nombre, cantidad, fecha_vencimiento);
                pausar();
                break;
            case 2:
                mostrarInventario();
                printf("Ingrese el índice de la medicina: ");
                do {
                    printf("Ingrese el índice: ");
                    scanf("%d", &indice);
                    if(indice <= 0 || indice > contarProductos()) {
                        printf("Ingrese un número válido.\n");
                    }
                } while (indice <= 0 || indice > contarProductos());
                do {
                    printf("Ingrese la cantidad de nuevos productos: ");
                    scanf("%d", &cantidad);
                    if(cantidad <= 0) {
                        printf("Ingrese un número válido.\n");
                    }
                } while (cantidad <= 0);
                actualizarInventario(indice, cantidad);
                pausar();
                break;
            case 3:
                mostrarInventario();
                printf("Ingrese el nombre de la medicina: ");
                scanf("%s", nombre);
                do {
                    printf("Ingrese la cantidad: ");
                    scanf("%d", &cantidad);
                    if(cantidad <= 0) {
                        printf("Ingrese un número válido.\n");
                    }
                } while (cantidad <= 0);
                registrarDespacho(nombre, cantidad);
                pausar();
                break;
            case 4:
                generarAlertas();
                pausar();
                break;
            case 5:
                generarAlertasCaducidad();
                pausar();
                break;
            case 6:
                mostrarInventario();
                pausar();
                break;
            case 7:
                exit(0);
            default:
                printf("Opción inválida\n");
        }
    } while (opcion != 7);
    return 0;
}
