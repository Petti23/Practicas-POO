#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
	///INT,DOUBLE,FLOAT
	int cant = pepe->getCantidad();
	archivo.write(reinterpret_cast<const char*>(&cant), sizeof(int));
	///CHAR*
	int tam = strlen(pepe->getNombre()) + 1;
	archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
	archivo.write(pepe->getNombre(), tam);
	///CHAR
	char tipo = 'M';
	archivo.write(reinterpret_cast<const char*>(&tipo), sizeof(char));
	///VECTOR
	int enteros [3] = {1,2,3};
	archivo.write(reinterpret_cast<const char*>(enteros),3 * sizeof(int));
	
	return 0;
}

