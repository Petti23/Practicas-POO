#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

// --> (depende)
// parche --> juego
// puede pasar que: parche --> parche

class Producto{
    private:
        char nombre[100];
        char descripcion[200];
        float costo;
    public:
        Producto( char* nombre,  char* descripcion, float costo){
            strcpy(this->nombre, nombre);
            strcpy(this->descripcion, descripcion);
            this->costo = costo;
        }
}

class VideoJuego : public Producto{
    private:
        vector<Parche*> parches;
    public:
        VideoJuego( char* nombre,  char* descripcion, float costo) : Producto(nombre, descripcion, costo){
        }
        void agregarParche(Parche* parche){
            parches.push_back(parche);
        }
        vector<Parche*> getParches(){
            return parches;
        }
         char* getNombre(){
            return nombre;
        }
        float getCosto(){
            return costo;
        }
         char* getDescripcion(){
            return descripcion;
        }
        char getTipo(){
            return 'V'; // V para VideoJuego
        }
        
}

class Parche : public Producto{
    private:
        VideoJuego* juegoAsociado;
        vector<Producto*> dependencias;
    public:
        Parche( string& nombre,  string& descripcion, float costo, VideoJuego* juego)
        : Producto(nombre, descripcion, costo), juegoAsociado(juego) {}

        void agregarDependencia(Producto* dependencia) {
            dependencias.push_back(dependencia);
        }

        vector<Producto*> getDependencias()  {
            return dependencias;
        }

        VideoJuego* getJuegoAsociado()  {
            return juegoAsociado;
        }

        char getTipo()  override {
            return 'P'; 
        }

        bool puedeInstalarse(vector<Producto*> instalados)  {
            for (auto dep : dependencias) {
                bool encontrado = false;
                for (auto inst : instalados) {
                    if (inst->getNombre() == dep->getNombre() && inst->getTipo() == dep->getTipo()) {
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) return false;
            }
            return true;
        }
}

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
        }
        
        void mostrarJuegos()  {
            cout << "=== LISTA DE JUEGOS ===" << endl;
            for (auto juego : juegos) {
                cout << "Juego: " << juego->getNombre() << " | Precio: $" << juego->getCosto() << " | Parches: " << juego->getParches().size() << endl;
            }
        }
        
        void mostrarParches()  {
            cout << "=== LISTA DE PARCHES ===" << endl;
            for (auto parche : parches) {
                cout << "Parche: " << parche->getNombre()  << " | Juego: " << parche->getJuegoAsociado()->getNombre() << " | Dependencias: ";
                
                auto dependencias = parche->getDependencias();
                if (dependencias.empty()) {
                    cout << "Ninguna";
                } else {
                    for (auto dep : dependencias) {
                        cout << dep->getNombre() << " (" << (dep->getTipo() == 'V' ? "Juego" : "Parche") << ") ";
                        if (dep != dependencias.back()) cout << ", "; // Para no poner coma al final         
                    }
                }
                cout << endl;
            }
        }
        
        vector<VideoJuego*> obtenerJuegosConMasParches()  {
            vector<VideoJuego*> resultado;
            if (juegos.empty()) return resultado;
            
            size_t maxParches = 0;
            for (auto juego : juegos) {
                size_t numParches = juego->getParches().size();
                if (numParches > maxParches) {
                    maxParches = numParches;
                }
            }
            
            for (auto juego : juegos) {
                if (juego->getParches().size() == maxParches) {
                    resultado.push_back(juego);
                }
            }
            
            return resultado;
        }
        
        vector<VideoJuego*> obtenerJuegosMasCaros()  {
            vector<VideoJuego*> resultado;
            if (juegos.empty()) return resultado;
            
            float maxCosto = 0;
            for (auto juego : juegos) {
                if (juego->getCosto() > maxCosto) {
                    maxCosto = juego->getCosto();
                }
            }
            
            for (auto juego : juegos) {
                if (juego->getCosto() == maxCosto) {
                    resultado.push_back(juego);
                }
            }
            
            return resultado;
        }
        
        void guardarEnArchivoTexto()  {
            ofstream archivo("dataJuegos.txt");
            if (!archivo.is_open()) {
                cerr << "Error al abrir el archivo de texto" << endl;
                return;
            }
            
            archivo << "=== JUEGOS MÁS CAROS ===\n";
            auto juegosCaros = obtenerJuegosMasCaros();
            for (auto juego : juegosCaros) {
                archivo << "Nombre: " << juego->getNombre() << endl
                        << "Descripción: " << juego->getDescripcion() << endl
                        << "Precio: $" << juego->getCosto() << endl
                        << "Parches: " << juego->getParches().size() << "\n\n";
            }
            
            archivo << "=== JUEGOS CON MÁS PARCHES ===\n";
            auto juegosConParches = obtenerJuegosConMasParches();
            for (auto juego : juegosConParches) {
                archivo << "Nombre: " << juego->getNombre() << endl
                        << "Descripción: " << juego->getDescripcion() << endl
                        << "Precio: $" << juego->getCosto() << endl
                        << "Parches: " << juego->getParches().size() << "\n\n";
            }
            
            archivo.close();
        }
        
        void guardarEnArchivoBinario()  {
            // Guardar juegos
            ofstream archivoJuegos("dataJuegos.bin", ios::binary);
            if (!archivoJuegos.is_open()) {
                cerr << "Error al abrir el archivo binario de juegos" << endl;
                return;
            }
            
            for (auto juego : juegos) {
                size_t nombreSize = juego->getNombre().size();
                archivoJuegos.write(reinterpret_cast< char*>(&nombreSize), sizeof(size_t));
                archivoJuegos.write(juego->getNombre().c_str(), nombreSize);
                
                size_t descSize = juego->getDescripcion().size();
                archivoJuegos.write(reinterpret_cast< char*>(&descSize), sizeof(size_t));
                archivoJuegos.write(juego->getDescripcion().c_str(), descSize);
                
                float costo = juego->getCosto();
                archivoJuegos.write(reinterpret_cast< char*>(&costo), sizeof(float));
                
                size_t numParches = juego->getParches().size();
                archivoJuegos.write(reinterpret_cast< char*>(&numParches), sizeof(size_t));
            }
            archivoJuegos.close();
            
            // Guardar parches
            ofstream archivoParches("dataParches.bin", ios::binary);
            if (!archivoParches.is_open()) {
                cerr << "Error al abrir el archivo binario de parches" << endl;
                return;
            }
            
            for (auto parche : parches) {
                size_t nombreSize = parche->getNombre().size();
                archivoParches.write(reinterpret_cast< char*>(&nombreSize), sizeof(size_t));
                archivoParches.write(parche->getNombre().c_str(), nombreSize);
                
                size_t descSize = parche->getDescripcion().size();
                archivoParches.write(reinterpret_cast< char*>(&descSize), sizeof(size_t));
                archivoParches.write(parche->getDescripcion().c_str(), descSize);
                
                float costo = parche->getCosto();
                archivoParches.write(reinterpret_cast< char*>(&costo), sizeof(float));
                
                size_t juegoSize = parche->getJuegoAsociado()->getNombre().size();
                archivoParches.write(reinterpret_cast< char*>(&juegoSize), sizeof(size_t));
                archivoParches.write(parche->getJuegoAsociado()->getNombre().c_str(), juegoSize);
                
                size_t numDeps = parche->getDependencias().size();
                archivoParches.write(reinterpret_cast< char*>(&numDeps), sizeof(size_t));
                
                for (auto dep : parche->getDependencias()) {
                    // Guardar tipo (Juego o Parche)
                    char tipo = dep->getTipo();
                    archivoParches.write(&tipo, sizeof(char));
                    
                    // Guardar dependencia
                    size_t depSize = dep->getNombre().size();
                    archivoParches.write(reinterpret_cast< char*>(&depSize), sizeof(size_t));
                    archivoParches.write(dep->getNombre().c_str(), depSize);
                }
            }
            archivoParches.close();
        }
        
        // Método para verificar si un parche puede instalarse
        bool verificarInstalacion( Parche* parche,  vector<Producto*>& instalados)  {
            return parche->puedeInstalarse(instalados);h
        }
    };