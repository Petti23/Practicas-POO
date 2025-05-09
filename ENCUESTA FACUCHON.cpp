#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
using namespace std;

class Respuesta;

class Pregunta{
private:
	int id;
	char pregunta[200];
	vector<Respuesta*> respuestas;
public:
	Pregunta(const char* preg){strcpy(pregunta, preg);}
	const char* getPregunta(){return pregunta;}
	void addRespuestas(Respuesta* nueva){
		respuestas.push_back(nueva);
	}
	vector<Respuesta*> getRespuestas(){return respuestas;}
	int getId(){return id;}
};

class Respuesta {
private:
	int id;
	char respuesta[200];
	vector<Pregunta*>preguntas;
public:
	Respuesta(const char* res){strcpy(respuesta, res);}
	const char* getRespuesta(){return respuesta;}
	void addPregunta(Pregunta* nueva){
		preguntas.push_back(nueva);
	}
	vector<Pregunta*> getPreguntas(){return preguntas;}
	int getId(){return id;}
};


class Gestor{
private:
	vector<Respuesta*>respuestas;
	vector<Pregunta*>preguntas;
public:
	void agregarPregunta(Pregunta* nueva){preguntas.push_back(nueva);}
	void agregarRespuesta(Respuesta* nueva){respuestas.push_back(nueva);}
	
	void preguntaBinaria(Pregunta* p, string dir){
		ofstream archivo(dir, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int id = p->getId();
		archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
		
		int tam = strlen(p->getPregunta()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
		archivo.write(p->getPregunta(),tam);
		
		int cant = p->getRespuestas().size();
		archivo.write(reinterpret_cast<const char*>(&cant),sizeof(int));
		
		for(const auto& c: p->getRespuestas()){
			int id = c->getId();
			archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
		}
		
		archivo.close();
	}
		
	void respuestaBinaria(Respuesta* p, string dir){
		ofstream archivo(dir, ios::binary | ios::app);
		
		if(archivo.fail()){return;}
		
		int id = p->getId();
		archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
		
		int tam = strlen(p->getRespuesta()) + 1;
		archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
		archivo.write(p->getRespuesta(),tam);
	
		int cant = p->getPreguntas().size();
		archivo.write(reinterpret_cast<const char*>(&cant),sizeof(int));
	
		for(const auto& c: p->getPreguntas()){
			int id = c->getId();
			archivo.write(reinterpret_cast<const char*>(&id),sizeof(int));
		}
		
		archivo.close();
	}
	
	void guardarBinario(string dirPreguntas, string dirRespuestas){
		
		if(preguntas.empty() && respuestas.empty()){return;}
		
		for(const auto& p: preguntas){
			preguntaBinaria(p,dirPreguntas);
		}
		
		for(const auto& r: respuestas){
			respuestaBinaria(r,dirRespuestas);
		}
	}
	
	void responderPregunta(Pregunta* p) {
		if (!p) return;
		
		cout << "\nPregunta: " << p->getPregunta() << endl;
		
		vector<Respuesta*> opciones = p->getRespuestas();
		
		if (opciones.empty()) {
			cout << "No hay respuestas para esta pregunta.\n";
			return;
		}
		
		for (size_t i = 0; i < opciones.size(); ++i) {
			cout << "  " << (i + 1) << ") " << opciones[i]->getRespuesta() << endl;
		}
		
		int seleccion = 0;
		do {
			cout << "Seleccione una opción (1-" << opciones.size() << "): ";
			cin >> seleccion;
			
			if (cin.fail() || seleccion < 1 || seleccion > (int)opciones.size()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Entrada inválida. Intente de nuevo.\n";
				seleccion = 0;
			}
		} while (seleccion == 0);
			
		Respuesta* elegida = opciones[seleccion - 1];
		cout << "Ha elegido: " << elegida->getRespuesta() << endl;
			
		for (Pregunta* siguiente : elegida->getPreguntas()) {
			responderPregunta(siguiente); 
		}
	}
	
	
	void responderEncuesta() {
		if (preguntas.empty()) {
			cout << "No hay preguntas cargadas.\n";
			return;
		}
		
		cout << "Inicio de la encuesta:\n";
		
		for (Pregunta* p : preguntas) {
			responderPregunta(p);
		}
		
		cout << "\nEncuesta finalizada. Gracias.\n";
	}
	
	vector<Pregunta*> preuntasConMasRespuesta(){
		if (preguntas.empty()) {
			cout << "No hay preguntas registradas.\n";
			return {};
		}
		
		size_t maxRespuestas = 0;
		for (const Pregunta* p : preguntas) {
			maxRespuestas = max(maxRespuestas, p->getRespuestas().size());
		}
		
		vector<Pregunta*> resultado;
		for (Pregunta* p : preguntas) {
			if (p->getRespuestas().size() == maxRespuestas) {
				resultado.push_back(p);
			}
		}
		
		return resultado;
	}
	
	vector<Respuesta*> respuestasConMasPreguntasEncadenadas() {
		if (respuestas.empty()) {
			cout << "No hay respuestas registradas.\n";
			return {};
		}
		
		size_t maxPreguntas = 0;
		for (const Respuesta* r : respuestas) {
			maxPreguntas = max(maxPreguntas, r->getPreguntas().size());
		}
		
		vector<Respuesta*> resultado;
		for (Respuesta* r : respuestas) {
			if (r->getPreguntas().size() == maxPreguntas) {
				resultado.push_back(r);
			}
		}
		
		return resultado;
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

