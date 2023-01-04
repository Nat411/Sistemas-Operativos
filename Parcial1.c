#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


    int main(int argc, char *args[]){
        /* Se definen las tuberias, donde padre es la que recibe la producc�on del proceso padre
        e hijo es la que recibe la produccion del proceso hijo
        */
        int padre[2];
        int hijo[2];
        pipe(padre); //Se inicializan la tuberia padre
        pipe(hijo); // Se inicializa la tuberia hijo
        if ( padre < 0 || hijo < 0){
            perror("Se ha generado un error al crear alguna de las tuberias tuberias\n");
            return (1);
        }
        //creamos proceso hijo
        int mayusculas = fork();

        if (mayusculas < 0) {
            printf("Fallo al crear el proceso hijo");
            return(1);
        }

        //proceso hijo
        else if (mayusculas == 0){
            //creamos buffers de entrada y salida del proceso hijo donde mayus es el de salida y minus el de entrada
            char buf_minus_h[256];
            char buf_mayus_h[256];
            while (true) {
                //se define y limpia el buffer de entrada
                memset(buf_minus_h, 0, sizeof(buf_minus_h));
                //se lee la informacion enviada por padre y se guarda en buffer de entrada hijo
                read(padre[0], &buf_minus_h, sizeof(buf_minus_h));
                //Se limpia el buffer de salida
                memset(buf_mayus_h, 0, sizeof(buf_mayus_h));
                int i = 0;
                //ciclo para convertir la cadena de texto a mayusculas
                while (buf_minus_h[i]) {
                    buf_mayus_h[i] = toupper(buf_minus_h[i]);
                    i++;
                }
                //se escribe la informacion que el hijo produjo en la tuberia 
                write(hijo[1], &buf_mayus_h, sizeof(buf_mayus_h));
                
            }

            }
        
        else{
            //creamos buffers de entrada y salida para proceso padre, donde minus es el de entrada y mayus el de salida
            char buf_minus_p[256];
            char buf_mayus_p[256];

            while(true){
                printf("Ingrese la cadena de texto: ");
                //Se limpia el buffer de entrada
                memset(buf_minus_p, 0, sizeof(buf_minus_p));
                //Se hace el input de la cadena
                fgets(buf_minus_p, sizeof(buf_minus_p),stdin);
                // escribimos la cadena en la tuberia para que la reciba proceso hijo 
                write(padre[1], &buf_minus_p, sizeof(buf_minus_p));
                // Se limpia el bufer de salida
                memset(buf_mayus_p, 0, sizeof(buf_mayus_p));
                // leemos e imprimimos el buffer que envio el proceso hijo
                read(hijo[0], &buf_mayus_p, sizeof(buf_mayus_p));
                // Se imprime la cadena de texto en mayucula
                printf("%s", buf_mayus_p);

            }
        }
        //cerrramos tuberias
        close(padre[0]);
        close(padre[1]);
        close(hijo[0]);
        close(hijo[1]);
        //terminamos ejecucion 
        return 0;
        
       
    }
