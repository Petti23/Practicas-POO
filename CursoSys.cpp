#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

class Plan {
protected:
    string nombre;
public:
    virtual ~Plan() = default;
    virtual float getPromedio() = 0;
    virtual bool aprobo() = 0;
    virtual string getTipo() const {
        return nombre;
    }
};

class Invitado : public Plan {
private:
    float nota;
public:
    Invitado() {
        nombre = "Invitado";
        nota = 0;
    }

    bool aprobo() override {
        return nota > 60;
    }

    float getPromedio() override {
        return nota;
    }

    void setNota(float n) {
        nota = n;
    }

    float getNota() const {
        return nota;
    }
};

class Medio : public Plan {
private:
    float notas[3];
public:
    Medio() {
        nombre = "Medio";
        for (int i = 0; i < 3; i++) notas[i] = 0;
    }

    void setNotas(float n1, float n2, float n3) {
        notas[0] = n1;
        notas[1] = n2;
        notas[2] = n3;
    }

    float getPromedio() override {
        float suma = 0;
        for (int i = 0; i < 3; i++) suma += notas[i];
        return suma / 3;
    }

    bool aprobo() override {
        return getPromedio() > 70;
    }

    const float* getNotas() const {
        return notas;
    }
};

class Premium : public Plan {
private:
    float notas[5];
public:
    Premium() {
        nombre = "Premium";
        for (int i = 0; i < 5; i++) notas[i] = 0;
    }

    void setNotas(float n1, float n2, float n3, float n4, float n5) {
        notas[0] = n1;
        notas[1] = n2;
        notas[2] = n3;
        notas[3] = n4;
        notas[4] = n5;
    }

    float getPromedio() override {
        float suma = 0;
        for (int i = 0; i < 5; i++) suma += notas[i];
        return suma / 5;
    }

    bool aprobo() override {
        return getPromedio() > 80;
    }

    const float* getNotas() const {
        return notas;
    }
};

class Alumno {
private:
    string nombre, apellido;
    int codigo;
    Plan* plan;

public:
    Alumno(string nom, string ape, int cod, Plan* pl)
        : nombre(nom), apellido(ape), codigo(cod), plan(pl) {}

    ~Alumno() {
        delete plan;
    }

    Plan* getPlan() {
        return plan;
    }

    void setPlan(Plan* pl) {
        delete plan;
        plan = pl;
    }

    string getNombre() const {
        return nombre;
    }

    string getApellido() const {
        return apellido;
    }

    int getCodigo() const {
        return codigo;
    }

    float getPromedio() {
        return plan->getPromedio();
    }

    bool aprobo() {
        return plan->aprobo();
    }

    string getTipo() {
        return plan->getTipo();
    }
};

class Gestor {
private:
    vector<Alumno*> alumnos;

public:
    ~Gestor() {
        for (auto a : alumnos) delete a;
    }

    void agregarAlumno(Alumno* alumno) {
        alumnos.push_back(alumno);
    }

    void listarAlumnosAprobados() {
        cout << "Alumnos que aprobaron:\n";
        for (auto a : alumnos) {
            if (a->aprobo()) {
                cout << a->getNombre() << " " << a->getApellido() << endl;
            }
        }
    }

    void cambiarPlan(Alumno* alumno, string nuevoPlan) {
        if (nuevoPlan == "Medio") {
            alumno->setPlan(new Medio());
        } else if (nuevoPlan == "Premium") {
            alumno->setPlan(new Premium());
        } else {
            alumno->setPlan(new Invitado());
        }
    }

    void listarPromedioMasAlto() {
        float maxPromedio = 0;
        vector<Alumno*> mejores;
        for (auto a : alumnos) {
            float prom = a->getPromedio();
            if (prom > maxPromedio) {
                maxPromedio = prom;
                mejores.clear();
                mejores.push_back(a);
            } else if (prom == maxPromedio) {
                mejores.push_back(a);
            }
        }
        cout << "Mejor promedio: " << maxPromedio << "\nAlumnos:\n";
        for (auto a : mejores) {
            cout << a->getNombre() << " " << a->getApellido() << endl;
        }
    }

    void listarCantidadAlumnosPorTipo() {
        map<string, int> mapa;
        for (auto a : alumnos) {
            mapa[a->getTipo()]++;
        }
        cout << "Cantidad por tipo:\n";
        for (auto& p : mapa) {
            cout << p.first << ": " << p.second << endl;
        }
    }

    void guardarAlumnos() {
        ofstream outInv("invitados.bin", ios::binary);
        ofstream outMed("medios.bin", ios::binary);
        ofstream outPre("premium.bin", ios::binary);

        for (auto a : alumnos) {
            string tipo = a->getTipo();
            int codigo = a->getCodigo();
            string nombre = a->getNombre();
            string apellido = a->getApellido();
            if (tipo == "Invitado") {
                float nota = ((Invitado*)a->getPlan())->getNota();
                outInv.write((char*)&codigo, sizeof(int));
                outInv.write((char*)&nota, sizeof(float));
            } else if (tipo == "Medio") {
                const float* notas = ((Medio*)a->getPlan())->getNotas();
                outMed.write((char*)&codigo, sizeof(int));
                outMed.write((char*)notas, 3 * sizeof(float));
            } else if (tipo == "Premium") {
                const float* notas = ((Premium*)a->getPlan())->getNotas();
                outPre.write((char*)&codigo, sizeof(int));
                outPre.write((char*)notas, 5 * sizeof(float));
            }
        }

        outInv.close();
        outMed.close();
        outPre.close();
    }
};

int main() {
    Gestor gestor;

    Alumno* a1 = new Alumno("Juan", "Perez", 1, new Invitado());
    ((Invitado*)a1->getPlan())->setNota(75);

    Alumno* a2 = new Alumno("Maria", "Lopez", 2, new Medio());
    ((Medio*)a2->getPlan())->setNotas(85, 90, 75);

    Alumno* a3 = new Alumno("Pedro", "Gomez", 3, new Premium());
    ((Premium*)a3->getPlan())->setNotas(90, 85, 88, 92, 84);

    gestor.agregarAlumno(a1);
    gestor.agregarAlumno(a2);
    gestor.agregarAlumno(a3);

    gestor.listarAlumnosAprobados();
    gestor.cambiarPlan(a1, "Medio");
    gestor.listarPromedioMasAlto();
    gestor.listarCantidadAlumnosPorTipo();
    gestor.guardarAlumnos();

    return 0;
}
