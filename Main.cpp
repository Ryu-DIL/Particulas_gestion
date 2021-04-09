/*********************************************************************************/
/*Nombre del programa: Particulas especiales									 */
/*Autor de la parte 1: Jeremías Daniel Ioannu Lapido                             */
/*Autor de la parte 2: David Baena Asensio			                             */
/*Autor de la parte 3: Óscar Rodriguez López			                         */
/*Autora de la parte 4: Irene Sánchez Carvajal                                   */
/*Fecha de entrega: 05/01/2020 													 */
/*********************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <locale.h>

#define TAM 500
#define TAMF 15
#define TAMG 15
#define T 9

using namespace std;

/*
	A lo largo de las anotaciones y en los nombres de las funciones y procedimientos se usarán términos como Particula, Nombre, Simbolo, Familia, Grupo, Pais, Carga, Masa y Año. 
	Esto no debe confundirnos, no se refiere a campos con ese contenido, sino a su espacio en la estructura Particula.
*/

struct Particula                                     //Estructura elemental, los nombres de las variables son esos aunque su contenido sea variable dependiendo del fichero en uso y posiblemente no coincida
{
	string Nombre, Simbolo, Familia, Grupo, Pais;
	float Carga, Masa;
	short Anyo;
};

void Menu(string [T], bool, string); 					//Switch para elegir qué hacer con los datos
void LeerIni(ifstream& , string[T] , bool&);       	    //Lectura del archivo de configuración
void LeerFamiliasYGrupos(ifstream &, string [TAMF], short &, string [TAMG], short &);	 //Lee las Familias y Grupos válidos del .txt
bool LeerParticulas(ifstream &, struct Particula [TAM], short &, string [TAMF], short, string [TAMG], short );	 //Lee las Particulas del archivo .txt
void CargarFichero(struct Particula [TAM], short &, string [TAMF], short &, string [TAMG], short &, string &, bool &);	//Lee un fichero y lo pasa a memoria

void CrearFichero(struct Particula particulas[TAM], short &tamp, string familias[TAMF], short &tamfam, string grupos[TAMG], short &tamgrup, bool &formato, string &nombre2, string &nombre);	//Crea un fichero de escritura en el que se guardarán datos
void PedirFamiliasYGrupos(string familias[TAMF], short &tamfam, string grupos[TAMG], short &tamgrup, string configuracion[T]);		//Pide las familias y grupos válidos
void PedirParticulas(struct Particula particulas[TAM], short &tamp, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, string nombre2, string configuracion[T]);		//Pide Particulas al usuario cumpliendo la pertenencia a Familia y Grupo válidos

void CambiarIni (string [T] , bool& , bool&);        //Opción 1
void GuardarIni (string [T] , string , bool&);       //Opción 2
void MostrarIni (string [T]);                        //Se usa en Opción 1 para saber cual es la configuración actual
void ComprobarGuardadoIni (bool& , string[T], string); 		//Se usa para recordar al usuario que guarde antes de salir
bool ComprobacionFormato (string[T], bool&);         //Si es true el campo se expresará con 3 decimales, si es false con ninguno, para diferenciar Notación científica de coma fija
bool ComprobacionCadena (short, string [T] , string); // Devuelve true si la cadena pertenece al string de cadenas (Para evitar la duplicidad de cadenas en la configuración y para evitar introducir una Familia o Grupo incorrectos al editar o crear una Particula o buscar por Familia o Familia y Grupo)

void ListarParticulas(struct Particula [TAM], short, bool, string [T]);  	//Muestra la información de todas las Particulas en memoria

void GuardarFichero(bool &, struct Particula [TAM], short, bool, string [TAMF], short, string [TAMG], short, string); //Guarda el fichero con el formato para Familias, Grupos y Particulas
void ComprobarGuardadoFichero(bool &, struct Particula [TAM], short, bool, string [TAMF], short, string [TAMG], short, string);
void GuardarFicha(ofstream &, struct Particula, bool); //Guarda cada Particula
void GuardarMasa(ofstream &, float, bool);             //Guarda las Masas de memoria a fichero teniendo en cuenta el tipo de Notación

void CrearParticula (struct Particula [TAM], short &, string [TAMF], short, string [TAMG], short, string configuracion[T], string); //Crea una nueva Particula
void EditarParticula (struct Particula [TAM], short, string [T], string, bool formato, string [TAMF], short, string [TAMG], short, bool &); //Edita la información de una Particula dada
void BorrarFicha(struct Particula [TAM], short &, string [T], string, bool &); //Borra una Particula dado su Simbolo

void Busquedas (string [T], string, struct Particula [TAM], short, string [TAMF], short, string [TAMG], short, bool);   //Switch para elegir que búsqueda hacer
void MostrarEncuentro (struct Particula, string [T], bool);                            //Muestra una particula, diferenciando Notación científica de coma fija y si se desconoce algún dato
void LeerMasa (float, bool);                                                           //Se usa en MostrarEncuentro para mostrar la Masa en sus dos Notaciones
void BuscarPorNombre (struct Particula [TAM], short, string, string [T], bool);        //Busca un Nombre exacto entre todas las Particulas de la memoria
void BuscarPorContener (struct Particula [TAM], short, string, string [T], bool);      //Busca todas las Particulas de la memoria que contengan una cadena dada en su Nombre
void BuscarMasaMenor (struct Particula [TAM], short, string, string [T], bool);        //Busca todas las Particulas de la memoria cuya Masa sea menor a un número dado
void BuscarPorFamilia (struct Particula [TAM] , short, string [TAMF], short, string, string [T], bool);                                 //Busca todas las Particulas de la memoria que pertenezcan a una Familia dada
void BuscarPorFamiliaYGrupo (struct Particula [TAM], short, string [TAMF], short, string [TAMG], short, string, string [T], bool);      //Busca todas las Particulas de la memoria que pertenezcan a una Familia y Grupo dados

bool PrimeraAlfabeticaTrue(string , string); 		 //Devuelve true si el primer string va antes alfabéticamente que el segundo (Pasa las mayusculas a minusculas e ignora los espacios)
void str_a_str_min(string &);		 //Pasa un string a minusculas
void MeterUnaOrdenada(struct Particula [TAM], short&);		//Dado un vector de Particulas con n elementos y uno en la posición n (Está en memoria, pero no marcado como existente), coloca la Particula en su posición teniendo en cuenta los criterios de orden alfabético por Familia, Grupo y Nombre, en ese orden (Aumentando en 1 el tamaño del vector, obviamente)

int main ()
{
	bool formato, abrir;
	string configuracion[T], nombre, nombre_ini, nombre2, nombre2_fich;
	ifstream fich;
	
	setlocale(LC_ALL, "spanish");  //Usaremos tildes
	
	do
	{
		abrir=true;
		cout << "Dame el nombre del fichero con las especificaciones (Sin extension de archivo): ";  	//Se pide el nombre hasta que exista el fichero
		cin >> nombre;
		nombre_ini=nombre+".ini";
		
		fich.open(nombre_ini.c_str());
		if (!fich)
		{
			cout << "Error abriendo el fichero." << endl;
			abrir=false;
		}
		else
		{
			LeerIni(fich , configuracion , formato);
			fich.close();
			Menu(configuracion, formato, nombre);
		}
	}while(abrir==false);
	cout << endl << endl << "Gracias por usar nuestro programa." << endl << endl;
	
	system("pause");
	return 0;
}

bool LeerParticulas(ifstream &fich, struct Particula particulas[TAM], short &tamp, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup)	
{
	short i=0, j;
	bool Leer = true;
	
	while ( (getline (fich , particulas[i].Nombre)) && (Leer))
	{
		fich >> particulas[i].Simbolo;
		fich.ignore();
		j=i-1;
		while((j>0) && (Leer))
		{
			if((particulas[i].Simbolo == particulas[j].Simbolo) && (particulas[i].Nombre == particulas[j].Nombre))
			{
				Leer = false;
			}
			j--;
			
		}
		fich >> particulas[i].Masa;
		fich.ignore();
		if((particulas[i].Masa < 0) && (particulas[i].Masa != -1))		//No puede ser negativo a no ser que sea -1 (Desconocida)
		{
			Leer = false;
		}
		fich >> particulas[i].Carga;
		fich.ignore();
		if((particulas[i].Carga > 1) || (particulas[i].Carga < -1))		//Tiene que estar entre -1 y 1
		{
			Leer = false;
		}
		fich >> particulas[i].Familia;
		fich.ignore();
		if(!ComprobacionCadena (tamfam, familias, particulas[i].Familia))
		{
			Leer = false;
		}
		fich >> particulas[i].Grupo;
		fich.ignore();
		if(!ComprobacionCadena (tamgrup, grupos, particulas[i].Grupo))
		{
			Leer = false;
		}
		getline(fich , particulas[i].Pais);
		fich >> particulas[i].Anyo;
		fich.ignore();
		if((particulas[i].Anyo < 0) && (particulas[i].Anyo != -1))		//No puede ser negativo a no ser que sea -1 (Desconocida)
		{
			Leer = false;
		}
		MeterUnaOrdenada(particulas, i);
	}
	tamp=i;
	
	return Leer;
}

void LeerFamiliasYGrupos(ifstream &fich, string familias[TAMF], short &tamfam, string grupos[TAMG], short &tamgrup)
{
	short i=0,j, tot;
	string frase;
	tamfam=0;
	tamgrup=0;
	
	fich >> frase;
	fich.ignore();
	tot = frase.length();
	while(-frase.find(';',i) != 1)
	{
		j = frase.find(';',i);
		familias[tamfam] = frase.substr(i,j-i);
		i = j+1;
		j=j+2;
		tamfam++;
	}
	familias[tamfam] = frase.substr(j-1,tot-j+1);
	tamfam++;
	i=0;
	
	fich >> frase;
	fich.ignore();
	tot = frase.length();
	
	while(-frase.find(';',i) != 1)
	{
		j = frase.find(';',i);
		grupos[tamgrup] = frase.substr(i,j-i);
		i = j+1;
		j=j+2;
		tamgrup++;
	}
	grupos[tamgrup] = frase.substr(j-1,tot-j+1);
	tamgrup++;
}

void CargarFichero(struct Particula particulas[TAM], short &tamp, string familias[TAMF], short &tamfam, string grupos[TAMG], short &tamgrup, string &nombre2, bool &abrir)	//Mucho Indentado
{
	ifstream fich;
	string nombre2_fich;
	bool Bien = true;
	
	do
	{			
		abrir = true;
		cout << "Dame el nombre del fichero con los datos que quieres trabajar (Sin extension de archivo): ";
		cin >> nombre2;
		nombre2_fich = nombre2 + ".txt";
		fich.open(nombre2_fich.c_str());
		if (!fich)
		{
			cout << "Error abriendo el fichero." << endl;
			abrir = false;
		}
		else
		{
			LeerFamiliasYGrupos(fich, familias, tamfam, grupos, tamgrup);
			Bien = LeerParticulas(fich, particulas, tamp, familias, tamfam, grupos, tamgrup);
			if(Bien)
			{
				cout << "Información cargada con éxito." << endl;
			}
			else
			{
				cout << "Hay un error en el fichero." << endl;
			}
			
		}
		fich.close();
	}while(!Bien);
}

void CrearFichero(struct Particula particulas[TAM], short &tamp, string familias[TAMF], short &tamfam, string grupos[TAMG], short &tamgrup, bool &formato, string &nombre2, string configuracion[T], string &nombre) //Else y mucho indentado
{
	ofstream fich;
	char opcion;
	string nombre2_fich;
	bool Guardado;

	cout << "Dame el nombre del fichero en el que quieres guardar datos(Sin extension de archivo): ";
	cin >> nombre2;
	nombre2_fich=nombre2 + ".txt";
	
	fich.open(nombre2_fich.c_str());
	if (!fich)
	{
		cout << "Error creando el fichero." << endl;
	}
	else
	{
		PedirFamiliasYGrupos(familias, tamfam, grupos, tamgrup, configuracion);
		PedirParticulas(particulas, tamp, familias, tamfam, grupos, tamgrup, nombre2, configuracion);
		do
		{			
			cout << "¿Quieres cambiar también la configuración?: ";
			cin >> opcion;
		}while(opcion != 'S' && opcion != 'N');	
		
		if(opcion == 'S')
		{
			CambiarIni (configuracion, Guardado, formato);
			GuardarIni (configuracion, nombre, Guardado);
		}
	}

	
}

void PedirFamiliasYGrupos(string familias[TAMF], short &tamfam, string grupos[TAMG], short &tamgrup, string configuracion[T]) //Mucho indentado
{
	short i;
	bool Seguir = true;
	
	cin.ignore();
	i = 0;
	while((i<TAMF) && (Seguir))
	{
   		cout<< "Introduce la nueva " << configuracion[5] << "(Pon un 0 para parar): ";
   	 	getline ( cin, familias[i] );
   	 	if(familias[i] == "0")
   	 	{
   	 		Seguir = false;
		}
		else
			i++;
	}
	tamfam = i;
	
	Seguir = true;
	i = 0;
	while((i<TAMG) && (Seguir))
	{
   		cout<< "Introduce el nuevo " << configuracion[6] << "(Pon un 0 para parar): ";
   	 	getline ( cin, grupos[i] );
   	 	if(grupos[i] == "0")
   	 	{
   	 		Seguir = false;
		}
		else
			i++;
	}
	tamgrup = i;
}

void PedirParticulas(struct Particula particulas[TAM], short &tamp, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, string nombre2, string configuracion[T])
{
	short i;
	char resp = 'S';
	
	i = 0;
	while((i<TAM) && (resp == 'S'))
	{
		CrearParticula (particulas, i, familias, tamfam, grupos, tamgrup, configuracion, nombre2);
		cout << endl << "¿Quieres seguir introducuiendo " << nombre2 << "?('S' para sí, 'N' para no): ";
		cin >> resp;
		i++;
	}
	tamp = i;
}

void LeerIni (ifstream &fich ,  string configuracion[T] , bool &formato )
{
	short i=0;
	
	while ( i < T)
	{
		getline(fich,configuracion[i] );  //0: Tipo de notacion. 1: Nombre. 2: Simbolo. 3: Masa. 4: Carga. 5: Familia. 6: Grupo. 7: Pais. 8: Año.
		i++;
	}
	if (i == T)
	{
		cout << "Se ha leído el fichero exitosamente." << endl;
		ComprobacionFormato(configuracion , formato);
	}
	else
	{
		cout << "Ha habido un error abriendo el fichero." << endl;
	}
}

void Menu(string configuracion[T], bool formato, string nombre) //Mucho indentado
{
	short resp, tamp, tamfam, tamgrup;
	bool Seguir = true, GuardadoIni = true, GuardadoFich = true, abrir;
	char opcion;
	struct Particula particulas[TAM];
	string familias[TAMF], grupos[TAMG], nombre2, nombre2_fich;

	do
	{
		CargarFichero(particulas, tamp, familias, tamfam, grupos, tamgrup, nombre2, abrir);  	//Se pide el nombre hasta que exista el fichero
	}while(!abrir);
	nombre2_fich=nombre2 + ".txt";
	
	do
	{
		cout << endl << endl;
		cout << "	MENU	";
		cout << endl;
		cout << "Se está usando el formato: " << configuracion[0] << endl;
		cout << "Elige una de estas opciones: " << endl;
		cout << "0) Salir " << endl;
		cout << "1) Modificar configuración" << endl;
		cout << "2) Guardar configuración " << nombre << endl;
		cout << "3) Cargar o crear un nuevo catalogo" << endl;
		cout << "4) Guardar catalogo " << nombre2_fich << endl;
		cout << "5) Crear nueva ficha de " << nombre2 << endl;
		cout << "6) Listar todas las fichas del catalogo" << endl;
		cout << "7) Modificar los datos de una determinada ficha" << endl;
		cout << "8) Borrar una ficha" << endl;
		cout << "9) Búsquedas" << endl;
		cin >> resp;
		
		switch(resp)
		{
			case 0:
				ComprobarGuardadoIni (GuardadoIni, configuracion, nombre);
				ComprobarGuardadoFichero(GuardadoFich, particulas, tamp, formato, familias, tamfam, grupos, tamgrup, nombre2_fich);
				Seguir = false;
				break;
				
			case 1:
				MostrarIni (configuracion);
				CambiarIni (configuracion, GuardadoIni, formato );
				break;
				
			case 2: 
				
				if(GuardadoIni)
				{
					cout << "La información ya está guardada." << endl;
				}
				else
				{
					GuardarIni (configuracion, nombre, GuardadoIni);
				}
				break;
				
			case 3:
				CargarFichero(particulas, tamp, familias, tamfam, grupos, tamgrup, nombre2, abrir);
				if(!abrir)
				{
					do
					{
						cout << "Ese fichero no existe, quieres crear uno nuevo ahora mismo?('S' para sí, 'N' para no): ";
						cin >> opcion;
					}while((opcion != 'S') && (opcion != 'N'));
					
					if(opcion == 'S')
					{
						CrearFichero(particulas, tamp, familias, tamfam, grupos, tamgrup, formato, nombre2, configuracion, nombre);
						nombre2_fich=nombre2 + ".txt";
					}
					else
					{
						cout << "De acuerdo, creación de fichero abortada." << endl;
					}
				}
				nombre2_fich = nombre2 + ".txt";
				GuardadoFich = false;
				break;
				
			case 4: 
				if(GuardadoFich)
				{
					cout << "La información ya está guardada." << endl;
				}
				else
				{
					GuardarFichero(GuardadoFich, particulas, tamp, formato, familias, tamfam, grupos, tamgrup, nombre2_fich);
				}
				break;
				
			case 5: 
				CrearParticula (particulas, tamp, familias, tamfam, grupos, tamgrup, configuracion, nombre2);
				MeterUnaOrdenada(particulas, tamp);
				GuardadoFich = false;
				break;
				
			case 6: 
				ListarParticulas(particulas, tamp, formato, configuracion);
				break;
				
			case 7: 
				EditarParticula (particulas, tamp, configuracion, nombre2, formato, familias, tamfam, grupos, tamgrup, GuardadoFich);
				break;
				
			case 8: 
				BorrarFicha(particulas, tamp, configuracion, nombre2, GuardadoFich);
				break;
				
			case 9: 
				Busquedas(configuracion, nombre2, particulas, tamp, familias, tamfam, grupos, tamgrup, formato);
				break;
			
			default: 
				cout << "Opción inválida.";
				break;
		}
	}while (Seguir == true);
}

void MostrarIni (string configuracion[T])
{
	short i=0;
	
	cout << endl << "Muy bien, este es tu menú de configuarción: " << endl << endl;
	
	while (i < T)
	{
		cout << configuracion[i] << endl;
		i++;
	}
	cout << endl;
}

void CambiarIni (string configuracion[T], bool &Guardado , bool &formato) //DEMASIADO indentado
{
	short i;
	bool ok ;
	char cent;
	bool band , cadena , encontrado; 
	string nuevafrase, frase;
	
	do
	{	
		encontrado=false , cadena=false , band=false;							//Se inicializan los bool dentro del bucle para evitar que la anterior edició afecte a esta
		
		cout << "Recuerde que sólo hay dos formatos válidos : Notacion cientifica / Notacion coma fija." << endl,
		cin.ignore();
		cout << "¿Qué  quieres cambiar del menu configuración?: " << endl;
		getline (cin , frase);
		cout << "Escribe la nueva frase: " << endl;
		getline (cin , nuevafrase);
		
		cadena = ComprobacionCadena (T, configuracion , nuevafrase );		//Comprueba si ya hay algún campo llamado así para evitar repeticiones

		if (cadena == true)												
		{
			cout << "Campo ya existente en la configuracion" << endl;		//Si ya existe no se modifica
		}	
		else
		{
			while ( (i < T) && (encontrado !=true))
			{	
				if (configuracion[i]==frase)
				{
					configuracion[i]=nuevafrase;
					encontrado=true;										//Si se encuentra el elemento a modificar se detiene la búsqueda
					ok = ComprobacionFormato(configuracion, formato);
					if(ok)
					{
						band=true;											//Se ha modificado correctamente
					}
					else
					{
						configuracion[i]=frase;
					
						cout << "Su cambio de formato no es válido, pruebe otra vez." << endl;
					}
				}
				
				i++;
			}
		}
		
		if (band == true)
		{
			
			cout << "Se ha cambiado la configuración exitosamente." << endl;
			cout << "Recuerde guardar antes de cerrar el programa." << endl;
			Guardado= false;
		
		}
		else
		{
			cout << "No se ha podido cambiar la configuración, prueba otra vez." << endl;
		}
		
		cout << "Pulsa 'A' si quieres  salir y otro caracter si no." << endl;
		cin >> cent;
			
	}while ((cent != 'A'));
}

bool ComprobacionCadena (short tam, string vector[TAMF], string cadena)		//Esta funcion se usa tanto para la configuración como para las Familias y Grupos, por tanto se usa como tamaño del vector el máximo entre los tres, que es común a Familias y Grupos (TAMF y TAMG es 15 mientras que T es 9)
{
	short i;
	bool encuentro = false;
	
	for (i=0 ; i < tam ; i++)
	{
		if (vector[i]== cadena)
		{
			encuentro=true;
		}
	}
	
	return encuentro;
}

void GuardarIni (string configuracion[T], string nombre , bool &Guardado) //DEMASIADO indentado
{
	short i;
	ofstream fich;
	string nombrenuevo;
	char resp;
	
	if (Guardado== true)
	{
		cout << "Aun no has modificado el fichero de configuracion" << endl;
	}	
	else
	{
		do	
		{
			cout << "Quieres guardar la nueva configuarcion con el mismo nombre? S si , N no" << endl;
			cin >> resp;
		}while (resp != 'S' && resp != 'N');
	
		if (resp == 'S')
		{
			nombre = nombre + ".ini";
			fich.open(nombre.c_str());
			if (!fich)
			{
				cout << "Error guardando el fichero" << endl;
			}
			else
			{
				fich << configuracion[0];
				for (i=1 ; i < T ; i++)
				{
					fich << endl << configuracion[i];
				}
				fich.close();
				Guardado= true;
				
				cout << "Guardado completado con exito" << endl;
			}
		}
		else
		{
			cout << "Dame el nombre con el que quieres llamar al fichero (Sin extension de archivo):" << endl;
			cin >> nombrenuevo;
			nombrenuevo = nombrenuevo + ".ini";
			fich.open(nombrenuevo.c_str());
			if (!fich)
			{
				cout << "Error guardando el fichero" << endl;
			}
			else
			{
				fich << configuracion[0];
				for (i=1 ; i < T ; i++)
				{
					fich << endl << configuracion[i];
				}
				fich.close();
				Guardado=true;
			
				cout << "Guardado completado con exito" << endl;
			
			}		
		}
	}	
}

void ComprobarGuardadoIni (bool &Guardado, string configuracion[T], string nombre)
{
	char opcion;
	
	while (Guardado == false)
	{
		cout << "No has guardado la nueva configuracion, quieres continuar?(S para Si , N para no)" << endl;
		do
		{
			cin >> opcion;
			
		}while(opcion != 'S' && opcion != 'N');
		
		if (opcion == 'S')
		{
			Guardado= true;
		}
		else
		{
			GuardarIni (configuracion, nombre , Guardado);
		}
	}
}

bool ComprobacionFormato (string configuracion[T] , bool &formato ) //QUE MIERDA ESOS IF ELSE
{
	bool ok = true;
	
	string formato1= "Notacion cientifica" ;
	string formato2= "Notacion coma fija" ;
	
	if (configuracion[0]==formato1)
	{
		formato=true;
	}
	else
	{
		if(configuracion[0]==formato2)
		{
			formato=false;
		}
		else
		{
			cout << "Formato no válido o en posición incorrecta, rectifíquelo " << configuracion[0] << endl; 		//Distinto de ambos formatos
			ok= false ;
		}
	}
	
	return ok;
}

void ListarParticulas(struct Particula particulas[TAM], short tamp , bool formato, string configuracion[T])
{
	short i;
	
	cout << "Hay " << tamp << " fichas de particulas y estos son sus datos" <<  endl;
	
	for (i=0 ; i < tamp ; i++)
	{
		MostrarEncuentro(particulas[i], configuracion, formato);
	}
}

void ComprobarGuardadoFichero(bool &Guardado, struct Particula particulas[TAM], short tamp, bool formato, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, string nombre2_fich)
{
	char opcion;
	
	if(!Guardado)
	{
		do
		{
			cout << "La información aún no está guardada, quieres hacerlo ahora?(S para si , N para no):" << endl;
			cin >> opcion;
		}while(opcion != 'S' && opcion != 'N');
		
		if(opcion == 'S')
		{
			GuardarFichero(Guardado, particulas, tamp, formato, familias, tamfam, grupos, tamgrup, nombre2_fich);
		}
	}
}

void GuardarFichero(bool &Guardado, struct Particula particulas[TAM], short tamp, bool formato, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, string nombre2_fich)
{
	short i;
	char resp;
	string nombrenuevo;
	ofstream f;
	
	do	
	{
		cout << "Quieres guardar la nueva configuarcion con el mismo nombre?(S para si , N para no)" << endl;
		cin >> resp;
	}while (resp != 'S' && resp != 'N');
	
	if (resp == 'S')
	{
		f.open(nombre2_fich.c_str());
		if (!f)
		{
			cout << "Error guardando el fichero" << endl;
		}
	}
	else
	{
		cout << "Dame el nombre con el que quieres llamar al fichero (Sin extension de archivo):" << endl;
		cin >> nombrenuevo;
		nombrenuevo = nombrenuevo + ".txt";
		f.open(nombrenuevo.c_str());
		if (!f)
		{
			cout << "Error guardando el fichero" << endl;
		}
	}
	
	f << familias[0];
	for(i=1;i<tamfam;i++)
	{
		f << ";" << familias[i];
	}
	f << endl;
	
	f << grupos[0];
	for(i=1;i<tamgrup;i++)
	{
		f << ";" << grupos[i];
	}
	
	for(i=0;i<tamp;i++)
	{
		f << endl;										//Se pone el salto de linea para separarlo de los grupos y dentro del bucle porque entre ficha y ficha, en la funcion Guardar Ficha, no se ha puesto salto del linea al final para evitar que en la última particula haya un salto despúes
		GuardarFicha(f, particulas[i], formato);
	}
	cout << endl << "Información guardada con éxito." << endl;
	Guardado = true;
}

void GuardarFicha(ofstream &f, struct Particula particula, bool formato)
{
	f << particula.Nombre << endl;
	f << particula.Simbolo << endl;
	
	GuardarMasa(f, particula.Masa, formato);
	
	f << "	"; 							//Tabulado
	if(particula.Carga > 0)
	{
		f << "+";						//Si es positivo se pone el +, sino es porque es negativo, y ya se pone sólo. El 0 no lleva signo
	}
	
	if((particula.Carga != -1) && (particula.Carga != 0) && (particula.Carga != 1))  //Si no es entero lleva tres decimales
	{
		f.precision(3);
	}
	f << particula.Carga << endl;
	f << particula.Familia << "	" << particula.Grupo << endl;  		//Tabulado
	f << particula.Pais << endl;
	f << particula.Anyo;
}

void GuardarMasa(ofstream &f, float masa, bool formato) //En python mas facil, seguro hay maneras aqui
{
	short indice_masa = 0, i;
	
	if(masa==-1)			//Si es -1 es desconocida, y se guarda así
	{
	    f << masa;
    }
	else
	{
		if(!formato)  //Notacion coma fija
		{
			while(masa>10000)   //Como la notacion cientifica tiene 3 decimales ponemos un par de 0´s para evitar errores (Más vale prevenir que curar)
			{
				masa = masa/10;
				indice_masa++;
			}
			f << masa;
			for(i=0; i<indice_masa; i++)
			{
				f << "0";
			}
		}
		else  //Notacion cientifica
		{
			while(masa>10) 		//Tiene que ser menor a 10
			{
				masa = masa/10;
				indice_masa++;
			}
			f.precision(3);
			f << masa;
			f << "e+";
			f.width(3);
			f.fill('0');
			f << indice_masa;
		}
	}
}

void CrearParticula (struct Particula particulas[TAM], short& num, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, string configuracion[T], string nombre2)	//Indentado
{	
	bool repeticion;
	short i;
	
	cin.ignore();
	cout << "Rellene los datos de la nueva " << nombre2 << ": " << endl;
	
	do
	{
		repeticion = false;
		
		cout << configuracion[1] << ": ";
		getline(cin, particulas[num].Nombre);
		cout << configuracion[2] << ": ";
		getline(cin, particulas[num].Simbolo);
		
		i=0;
		while((i<num) && (!repeticion))
		{
			if((particulas[num].Nombre == particulas[i].Nombre) && (particulas[num].Simbolo == particulas[i].Simbolo))
			{
				cout << "Ya existe una " << nombre2 << " con ese " << configuracion[1] << " y " << configuracion[2] << endl;
				repeticion = true;
			}
			i++;
		}
	}while(repeticion);

	
	do
	{
		cout << configuracion[3] << ": ";
		cin >> particulas[num].Masa;
	}while((particulas[num].Masa < 0) && (particulas[num].Masa != -1));		//No puede ser negativa a no ser que sea -1 (Desconocida)
	
	do
	{
		cout << configuracion[4] << ": ";
		cin >> particulas[num].Carga;
		cin.ignore();
	}while((particulas[num].Carga>1) || (particulas[num].Carga<-1));
	
	do
	{
		cout << configuracion[5] << ": ";
		getline(cin, particulas[num].Familia);
	}while(!ComprobacionCadena (tamfam, familias, particulas[num].Familia));		//Comprueba la validez de la Familia
	
	do
	{
		cout << configuracion[6] << ": ";
		getline(cin, particulas[num].Grupo);				
	}while(!ComprobacionCadena (tamgrup, grupos, particulas[num].Grupo));		//Comprueba la validez del Grupo

	cout << configuracion[7] << ": ";
	getline(cin, particulas[num].Pais);
	
	do
	{
		cout << configuracion[8] << ": ";
		cin >>  particulas[num].Anyo;
	}while((particulas[num].Anyo < 0) && (particulas[num].Anyo != -1));		//No puede ser negativa a no ser que sea -1 (Desconocida)
	
}

void EditarParticula (struct Particula particulas[TAM], short tamp, string configuracion[T], string nombre2, bool formato, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, bool &GuardadoFich) //Indentado
{
	short pos = -1, i, j;
	bool repeticion;
	string simbolo;
	
	cout << "Introduce el " << configuracion[2] << " de la " << nombre2 << " que quieres editar: ";
	cin >> simbolo;
	
	i=0;
	while((i<tamp) && (pos==-1))		//Una vez se encuentra esa Particula se deja de buscar
	{
		if(particulas[i].Simbolo == simbolo)
		{
			pos = i;
		}
		i++;
	}
	
	if(pos == -1)    //pos se ha inicializado a -1, si no se encuentra el Simbolo (No se ha entrado en el if para editarlo) entra aquí
	{
		cout << "No se ha encontrado esa " << nombre2;
	}
	else
	{
		cout << "La "<< nombre2 << " a editar es: " << endl;
		MostrarEncuentro(particulas[pos], configuracion, formato);
		cout << "Vuelva a escribir los datos de la " << nombre2 << ": ";
		cin.ignore();
		
		do
		{
			repeticion = false;
			cout << endl;
			cout << configuracion[1] << ": ";
			getline(cin, particulas[pos].Nombre);
			cout << configuracion[2] << ": ";
			getline(cin, particulas[pos].Simbolo);
			
			j=0;
			while((j<tamp) && (!repeticion))
			{
				if((particulas[pos].Nombre == particulas[j].Nombre) && (particulas[pos].Simbolo == particulas[j].Simbolo))		//No se puede repetir Nombre y Simbolo
				{
					if(pos != j)																								//A no ser que sea esa misma Partícula
					{
						cout << "Ya existe una " << nombre2 << " con ese " << configuracion[1] << " y " << configuracion[2] << endl;
						repeticion = true;
					}
				}
				j++;
			}
		}while(repeticion);
		
		do
		{
			cout << configuracion[3] << ": ";
			cin >> particulas[pos].Masa;
		}while((particulas[pos].Masa < 0) && (particulas[pos].Masa != -1));		//No puede ser negativa a no ser que sea -1 (Desconocida)
		
		do
		{
			cout << configuracion[4] << ": ";
			cin >> particulas[pos].Carga;
			cin.ignore();
		}while((particulas[pos].Carga>1) || (particulas[pos].Carga<-1));
		
		do
		{
			cout << configuracion[5] << ": ";
			getline(cin, particulas[pos].Familia);
		}while(!ComprobacionCadena(tamfam, familias, particulas[pos].Familia));
		
		do
		{
			cout << configuracion[6] << ": ";
			getline(cin, particulas[pos].Grupo);
		}while(!ComprobacionCadena(tamgrup, grupos, particulas[pos].Grupo));
		
		cout << configuracion[7] << ": ";
		getline(cin, particulas[pos].Pais);
		
		do
		{
			cout << configuracion[8] << ": ";
			cin >>  particulas[pos].Anyo;
		}while((particulas[pos].Anyo < 0) && (particulas[pos].Anyo != -1));		//No puede ser negativa a no ser que sea -1 (Desconocida)
			
		cout << endl << "Particula editada con éxito." << endl;	
		GuardadoFich = false;
		tamp++;
		}
}
	
void BorrarFicha(struct Particula particulas[TAM], short &tamp, string configuracion[T], string nombre2, bool &GuardadoFich) //Se puede hacer algo con esos if else
{
	short i, pos;
	string simbolo;
	bool Encontrado = false;
	
	cout << "Introduce el " << configuracion[2] << " de la " << nombre2 << " que quieres eliminar: ";
	cin >> simbolo;
	
	i = 0;
	while((i<tamp) && (!Encontrado))
	{
		if(particulas[i].Simbolo == simbolo)
		{
			pos = i;
			Encontrado = true;
		}
		else
		{
			i++;
		}
	}
	
	if(!Encontrado)    //De las dos condiciones de salida es la que tendría que cambiar si se encuentra
	{
		cout << "No se ha encontrado esa " << nombre2;
	}
	else
	{
		for(i=pos+1; i<tamp; i++)		//Se pisa la Particula i-1 con la i y se reduce en uno el tamaño del vector (Los datos de la Particula tamp seguirían en memoria pero no declarados)
		{
			particulas[i-1] = particulas[i];
		}
		tamp = tamp - 1;
		cout << "Ficha eliminada con éxito." << endl;
		GuardadoFich = false;
	}
}

void Busquedas(string configuracion[T], string nombre2, struct Particula particulas[TAM], short tamp, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, bool formato) //Igual que Menu, se puede mejorar
{
	char caso;
	bool Quedarse = true;
	
	do
	{
		cout << endl << endl;
		cout << "Elige una de estas opciones: " << endl;
		cout << "a) Buscar una ficha por " << configuracion[1] << " exacto" << endl;
		cout << "b) Buscar todas las fichas que contengan en " << configuracion[1] << " las palabras..." << endl;
		cout << "c) Buscar las fichas que tengan " << configuracion[3] << " inferior a..." << endl;
		cout << "d) Mostrar " << nombre2 << " que pertenecen a la " << configuracion[5] << " ..." << endl;
		cout << "e) Mostrar " << nombre2 << " de una " << configuracion[5] << " y " << configuracion[6] << endl;
		cout << "f) Salir" << endl;
		cin >> caso;
		
		switch(caso)
		{
			case 'a':
				BuscarPorNombre(particulas, tamp, nombre2, configuracion, formato);
				break;
				
			case 'b':
				BuscarPorContener(particulas, tamp, nombre2, configuracion, formato);
				break;
				
			case 'c':
				BuscarMasaMenor(particulas, tamp, nombre2, configuracion, formato);
				break;
				
			case 'd':
				BuscarPorFamilia(particulas, tamp, familias, tamfam, nombre2, configuracion, formato);
				break;
				
			case 'e':
				BuscarPorFamiliaYGrupo(particulas, tamp, familias, tamfam, grupos, tamgrup, nombre2, configuracion, formato);
				break;
				
			case 'f':
				cout << "Saliendo del menú de búsquedas." << endl;
				Quedarse = false;
				break;
				
			default:
				cout << "Opción invalida.";
				break;
		}
	}while(Quedarse==true);
}

void MostrarEncuentro(struct Particula particulas, string configuracion[T], bool formato)
{
	
	cout << endl;
	cout << configuracion[1] << ": " << particulas.Nombre << endl;
	
	cout << configuracion[5] << ": ";
	if(particulas.Familia=="?") 		//Si este campo tiene ese valor es porque se desconoce
	{
		cout << "Desconocido";
	}
	else
	{
		cout << particulas.Familia;
	}
	cout << endl;
	
	cout << configuracion[6] << ": ";
	if(particulas.Grupo=="?") 		//Si este campo tiene ese valor es porque se desconoce
	{
		cout << "Desconocido";
	}
	else
	{
		cout << particulas.Grupo;
	}
	cout << endl;
	
	cout << configuracion[2] << ": " << particulas.Simbolo << endl;
	
	cout << configuracion[3] << ": ";
	LeerMasa(particulas.Masa, formato);
	
	cout << configuracion[4] << ": ";
	if((particulas.Carga != -1) && (particulas.Carga != 0) && (particulas.Carga != 1))  //Si no es entero lleva tres decimales
	{
		cout.precision(3);
	}
	cout << particulas.Carga << endl;
	
	cout << configuracion[7] << ": ";
	if(particulas.Pais=="") 		//Si este campo tiene ese valor es porque se desconoce
	{
		cout << "Desconocido";
	}
	else
	{
		cout << particulas.Pais;
	}
	cout << endl;
	
	cout << configuracion[8] << ": ";
	if(particulas.Anyo==-1) 		//Si este campo tiene ese valor es porque se desconoce
	{
		cout << "Desconocido";
	}
	else
	{
		cout << particulas.Anyo;
	}
	cout << endl;
}

void LeerMasa(float masa, bool formato) //En python mas facil, seguro hay maneras aqui
{
	short indice_masa = 0, i;
	
	if(masa==-1)
	{
		cout << "Desconocido"; 		//Si este campo tiene ese valor es porque se desconoce
	}
	else
	{
		if(!formato)  //Notacion coma fija
		{
			while(masa>10000)
			{
				masa = masa/10;
				indice_masa++;
			}
			cout << masa;
			for(i=0; i<indice_masa; i++)
			{
				cout << "0";
			}
		}
		else  //Notacion cientifica
		{
			while(masa>10)
			{
				masa = masa/10;
				indice_masa++;
			}
			cout.precision(3);
			cout << masa;
			cout << "e+";
			cout.width(3);
			cout.fill('0');
			cout << indice_masa;
		}
	}
	cout << endl;
}

void BuscarPorNombre (struct Particula particulas[TAM] , short tamp, string nombre2, string configuracion[T], bool formato) //Mejorable sin tanta tonteria
{
	string nombre;
	bool encontrado = false;
	short j;
	j=0;
	
	cin.ignore();
	cout << "Introduce el " << configuracion[1] << " de la " << nombre2 << " que quieres buscar: ";
	getline ( cin , nombre);
	while ( j<tamp && encontrado==false )
	{
		if (particulas[j].Nombre == nombre)
	    {
	        encontrado = true;
	        cout << "La " << nombre2 << " introducida es: " << endl;
			MostrarEncuentro(particulas[j], configuracion, formato);
	    }
	    j++;
	}
	
	if ( encontrado == false )
	{
	    cout << endl << "No hay ninguna " << nombre2 << " con ese " << configuracion[1] ;
	}
}

void BuscarPorContener (struct Particula particulas[TAM], short tamp, string nombre2, string configuracion[T], bool formato)//Mejorable sin tanta tonteria
{
    string palabra;
    short j, si;
    bool encontrado = false;
    
    cin.ignore();
    cout<< "Introduce la palabra que deseas buscar: ";
    getline (cin, palabra);
    for ( j=0; j<tamp; j++ )
    {
    	si = particulas[j].Nombre.find (palabra);
        if( si >= 0 )
        {
        	encontrado = true;
        	cout << endl;
            MostrarEncuentro(particulas[j], configuracion, formato);
            cout << endl;
        }
    }
    
    if (!encontrado)
    {
    	cout << "No se han encontrado " << nombre2 << " que contengan la palabra " << palabra << " en " << configuracion[1] << endl;
	}
}

void BuscarMasaMenor (struct Particula particulas[TAM], short tamp, string nombre2, string configuracion[T], bool formato) 
{
	bool encontrado = false;
	float m;
	short i;
	do
	{
		cout << "Introduce la " << configuracion[3] << " máxima de las " << nombre2 << " que deseas buscar: ";
		cin >> m ;
	}while (m<0);
	for (i=0; i<tamp; i++ )
	{
	    if ((particulas[i].Masa <= m) && (particulas[i].Masa != -1))
	    {
	    	cout << endl;
		    MostrarEncuentro(particulas[i], configuracion, formato);
		    cout << endl;
		    encontrado = true;
	    }
	}
	
	if (!encontrado)
	{
		cout << "No se ha encontrado ninguna " << nombre2 << " con " << configuracion[3] << " menor a esa." << endl;
	}
}

void BuscarPorFamilia (struct Particula particulas[TAM], short tamp, string familias[TAMF], short tamfam, string nombre2, string configuracion[T], bool formato) //Mucho if else
{
    string familia;
    struct Particula par;
    short i;
    bool encontrado = false, Seguir = true, Comparar = false;
    
    cin.ignore();
    do
    {
    	cout << "Introduce la " << configuracion[5] << " de las " << nombre2 << " de la cual deseas información: ";
    	getline ( cin, familia );
	}while (!ComprobacionCadena (tamfam, familias, familia));
	
	i = 0;
    while ((i<tamp) && (Seguir))
    {
        if ( particulas[i].Familia == familia )
        {
        	Comparar = true;
            encontrado = true;
            cout << endl;
            MostrarEncuentro(particulas[i], configuracion, formato);
            cout << endl;
        }
        else																//Si las familias son distintas
        {
        	if(Comparar)													//Pero ha habido una igual antes
        	{
        		Seguir = false;												//Se corta el bucle, porque está ordenado alfabéticamente
			}
		}
        i++;
    }
    
	if (!encontrado)
	{
		cout << "No se ha encontrado ninguna " << nombre2 << " de esa " << configuracion[5] << endl;
	}
}

void BuscarPorFamiliaYGrupo (struct Particula particulas[TAM], short tamp, string familias[TAMF], short tamfam, string grupos[TAMG], short tamgrup, string nombre2, string configuracion[T], bool formato) //WTF IF ELSE
{
    string familia, grupo;
    short i;
    bool encontrado = false, Comparar = false, Seguir = true;
    
    cin.ignore();
    do
    {
    	cout<< "Introduce la " << configuracion[5] << " de " << nombre2 << " de la cual deseas información: ";
    	getline ( cin, familia );
	}while (!ComprobacionCadena (tamfam, familias, familia));
	
	do
    {
		cout << "Introduce el " << configuracion[6] << " de " << nombre2 << " del cual deseas informacion: ";
    	getline ( cin, grupo );
	}while (!ComprobacionCadena (tamgrup, grupos, grupo));
	
	i = 0;
	while((i<tamp) && (Seguir))
    {
        if (particulas[i].Familia == familia)
        {
        	if(particulas[i].Grupo == grupo)
        	{
        		Comparar = true;
        		cout << endl;
	        	MostrarEncuentro(particulas[i], configuracion, formato);
    	    	cout << endl;
        		encontrado = true;
			}
        }
        else																//Si las familias son distintas
        {
        	if(Comparar)													//Pero ha habido una igual antes
        	{
        		Seguir = false;												//Se corta el bucle, porque está ordenado alfabéticamente
			}
		}
        i++;
    }
    
	if (!encontrado)
	{
		cout << "No se ha encontrado ninguna " << nombre2 << " de " << configuracion[5] << " y de " << configuracion[6] << " igual a los dados."<< endl;
	}
}

void MeterUnaOrdenada(struct Particula particulas[TAM], short &tam) //Demasiadas condiciones
{
	bool Condicion = true;
	short j;
	struct Particula auxiliar;
	
	j = tam;
	tam++;
	while(j>0)
	{
		if(particulas[j-1].Familia == particulas[j].Familia)
		{
			if(particulas[j-1].Grupo == particulas[j].Grupo)
			{
				Condicion = PrimeraAlfabeticaTrue(particulas[j-1].Nombre, particulas[j].Nombre); 			//Con Familia y Grupo iguales se ordena según el Nombre
				if(!Condicion)
				{
					auxiliar = particulas[j-1];
					particulas[j-1] = particulas[j];
					particulas[j] = auxiliar;
					j--;
				}
			}
			else
			{
				Condicion = PrimeraAlfabeticaTrue(particulas[j-1].Grupo, particulas[j].Grupo);				//Con Familia igual y Grupo distinto se ordena según el Grupo
				if(!Condicion)
				{
					
					auxiliar = particulas[j-1];
					particulas[j-1] = particulas[j];
					particulas[j] = auxiliar;
					j--;
					
				}
			}	
		}
		else
		{
			Condicion = PrimeraAlfabeticaTrue(particulas[j-1].Familia, particulas[j].Familia);				//Con Familia distinta se ordena según la Familia
			if(!Condicion)
			{
				auxiliar = particulas[j-1];
				particulas[j-1] = particulas[j];
				particulas[j] = auxiliar;
				j--;
			}
		}
		j--;
	}
}

void str_a_str_min(string &pal) //En python ya existe
{
	short i, tam;
	
	tam = pal.length();
	for(i=0;i<tam;i++)
	{
		if ((pal[i]>='A') && (pal[i]<='Z'))
		{
			pal[i] = char(int(pal[i]) + 32);
		}
	}
}

bool PrimeraAlfabeticaTrue(string pal1, string pal2) //Innecesario
{
	string primera, segunda;
	bool Orden = true, Seguir = true;
	short i, j;
	unsigned short longprim, longsegun;
	primera = pal1;
	segunda = pal2;
	str_a_str_min(primera);
	str_a_str_min(segunda);
	
	longprim = primera.length();
	longsegun = segunda.length();
	
	i=0;
	j=0;
	while((i<longprim) && (j<longsegun) && (Seguir == true))
	{
		if(primera[i] != ' ')
		{
			if(segunda[j] != ' ')
			{
				if(segunda[j] != primera[i])
				{
					if(int(segunda[j]) < int(primera[i]))
					{
						Orden = false;
					}
					Seguir = false;
				}
				j++;
			}
		i++;
		}
		else
		{
			if(segunda[j] != ' ')
			{
				j++;
			}
		}
	}
	
	if(j == longsegun)
	{
		Orden = false;
	}
	
	return Orden;
}

