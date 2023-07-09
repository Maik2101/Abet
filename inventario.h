#ifndef INVENTARIO_H
#define INVENTARIO_H

#define LIMPIAR_PANTALLA "clear"
#define ARCHIVO_INVENTARIO "inventario.txt"
#define NIVEL_MINIMO 5
#define DIAS_PARA_CADUCAR 30

void pausar();
void limpiarPantalla();

int esFechaValida(char *fecha);
void agregarMedicina(char *nombre, int cantidad, char *fecha_vencimiento);
void actualizarInventario(int indice, int cantidad);
void registrarDespacho(char *nombre, int cantidad);
void generarAlertas();
int diasEntreFechas(int dia1, int mes1, int anio1, int dia2, int mes2, int anio2);
void generarAlertasCaducidad();
void mostrarInventario();
int contarProductos();

#endif