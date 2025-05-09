#include <iostream>
using namespace std;

template<typename T>
struct nodo_cola {
	T dato;
	nodo_cola<T>* link;
};

template<typename T>
class Cola {
private:
	nodo_cola<T>* frente;
	nodo_cola<T>* fondo;
	
	bool colaVacia() {
		return (frente == nullptr);
	}
	
public:
		Cola() {
			frente = nullptr;
			fondo = nullptr;
		}
		
		void cola_agregar(T dato) {
			nodo_cola<T>* nuevo_nodo = new nodo_cola<T>;
			nuevo_nodo->dato = dato;
			nuevo_nodo->link = nullptr;
			
			if (colaVacia()) {
				frente = nuevo_nodo;
			} else {
				fondo->link = nuevo_nodo;
			}
			fondo = nuevo_nodo;
		}
		
		T obtenerElemento() {
			if (colaVacia()) {
				throw runtime_error("Error: la cola está vacía.");
			}
			
			nodo_cola<T>* aux = frente;
			T dato = aux->dato;
			frente = frente->link;
			
			if (frente == nullptr) {
				fondo = nullptr;
			}
			
			delete aux;
			return dato;
		}
};

