#include <iostream>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    int opcion;
    do {
        std::cout << "Menú principal\n";
        std::cout << "1. Space-Invaders\n";
        std::cout << "2. Pacfight\n";
        std::cout << "0. Salir\n";
        std::cout << "Selecciona una opción (0, 1 o 2): ";
        std::cin >> opcion;

        if (opcion == 1) {
#ifdef _WIN32
            system(".\\bin\\Tabletop.exe");
#else
            system("./bin/Tabletop");
#endif
        } else if (opcion == 2) {
#ifdef _WIN32
            system(".\\bin\\PacFight.exe");
#else
            system("./bin/PacFight");
#endif
        } else if (opcion != 0) {
            std::cout << "Opción no válida." << std::endl;
        }
    } while (opcion != 0);
    return 0;
}
