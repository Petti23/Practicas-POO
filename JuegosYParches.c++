#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;

// Declaración adelantada
class Parche;

class Producto {
protected:
    char nombre[100];
    char descripcion[200];
    float costo;

    
public:
    Producto(const char* nombre, const char* descripcion, float costo) {
        strcpy(this->nombre, nombre);
        strcpy(this->descripcion, descripcion);
        this->costo = costo;
    }

    virtual ~Producto() {}

    virtual char* getNombre() {
        return nombre;
    }

    virtual char* getDescripcion() {
        return descripcion;
        
    }

    virtual float getCosto() {
        return costo;
    }

    virtual char getTipo() = 0; // Método virtual puro
};

class VideoJuego : public Producto {
private:
    vector<Parche*> parches;

public:
    VideoJuego(const char* nombre, const char* descripcion, float costo)
        : Producto(nombre, descripcion, costo) {}

    void agregarParche(Parche* parche) {
        parches.push_back(parche);
    }

    vector<Parche*> getParches() {
        return parches;
    }

    char getTipo() override {
        return 'V';
    }
};

class Parche : public Producto {
private:
    VideoJuego* juegoAsociado;
    vector<Producto*> dependencias;

public:
    Parche(const char* nombre, const char* descripcion, float costo, VideoJuego* juego)
        : Producto(nombre, descripcion, costo), juegoAsociado(juego) {}

    void agregarDependencia(Producto* dependencia) {
        dependencias.push_back(dependencia);
    }

    vector<Producto*> getDependencias() {
        return dependencias;
    }

    VideoJuego* getJuegoAsociado() {
        return juegoAsociado;
    }

    char getTipo() override {
        return 'P';
    }

    bool puedeInstalarse(vector<Producto*> instalados) {
        for (auto dep : dependencias) {
            bool encontrado = false;
            for (auto inst : instalados) {
                if (strcmp(inst->getNombre(), dep->getNombre()) == 0 &&
                    inst->getTipo() == dep->getTipo()) {
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado)
                return false;
        }
        return true;
    }
};

class Gestor {
private:
    vector<VideoJuego*> juegos;
    vector<Parche*> parches;

public:
    ~Gestor() {
        for (auto juego : juegos) delete juego;
        for (auto parche : parches) delete parche;
    }

    void agregarJuego(VideoJuego* juego) {
        juegos.push_back(juego);
    }

    void agregarParche(Parche* parche) {
        parches.push_back(parche);
        parche->getJuegoAsociado()->agregarParche(parche);
    }

    void mostrarJuegos() {
        cout << "=== LISTA DE JUEGOS ===" << endl;
        for (auto juego : juegos) {
            cout << "Juego: " << juego->getNombre()
                 << " | Precio: $" << juego->getCosto()
                 << " | Parches: " << juego->getParches().size() << endl;
        }
    }

    void mostrarParches() {
        cout << "=== LISTA DE PARCHES ===" << endl;
        for (auto parche : parches) {
            cout << "Parche: " << parche->getNombre()
                 << " | Juego: " << parche->getJuegoAsociado()->getNombre()
                 << " | Dependencias: ";

            auto deps = parche->getDependencias();
            if (deps.empty()) {
                cout << "Ninguna";
            } else {
                for (auto dep : deps) {
                    cout << dep->getNombre() << " (" << dep->getTipo() << ") ";
                    if (dep != deps.back()) cout << ", ";
                }
            }
            cout << endl;
        }
    }

    vector<VideoJuego*> obtenerJuegosConMasParches() {
        vector<VideoJuego*> aux;
        if (juegos.empty()) return aux;

        size_t maxParches = 0;
        for (auto juego : juegos) {
            size_t num = juego->getParches().size();
            if (num > maxParches) maxParches = num;
        }

        for (auto juego : juegos) {
            if (juego->getParches().size() == maxParches) {
                aux.push_back(juego);
            }
        }
        return aux;
    }

    vector<VideoJuego*> obtenerJuegosMasCaros() {
        vector<VideoJuego*> aux;
        if (juegos.empty()) return aux;

        float maxCosto = 0;
        for (auto juego : juegos) {
            if (juego->getCosto() > maxCosto)
                maxCosto = juego->getCosto();
        }

        for (auto juego : juegos) {
            if (juego->getCosto() == maxCosto) {
                aux.push_back(juego);
            }
        }
        return aux;
    }

    bool verificarInstalacion(Parche* parche, vector<Producto*>& instalados) {
        return parche->puedeInstalarse(instalados);
    }

    //funcion para guardar los datos en un archivo binario con su tamaño
    void guardarDatos() {
        ofstream out("datos.bin", ios::binary);
        size_t size = juegos.size();
        out.write((char*)&size, sizeof(size_t));
        for (auto juego : juegos) {
            size_t nombreSize = strlen(juego->getNombre()) + 1;
            out.write((char*)&nombreSize, sizeof(size_t));
            out.write(juego->getNombre(), nombreSize);
            out.write((char*)&juego->getCosto(), sizeof(float));
        }
        size = parches.size();
        out.write((char*)&size, sizeof(size_t));
        for (auto parche : parches) {
            size_t nombreSize = strlen(parche->getNombre()) + 1;
            out.write((char*)&nombreSize, sizeof(size_t));
            out.write(parche->getNombre(), nombreSize);
            out.write((char*)&parche->getCosto(), sizeof(float));
        }
        out.close();
    }
};

// Función principal
int main() {
    Gestor gestor;

    // Crear videojuegos
    auto* juego1 = new VideoJuego("Juego1", "Descripcion Juego1", 50.0f);
    auto* juego2 = new VideoJuego("Juego2", "Descripcion Juego2", 60.0f);

    gestor.agregarJuego(juego1);
    gestor.agregarJuego(juego2);

    // Crear parches
    auto* parche1 = new Parche("Parche1", "Descripcion Parche1", 10.0f, juego1);
    auto* parche2 = new Parche("Parche2", "Descripcion Parche2", 20.0f, juego1);
    auto* parche3 = new Parche("Parche3", "Descripcion Parche3", 30.0f, juego2);

    // Agregar dependencias
    parche1->agregarDependencia(juego2); // depende de juego2
    parche2->agregarDependencia(parche1); // depende del parche1

    gestor.agregarParche(parche1);
    gestor.agregarParche(parche2);
    gestor.agregarParche(parche3);

    gestor.mostrarJuegos();
    gestor.mostrarParches();

    // Probar instalación
    vector<Producto*> instalados = { juego2, parche1 };
    cout << "¿Se puede instalar parche2? "
         << (gestor.verificarInstalacion(parche2, instalados) ? "Sí" : "No") << endl;

    return 0;
}
