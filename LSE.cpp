#include <iostream>
using namespace std;


template<typename T>
struct nodo_listase {
	T valor;
	nodo_listase* link;
};


template<typename T>
class Lista {
private:
	typedef nodo_listase<T> Nodo;
	Nodo* cabeza;
	
public:
	Lista() : cabeza(nullptr) {}
	
	
	void agregar(const T& valor) {
		Nodo* nuevo = new Nodo;
		nuevo->valor = valor;
		nuevo->link = cabeza;
		cabeza = nuevo;
	}
	
	
	T obtener() const {
		if (cabeza != nullptr) {
			return cabeza->valor;
		}
		return T();
	}
	
	
	void eliminar() {
		if (cabeza != nullptr) {
			Nodo* temp = cabeza;
			cabeza = cabeza->link;
			delete temp;
		}
	}
	

	void mostrar() const {
		Nodo* actual = cabeza;
		while (actual != nullptr) {
			cout << actual->valor << " -> ";
			actual = actual->link;
		}
		cout << "NULL" << endl;
	}
	
	~Lista() {
		Nodo* actual = cabeza;
		while (actual != nullptr) {
			Nodo* temp = actual;
			actual = actual->link;
			delete temp;
		}
	}
};

int main() {
	Lista<int> lista;
	lista.agregar(10);
	lista.agregar(20);
	lista.agregar(30);
	lista.mostrar();
	
	cout << "Primer elemento: " << lista.obtener() << endl;
	lista.eliminar();
	lista.mostrar();
	
	return 0;
}


