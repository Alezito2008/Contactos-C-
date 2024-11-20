#include <iostream>
#include <fstream>
#include <cstring>
#define ARCHIVO "contactos.dat"

using namespace std;

struct Contacto {
    char nombre[20];
    char apellido[20];
    unsigned int edad;
    unsigned int numero;
    char email[20];
};

void agregarContacto(Contacto* contacto);
void mostrarContacto(Contacto* contacto);
int mostrarMenu();
Contacto* leerContacto();
Contacto* obtenerContactos();
bool eliminarContacto(char nombre[]);
bool actualizarEdad(char nombre[], int edad);

int main() {
    for (;;) {
        int opcion = mostrarMenu();
        switch (opcion) {
            case 1: {
                cin.ignore();
                Contacto* c = leerContacto();
                agregarContacto(c);
                break;
            }
            case 2: {
                ifstream file;
                file.open(ARCHIVO);
                Contacto c;
                while (file.read(reinterpret_cast<char*>(&c), sizeof(Contacto))) {
                    mostrarContacto(&c);
                }
                system("pause");
                break;
            }
            case 3: {
                char nombre[20];
                cout << endl << "Nombre: ";
                cin.ignore();
                cin.getline(nombre, sizeof(nombre));

                ifstream file;
                file.open(ARCHIVO);
                Contacto c;
                while (file.read(reinterpret_cast<char*>(&c), sizeof(Contacto))) {
                    if (!strcmp(nombre, c.nombre)) {
                        mostrarContacto(&c);
                    }
                }
                system("pause");
                break;
            }
            case 4: {
                char nombre[20];
                cout << endl << "Nombre: ";
                cin.ignore();
                cin.getline(nombre, sizeof(nombre));

                if (eliminarContacto(nombre)) {
                    cout << "Contacto eliminado!" << endl;
                } else {
                    cout << "No se elimino el contacto" << endl;
                }
                break;
            }
            case 5: {
                char nombre[20];
                cout << endl << "Nombre: ";
                cin.ignore();
                cin.getline(nombre, sizeof(nombre));

                int edad;
                cout << "Nueva edad: ";
                cin >> edad;

                if (actualizarEdad(nombre, edad)) {
                    cout << "Se actualizo la edad" << endl;
                    system("pause");
                } else {
                    cout << "Hubo un error al actualizar la edad" << endl;
                    system("pause");
                }
                break;
            }
            case 6: {
                return 0;
            }
        }
    }

    return 0;
}

int mostrarMenu() {
    int eleccion;
    do {
        cout
        << endl
        << "-- Menu --" << endl
        << "1. Agregar contacto" << endl
        << "2. Obtener contactos" << endl
        << "3. Buscar contacto" << endl
        << "4. Eliminar contacto" << endl
        << "5. Actualizar edad" << endl
        << "6. Salir" << endl;

        cin >> eleccion;
    } while (eleccion < 1 || eleccion > 6);
    return eleccion;
}

Contacto* leerContacto() {
    Contacto* c = new Contacto;
    cout << "-- CREAR CONTACTO --" << endl;
    cout << "Nombre: ";
    cin.getline(c->nombre, sizeof(c->nombre));
    cout << "Apellido: ";
    cin.getline(c->apellido, sizeof(c->apellido));
    cout << "Edad: ";
    cin >> c->edad;
    cout << "Numero: ";
    cin >> c->numero;
    cout << "Email: ";
    cin >> c->email;
    return c;
}

void agregarContacto(Contacto* c) {
    ofstream file;
    file.open(ARCHIVO, ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(c), sizeof(Contacto));
    file.close();
}

void mostrarContacto(Contacto* c) {
    cout
    << endl
    << "-- Contacto --" << endl
    << "Nombre: " << c->nombre << endl
    << "Apellido: " << c->apellido << endl
    << "Edad: " << c->edad << endl
    << "Numero: " << c->numero << endl
    << "Email: " << c->email << endl;
}

bool eliminarContacto(char nombre[]) {
    bool encontrado;

    ifstream contactos_file;
    ofstream temp_file;
    contactos_file.open(ARCHIVO, ios::binary);
    temp_file.open("temp.dat", ios::binary);

    if (!contactos_file.good() || !temp_file.good()) {
        return false;
    }

    Contacto* c = new Contacto;
    while (contactos_file.read(reinterpret_cast<char*>(c), sizeof(Contacto))) {
        if (strcmp(c->nombre, nombre)) {
            temp_file.write(reinterpret_cast<char*>(c), sizeof(Contacto));
        } else {
            encontrado = true;
        }
    }

    contactos_file.close();
    temp_file.close();
    
    if (encontrado) {
        remove(ARCHIVO);
        rename("temp.dat", ARCHIVO);
    } else {
        remove("temp.dat");
    }
    
    return encontrado;
}

bool actualizarEdad(char nombre[], int edad) {
    fstream file;
    file.open(ARCHIVO, ios::in | ios::out | ios::binary);
    Contacto* c = new Contacto;
    while (file.read(reinterpret_cast<char*>(c), sizeof(Contacto))) {
        if (!strcmp(nombre, c->nombre)) {
            file.seekp(static_cast<int>(file.tellg()) - sizeof(Contacto));
            c->edad = edad;
            file.write(reinterpret_cast<char*>(c), sizeof(Contacto));
            return true;
        }
    }

    return false;
}
