/**
 * @file main.cpp
 * @brief Implementación del Juego de la Vida de Conway utilizando matrices bidimensionales.
 * @details Utiliza la API de Windows y secuencias ANSI para una animación fluida en consola,
 * eliminando el parpadeo durante la transición de generaciones.
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> 

using namespace std;


void limpiar_pantalla();
void gotoxy(int x, int y);
void esperar (int ms);
void inicializar_tablero(vector<vector<int>>& tablero, int tamano);
void imprimir_tablero(const vector<vector<int>>& tablero, int tamano);
int calcular_vecinos_vivos(const vector<vector<int>>& tablero, int fila, int columna, int tamano);
void actualizar_tablero(vector<vector<int>>& tablero, int tamano);

/**
 * @brief Función principal que gestiona la entrada del usuario y el bucle de la simulación.
 * @return Retorna 0 al finalizar la ejecución correctamente.
 */
int main() {
    int tamanoTablero; 
    int numGeneraciones; 

    cout << "Ingrese tamano del tablero: ";
    cin >> tamanoTablero;
    cout << "Ingrese numero de generaciones: ";
    cin >> numGeneraciones;

    vector<vector<int>> tableroJuego(tamanoTablero, vector<int>(tamanoTablero, 0));
    
    inicializar_tablero(tableroJuego, tamanoTablero);

   
    limpiar_pantalla(); 

    for (int i = 0; i < numGeneraciones; ++i) {
        
        gotoxy(0, 0); 
        
        cout << "Generacion: " << i + 1 << " / " << numGeneraciones << "\n";
        imprimir_tablero(tableroJuego, tamanoTablero);
        actualizar_tablero(tableroJuego, tamanoTablero);
        
        // Pausa manual de 200 milisegundos
        esperar(200);
    }

    cout << "\nSimulacion terminada.\n";
    return 0;
}

/**
 * @brief Limpia la consola utilizando secuencias de escape ANSI.
 * @details \033[2J borra toda la pantalla y \033[H mueve el cursor a la posición inicial (0,0).
 */
void limpiar_pantalla() { 
    cout << "\033[2J\033[H"; 
}

/**
 * @brief Mueve el cursor de la consola a unas coordenadas específicas (X, Y).
 * @param x Coordenada X (Columna).
 * @param y Coordenada Y (Fila).
 */

void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

/**
 * @brief Pausa la ejecución del programa durante un tiempo específico.
 * @details Utiliza la función clock() de la librería ctime para generar la pausa.
 * @param ms Cantidad de milisegundos que durará la espera.
 */
void esperar(int ms) { 
    for(clock_t t = clock(); (clock() - t) * 1000 / CLOCKS_PER_SEC < ms;); 
}

/**
 * @brief Llena el tablero con un estado inicial aleatorio.
 * @details Asigna aleatoriamente un estado de viva (1) o muerta (0) a cada celda.
 * @param tablero Referencia a la matriz bidimensional del juego.
 * @param tamano Dimensiones del tablero (NxN).
 */
void inicializar_tablero(vector<vector<int>>& tablero, int tamano) {
    srand(time(0));
    for (int i = 0; i < tamano; ++i) {
        for (int j = 0; j < tamano; ++j) {
            tablero[i][j] = rand() % 2;
        }
    }
}

/**
 * @brief Imprime el tablero en la consola de manera visual.
 * @details Muestra un arroba ('@') para las células vivas y espacios en blanco (' ') para las muertas.
 * @param tablero Matriz constante que representa el estado actual de las células.
 * @param tamano Dimensiones del tablero (NxN).
 */
void imprimir_tablero(const vector<vector<int>>& tablero, int tamano) {
    for (int i = 0; i < tamano; ++i) {
        for (int j = 0; j < tamano; ++j) {
            if (tablero[i][j] == 1) {
                cout << (char)254 << " ";;
            } else {
                cout << "  ";
            }
        }
        cout << "\n";
    }
    
    for(int k = 0; k < tamano; ++k) cout << "--"; 
    cout << "\n";
}

/**
 * @brief Calcula el número de células vivas adyacentes a una coordenada específica.
 * @param tablero Matriz constante del estado actual.
 * @param fila Fila actual de la célula a evaluar.
 * @param columna Columna actual de la célula a evaluar.
 * @param tamano Dimensiones del tablero.
 * @return Entero con la cantidad de vecinos vivos (0-8).
 */
int calcular_vecinos_vivos(const vector<vector<int>>& tablero, int fila, int columna, int tamano) {
    int vecinosVivos = 0; 
    
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            
            if (i == 0 && j == 0) continue; 
            
            int nuevaFila = fila + i;
            int nuevaColumna = columna + j;
            
            if (nuevaFila >= 0 && nuevaFila < tamano && nuevaColumna >= 0 && nuevaColumna < tamano) {
                vecinosVivos += tablero[nuevaFila][nuevaColumna];
            }
        }
    }
    return vecinosVivos;
}

/**
 * @brief Aplica las reglas de Conway para generar la siguiente generación.
 * @details 
 * 1. Célula viva con <2 o >3 vecinos muere.
 * 2. Célula muerta con exactamente 3 vecinos revive.
 * @param tablero Referencia a la matriz bidimensional a actualizar.
 * @param tamano Dimensiones del tablero.
 */
void actualizar_tablero(vector<vector<int>>& tablero, int tamano) {
    vector<vector<int>> tableroTemporal = tablero; 

    for (int i = 0; i < tamano; ++i) {
        for (int j = 0; j < tamano; ++j) {
            int vecinos = calcular_vecinos_vivos(tablero, i, j, tamano);
            
            if (tablero[i][j] == 1) { 
                if (vecinos < 2 || vecinos > 3) {
                    tableroTemporal[i][j] = 0; 
                }
            } 
            else { 
                if (vecinos == 3) {
                    tableroTemporal[i][j] = 1; 
                }
            }
        }
    }
    
    tablero = tableroTemporal;
}