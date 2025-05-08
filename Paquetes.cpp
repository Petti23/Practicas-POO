#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

class Paquete {
    private:
        char codigo[100];
        char nombre[100];
        int version;
        vector<Paquete*> dependencias;
    public:
        Paquete(const char* codigo, const char* nombre, int version) {
            strcpy(this->codigo, codigo);
            strcpy(this->nombre, nombre);
            this->version = version;
        }

        const char* getCodigo() const { return codigo; }
        const char* getNombre() const { return nombre; }
        int getVersion() const { return version; }
        void agregarDependencia(Paquete* dependencia) { dependencias.push_back(dependencia); }
        vector<Paquete*> getDependencias() const { return dependencias; }
};


void imprimirArbolDependencias(ostream& os, const Paquete* paquete, int nivel = 1) {
    const auto& deps = paquete->getDependencias();
    if (deps.empty()) {
        os << string(nivel, '\t') << "(Sin dependencias)\n";
        return;
    }
    for (const auto& dep : deps) {
        os << string(nivel, '\t') << dep->getNombre() << " (v" << dep->getVersion() << ")\n";
        imprimirArbolDependencias(os, dep, nivel + 1);
    }
}

//Sobrecarga del operador 
ostream& operator<<(ostream& os, const Paquete& paquete) {
    os << "Dependencias de " << paquete.getNombre() << ":\n";
    imprimirArbolDependencias(os, &paquete, 1);
    return os;
}

class GestorPaquetes {
    private:
        vector<Paquete*> paquetes;
    public:
        ~GestorPaquetes() {
            for (auto paquete : paquetes) delete paquete;
        }

        void agregarPaquete(Paquete* paquete) { paquetes.push_back(paquete); }

        void mostrarDependenciasRec(const Paquete* paquete, int nivel) {
            if (paquete->getDependencias().empty()) {
                for (int i = 0; i < nivel; i++) cout << "\t";
                cout << "(Sin dependencias)\n";
                return;
            }
            for (auto dep : paquete->getDependencias()) {
                for (int i = 0; i < nivel; i++) cout << "\t";
                cout << dep->getNombre() << " (v" << dep->getVersion() << ")\n";
                mostrarDependenciasRec(dep, nivel + 1);
            }
        }

        void mostrarDependencias(const char* codigo) {
            for (auto paquete : paquetes) {
                if (strcmp(paquete->getCodigo(), codigo) == 0) {
                    cout << "Dependencias de " << paquete->getNombre() << ":\n";
                    mostrarDependenciasRec(paquete, 1);
                    return;
                }
            }
            cout << "Paquete no encontrado.\n";
        }

        void guardarDependenciasEnArchivo(const char* codigo) {
            for (auto paquete : paquetes) {
                if (strcmp(paquete->getCodigo(), codigo) == 0) {
                    string nombreArchivo = string(paquete->getNombre()) + ".txt";
                    ofstream archivo(nombreArchivo);
                    if (archivo.is_open()) {
                        archivo << *paquete;
                        archivo.close();
                        cout << "Archivo '" << nombreArchivo << "' generado correctamente.\n";
                    } else {
                        cout << "No se pudo abrir el archivo.\n";
                    }
                    return;
                }
            }
            cout << "Paquete no encontrado.\n";
        }

        void get5ConMayoresDependencias() {
            sort(paquetes.begin(), paquetes.end(), [](Paquete* a, Paquete* b) {
                return a->getDependencias().size() > b->getDependencias().size();
            });
            cout << "Los 5 paquetes con más dependencias son:\n";
            for (int i = 0; i < 5 ; i++) {
                cout << paquetes[i]->getNombre() << " - Dependencias: " << paquetes[i]->getDependencias().size() << endl;
            }
        }

        void getPaquetesSinDependencias() {
            cout << "Paquetes sin dependencias:\n";
            for (auto paquete : paquetes) {
                if (paquete->getDependencias().empty()) {
                    cout << paquete->getNombre() << endl;
                }
            }
        }
};

class ConjuntoPaquetes {
    private:
        vector<Paquete*> paquetes;
        char codigo[100];
        char nombre[100];
        int version;
    public:
        ConjuntoPaquetes(const char* codigo, const char* nombre, int version) {
            strcpy(this->codigo, codigo);
            strcpy(this->nombre, nombre);
            this->version = version;
        }

        void agregarPaquete(Paquete* paquete) { paquetes.push_back(paquete); }

        void instalacionRec(const Paquete* paquete, vector<Paquete*>& instalados) {
            if (paquete->getDependencias().empty()) return;
            for (auto dep : paquete->getDependencias()) {
                bool encontrado = false;
                for (auto inst : instalados) {
                    if (strcmp(inst->getCodigo(), dep->getCodigo()) == 0) {
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) {
                    instalados.push_back(dep);
                    instalacionRec(dep, instalados);
                }
            }
        }

        void guardarConjuntoEnArchivo(const char* nombreArchivo) {
            ofstream archivo(nombreArchivo);
            if (archivo.is_open()) {
                archivo << "Conjunto de paquetes: " << nombre << "\n";
                for (auto paquete : paquetes) {
                    archivo << paquete->getNombre() << " (v" << paquete->getVersion() << ")\n";
                }
                archivo.close();
                cout << "Archivo '" << nombreArchivo << "' generado correctamente.\n";
            } else {
                cout << "No se pudo abrir el archivo.\n";
            }
        }
}


int main() {
    GestorPaquetes gestor;
    Paquete* p1 = new Paquete("P1", "Paquete1", 1);
    Paquete* p2 = new Paquete("P2", "Paquete2", 2);
    Paquete* p3 = new Paquete("P3", "Paquete3", 3);
    Paquete* p4 = new Paquete("P4", "Paquete4", 4);
    Paquete* p5 = new Paquete("P5", "Paquete5", 5);

    p1->agregarDependencia(p2);
    p1->agregarDependencia(p3);
    p2->agregarDependencia(p4);
    p3->agregarDependencia(p4); // p3 también depende de p4
    p4->agregarDependencia(p5); // p4 depende de p5

    gestor.agregarPaquete(p1);
    gestor.agregarPaquete(p2);
    gestor.agregarPaquete(p3);
    gestor.agregarPaquete(p4);
    gestor.agregarPaquete(p5);

    gestor.mostrarDependencias("P1");
    gestor.guardarDependenciasEnArchivo("P1");

    // Crear conjunto de paquetes
    ConjuntoPaquetes conjunto("C001", "Conjunto Principal", 1);
    conjunto.agregarPaquete(p1);
    conjunto.agregarPaquete(p3);  // ya está como dependencia de p1, pero lo agregamos explícitamente

    // Instalar el conjunto
    conjunto.instalar();

    return 0;
}

