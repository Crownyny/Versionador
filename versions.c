/**
 * @file
 * @brief Implementacion del API de gestion de versiones
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @copyright MIT License
*/

#include "versions.h"

/**
 * @brief Crea una version en memoria del archivo
 * Valida si el archivo especificado existe y crea su hash
 * @param filename Nombre del archivo
 * @param hash Hash del contenido del archivo
 * @param comment Comentario
 * @param result Nueva version en memoria
 *
 * @return Resultado de la operacion
 */
return_code create_version(char * filename, char * comment, file_version * result);

/**
 * @brief Verifica si existe una version para un archivo
 *
 * @param filename Nombre del archivo
 * @param hash Hash del contenido
 *
 * @return 1 si la version existe, 0 en caso contrario.
 */
int version_exists(char * filename, char * hash);

/**
 * @brief Obtiene el hash de un archivo.
 * @param filename Nombre del archivo a obtener el hash
 * @param hash Buffer para almacenar el hash (HASH_SIZE)
 * @return Referencia al buffer, NULL si ocurre error
 */
char *get_file_hash(char * filename, char * hash);

/**
 * @brief Copia un archivo
 *
 * @param source Archivo fuente
 * @param destination Destino
 *
 * @return
 */
int copy(char * source, char * destination);

/**
* @brief Almacena un archivo en el repositorio
*
* @param filename Nombre del archivo
* @param hash Hash del archivo: nombre del archivo en el repositorio
*
* @return
*/
int store_file(char * filename, char * hash);

/**
* @brief Almacena un archivo en el repositorio
*
* @param hash Hash del archivo: nombre del archivo en el repositorio
* @param filename Nombre del archivo
*
* @return
*/
int retrieve_file(char * hash, char * filename);

/**
 * @brief Adiciona una nueva version de un archivo.
 *
 * @param filename Nombre del archivo.
 * @param comment Comentario de la version.
 * @param hash Hash del contenido.
 *
 * @return 1 en caso de exito, 0 en caso de error.
 */
int add_new_version(file_version * v);


return_code create_version(char * filename, char * comment, file_version * result) {

	struct stat s;
	char hash[HASH_SIZE];
	char new[PATH_MAX] = ""; 
	// Llena a estructura result recibida por referencia.
	// Debe validar:
	// 1. Que el archivo exista y sea un archivo regular
	if (stat(filename, &s) < 0 || !S_ISREG(s.st_mode)) {
		return VERSION_ERROR;
	}
	// 2. Obtiene y guarda en la estructura el HASH del archivo
	// Llena todos los atributos de la estructura y retorna VERSION_CREATED
	// En caso de fallar alguna validacion, retorna VERSION_ERROR
	strcpy(new, strcat(VERSIONS_DIR "/",get_file_hash(filename, hash)));

	strcpy(result->filename ,filename);
	strcpy(result->comment ,comment);
	strcpy(result->hash ,hash);

	return VERSION_CREATED;

}

return_code add(char * filename, char * comment) {

	file_version v;

	// 1. Crea la nueva version en memoria
	// Si la operacion falla, retorna VERSION_ERROR
	// create_version(filename, comment, &v)

	if (create_version(filename, comment, &v) == VERSION_ERROR)
		return VERSION_ERROR;


	// 2. Verifica si ya existe una version con el mismo hash
	// Retorna VERSION_ALREADY_EXISTS si ya existe
	//version_exists(filename, v.hash)

	if(version_exists(filename, v.hash))

	// 3. Almacena el archivo en el repositorio.
	// El nombre del archivo dentro del repositorio es su hash (sin extension)
	// Retorna VERSION_ERROR si la operacion falla
	//store_file(filename, v.hash)

	// 4. Agrega un nuevo registro al archivo versions.db
	// Si no puede adicionar el registro, se debe borrar el archivo almacenado en el paso anterior
	// Si la operacion falla, retorna VERSION_ERROR
	//add_new_version(&v)

	// Si la operacion es exitosa, retorna VERSION_ADDED
	return VERSION_ERROR;
}

int add_new_version(file_version * v) {
	FILE * fp = fopen(VERSIONS_DB_PATH, "ab");

	if(!fp)
		return VERSION_ERROR;
	// Adiciona un nuevo registro (estructura) al archivo versions.db
	fwrite(v, sizeof *v, 1, fp);	
	fclose(fp);
	return VERSION_CREATED;
}


void list(char * filename) {

	//Abre el la base de datos de versiones (versions.db)
	//Muestra los registros cuyo nombre coincide con filename.
	//Si filename es NULL, muestra todos los registros.

}

char *get_file_hash(char * filename, char * hash) {
	char *comando;
	FILE * fp;

	struct stat s;

	//Verificar que el archivo existe y que se puede obtener el hash
	if (stat(filename, &s) < 0 || !S_ISREG(s.st_mode)) {
		perror("stat");
		return NULL;
	}

	sha256_hash_file_hex(filename, hash);

	return hash;

}

int copy(char * source, char * destination) {
	// Copia el contenido de source a destination (se debe usar open-read-write-close, o fopen-fread-fwrite-fclose)
}

int version_exists(char * filename, char * hash) {
	FILE *fp;
	ssize_t read_count;
	file_version v;
	
	if((fp = fopen(VERSIONS_DB_PATH, "r")) == NULL)
		return -1;

	while(read_count = fread(&v, sizeof v, 1, fp), read_count > 0) {	
		if(strcmp(v.filename, filename) == 0 && strcmp(v.hash, hash) == 0) {
			fclose(fp);
			return VERSION_ALREADY_EXISTS;
		}
	}

	fclose(fp);
    return 1;  
}

int get(char * filename, int version) {

	file_version r;

	//1. Abre la BD y busca el registro r que coincide con filename y version
	//retrieve_file(r.hash, r.filename)

	FILE * fp = fopen(VERSIONS_DB_PATH, "r");
	if(!fp)
		return -1;

	int cont = 1;
	while(!feof(fp)) {
		if(fread(&r, sizeof(file_version), 1, fp) != 1)
			break;

		if (r.filename == filename ) {
			if (cont == version) {
				copy(r.hash, r.filename);
				return 1;
			}
			else
				cont++;
		}
	}

	fclose(fp);
	return 0;
}


int store_file(char * filename, char * hash) {
	char dst_filename[PATH_MAX];
	snprintf(dst_filename, PATH_MAX, "%s/%s", VERSIONS_DIR, hash);
	return copy(filename, dst_filename);
}

int retrieve_file(char * hash, char * filename) {
	char src_filename[PATH_MAX];
	snprintf(src_filename, PATH_MAX, "%s/%s", VERSIONS_DIR, hash);
	return copy(src_filename, filename);
}

