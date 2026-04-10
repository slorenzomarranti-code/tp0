#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	
	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    valor = config_get_string_value(config, "CLAVE");
	
	// Loggeamos el valor de config
	log_info(logger, "El valor de la clave leida es: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor usando los datos del config
    conexion = crear_conexion(ip, puerto);

    // Enviamos al servidor el valor de CLAVE como un mensaje simple
    enviar_mensaje(valor, conexion);

    // Armamos y enviamos el paquete con las líneas de consola
    paquete(conexion);

    // Limpiamos la memoria y cerramos la conexión
    terminar_programa(conexion, logger, config); 

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	
	return 0; // Es buena práctica que el main retorne 0 si todo salió bien
}

t_log* iniciar_logger(void)
{
	
    t_log* nuevo_logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);

    
    if (nuevo_logger == NULL) {
        printf("ERROR: No se pudo crear el logger.\n");
        exit(1);
	}

	return nuevo_logger ;
}

t_config* iniciar_config(void)
{
	
    t_config* nuevo_config = config_create("cliente.config");

    
    if (nuevo_config == NULL) {
        printf("ERROR: No se pudo leer el archivo cliente.config\n");
        exit(1);
	}

	return nuevo_config;
}


void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	while (1) 
    {
        leido = readline("> ");

        if (leido == NULL || strcmp(leido, "") == 0) {
            free(leido); // Liberamos antes de salir
            break;
        }

        log_info(logger, "Texto ingresado: %s", leido);

        //Libero la memoria de readline.
        free(leido);
    }
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!

	log_info(logger, "Lectura de consola finalizada.");
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
    t_paquete* paquete;

    paquete = crear_paquete();

    while (1) {
        leido = readline("Paquete> "); 

        if (leido == NULL || strcmp(leido, "") == 0) {
            free(leido);
            break;
        }

        //El tamaño es strlen(leido) + 1 para incluir el '\0'
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);

        free(leido); 
	}

    enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);
	
	// Leemos y esta vez agregamos las lineas al paquete
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}


void terminar_programa(int conexion, t_log* logger, t_config* config){
	
	if (logger != NULL) {
        log_destroy(logger); 
		
	}

	if (config != NULL) {
        config_destroy(config);
	}

    liberar_conexion(conexion);
}
