#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

// Clase para representar un enrutador
class enrutador
{
private:
    struct ruta
    {
        int coste{};
        string direccion{};
    };

    string nombre;                         // Nombre del enrutador
    map<string, int> enlaces;              // Enlaces directos del enrutador
    map<string, ruta> tablaDeEnrutamiento; // Tabla con la informacion de las mejores rutas y sus direcciones

public:
    enrutador(string nombre);
    string getNombre();
    map<string, int> getEnlaces();
    map<string, ruta> getTablaDeEnrutamiento();
    int buscarEnrutador(vector<enrutador>& enrutadores, string nombre);

    void agregarEnlace(string nombre, int costo);
    void actualizarTablaDeEnrutamiento(vector<enrutador>& enrutadores);
    void inicializarTablaDeEnrutamiento();
    void mostrarCostos();
    void mostrarCaminos();
    void actualizarRuta(int costoTotal, string destino, string nombreEnrutador, map<string, enrutador::ruta> tablaEnrutador, ruta rutaEnrutador);
    void caminoMasCorto(string destino);
	void eliminarEnlace(string nombre);
    ~enrutador();
};

class red
{
private:
    vector<enrutador> enrutadores;
    struct enlace // Estructura para introducir una conexion que tenga el enrutador que estamos agregando
    {
        string enrutador;
        int costo;
    };

public:
    red();
    void menu();
    void CrearRedManual();
    void CargarRed();
    void guardarRed();
    void CrearRedAleatoria();
    void agregarEnrutador();
    void eliminarEnrutador();
    void agregarEnlaces(vector<string>& nombres, enrutador&);
    void actualizarTabla();
    void mostrarRed();
    void caminoMasCorto();
    ~red();
};