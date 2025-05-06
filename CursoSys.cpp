#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

/*La empresa CursoSys lo contrata para desarrollar un software que permita registrar el dictado de su curso. La empresa dicta un curso y tiene diferentes tipos de alumnos: 

    Alumno invitado: es gratuito y aprueba el curso con una nota mayor a 60 en el examen. 
    Alumno medio: el cual paga una suscripción y aprueba el curso con 3 exámenes los cuales tienen que tener un promedio mayor a 70. 
    Alumnos premium: el cual paga una suscripción y aprueba el curso con 5 exámenes los cuales tienen que ser mayores a 70 y tener un promedio mayor a 80%

    El alumno tiene un código, nombre y apellido y las notas dependientes del tipo. 

    (25) Realice un diseño que permita guardar las entidades en uno o varios archivos binarios. 
    (20) Realice una función que liste los alumnos que aprobaron el curso. 
    (30) La empresa necesita una funcionalidad que permita al usuario invitado cambiar a usuario medio o premium. 
    (25) Utilizando STL realice:
        una función que indique cual es o son los alumnos que tienen el promedio más alto. 
        una función que indique la cantidad de alumnos por tipo de alumno, utilice map.

*/

class Plan{
    protected:
        string nombre;
    public:
        Plan(){}
        virtual float getPromedio() = 0;
        virtual void setNombre(string nombre){
            this->nombre = nombre;
        }
        virtual string getNombre(){
            return nombre;
        }
        virtual bool aprobo() = 0;
        string getTipo(){
            return nombre;
        }
};

class Invitado : public Plan{
    private:
        float nota;    
    public:
        Invitado(){
            setNombre("Invitado");
        }
        string getTipo(){
            return nombre;
        };
        bool aprobo(){
            return nota>60;
        }
        float getPromedio(){
            return nota;
        }
        void setNota(float nota){
            this->nota = nota;
        }
};
class Medio : public Plan{
    private:
        float notas[3];
    public:
        Medio(){
            setNombre("Medio");
        }
        void setNotas(float nota1, float nota2, float nota3){
            notas[0] = nota1;
            notas[1] = nota2;
            notas[2] = nota3;
        }
        float getPromedio(){
            float suma = 0;
            for(auto nota : notas){
                suma += nota;
            }
            float promedio = suma / 3;
            cout << "El promedio es: " << promedio << endl;
            return promedio;
        }
        bool aprobo(){
            float promedio = getPromedio();
            if(promedio < 70){
                return false;
            }
            return true;
        }

};
class Premium : public Plan{
    private:
        float notas[5];
    public:
        Premium(){
            setNombre("Premium");
        }
        void setNotas(float nota1, float nota2, float nota3, float nota4, float nota5){
            notas[0] = nota1;
            notas[1] = nota2;
            notas[2] = nota3;
            notas[3] = nota4;
            notas[4] = nota5;
        }
        float getPromedio(){
            float suma = 0;
            for(auto nota : notas){
                suma += nota;
            }
            float promedio = suma / 5;
            cout << "El promedio es: " << promedio << endl;
            return promedio;
        }
        bool aprobo(){
            float promedio = getPromedio();
            if(promedio < 80){
                return false;
            }
            return true;
        }
};

class Alumno{
    private:
        string nombre;
        string apellido;
        int codigo;
        Plan* plan;
    public:
        Alumno(string nombre, string apellido, int codigo, Plan* plan){
            this->nombre = nombre;
            this->apellido = apellido;
            this->codigo = codigo;
            this->plan = plan;
        }
        void setPlan(Plan* plan){
            this->plan = plan;
        }
        string getNombre(){
            return nombre;
        }
        bool aprobo(){
            return plan->aprobo();
        }
        float getPromedio(){
            return plan->getPromedio();
        }
        string getTipo(){
            return plan->getTipo();
        }
        
        
};

class Gestor{
    vector<Alumno*> alumnos;
    public:
        void agregarAlumno(Alumno* alumno){
            alumnos.push_back(alumno);
        }
        void listarAlumnosAprobados(){
            for(auto alumno : alumnos){
                if(alumno->aprobo()){
                    cout << "Alumno: " << alumno->getNombre() << endl;
                }
            }
        }
        void cambiarPlan(Alumno* alumno, string nuevoPlan){
            if (nuevoPlan == "Medio"){
                Medio* medio = new Medio();
                alumno->setPlan(medio);
            }
            else if (nuevoPlan == "Premium"){
                Premium* premium = new Premium();
                alumno->setPlan(premium);
            }
            else if (nuevoPlan == "Invitado"){
                Invitado* invitado = new Invitado();
                alumno->setPlan(invitado);
            }
        }
        void listarPromedioMasAlto(){
            float maxPromedio = 0;
            for(auto alumno : alumnos){
                float promedio = alumno->getPromedio();
                if(promedio > maxPromedio){
                    maxPromedio = promedio;
                }
            }
            cout << "El promedio más alto es: " << maxPromedio << endl;
        }
        void listarCantidadAlumnosPorTipo(){
            map<string, int> cantidadAlumnos;
            for(auto alumno : alumnos){
                string tipo = alumno->getTipo();
                cantidadAlumnos[tipo]++;
            }
            for(auto it : cantidadAlumnos){
                cout << "Tipo: " << it.first << ", Cantidad: " << it.second << endl;
            }
        }

        //ahora guardaremos en archivos binarios los tipos de alumnos, en 3 archivos diferentes
        void guardarAlumnos(){
            ofstream archivoInvitados("invitados.bin", ios::binary);
            ofstream archivoMedios("medios.bin", ios::binary);
            ofstream archivoPremiums("premium.bin", ios::binary);

            for(auto alumno : alumnos){
                if (alumno->getTipo() == "Invitado"){
                    archivoInvitados.write((char*)alumno, sizeof(Invitado));
                }
                else if (alumno->getTipo() == "Medio"){
                    archivoMedios.write((char*)alumno, sizeof(Medio));
                }
                else if (alumno->getTipo() == "Premium"){
                    archivoPremiums.write((char*)alumno, sizeof(Premium));
                }
            }
            archivoInvitados.close();
            archivoMedios.close();
            archivoPremiums.close();
        }

        
};

/*La empresa lo vuelve a contratar ya que piensa dictar varios cursos. 
Realice un diseño que permita trabajar con varios cursos e indique los archivos 
que usaría para guardar la información. */
//Podríamos crear una clase Curso que contenga un vector de alumnos y el nombre del curso,
//y cada curso tendrá su propio archivo binario.
class Curso{
    private:
        string nombre;
        vector<Alumno*> alumnos;
    public:
        Curso(string nombre){
            this->nombre = nombre;
        }
        void agregarAlumno(Alumno* alumno){
            alumnos.push_back(alumno);
        }
        string getNombre(){
            return nombre;
        }
        vector<Alumno*> getAlumnos(){
            return alumnos;
        }
        void guardarAlumnos(){
            ofstream archivo(nombre + ".bin", ios::binary);
            for(auto alumno : alumnos){
                archivo.write((char*)alumno, sizeof(Alumno));
            }
            archivo.close();
        }
};