#include "Red.h"

enrutador::enrutador(string nombre)
{
    this->nombre = nombre;
}

string enrutador::getNombre()
{
    return nombre;
}

map<string, int> enrutador::getEnlaces()
{
    return enlaces;
}

map<string, enrutador::ruta> enrutador::getTablaDeEnrutamiento()
{
    return tablaDeEnrutamiento;
}
// Busca el enrutador con el nombre dado en el vector de enrutadores
int enrutador::buscarEnrutador(vector<enrutador>& enrutadores, string nombre)
{
    for (int i = 0; i < enrutadores.size(); i++)
    {
        if (enrutadores[i].getNombre() == nombre)
        {
            return i;
        }
    }
    return -1;
}

// Agrega un enlace directo al enrutador
void enrutador::agregarEnlace(string nombre, int costo)
{
    enlaces.insert(pair<string, int>(nombre, costo));
}
// Guarda las rutas asociadas a los enlaces directos del enrutador en la tabla de enrutamiento
void enrutador::inicializarTablaDeEnrutamiento()
{
    string destino;
    ruta rutaDestino;
    for (auto enlace : enlaces)
    {
        destino = enlace.first;
        rutaDestino.coste = enlace.second;
        rutaDestino.direccion = nombre + '-' + destino; // Direccion que se utiliza para llegar desde el enrutador origen al enrutador destino
        tablaDeEnrutamiento[destino] = rutaDestino;     // Agregamos la ruta a la tabla de enrutamiento
    }
}

// Calcula las mejores rutas para acceder a cada enrutador y las guarda en la tabla de enrutamiento
void enrutador::actualizarTablaDeEnrutamiento(vector<enrutador>& enrutadores)
{

    string nombreEnrutador{};           // Aqui guardaremos el nombre del enrutador que estemos revisando
    map<string, ruta> tablaEnrutador; // Aqui guardaremos las rutas que contiene el enrutador
    string destino{};                   // Aqui se almacena el nombre del enrutador destino
    int costoTotal{};                   // Aqui se almacena el costo total de la ruta
    ruta rutaEnrutador{};               // Aqui almacenaremos la direccion del enrutador para llegar a algun nodo
    // Recorro el vector con todos los enrutadores para revisar todas las conexiones y calcular las mejores rutas
    for (int i = 0; i < enrutadores.size(); i++)
    {
        nombreEnrutador = enrutadores[i].getNombre();             // Guardamos el nombre del enrutador
        tablaEnrutador = enrutadores[i].getTablaDeEnrutamiento(); // Obtenemos la tabla del enrutador

        if (nombreEnrutador != nombre && tablaDeEnrutamiento.find(nombreEnrutador) != tablaDeEnrutamiento.end())
        {
            actualizarRuta(costoTotal, destino, nombreEnrutador, tablaEnrutador, rutaEnrutador);
        }
        else
        {
            if (nombreEnrutador != nombre)
            {
                int posicion;
                tablaDeEnrutamiento.insert(pair<string, enrutador::ruta>(nombreEnrutador, rutaEnrutador));
                // Buscamos en las conexiones del enrutador actual para encontrar una ruta por medio de otro enrutador conocido
                for (auto conexion : enrutadores[i].getTablaDeEnrutamiento())
                { // Recorro los enlaces directos del enrutador
                    if (tablaDeEnrutamiento.find(conexion.first) != tablaDeEnrutamiento.end())
                    {
                        posicion = buscarEnrutador(enrutadores, conexion.first);
                        rutaEnrutador = enrutadores[posicion].getTablaDeEnrutamiento()[nombreEnrutador];
                        if (posicion != -1)
                        {
                            costoTotal = conexion.second.coste + tablaDeEnrutamiento[enrutadores[posicion].getNombre()].coste;
                            tablaDeEnrutamiento[nombreEnrutador].coste = costoTotal;
                            tablaDeEnrutamiento[nombreEnrutador].direccion = tablaDeEnrutamiento[enrutadores[posicion].getNombre()].direccion + '-' + nombreEnrutador;
                            //cout << nombre + '-' + conexion.second.direccion << endl;
                            break;
                        }
                    }
                }
                actualizarRuta(costoTotal, destino, nombreEnrutador, tablaEnrutador, rutaEnrutador);
            }
        }
    }
}

void enrutador::mostrarCostos()
{
    cout << "Costos del enrutador " << nombre << endl;
    for (auto enlace : tablaDeEnrutamiento)
    {
        cout << "Destino "
            << enlace.first << " Costo: " << enlace.second.coste << endl;
    }
    cout << endl;
}

void enrutador::mostrarCaminos()
{
    cout << "Caminos del enrutador " << nombre << endl;
    for (auto enlace : tablaDeEnrutamiento)
    {
        cout << "Destino "
            << enlace.first << " Ruta: " << enlace.second.direccion << endl;
    }
    cout << endl;
}
// Calcula la ruta mas corta para llegar a un enrutador destino
// y la guarda en la tabla de enrutamiento
void enrutador::actualizarRuta(int costoTotal, string destino, string nombreEnrutador, map<string, enrutador::ruta> tablaEnrutador, ruta rutaEnrutador)
{
    for (auto conexion : tablaEnrutador)
    {
        destino = conexion.first;
        rutaEnrutador = conexion.second;
        if (tablaDeEnrutamiento.find(destino) != tablaDeEnrutamiento.end() && destino != nombre)
        {
            costoTotal = rutaEnrutador.coste + tablaDeEnrutamiento[nombreEnrutador].coste;
            if (tablaDeEnrutamiento[destino].coste > costoTotal || tablaDeEnrutamiento[destino].coste == 0)
            { // Si el costo total es menor guardamos la nueva ruta con menor costo en la tabla del enrutador origen
                tablaDeEnrutamiento[destino].coste = costoTotal;
                tablaDeEnrutamiento[destino].direccion = nombre + '-' + rutaEnrutador.direccion;
            }
        }
        else if (destino != nombre)
        { // Si pasa a esta condicion significa que el enrutador destino no se encuentra en la tabla de enrutamiento del origen
            // entonces agregamos la ruta del enrutador intermedio
            tablaDeEnrutamiento.insert(pair<string, enrutador::ruta>(destino, rutaEnrutador)); // agregamos una ruta hacia el enrutador destino
            costoTotal = rutaEnrutador.coste + tablaDeEnrutamiento[nombreEnrutador].coste;
            tablaDeEnrutamiento[destino].coste = costoTotal;
            tablaDeEnrutamiento[destino].direccion = tablaDeEnrutamiento[nombreEnrutador].direccion + '-' + destino;
        }
    }
}

void enrutador::caminoMasCorto(string destino) {
    if (tablaDeEnrutamiento.find(destino) != tablaDeEnrutamiento.end()) {
        cout << "El camino mas corto para llegar al enrutador " << destino << " comenzando desde " << nombre << " es: " << tablaDeEnrutamiento[destino].direccion << endl;
    }
    else {
        cout << "No existe un camino hacia el enrutador " << destino << endl;
    }
}

void enrutador::eliminarEnlace(string nombre) {
	enlaces.erase(nombre);
    tablaDeEnrutamiento.clear();
    inicializarTablaDeEnrutamiento();
}

enrutador::~enrutador()
{
}







red::red()
{
}

void red::menu()
{
    int opcion;
    do
    {
        cout << "1. Crear red manualmente" << endl;
        cout << "2. Crear red aleatorea" << endl;
        cout << "3. Cargar red desde archivo" << endl;
        cout << "4. Agregar enrutador" << endl;
        cout << "5. Eliminar enrutador" << endl;
        cout << "6. Actualizar direcciones de la red" << endl;
        cout << "7. Mostrar red" << endl;
        cout << "8. Encontrar camino mas corto" << endl;
        cout << "9. Guardar red" << endl;
        cout << "10. Salir" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();
        switch (opcion)
        {
        case 1:
            CrearRedManual();
            break;
        case 2:
            CrearRedAleatoria();
            break;
        case 3:
            CargarRed();
            break;
        case 4:
            agregarEnrutador();
            break;
        case 5:
            eliminarEnrutador();
            break;
        case 6:
            actualizarTabla();
            break;
        case 7:
            mostrarRed();
            break;
        case 8:
            caminoMasCorto();
            break;
		case 9:
			guardarRed();
			break;
        case 10:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion invalida" << endl;
            break;
        }
    } while (opcion != 10);
}
void red::CrearRedManual()
{
    enrutadores.clear(); //Si se habia creado una red anteriormente se eliminan todos los enrutadores para crear una nueva red
    int cantidad = 0;
    vector<string> nombres;
    string nombre{};
    do
    {
        cout << "\nIngrese el numero de enrutadores de la red: ";
        cin >> cantidad;
        if (cantidad < 2)
        {
            cout << "La red necesita minimo 2 enrutadores para ser creada\n\n";
        }
    } while (cantidad < 2);
    cin.ignore();

    for (int i = 1; i <= cantidad; i++)
    {
        cout << "\nIngrese el nombre del enrutador numero " << i << " :";
        cin >> nombre;
        enrutador router(nombre);
        enrutadores.push_back(router);
        nombres.push_back(nombre);
    }
    cin.clear();
    cin.ignore();

    cout << "\n\nA continuacion ingrese las conexiones directas que tendra cada enrutador, luego el nombre del enrutador destino y el costo de envio\n";
    for (auto& enrutador : enrutadores)
    {
        cout << "\nEnrutador " << enrutador.getNombre() << endl;
        agregarEnlaces(nombres, enrutador);         // Agregamos los enlaces al enrutador
        enrutador.inicializarTablaDeEnrutamiento(); // Inicializamos la tabla de enrutamiento del enrutador
    }
    cout << "Red creada exitosamente\n";
}

void red::CargarRed()
{
    enrutadores.clear(); //Si se habia creado una red anteriormente se eliminan todos los enrutadores para crear una nueva red
    ifstream archivo;
    string nombreArchivo;
    string nombreEnrutador;
    string nombreEnrutadorDestino;
    int costo;
    int cantidadEnrutadores;
    int cantidadEnlaces;
    vector<string> nombres;
    cout << "Ingrese el nombre del archivo: ";
    cin >> nombreArchivo;
    archivo.open(nombreArchivo);
    if (archivo.is_open())
    {
        cout << "Cargando red...\n\n";
        archivo >> cantidadEnrutadores;
        for (int i = 0; i < cantidadEnrutadores; i++)
        {
            archivo >> nombreEnrutador;
            enrutador router(nombreEnrutador);
            enrutadores.push_back(router);
            nombres.push_back(nombreEnrutador);
        }
        for (auto& enrutador : enrutadores)
        {
            archivo >> cantidadEnlaces;
            for (int i = 0; i < cantidadEnlaces; i++)
            {
                archivo >> nombreEnrutadorDestino;
                archivo >> costo;
                enrutador.agregarEnlace(nombreEnrutadorDestino, costo);
            }
            enrutador.inicializarTablaDeEnrutamiento();
        }
        cout << "Red cargada exitosamente\n";
    }
    else
    {
        cout << "No se pudo abrir el archivo\n";
    }
    archivo.close();
}

void red::agregarEnrutador()
{   
    if (enrutadores.size() == 0)
    {
        cout << "No se ha encontrado ninguna red\n";
        cout << "Para poder agregar enrutadores individualmente primero cree una red con minimo 2 conexiones\n\n";
        return;
    }
    string nombre;
    int cantidad;
    vector<string> nombres;
    bool existe = false;
    do
    {
        cout << "Ingrese el nombre del enrutador: ";
        cin >> nombre;
        for (auto& enrutador : enrutadores) {
            if (enrutador.getNombre() == nombre) {
                cout << "El enrutador ya existe\n";
                existe = true;
            }
        }
    } while (existe);

    enrutador router(nombre);
    enrutadores.push_back(router);
    for (auto& enrutador : enrutadores) {
        nombres.push_back(enrutador.getNombre());
    }
    
    cout << "A continuacion ingrese los enrutadores a los que se conectara el nuevo enrutador\n";
    agregarEnlaces(nombres, router); // Aqui agregamos los enlaces del nuevo enrutador
}
void red::guardarRed()
{
    
	if (enrutadores.size() == 0)
	{
		cout << "No se ha creado ninguna red\n";
		return;
	}
    ofstream archivo;
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo: ";
    cin >> nombreArchivo;
    archivo.open(nombreArchivo);
    if (archivo.is_open())
    {
        cout << "Guardando red...\n\n";
        archivo << enrutadores.size() << endl;
        for (auto& enrutador : enrutadores)
        {
            archivo << enrutador.getNombre() << endl;
        }
        for (auto& enrutador : enrutadores)
        {
            archivo << enrutador.getEnlaces().size() << endl;
            for (auto& enlace : enrutador.getEnlaces())
            {
                archivo << enlace.first << " " << enlace.second << endl;
            }
        }
        cout << "Red guardada exitosamente\n";
    }
    else
    {
        cout << "No se pudo guardar el archivo\n";
    }
    archivo.close();
}

void red::CrearRedAleatoria() {
	enrutadores.clear(); //Si se habia creado una red anteriormente se eliminan todos los enrutadores para crear una nueva red
    int cantidad = 0;
    map <string, int> enlaces;
    vector<enrutador> disponibles;
    do
    {
        cout << "\nIngrese el numero de enrutadores para la creacion de la red aleatoria: ";
        cin >> cantidad;
        if (cantidad < 2)
        {
            cout << "La red debe tener al menos 2 enrutadores\n";
        }
    } while (cantidad < 2);
    cin.ignore(); // Limpiamos el buffer
    cout << "Creando red...\n\n";
    for (int i = 1; i <= cantidad; i++)
    {
        enrutador router("R" + to_string(i)); // Creamos un enrutador con el nombre R1, R2, R3, etc
        enrutadores.push_back(router);
    }
    for (auto& enrutador : enrutadores)
    {
        enlaces = enrutador.getEnlaces(); // Obtenemos los enlaces del enrutador
        // En este vector guardamos los nombres de los enrutadores que no tienen enlaces con el enrutador actual
        // Y luego escogemos entre ellos aleatoriamente
        for (auto& enrutador2 : enrutadores) {
            if (enrutador2.getNombre() != enrutador.getNombre() && enlaces.find(enrutador2.getNombre()) == enlaces.end()) {
                disponibles.push_back(enrutador2);
            }
        }
        if (disponibles.size() == 0)
            continue;// Si no hay enrutadores disponibles para agregar, pasamos al siguiente enrutador
		
        // Generamos un numero aleatoreo entre 1 y la cantidad de enrutadores disponibles para que se genere minimo un enlace
        int cantidadEnlaces = rand() % disponibles.size() + 1;
        for (int i = 0; i < cantidadEnlaces; i++) // En este ciclo se generan las conexiones aleatorias entre los enrutadores
        {
            int posicion = rand() % disponibles.size(); // Generamos un numero aleatorio entre 0 y el tamaño del vector disponibles
            int costo = rand() % 10 + 1; // Costo entre 1 y 10
            enrutador.agregarEnlace(disponibles[posicion].getNombre(), costo); // Agregamos el enlace al enrutador origen
            disponibles[posicion].agregarEnlace(enrutador.getNombre(), costo); // Agregamos el enlace al enrutador destino
            disponibles.erase(disponibles.begin() + posicion); // Eliminamos el enrutador destino del vector disponibles

        }
        enrutador.inicializarTablaDeEnrutamiento(); // Inicializamos la tabla de enrutamiento del enrutador
        disponibles.clear(); // Limpiamos el vector disponibles para usarlo con el siguiente enrutador
    }
	cout << "Red creada exitosamente\n\n";
}

void red::agregarEnlaces(vector<string>& nombres, enrutador& enrutador)
{
    enlace enlace;                                      // Objeto tipo enlace para guardar el nombre y el costo de la conexion
    map<string, int> conexion = enrutador.getEnlaces(); // Obtenemos los enlaces asociados al enrutador
    bool valido = 0;
    int contador = 1;
    while (true)
    {
        conexion = enrutador.getEnlaces();
        int espacioLibre = nombres.size() - conexion.size() - 1; // Calculamos el espacio libre que tiene el enrutador para agregar enlaces
        if (espacioLibre == 0)
        {
            cout << "El enrutador " << enrutador.getNombre() << " ya tiene todas las conexiones posibles\n";
            break;
        }
        cout << "Conexiones posibles restantes para el enrutador " << enrutador.getNombre() << ": " << espacioLibre << endl;
        cout << " 1. Agregar conexion\n 2. Continuar\n";
        int opcion;
        cin >> opcion;
        if (opcion == 1)
        {
            do
            { // Agregamos el nombre del enrutador que estara enlazado

                cout << "\nIngrese el nombre del enrutador para crear el enlace: ";
                cin >> enlace.enrutador;

                for (auto nombre : nombres) // Nos aseguramos de que el nombre ingresado si sea valido
                    if (enlace.enrutador == nombre && enlace.enrutador != enrutador.getNombre())
                    {
                        valido = 1;
                        break;
                    }
                    else if (enlace.enrutador == enrutador.getNombre())
                    {
                        cout << "El enrutador no puede estar conectado a si mismo\n";
                        break;
                    }

                if (nombres.size() == conexion.size() + 1) // Verificamos que se puedan agregar mas conexiones
                {
                    cout << "El enrutador ya esta conectado a todos los enrutadores\n";
                    return;
                }
                else if (conexion.find(enlace.enrutador) != conexion.end())
                {
                    cout << "Estos dos enrutadores ya tienen un enlace, por favor ingrese otro\n";
                    valido = 0;
                }
            } while (!valido); // Mientras no se ingrese un nombre registrado seguira pidiendo el ingreso del nombre
            valido = 0;
            do
            {
                cout << "\nIngrese el costo de la conexion (Debe ser mayor que 0): ";
                cin >> enlace.costo;
            } while (enlace.costo <= 0); // Mientras el costo sea menor o igual a 0 seguira pidiendo el ingreso del costo

            enrutador.agregarEnlace(enlace.enrutador, enlace.costo); // Se agrega el enlace directo al enrutador origen
            // Se agrega el enlace directo al enrutador destino tambien
            enrutadores[enrutador.buscarEnrutador(enrutadores, enlace.enrutador)].agregarEnlace(enrutador.getNombre(), enlace.costo);
            cout << "Enlace numero " << contador << " agregado\n";
            cout << "Se ha enlazado el enrutador " << enrutador.getNombre() << " con el enrutador " << enlace.enrutador << endl;
            cin.ignore();
            cin.clear();
            contador++;
        }
        else if (opcion == 2)
        {
            if (conexion.size()==0)
                cout << "El enrutador debe tener al menos una conexion\n";
            else
                break;
        }
        else
            cout << "Opcion invalida\n";
    }
}

void red::actualizarTabla()
{   
    cout << "Actualizando red...\n";
    if (enrutadores.size() == 0)
    {
        cout << "No se ha encontrado ninguna red\n\n";
        return;
    }
    for (auto& enrutador : enrutadores)
        enrutador.inicializarTablaDeEnrutamiento(); // Reestablecemos los valores de la tabla de enrutamiento

    for (auto& enrutador : enrutadores)
        enrutador.actualizarTablaDeEnrutamiento(enrutadores); // Actualizamos las rutas de cada enrutador

    for (auto& enrutador : enrutadores)
        enrutador.actualizarTablaDeEnrutamiento(enrutadores); // Actualizamos las rutas de cada enrutador

    cout << "Rutas actualizadas con exito\n";
}
void red::mostrarRed()
{
    if (enrutadores.size()==0)
    {
        cout << "No se ha encontrado ninguna red\n\n";
        return;
    }
    for (auto enrutador : enrutadores)
    {
        enrutador.mostrarCostos();
    }
}
void red::caminoMasCorto() {
    if (enrutadores.size() == 0)
    {
        cout << "No se ha encontrado ninguna red\n\n";
        return;
    }
    string origen, destino;
    cout << "Ingrese el nombre del enrutador origen: ";
    cin >> origen;
    cout << "Ingrese el nombre del enrutador destino: ";
    cin >> destino;
    enrutadores[enrutadores[0].buscarEnrutador(enrutadores, origen)].caminoMasCorto(destino);
}

void red::eliminarEnrutador() {
    if (enrutadores.size() == 0)
    {
        cout << "\nNo hay ningun enrutador para eliminar\n";
        return;
    }
    else if (enrutadores.size() == 2) {
        cout << "\nLa red solo tiene 2 enrutadores, no puede eliminar mas\n";
    }
	string nombre;
	cout << "Ingrese el nombre del enrutador que desea eliminar: ";
	cin >> nombre;
	int posicion = enrutadores[0].buscarEnrutador(enrutadores, nombre);
	if (posicion == -1) {
		cout << "No se encontro el enrutador\n";
		return;
	}
	enrutadores.erase(enrutadores.begin() + posicion);
	for (auto& enrutador : enrutadores) {
		enrutador.eliminarEnlace(nombre);
	}
	cout << "Enrutador eliminado exitosamente\n";
}

red::~red()
{
}