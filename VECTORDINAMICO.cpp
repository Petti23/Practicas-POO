#include <iostream>
using namespace std;

template <class T>
class vectorOrdenado{
private:
	T* datos;
	int capacidad;
	int cantidad;
	
	void redimensionar(){
		capacidad *= 2;
		T* nuevo = new T[capacidad];
		
		for(int i = 0; i< cantidad; i++){
			nuevo[i] = datos[i];
		}
		
		delete [] datos;
		datos = nuevo;
	}
	void ordenar_vector(){
		for (int i = 0; i<cantidad; i++){
			for(int j = 1; j<cantidad; j++){
				if(datos[i]<datos[j]){
					T aux = datos[i];
					datos[i] = datos[j];
					datos[j] = aux;
				}
			}
			
		}
	}
	
	bool seRepite(T dato){///<----
		for(int i = 0; i<cantidad; i++){
			if (dato == this->datos[i]){
				return true;
			}
		}
		return false;
	}
public:
	vectorOrdenado(): capacidad(2){
		cantidad = 0;
		datos = new T[capacidad];
	}
	
	void agregar(const T& elemento){
		if(seRepite(elemento)){///<------
			return;
		}
		if(cantidad == capacidad){
			redimensionar();
		}
		datos[cantidad] = elemento;
		cantidad ++;
		
		ordenar_vector();
	}
	
	T eliminarElemento(int indice){
		if(indice < 0 || indice >=cantidad){
			return {};
		}
		T pepe = datos[indice];
		for(indice; indice < cantidad -1; indice++){
			datos[indice] = datos[indice+1];
		
		}
		cantidad--;
		return pepe;
	}
	
	void imprimirElementos(){
		for(int i = 0; i < cantidad; i++){
		cout<<datos[i]<<endl;
		}
	}
};

