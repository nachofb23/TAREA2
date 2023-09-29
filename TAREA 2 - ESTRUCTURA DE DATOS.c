#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define MAX_TURISTAS 100 
#define MAX_LUGARES_FAVORITOS 100 
#define MAX_PUNTOS_INTERES 100

struct TuristaInfo {
    char nombre[MAX_LENGTH];
    char lugaresFavoritos[MAX_LENGTH];
};

struct TuristasPorPais {
    struct TuristaInfo* turistas;
    int size;
};

typedef struct {
    char clave[MAX_LENGTH];
    struct TuristasPorPais* contenido;
} ClaveContenido;

typedef struct {
    ClaveContenido* pairs;
    int size;
} Mapa;

struct PuntoInteres {
    char nombre[MAX_LENGTH];
    char tipo[MAX_LENGTH];
    char direccion[MAX_LENGTH];
    char horario[MAX_LENGTH];
    char descripcion[MAX_LENGTH];
};

struct PuntosInteresPorTipo {
    char tipo[MAX_LENGTH]; 
    char nombres[MAX_PUNTOS_INTERES][MAX_LENGTH]; 
    int numNombres; 
};

struct Turista {
    char pasaporte[MAX_LENGTH];
    char nombre[MAX_LENGTH];
    char pais[MAX_LENGTH];
    struct PuntoInteres lugaresFavoritos[MAX_LUGARES_FAVORITOS];
    int numLugaresFavoritos;  
};

int Verificacion(char* palabra) {
    if (strlen(palabra) > MAX_LENGTH) {
        return -1;
    }
    return 0;
}

void MostrarMapa(Mapa* mapaTuristas, Mapa* mapaPuntoInteres) {
    printf("Contenido del Mapa de Turistas:\n");
    for (int i = 0; i < mapaTuristas->size; i++) {
        printf("País: %s\n", mapaTuristas->pairs[i].clave);
        struct TuristasPorPais* turistasPorPais = (struct TuristasPorPais*)mapaTuristas->pairs[i].contenido;
        for (int j = 0; j < turistasPorPais->size; j++) {
            printf("Nombre del Turista: %s\n", turistasPorPais->turistas[j].nombre);
            printf("Lugares Favoritos:\n");
            printf("%s\n", turistasPorPais->turistas[j].lugaresFavoritos);
        }
        printf("--------------------------\n");
    }

    printf("Contenido del Mapa de Puntos de Interés:\n");
    for (int i = 0; i < mapaPuntoInteres->size; i++) {
        printf("Tipo: %s\n", mapaPuntoInteres->pairs[i].clave);
        struct PuntosInteresPorTipo* puntosInteresPorTipo = (struct PuntosInteresPorTipo*)mapaPuntoInteres->pairs[i].contenido;
        for (int j = 0; j < puntosInteresPorTipo->numNombres; j++) {
            printf("Nombre del Punto de Interés: %s\n", puntosInteresPorTipo->nombres[j]);
        }
        printf("--------------------------\n");
    }
}


Mapa* crearMapaVacio() {
    Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
    if (mapa) {
        mapa->size = 0;
        mapa->pairs = (ClaveContenido*)malloc(sizeof(ClaveContenido) * MAX_LENGTH); // Asigna memoria para el arreglo
        if (mapa->pairs == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria para el mapa.\n");
            free(mapa);
            mapa = NULL;
        }
    }
    return mapa;
}

void insertarMapaTurista(Mapa* mapaTuristas, const char* pais, const char* nombreTurista, const char* lugaresFavoritos) {
    int indicePais = -1;

    for (int i = 0; i < mapaTuristas->size; i++) {
        if (strcmp(mapaTuristas->pairs[i].clave, pais) == 0) {
            indicePais = i;
            break;
        }
    }

    if (indicePais == -1) {
        // El país no existe en el mapa, crear una nueva entrada
        if (mapaTuristas->size < MAX_LENGTH) {
            strcpy(mapaTuristas->pairs[mapaTuristas->size].clave, pais);
            struct TuristasPorPais* nuevosTuristas = (struct TuristasPorPais*)malloc(sizeof(struct TuristasPorPais));
            if (nuevosTuristas != NULL) {
                struct TuristaInfo nuevoTurista;
                strcpy(nuevoTurista.nombre, nombreTurista);
                strcpy(nuevoTurista.lugaresFavoritos, lugaresFavoritos);
                nuevosTuristas->turistas = (struct TuristaInfo*)malloc(sizeof(struct TuristaInfo) * MAX_TURISTAS);
                nuevosTuristas->turistas[0] = nuevoTurista;
                nuevosTuristas->size = 1;
                mapaTuristas->pairs[mapaTuristas->size].contenido = nuevosTuristas;
                (mapaTuristas->size)++;
            } else {
                fprintf(stderr, "Error: No se pudo asignar memoria para los turistas.\n");
            }
        } else {
            fprintf(stderr, "Error: Se ha alcanzado el máximo de países en el mapa.\n");
        }
    } else {
        // El país ya existe en el mapa, agregar el nuevo turista a la lista de turistas
        struct TuristasPorPais* turistas = (struct TuristasPorPais*)mapaTuristas->pairs[indicePais].contenido;
        int numTuristas = turistas->size;

        if (numTuristas < MAX_TURISTAS) {
            struct TuristaInfo nuevoTurista;
            strcpy(nuevoTurista.nombre, nombreTurista);
            strcpy(nuevoTurista.lugaresFavoritos, lugaresFavoritos);
            turistas->turistas[numTuristas] = nuevoTurista;
            (turistas->size)++;
        } else {
            fprintf(stderr, "Error: Se ha alcanzado el máximo de turistas para este país.\n");
        }
    }
}

void insertarMapaPuntoInteres(Mapa* mapa, char* tipo, char* nombre) {
    if (mapa->size < MAX_PUNTOS_INTERES) {
        int indiceTipo = -1;

        for (int i = 0; i < mapa->size; i++) {
            if (strcmp(mapa->pairs[i].clave, tipo) == 0) {
                indiceTipo = i;
                break;
            }
        }

        if (indiceTipo == -1) {
            // El tipo no existe en el mapa, crear una nueva entrada
            strcpy(mapa->pairs[mapa->size].clave, tipo);
            struct PuntosInteresPorTipo* nuevoTipo = (struct PuntosInteresPorTipo*)malloc(sizeof(struct PuntosInteresPorTipo));
            strcpy(nuevoTipo->tipo, tipo);
            nuevoTipo->nombres[0][0] = '\0'; // Inicializar la lista de nombres
            strcpy(nuevoTipo->nombres[0], nombre);
            nuevoTipo->numNombres = 1;
            mapa->pairs[mapa->size].contenido = (void*)nuevoTipo; // Usar (void*) para evitar el error
            (mapa->size)++;
        } else {
            // El tipo ya existe en el mapa, agrega el nombre a la lista
            struct PuntosInteresPorTipo* tipoExistente = (struct PuntosInteresPorTipo*)mapa->pairs[indiceTipo].contenido;
            if (tipoExistente->numNombres < MAX_PUNTOS_INTERES) {
                strcpy(tipoExistente->nombres[tipoExistente->numNombres], nombre);
                (tipoExistente->numNombres)++;
            } else {
                fprintf(stderr, "Error: Se ha alcanzado el máximo de nombres de puntos de interés para este tipo.\n");
            }
        }
    } else {
        fprintf(stderr, "Error: El mapa está lleno. No se puede insertar más elementos.\n");
    }
}


void RegistrarPuntoInteres(struct PuntoInteres* lista, int* numPuntosInteres, char* nombre, char* tipo, char* direccion, char* horario, char* descripcion) {
    if (*numPuntosInteres >= MAX_LUGARES_FAVORITOS) {
        fprintf(stderr, "Error: Se ha alcanzado el máximo de lugares favoritos.\n");
        return;
    }

    // Se copian los valores proporcionados (nombre, tipo, dirección, horario, descripción)
    // en la estructura del nuevo punto de interés.
    strcpy(lista[*numPuntosInteres].nombre, nombre);
    strcpy(lista[*numPuntosInteres].tipo, tipo);
    strcpy(lista[*numPuntosInteres].direccion, direccion);
    strcpy(lista[*numPuntosInteres].horario, horario);
    strcpy(lista[*numPuntosInteres].descripcion, descripcion);

    (*numPuntosInteres)++;
}

void MostrarDatosPuntoInteres(struct PuntoInteres* lista, int numPuntosInteres, const char* nombre) {
    int encontrado = 0; // Se utiliza para indicar si se encontró el punto de interés con el nombre dado.

    for (int i = 0; i < numPuntosInteres; i++) {
        if (strcmp(lista[i].nombre, nombre) == 0) {
            printf("Nombre: %s\n", lista[i].nombre);
            printf("Tipo: %s\n", lista[i].tipo);
            printf("Dirección: %s\n", lista[i].direccion);
            printf("Horario: %s\n", lista[i].horario);
            printf("Descripción: %s\n", lista[i].descripcion);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("El lugar con nombre \"%s\" no existe en la base de datos.\n", nombre);
        printf("\n");
    }
}

void EliminarPuntoInteres(Mapa* mapaPuntoInteres, struct PuntoInteres* listaPuntosInteres, int* numPuntosInteres, const char* nombre) {
    // Paso 1: Buscar y eliminar el punto de interés del mapa
    int encontradoEnMapa = 0;

    for (int i = 0; i < mapaPuntoInteres->size; i++) {
        if (strcmp(mapaPuntoInteres->pairs[i].clave, listaPuntosInteres->tipo) == 0) {
            // El tipo coincide en el mapa, buscar el punto de interés dentro de ese tipo.
            struct PuntosInteresPorTipo* tipoPuntosInteres = (struct PuntosInteresPorTipo*)mapaPuntoInteres->pairs[i].contenido;
            for (int j = 0; j < tipoPuntosInteres->numNombres; j++) {
                if (strcmp(tipoPuntosInteres->nombres[j], nombre) == 0) {
                    // Coincide el nombre, elimina el punto de interés del mapa.
                    for (int k = j; k < tipoPuntosInteres->numNombres - 1; k++) {
                        strcpy(tipoPuntosInteres->nombres[k], tipoPuntosInteres->nombres[k + 1]);
                    }
                    tipoPuntosInteres->numNombres--;
                    encontradoEnMapa = 1;
                    break;
                }
            }
            if (encontradoEnMapa) {
                break;
            }
        }
    }

    // Paso 2: Buscar y eliminar el punto de interés de la lista
    int encontradoEnLista = 0;

    for (int i = 0; i < *numPuntosInteres; i++) {
        if (strcmp(listaPuntosInteres[i].nombre, nombre) == 0) {
            // Coincide el nombre, elimina el punto de interés de la lista moviendo los elementos posteriores.
            for (int j = i; j < *numPuntosInteres - 1; j++) {
                listaPuntosInteres[j] = listaPuntosInteres[j + 1];
            }
            (*numPuntosInteres)--;
            encontradoEnLista = 1;
            printf("Punto de interés \"%s\" eliminado con éxito.\n", nombre);
            break;
        }
    }

    if (!encontradoEnMapa && !encontradoEnLista) {
        printf("El lugar con nombre \"%s\" no existe en la base de datos. No se pudo eliminar.\n", nombre);
        printf("\n");
    }
}

void RegistrarTurista(struct Turista* listaTuristas, int* numTuristas, char* pasaporte, char* nombre, char* pais) {
    if (*numTuristas >= MAX_TURISTAS) {
        fprintf(stderr, "Error: Se ha alcanzado el máximo de turistas.\n");
        return;
    }

    // Copiar los datos del turista proporcionados por el usuario
    strcpy(listaTuristas[*numTuristas].pasaporte, pasaporte);
    strcpy(listaTuristas[*numTuristas].nombre, nombre);
    strcpy(listaTuristas[*numTuristas].pais, pais);
    listaTuristas[*numTuristas].numLugaresFavoritos = 0;

    (*numTuristas)++;
}


void AgregarLugarFavoritoATurista(struct Turista* listaTuristas, int numTuristas, const char* pasaporte, const char* nombreLugar) {
    int encontrado = 0;

    for (int i = 0; i < numTuristas; i++) {
        if (strcmp(listaTuristas[i].pasaporte, pasaporte) == 0) {
            if (listaTuristas[i].numLugaresFavoritos >= MAX_LUGARES_FAVORITOS) {
                fprintf(stderr, "Error: Se ha alcanzado el máximo de lugares favoritos para este turista.\n");
                return;
            }

            // Agregar el nuevo lugar favorito a la estructura del turista
            strcpy(listaTuristas[i].lugaresFavoritos[listaTuristas[i].numLugaresFavoritos].nombre, nombreLugar);
            (listaTuristas[i].numLugaresFavoritos)++;
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontró al turista con pasaporte \"%s\". No se pudo agregar el lugar favorito.\n", pasaporte);
    }
}

void TuristasPorPais(const struct Turista* listaTuristas, Mapa* mapaTurista, int numTuristas, const char* pais) {
    for (int i = 0; i < numTuristas; i++) {
        if (strcmp(listaTuristas[i].pais, pais) == 0) {
            char lugaresFavoritos[MAX_LENGTH];
            lugaresFavoritos[0] = '\0'; // Inicializa la cadena como vacía

            for (int j = 0; j < listaTuristas[i].numLugaresFavoritos; j++) {
                snprintf(lugaresFavoritos + strlen(lugaresFavoritos), MAX_LENGTH - strlen(lugaresFavoritos), "%s;", listaTuristas[i].lugaresFavoritos[j].nombre);
            }
            insertarMapaTurista(mapaTurista, pais, listaTuristas[i].nombre, lugaresFavoritos);
        }
    }
}

void MostrarTuristasPorPais(Mapa* mapaTuristas, const char* pais) {
    printf("Turistas de %s:\n", pais);

    int encontrado = 0; // Para rastrear si se encontraron turistas para el país dado

    for (int i = 0; i < mapaTuristas->size; i++) {
        if (strcmp(mapaTuristas->pairs[i].clave, pais) == 0) {
            struct TuristasPorPais* turistasPorPais = mapaTuristas->pairs[i].contenido;
            for (int j = 0; j < turistasPorPais->size; j++) {
                printf("------------------------------\n");
                printf("Nombre del Turista: %s\n", turistasPorPais->turistas[j].nombre);
                printf("Lugares Favoritos =\n");

                char* lugaresFavoritos = strdup(turistasPorPais->turistas[j].lugaresFavoritos);
                char* lugar = strtok(lugaresFavoritos, ";");

                while (lugar != NULL) {
                    printf("- %s\n", lugar);
                    lugar = strtok(NULL, ";");
                }

                printf("------------------------------\n");
            }
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontraron turistas para el país \"%s\".\n", pais);
    }
}

void MostrarPuntosDeInteresPorTipo(Mapa* mapa, struct PuntoInteres* listaPuntos, int numPuntosInteres, char* tipo) { 
  if (mapa->size != 0){
    for (int i = 0; i < mapa->size; i++) {
      if (strcmp(mapa->pairs[i].clave, tipo) == 0) {
        struct PuntosInteresPorTipo* tipoExistente = (struct PuntosInteresPorTipo*)mapa->pairs[i].contenido;

        if (tipoExistente->numNombres == 0) {
                printf("No hay actualmente lugares agregados de este tipo.\n");
                return;
        }else{
          printf("Puntos de interés de tipo %s =\n", tipo);
            for (int j = 0; j < tipoExistente->numNombres; j++) {
              printf("- %s\n", tipoExistente->nombres[j]);
              return;
            }
        }
      }else{
        printf("No se encontraron puntos de interés de tipo %s.\n", tipo);
      }
    }
  }
}

void ImportarPuntosDeInteres(struct PuntoInteres* listaPuntosInteres, Mapa* mapaPuntoInteres, int* numPuntosInteres, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
      fprintf(stderr, "Error: No se pudo abrir el archivo CSV \"%s\".\n", nombreArchivo);
      return;
    }

    fscanf(archivo, "%*[^\n]\n"); 

    char linea[MAX_LENGTH * 5];
    while (fgets(linea, sizeof(linea), archivo) != NULL) {

      char nombre[MAX_LENGTH];
      char tipo[MAX_LENGTH];
      char direccion[MAX_LENGTH];
      char horario[MAX_LENGTH];
      char descripcion[MAX_LENGTH];

      if (sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^\n]", nombre, tipo, direccion, horario, descripcion) == 5) {
        RegistrarPuntoInteres(listaPuntosInteres, numPuntosInteres, nombre, tipo, direccion, horario, descripcion);
        insertarMapaPuntoInteres(mapaPuntoInteres, tipo, nombre);
      } else {
          fprintf(stderr, "Error: Formato de línea incorrecto en el archivo CSV.\n");
      }
    }

    fclose(archivo);
    printf("Puntos de interés importados desde \"%s\" con éxito.\n", nombreArchivo);
}

void ImportarTuristas(struct Turista* listaTuristas, Mapa* mapaTurista, int* numTuristas, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
      fprintf(stderr, "Error: No se pudo abrir el archivo CSV \"%s\".\n", nombreArchivo);
      return;
    }

    fscanf(archivo, "%*[^\n]\n");

    char linea[MAX_LENGTH * 10];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
      char pasaporte[MAX_LENGTH];
      char nombre[MAX_LENGTH];
      char pais[MAX_LENGTH];
      char lugaresFavoritos[MAX_LENGTH * 5];

      if (sscanf(linea, "%[^,],%[^,],%[^,],%[^\n]", pasaporte, nombre, pais, lugaresFavoritos) == 4) {
          // Registramos al turista en la lista.
          RegistrarTurista(listaTuristas, numTuristas, pasaporte, nombre, pais);

          // Procesamos y agregamos los lugares favoritos del turista.
          char* token = strtok(lugaresFavoritos, ";");
          while (token != NULL) {
              AgregarLugarFavoritoATurista(listaTuristas, *numTuristas, pasaporte, lugaresFavoritos);
              token = strtok(NULL, ";");
          }
          insertarMapaTurista(mapaTurista, pais, nombre, lugaresFavoritos);
      } else {
          fprintf(stderr, "Error: Formato de línea incorrecto en el archivo CSV.\n");
      }
    }

    fclose(archivo);
    printf("Turistas importados desde \"%s\" con éxito.\n", nombreArchivo);
}

void ExportarPuntosDeInteres(const char* nombreArchivo, struct PuntoInteres* listaPuntosInteres, int numPuntosInteres) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo CSV \"%s\" para escritura.\n", nombreArchivo);
        return;
    }

    // Escribe la cabecera del archivo CSV
    fprintf(archivo, "Nombre,Tipo,Direccion,Horario,Descripcion\n");

    // Escribe los datos de cada punto de interés en el archivo CSV
    for (int i = 0; i < numPuntosInteres; i++) {
        fprintf(archivo, "%s,%s,%s,%s,%s\n", listaPuntosInteres[i].nombre, listaPuntosInteres[i].tipo,
                listaPuntosInteres[i].direccion, listaPuntosInteres[i].horario, listaPuntosInteres[i].descripcion);
    }

    fclose(archivo);
    printf("Puntos de interés exportados a \"%s\" con éxito.\n", nombreArchivo);
}

void ExportarTuristas(const char* nombreArchivo, struct Turista* listaTuristas, int numTuristas) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo CSV \"%s\" para escritura.\n", nombreArchivo);
        return;
    }

    // Escribe la cabecera del archivo CSV
    fprintf(archivo, "Pasaporte,Nombre,Pais,LugaresFavoritos\n");

    // Escribe los datos de cada turista en el archivo CSV
    for (int i = 0; i < numTuristas; i++) {
        fprintf(archivo, "%s,%s,%s,", listaTuristas[i].pasaporte, listaTuristas[i].nombre, listaTuristas[i].pais);

        // Escribe la lista de lugares favoritos del turista
        for (int j = 0; j < listaTuristas[i].numLugaresFavoritos; j++) {
            fprintf(archivo, "%s", listaTuristas[i].lugaresFavoritos[j].nombre);
            if (j < listaTuristas[i].numLugaresFavoritos - 1) {
                fprintf(archivo, ";");
            }
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("Turistas exportados a \"%s\" con éxito.\n", nombreArchivo);
}

int main(void) {
    int opcion = 0;
    char nombreSitio[MAX_LENGTH];
    char tipoSitio[MAX_LENGTH];
    char direccion[MAX_LENGTH];
    char horario[MAX_LENGTH];
    char descripcion[MAX_LENGTH];
    char pasaporte[MAX_LENGTH];
    char nombreTurista[MAX_LENGTH];
    char pais[MAX_LENGTH];
    char paisAMostrar[MAX_LENGTH];
    char tipoAMostrar[MAX_LENGTH];
    char nombreArchivoCSV[MAX_LENGTH];

    Mapa* MapaPuntoInteres = crearMapaVacio();
    Mapa* MapaTuristaPais = crearMapaVacio();

    struct PuntoInteres listaPuntoInteres[MAX_LUGARES_FAVORITOS];
    int numPuntosInteres = 0;
    struct Turista listaTuristas[MAX_TURISTAS];
    int numTuristas = 0;

    while (opcion != 10) {
        printf("------------------------------\n");
        printf("Ingrese la opción con la que desea trabajar\n");
        printf("1.- Registrar punto de interés\n");
        printf("2.- Mostrar punto de interés\n");
        printf("3.- Eliminar punto de interés\n");
        printf("4.- Registrar turista\n");
        printf("5.- Agregar lugar favorito del turista\n");
        printf("6.- Mostrar turistas por país\n");
        printf("7.- Mostrar punto de interes por tipo\n");
        printf("8.- Importar archivo puntos de interés y archivo de turistas\n");
        printf("9.- Exportar archivo punto de interés y archivo de turistas\n");
        printf("10.- Salir.\n");
        scanf("%d", &opcion);
        printf("------------------------------\n");

        switch (opcion) {
            case 1:
                printf("Por favor ingrese el nombre del sitio: ");
                scanf(" %[^\n]", nombreSitio);

                while (Verificacion(nombreSitio) == -1) {
                    printf("El nombre del sitio supera los caracteres permitidos (Máximo 100 caracteres).\n");
                    printf("Re ingrese nuevamente = ");
                    scanf(" %[^\n]", nombreSitio);
                }

                printf("Por favor ingrese el tipo de lugar que conforma el sitio (Museo, plaza, restaurant, etc.) : ");
                scanf(" %[^\n]", tipoSitio);

                while (Verificacion(tipoSitio) == -1) {
                    printf("El nombre del tipo de sitio supera los caracteres permitidos (Máximo 100 caracteres).\n");
                    printf("Re ingrese nuevamente = ");
                    scanf(" %[^\n]", tipoSitio);
                }

                printf("Por favor ingrese la dirección del sitio: ");
                scanf(" %[^\n]", direccion);

                while (Verificacion(direccion) == -1) {
                    printf("La dirección del sitio supera los caracteres permitidos (Máximo 100 caracteres).\n");
                    printf("Re ingrese nuevamente = ");
                    scanf(" %[^\n]", direccion);
                }

                printf("Por favor ingresar el horario disponible del sitio: ");
                scanf(" %[^\n]", horario);

                printf("Por favor ingresar breve descripción del sitio: ");
                scanf(" %[^\n]", descripcion);

                while (Verificacion(descripcion) == -1) {
                    printf("La descripción del sitio supera los caracteres permitidos (Máximo 100 caracteres).\n");
                    printf("Re ingrese nuevamente = ");
                    scanf(" %[^\n]", descripcion);
                }

                RegistrarPuntoInteres(listaPuntoInteres, &numPuntosInteres, nombreSitio, tipoSitio, direccion, horario, descripcion);

                insertarMapaPuntoInteres(MapaPuntoInteres, tipoSitio, nombreSitio);
                printf("Punto de interés registrado con éxito.\n");
                break;
            case 2:
                printf("Por favor ingrese el nombre del sitio: ");
                scanf(" %[^\n]", nombreSitio);

                MostrarDatosPuntoInteres(listaPuntoInteres, numPuntosInteres, nombreSitio);
                break;
            case 3:
                printf("Por favor ingrese el nombre del sitio que desea eliminar: ");
                scanf(" %[^\n]", nombreSitio);

                EliminarPuntoInteres(MapaPuntoInteres, listaPuntoInteres, &numPuntosInteres, nombreSitio);
                break;
            case 4:
                printf("Por favor ingrese el pasaporte del turista: ");
                scanf(" %[^\n]", pasaporte);

                printf("Por favor ingrese el nombre del turista: ");
                scanf(" %[^\n]", nombreTurista);

                printf("Por favor ingrese el país del turista: ");
                scanf(" %[^\n]", pais);

                RegistrarTurista(listaTuristas, &numTuristas, pasaporte, nombreTurista, pais);
                printf("Turista registrado con éxito.\n");
                break;
            case 5:
                printf("Por favor ingrese el pasaporte del turista: ");
                scanf(" %[^\n]", pasaporte);

                printf("Por favor ingrese el nombre del lugar favorito: ");
                scanf(" %[^\n]", nombreSitio);

                AgregarLugarFavoritoATurista(listaTuristas, numTuristas, pasaporte, nombreSitio);
                printf("Lugar \"%s\" agregado a los lugares favoritos del turista con pasaporte \"%s\".\n", nombreSitio, pasaporte);
                break;
            case 6:
                printf("Por favor ingrese el país para mostrar turistas: ");
                scanf(" %[^\n]", paisAMostrar);

                TuristasPorPais(listaTuristas, MapaTuristaPais, numTuristas, pais);
                MostrarTuristasPorPais(MapaTuristaPais, paisAMostrar);
                break;
            case 7:
                printf("Por favor ingrese el tipo de punto de interés a mostrar: ");
                scanf(" %[^\n]", tipoAMostrar);

                MostrarPuntosDeInteresPorTipo(MapaPuntoInteres, listaPuntoInteres, numPuntosInteres, tipoAMostrar);
                break;
            case 8:
                printf("Por favor ingrese el nombre del archivo CSV de puntos de interés: ");
                scanf(" %[^\n]", nombreArchivoCSV);
                ImportarPuntosDeInteres(listaPuntoInteres, MapaPuntoInteres, &numPuntosInteres, nombreArchivoCSV);
              
                printf("Por favor ingrese el nombre del archivo CSV de turistas: ");
                scanf(" %[^\n]", nombreArchivoCSV);
                ImportarTuristas(listaTuristas, MapaTuristaPais, &numTuristas, nombreArchivoCSV);
                MostrarMapa(MapaTuristaPais, MapaPuntoInteres);
                break;
            case 9:
                printf("Ingrese el nombre del archivo CSV de puntos de interés para exportarlo: ");
                scanf(" %[^\n]", nombreArchivoCSV);
                ExportarPuntosDeInteres(nombreArchivoCSV, listaPuntoInteres, numPuntosInteres);

                printf("Ingrese el nombre del archivo CSV de turistas para exportarlo: ");
                scanf(" %[^\n]", nombreArchivoCSV);
                ExportarTuristas(nombreArchivoCSV, listaTuristas, numTuristas);
                break;
            case 10:
                printf("Salida exitosa.\n");
                free(MapaPuntoInteres);
                free(MapaTuristaPais->pairs);
                free(MapaTuristaPais);
                break;
            default:
                printf("Opción no válida. Por favor, ingrese nuevamente.\n");
                break;
        }
    }
    return 0;
}