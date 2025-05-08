#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;



class Paquete{
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

        char* getCodigo() { return codigo; }
        char* getNombre() { return nombre; }
        int getVersion() { return version; }
        void agregarDependencia(Paquete* dependencia) { dependencias.push_back(dependencia); }
        vector<Paquete*> getDependencias() { return dependencias; }
};

//ACA SOBREESCRIBIMOS EL OPERADOR <<

class GestorPaquetes{
    private:
        vector<Paquete*> paquetes;
    public:
        ~GestorPaquetes() {
            for (auto paquete : paquetes) delete paquete;
        }

        void agregarPaquete(Paquete* paquete) { paquetes.push_back(paquete); }

        //implemente la función que permita saber todas las dependencias de un paquete (incluyendo las transitivas), dado el código del paquete. 
        void mostrarDependenciasRec(Paquete* paquete, int nivel) {
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

        void guardarDependencias(const char* codigo) {
            for (auto paquete : paquetes) {
                if (strcmp(paquete->getCodigo(), codigo) == 0) {
                    ofstream out(string(codigo) + ".txt"); // Crear archivo con el nombre del código
                    if (out.is_open()) {
                        out << paquete; // Usar el operador << para escribir el árbol de dependencias
                        out.close();
                        cout << "Árbol de dependencias guardado en " << codigo << ".txt\n";
                    } else {
                        cout << "Error al abrir el archivo.\n";
                    }
                    return;
                }
            }
            cout << "Paquete no encontrado.\n";
        }
        
};

