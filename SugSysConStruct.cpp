#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;

struct permiso{
    string nombre;
};

class Permiso{
    private:
        string nombre;
    public:
        Permiso(string nombre) : nombre(nombre) {}
        string getNombre() { return nombre; }  
        permiso getDatos(){
            permiso p;
            p.nombre = nombre;
            return p;
        } 
};

struct cargo{
    string nombre;
    int cantPermisos;
};

class Cargo{
    private:
        string nombre;
        vector<Permiso*> permisos;
    public:
        Cargo(string nombre) : nombre(nombre) {}
        void agregarPermiso(Permiso* permiso) { permisos.push_back(permiso); }
        string getNombre() { return nombre; }
        vector<Permiso*> getPermisos() { return permisos; }
        cargo getDatos(){
            cargo c;
            c.nombre = nombre;
            c.cantPermisos = permisos.size();
            return c;
        }

};

struct usuario{
    char nombre[200];
    long int id;
    int cantCArgos;
};

class Usuario{
    private:
        char nombre[200];
        long int id;
        vector<Cargo*> cargos;
    public:
        Usuario(string nombre, long int id) {
            strcpy(this->nombre, nombre.c_str());
            this->id = id;
        }
        void agregarCargo(Cargo* cargo) { cargos.push_back(cargo); }
        string getNombre() { return nombre; }
        long int getId() { return id; }
        vector<Cargo*> getCargos() { return cargos; }
        usuario getDatos(){
            usuario u;
            strcpy(u.nombre, nombre);
            u.id = id;
            u.cantCArgos = cargos.size();
            return u;
        }
};

class Sistema{
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

        // Función para guardar los datos en un archivo binario
        void guardarEnBinario(const string nombreArchivo) {
            ofstream archivoUsuariosCargos(nombreArchivo, ios::binary);
            if (!archivoUsuariosCargos) {
                cout << "Error al abrir el archivo de usuarios y cargos." << endl;
                return;
            }
            size_t size = usuarios.size();
            archivoUsuariosCargos.write((char*)&size, sizeof(size_t));
            for (auto user : usuarios) {
                usuario u = user->getDatos();
                archivoUsuariosCargos.write((char*)&u, sizeof(user));
                for(auto car : user->getCargos()){
                    cargo c = car->getDatos();
                    archivoUsuariosCargos.write((char*)&c, sizeof(car));
                    for(auto per : car->getPermisos()){
                        permiso p = per->getDatos();
                        archivoUsuariosCargos.write((char*)&p, sizeof(per));
                    }
                }
            }
        }
};