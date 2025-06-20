# Space Invaders

Bienvenido a Space Invaders, una versión moderna y desafiante del clásico videojuego arcade. Este proyecto ha sido desarrollado en C++ utilizando la biblioteca SFML, incorporando gráficos personalizados, animaciones fluidas, efectos de sonido y una pantalla de Game Over distintiva.

---

## ¿Qué es este proyecto?

Space Invaders es un remake que busca ofrecer una experiencia renovada del clásico, integrando arte original, jugabilidad pulida y una dificultad progresiva que representa un reto constante para el jugador.

---

# Descripción general

Space Invaders es una recreación del clásico videojuego arcade, desarrollada como proyecto educativo. El objetivo principal es defender la Tierra de oleadas de invasores espaciales controlando una nave que puede moverse horizontalmente y disparar hacia arriba.

---

## Características principales

- **Gráficos en alta definición:** Sprites y fondos diseñados especialmente para este proyecto.
- **Animaciones fluidas:** Enemigos y disparos cuentan con animaciones detalladas.
- **Dificultad progresiva:** El nivel de desafío aumenta conforme avanza la partida.
- **Pantalla de Game Over personalizada:** Mensaje centralizado y tipografía exclusiva.

---

## Tecnologías utilizadas

- Lenguaje de programación: C++
- Librerías/gráficos: Tabletop, PACFight

---

## Autores

- **Braulio Jasib Sánchez Vázquez** — 24110280
- **Ander Azariel Jimenez Palafox** — 24110210

---

## Cómo jugar

1. Ejecuta el archivo `Game.cpp` utilizando el siguiente comando en la terminal:
   ```
   make runGame
   ```
   ![MRG](assets/images/README%20PICTURES/Readme%20(1).png)
2. Al ejecutar el comando, se desplegará un menú en la terminal donde podrás elegir un nivel para jugar.
   ![MRG](assets/images/README%20PICTURES/Readme%20(2).png)
3. El nivel "Space Invaders" corresponde al nivel diseñado en `Tabletop.cpp`.
   ![MRG](assets/images/README%20PICTURES/Readme%20(3).png)
4. El nivel "PacFight" corresponde al archivo `PacFight.cpp`.
   ![MRG](assets/images/README%20PICTURES/Readme%20(4).png)

Asegúrese de tener instalado MinGW y SFML en su sistema.

---

## Estructura del proyecto

```
Space-Invaders/
├── assets/
│   ├── images/        # Sprites y fondos
│   ├── Audios/        # Efectos de sonido
│   └── fonts/         # Tipografías exclusivas
├── bin/               # Ejecutables
├── include/           # Archivos de cabecera
├── src/               # Código fuente
├── makefile           # Archivo de compilación
└── README.md          # Este documento
```

---

## Créditos especiales

- Tipografía: AngelicWar.ttf
- Sonidos: Efectos 8-bit libres de derechos
- Inspiración: Space Invaders original

---

# Librerías utilizadas en PacFight.cpp y Tabletop.cpp

A continuación se listan las librerías y cabeceras incluidas en los archivos `src/PacFight.cpp` y `src/Tabletop.cpp`:

### Librerías presentes en ambos archivos:
- `<SFML/Graphics.hpp>`
- `<vector>`
- `<algorithm>`
- `<array>`
- `<functional>`
- `"../include/PlayerHealth.hpp"`

### Librerías adicionales en Tabletop.cpp:
- `"../include/EnemyMovementManager.hpp"`
- `"../include/EnemyHealthManager.hpp"`
- `"../include/Enemy.hpp"`
- `"../include/Barrier.hpp"`
- `"../include/EnemyShooter.hpp"`
- `"../include/Jefe.hpp"`
- `<iostream>`
