#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;

Class Permiso{
    private:
        string nombre;
    public:
        Permiso(string nombre) : nombre(nombre) {}
        string getNombre() { return nombre; }   
};

Class Cargo{
    private:
        string nombre;
        vector<Permiso*> permisos;
    public:
        Cargo(string nombre) : nombre(nombre) {}
        void agregarPermiso(Permiso* permiso) { permisos.push_back(permiso); }
        string getNombre() { return nombre; }
        vector<Permiso*> getPermisos() { return permisos; }

};

Class Usuario{
    private:
        char nombre[2000];
        long int id;
        vector<Cargo*> cargos;
    public:
        Usuario(string nombre, long int id) : nombre(nombre), id(id) {}
        void agregarCargo(Cargo* cargo) { cargos.push_back(cargo); }
        string getNombre() { return nombre; }
        long int getId() { return id; }
        vector<Cargo*> getCargos() { return cargos; }
};

Class Sistema{
    private:
        vector<Usuario*> usuarios;
        vector<Cargo*> cargos;
        vector<Permiso*> permisos;
    public:
        void agregarUsuario(Usuario* usuario) { usuarios.push_back(usuario); }
        void agregarCargo(Cargo* cargo) { cargos.push_back(cargo); }
        void agregarPermiso(Permiso* permiso) { permisos.push_back(permiso); }

        void buscarPermisoEnUsuario(long int id, string nombrePermiso) {
            for (auto usuario : usuarios) {
                if (usuario->getId() == id) {
                    cout << "Usuario: " << usuario->getNombre() << endl;
                    for (auto cargo : usuario->getCargos()) {
                        for (auto permiso : cargo->getPermisos()) {
                            if (permiso->getNombre() == nombrePermiso) {
                                cout << "Permiso encontrado en cargo: " << cargo->getNombre() << endl;
                                return;
                            }
                        }
                    }
                    cout << "Permiso no encontrado" << endl;
                    return;
                }
            }
            cout << "Usuario no encontrado" << endl;
        }
        void buscarCargosConMasPermisos() {
            map<string, int> mapa;
            for (auto cargo : cargos) {
                mapa[cargo->getNombre()] = cargo->getPermisos().size();
            }
            int maxPermisos = 0;
            for (auto& p : mapa) {
                if (p.second > maxPermisos) {
                    maxPermisos = p.second;
                }
            }
            cout << "Cargos con mas permisos:\n";
            for (auto& p : mapa) {
                if (p.second == maxPermisos) {
                    cout << p.first << ": " << p.second << endl;
                }
            }
        }

        // Función para verificar todos los permisos que están en varios cargos
        void verificarPermisosEnCargos() {
            map<string, vector<Cargo*>> mapa;
            for (auto cargo : cargos) {
                for (auto permiso : cargo->getPermisos()) {
                    mapa[permiso->getNombre()].push_back(cargo);
                }
            }
            cout << "Permisos en varios cargos:\n";
            for (auto& p : mapa) {
                if (p.second.size() > 1) {
                    cout << p.first << ": ";
                    for (auto cargo : p.second) {
                        cout << cargo->getNombre();
                        if (cargo != p.second.back()) cout << ", ";
                    }
                    cout << endl;
                }
            }
        }

        void usuariosConMasPermisos() {
            map<string, int> mapa;
            for (auto usuario : usuarios) {
                for (auto cargo : usuario->getCargos()) {
                    mapa[usuario->getNombre()] += cargo->getPermisos().size();
                }
            }
            int maxPermisos = 0;
            for (auto& p : mapa) {
                if (p.second > maxPermisos) {
                    maxPermisos = p.second;
                }
            }
            cout << "Usuarios con mas permisos:\n";
            for (auto& p : mapa) {
                if (p.second == maxPermisos) {
                    cout << p.first << ": " << p.second << endl;
                }
            }
        }

        // Función para guardar los datos en un archivo binario, 
        // en uno guardaremos los usuarios con sus cargos,
        // en otro guardaremos los cargos con sus permisos y en otro los permisos (3 archivos)
        void guardarEnBinario(const char* nombreArchivo, const char* nombreArchivo2, const char* nombreArchivo3) {
            ofstream archivoUsuariosCargos(nombreArchivo, ios::binary);
            if (!archivoUsuariosCargos) {
                cout << "Error al abrir el archivo de usuarios y cargos." << endl;
                return;
            }
            size_t size = usuarios.size();
            archivoUsuariosCargos.write((char*)&size, sizeof(size_t));
            for (auto usuario : usuarios) {
                archivoUsuariosCargos.write((char*)&usuario->getNombre(), sizeof(usuario->getNombre()));
                archivoUsuariosCargos.write((char*)&usuario->getId(), sizeof(long int));
                size_t sizeCargos = usuario->getCargos().size();
                archivoUsuariosCargos.write((char*)&sizeCargos, sizeof(size_t));
                for (auto cargo : usuario->getCargos()) {
                    archivoUsuariosCargos.write(cargo->getNombre().c_str(), cargo->getNombre().size() + 1);
                }
            }
            archivoUsuariosCargos.close();
            cout << "Datos de usuarios y cargos guardados en " << nombreArchivo << endl;

            ofstream archivoCargosPermisos(nombreArchivo2, ios::binary);
            if (!archivoCargosPermisos) {
                cout << "Error al abrir el archivo de cargos y permisos." << endl;
                return;
            }
            size = cargos.size();
            archivoCargosPermisos.write((char*)&size, sizeof(size_t));
            for (auto cargo : cargos) {
                archivoCargosPermisos.write((char*)&cargo->getNombre(), sizeof(cargo->getNombre()));
                size_t sizePermisos = cargo->getPermisos().size();
                archivoCargosPermisos.write((char*)&sizePermisos, sizeof(size_t));
                for (auto permiso : cargo->getPermisos()) {
                    archivoCargosPermisos.write(permiso->getNombre().c_str(), permiso->getNombre().size() + 1);
                }
            }
            +
            archivoCargosPermisos.close();
            cout << "Datos de cargos y permisos guardados en " << nombreArchivo2 << endl;

            ofstream archivoPermisos(nombreArchivo3, ios::binary);
            if (!archivoPermisos) {
                cout << "Error al abrir el archivo de permisos." << endl;
                return;
            }
            size = permisos.size();
            archivoPermisos.write((char*)&size, sizeof(size_t));
            for (auto permiso : permisos) {
                archivoPermisos.write(permiso->getNombre().c_str(), permiso->getNombre().size() + 1);
            }
            archivoPermisos.close();
            cout << "Datos de permisos guardados en " << nombreArchivo3 << endl;
        }
};