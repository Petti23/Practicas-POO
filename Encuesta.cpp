#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;

class Pregunta;

class Respuesta{
    private:
        char * texto;
        vector<Pregunta*> preguntasEncadenadas;
    public:
        Respuesta(char * texto) : texto(texto){}

        char * getTexto(){return texto;}
        vector<Pregunta*> getPreguntasEncadenadas(){return preguntasEncadenadas;}
        void addPregunta(Pregunta* pregunta){preguntasEncadenadas.push_back(pregunta);}
};

class Pregunta{
    private:
        char * texto;
        vector<Respuesta*> respuestas;
    public:
        Pregunta(char * texto) : texto(texto){}
        char * getTexto(){return texto;}
        vector<Respuesta*> getRespuestas(){return respuestas;}
        void addRespuesta(Respuesta* respuesta){respuestas.push_back(respuesta);}

        void guardarEnArchivoBinario(char * direccion){
            ofstream out(direccion, ios::binary);
            if(out.fail())return;

            size_t respSize = respuestas.size();
            out.write((char*)&respSize, sizeof(size_t));
            for(auto& res : respuestas){
                size_t contentSize = strlen(res->getTexto())+1;
                out.write((char*)&contentSize, sizeof(size_t));
                out.write(res->getTexto(), contentSize);

                size_t pregsSize = (res->getPreguntasEncadenadas().size())+1;
                out.write((char*)&pregsSize, sizeof(size_t));
                for(auto& pregs : res->getPreguntasEncadenadas()){
                    size_t textoSize = strlen(pregs->getTexto())+1;
                    out.write((char*)&textoSize, sizeof(size_t));
                    out.write(pregs->getTexto(), textoSize);
                }

            }
        }
};

class Encuesta{
    private:
        int id;
        vector<Pregunta*> preguntas;
    public:
        Encuesta(int id) : id(id){
            vector<Pregunta*> preguntas;
            this->preguntas = preguntas;
        }
        vector<Pregunta*> getPreguntas(){return preguntas;}


        
    


};

class Sistema{
    private:
        vector<Encuesta*> encuestas;
    public:
    void agregarEncuesta(Encuesta* encuesta){ encuestas.push_back(encuesta);}


};