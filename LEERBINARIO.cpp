void leerArchivo(string dir, string tipo) {
	ifstream archivo(dir, ios::binary);
	if (archivo.fail()) {
		cout << "No se pudo abrir el archivo: " << dir << endl;
		return;
	}
	
	while (!archivo.eof()) {
		int cod, tam;
		archivo.read(reinterpret_cast<char*>(&cod), sizeof(cod));
		
		if (archivo.eof()) break; // Previene leer al final
		
		archivo.read(reinterpret_cast<char*>(&tam), sizeof(tam));
		
		char* buffer = new char[tam + 1];
		archivo.read(buffer, tam);
		buffer[tam] = '\0'; // Agregar terminador nulo
		
		int cantNotas;
		archivo.read(reinterpret_cast<char*>(&cantNotas), sizeof(cantNotas));
		
		vector<int> notas(cantNotas);
		for (int i = 0; i < cantNotas; ++i) {
			archivo.read(reinterpret_cast<char*>(&notas[i]), sizeof(notas[i]));
		}
		
		Alumno* nuevo = nullptr;
		if (tipo == "Invitado") {
			nuevo = new Invitado(cod, buffer);
		} else if (tipo == "Medio") {
			nuevo = new Medio(cod, buffer);
		} else if (tipo == "Premium") {
			nuevo = new Premium(cod, buffer);
		}
		
		if (nuevo != nullptr) {
			nuevo->cargarNotas(notas);
			cargarAlumno(nuevo);
		}
		
		delete[] buffer;
	}
	
	archivo.close();
}
