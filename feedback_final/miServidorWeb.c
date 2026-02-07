#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#define BASE_DIR "miServidorWebDir"
#define BUFFER 1024

void ruta_base(char *destino, const char *subruta) {
    char *home = getenv("HOME");
    sprintf(destino, "%s/%s/%s", home, BASE_DIR, subruta);
}

void copiar_archivo(const char *origen, const char *destino) {
    int fd_origen = open(origen, O_RDONLY);
    int fd_destino = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    char buffer[BUFFER];
    int leidos;

    while ((leidos = read(fd_origen, buffer, BUFFER)) > 0) {
        write(fd_destino, buffer, leidos);
    }

    close(fd_origen);
    close(fd_destino);
}

void *borrar_archivo(void *arg) {
    char *ruta = (char *)arg;
    unlink(ruta);
    pthread_exit(NULL);
}

void *get_archivo(void *arg) {
    char **rutas = (char **)arg;
    copiar_archivo(rutas[0], rutas[1]);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if (argc < 3) return -1;

    char base[BUFFER];
    char ruta[BUFFER];

    char *home = getenv("HOME");
    sprintf(base, "%s/%s", home, BASE_DIR);
    mkdir(base, 0755);

    // APARTADO 1
    if (strcmp(argv[1], "addDir") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            ruta_base(ruta, argv[2]);
            mkdir(ruta, 0755);
            exit(0);
        }
        wait(NULL);
        return 0;
    }

    // APARTADO 1
    if (strcmp(argv[1], "rmDir") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            ruta_base(ruta, argv[2]);
            rmdir(ruta);
            exit(0);
        }
        wait(NULL);
        return 0;
    }

    // APARTADO 2
    if (strcmp(argv[1], "addArchivo") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            char destino[BUFFER];
            ruta_base(destino, argv[3]);
            strcat(destino, "/");
            strcat(destino, strrchr(argv[2], '/') + 1);
            copiar_archivo(argv[2], destino);
            exit(0);
        }
        return 0;
    }

    // APARTADO 2
    if (strcmp(argv[1], "addArchivoCeros") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            int tam = atoi(argv[4]);
            ruta_base(ruta, argv[3]);
            strcat(ruta, "/");
            strcat(ruta, argv[2]);
            int fd = open(ruta, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            for (int i = 0; i < tam; i++) write(fd, "0", 1);
            close(fd);
            exit(0);
        }
        return 0;
    }

    // APARTADO 3
    if (strcmp(argv[1], "rmArchivo") == 0) {
        pthread_t hebra_borrado;
        ruta_base(ruta, argv[2]);
        pthread_create(&hebra_borrado, NULL, borrar_archivo, ruta);
        pthread_join(hebra_borrado, NULL);
        return 0;
    }

    // APARTADO 3
    if (strcmp(argv[1], "getArchivo") == 0) {
        pthread_t hebra_copia;
        char origen[BUFFER];
        ruta_base(origen, argv[2]);
        char *rutas[2] = {origen, argv[3]};
        pthread_create(&hebra_copia, NULL, get_archivo, rutas);
        pthread_join(hebra_copia, NULL);
        return 0;
    }

    return 0;
}
