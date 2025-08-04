#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <SFML/Graphics.hpp>
using namespace std;

// Estructuras globales para mantener la info
float precioMesa, precioSilla;
vector<vector<float>> restricciones; // cada restricci�n: [coef_x, coef_y, constante]

// Funci�n objetivo
float coefX, coefY; // coeficientes de la funci�n objetivo z = coefX * x + coefY * y

// Funcion para validar que los input sean solo numeros
float validaInput(const string& mensaje, float min = -INFINITY, float max = INFINITY) {
    float valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (!cin.fail() && valor >= min && valor <= max) {
            return valor;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada no valida. Ingrese un numero";
        if (min != -INFINITY && max != INFINITY)
            cout << " entre " << min << " y " << max;
        cout << ".\n";
    }
}


// Funci�n para ingresar precios


void opcion1() {
    precioMesa = validaInput("Ingrese el precio de cada mesa: ", 0);
    precioSilla = validaInput("Ingrese el precio de cada silla: ", 0);

    cout << fixed << setprecision(2);
    cout << "Los precios fueron registrados: mesas USD " << precioMesa
        << " y sillas USD " << precioSilla << ".\n";
}


// Funci�n para ingresar restricciones

void opcion2() {
    int n = static_cast<int>(validaInput("Ingrese el numero de restricciones: ", 1));

    restricciones.clear();
    for (int i = 0; i < n; ++i) {
        cout << "Restriccion " << i + 1 << ": formato ax + by <= c\n";
        float a = validaInput("Ingrese a: ");
        float b = validaInput("Ingrese b: ");
        float c = validaInput("Ingrese c: ");
        restricciones.push_back({ a, b, c });
    }

    cout << "Restricciones registradas correctamente.\n";
}


// Funci�n para ingresar funci�n de ganancia

void opcion3() {
    coefX = validaInput("Ingrese el coeficiente de x (mesas) en la funci�n de ganancia: ");
    coefY = validaInput("Ingrese el coeficiente de y (sillas) en la funci�n de ganancia: ");

    cout << "Funcion de ganancia: Max Z = " << coefX << "x + " << coefY << "y\n";
}


// Verifica si un punto (x, y) satisface todas las restricciones
bool esFactible(int x, int y) {
    for (const auto& r : restricciones) {
        if (r[0] * x + r[1] * y > r[2])
            return false;
    }
    return true;
}

// C�lculo de la soluci�n �ptima (m�todo de fuerza bruta discreto)
int optX = 0, optY = 0;
float mejorGanancia = -1;
void opcion4() {
    int maxX = 100, maxY = 100;
    mejorGanancia = -1;
    for (int x = 0; x <= maxX; ++x) {
        for (int y = 0; y <= maxY; ++y) {
            if (esFactible(x, y)) {
                float ganancia = coefX * x + coefY * y;
                if (ganancia > mejorGanancia) {
                    mejorGanancia = ganancia;
                    optX = x;
                    optY = y;
                }
            }
        }
    }
    cout << "Solucion optima: No. de sillas = " << optY << ", No. de mesas = " << optX << ", Ganancia maxima = $" << mejorGanancia << "\n";
}

// Opcion 5: Visualizacion grafica con SFML
void opcion5() {
    const int windowSize = 600;

    // Paso 1: detectar m�ximos reales
    int maxX = 0, maxY = 0;
    for (int x = 0; x <= 100; ++x) {
        for (int y = 0; y <= 100; ++y) {
            if (esFactible(x, y)) {
                if (x > maxX) maxX = x;
                if (y > maxY) maxY = y;
            }
        }
    }

    // Aumentar ligeramente el margen
    maxX += 5;
    maxY += 5;

    // Paso 2: calcular escala din�mica
    float escalaX = windowSize / static_cast<float>(maxX);
    float escalaY = windowSize / static_cast<float>(maxY);

    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Area Factible y Soluci�n Optima");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Paso 3: Dibujar ejes
        sf::Vertex ejeX[] = {
            sf::Vertex(sf::Vector2f(0, windowSize), sf::Color::Black),
            sf::Vertex(sf::Vector2f(windowSize, windowSize), sf::Color::Black)
        };
        sf::Vertex ejeY[] = {
            sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black),
            sf::Vertex(sf::Vector2f(0, windowSize), sf::Color::Black)
        };
        window.draw(ejeX, 2, sf::Lines);
        window.draw(ejeY, 2, sf::Lines);

        // Paso 4: Dibujar �rea factible
        for (int x = 0; x <= 100; ++x) {
            for (int y = 0; y <= 100; ++y) {
                if (esFactible(x, y)) {
                    sf::RectangleShape punto(sf::Vector2f(2, 2)); // m�s grande
                    punto.setFillColor(sf::Color(150, 200, 255));
                    punto.setPosition(x * escalaX, windowSize - y * escalaY);
                    window.draw(punto);
                }
            }
        }

        // Paso 5: Dibujar punto �ptimo
        sf::CircleShape optimo(5); // m�s grande
        optimo.setFillColor(sf::Color::Red);
        optimo.setOutlineThickness(1);
        optimo.setOutlineColor(sf::Color::Black);
        optimo.setPosition(optX * escalaX - 5, windowSize - optY * escalaY - 5); // centrado
        window.draw(optimo);

        window.display();
    }
}

// Men� principal

int main() {
    int opcion;
    do {
        cout << "-------------------------------------------------------------\n";
        cout << "\nMENU PRINCIPAL\n";
        cout << "1. Ingreso de precios de venta\n";
        cout << "2. Ingreso de restricciones de produccion\n";
        cout << "3. Ingreso de la funcion de ganancia\n";
        cout << "4. Calculo de la soluci�n optima\n";
        cout << "5. Solucion grafica del problema de optimizacion\n";
        cout << "6. Salir\n";
        cout << "-------------------------------------------------------------\n";
        opcion = static_cast<int>(validaInput("Seleccione una opcion (1-6): ", 1, 6));

        switch (opcion) {
        case 1: opcion1(); break;
        case 2: opcion2(); break;
        case 3: opcion3(); break;
        case 4: opcion4(); break;
        case 5: opcion5(); break;
        case 6: cout << "Saliendo del programa.\n"; break;
        }
    } while (opcion != 6);

    return 0;
}
