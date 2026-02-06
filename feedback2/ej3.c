#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Apartado 1: Unir dos archivos en uno nuevo
void concatenar_archivos(const char *dir, const char *f1, const char *f2, const char *f_out) {
    char r1[512], r2[512], r_out[512], buffer[BUFFER_SIZE];
    int fd1, fd2, fd_out;
    ssize_t n;

    // Rutas completas de los archivos
    sprintf(r1, "%s/%s", dir, f1);
    sprintf(r2, "%s/%s", dir, f2);
    sprintf(r_out, "%s/%s", dir, f_out);

    // Abrir archivos
    fd1 = open(r1, O_RDONLY);
    fd2 = open(r2, O_RDONLY);
    fd_out = open(r_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Verificar que los archivos se abrieron correctamente
    if (fd1 < 0 || fd2 < 0 || fd_out < 0) {
        perror("Error al abrir los ficheros");
        return;
    }

    // Se lee del primer archivo y se escribe en el resultado
    while ((n = read(fd1, buffer, BUFFER_SIZE)) > 0) write(fd_out, buffer, n);
    // Se lee del segundo archivo y se escribe en el resultado
    while ((n = read(fd2, buffer, BUFFER_SIZE)) > 0) write(fd_out, buffer, n);

    printf("Éxito: Archivos concatenados en %s\n", r_out);

    close(fd1); close(fd2); close(fd_out);
}

// Apartado 2: Listado de archivos con inodo y enlaces duros
void listar_archivos(const char *directorio) {
    DIR *d = opendir(directorio);
    struct dirent *ent;
    struct stat info;
    char ruta[512];

    if (d == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("Listado de archivos en: %s\n", directorio);
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue; // Ignorar archivos ocultos

        sprintf(ruta, "%s/%s", directorio, ent->d_name);
        
        // Con stat obtenemos el inodo y número de enlaces duros
        if (stat(ruta, &info) == 0) {
            printf("Nombre: %s | Inodo: %lu | Enlaces Duros: %lu\n", 
                    ent->d_name, info.st_ino, info.st_nlink);
        }
    }
    closedir(d);
}

int main(int argc, char *argv[]) {
    if (argc == 5) {
        // Ejecuta Apartado 1 si recibe dir, f1, f2 y f_resultado
        concatenar_archivos(argv[1], argv[2], argv[3], argv[4]);
    } else if (argc == 2) {
        // Ejecuta Apartado 2 si solo recibe el nombre del directorio
        listar_archivos(argv[1]);
    } else {
        printf("Uso Apartado 1: %s <dir> <f1> <f2> <f_resultado>\n", argv[0]);
        printf("Uso Apartado 2: %s <dir>\n", argv[0]);
        return 1;
    }
    return 0;
}