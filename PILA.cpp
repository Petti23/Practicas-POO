#include <iostream>
using namespace std;

template<typename T>
struct nodo_pila {
	T valor;
	nodo_pila* link;
};

template<typename T>
class Pila {
private:
	typedef nodo_pila<T> Nodo;
	Nodo* cima;
	
public:
	Pila() : cima(nullptr) {}
	
	
	void agregarElemento(const T& valor) {
		Nodo* nuevo = new Nodo;
		nuevo->valor = valor;
		nuevo->link = cima;
		cima = nuevo;
	}
	
	
	void desempilar() {
		if (cima != nullptr) {
			Nodo* temp = cima;
			cima = cima->link;
			delete temp;
		}
	}
	
	
	T obtener() const {
		if (cima != nullptr) {
			return cima->valor;
		}
		return T(); 
	}
	
	
	void mostrar() const {
		Nodo* actual = cima;
		while (actual != nullptr) {
			cout << actual->valor << " -> ";
			actual = actual->link;
		}
		cout << "NULL" << endl;
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

