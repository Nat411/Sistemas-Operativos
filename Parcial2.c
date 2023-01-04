
/*Parcial 2 sistemas operativos 
Desarrollado Por: Natalia Andrea Alvares Hoyos y Juan Felipe L�pez Ram�rez Para la materia sistemas operativos 2022-2
*/
#include<stdio.h>
#include<stdbool.h>
#include <time.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/time.h>
//Se define la funci�n calc_t que realiza la el calculo del tiempo que tarda en hacer tlb hit o tlb miss 
float calc_t(struct timespec* start, struct timespec* end) {
    return (end->tv_sec - start->tv_sec) + 1e-9 * (end->tv_nsec - start->tv_nsec);
}
//La struct pag nos ser� de ayuda dado que es el tipo de dato que usaremos para manejar el TLB este contiene: La direcci�n de memoria virtual, el n�mero de p�gina y el desplazamiento 
struct pag {
    double dec;
    int pag;
    int des;
};

int main() {
    //variables para almacenar el intervalo de tiempo
    struct timespec inicio;
    struct timespec fin;


    char entrada;
    double decimal;
    double  division;
    char dato[30];
    int numero;
    int pagina, desplazamiento, i, indice, count = 0;
    double tamano = 4096;
    struct pag  p;
    //se crea el TLB empleando la funci�n malloc donde este contiene 10 espacios del tipo de struct pag que definimos en la parte de arriba
    struct pag* tlb = malloc(10 * sizeof(p));
    bool estado = false;


    while (true)
    {
        indice = 0, decimal = 0, division = 0;
        bool estado;

        //se hace el input para ingresar la direcci�n virtual
        printf("Ingrese el numero de direccion virtual: ");
        fgets(dato, 30, stdin);
        //se inicia el conteo de tiempo de traducci�n 
        clock_gettime(CLOCK_REALTIME, &inicio);
        
        //L�gica para hacer el cierre del programa si se ingresa "s"
        if (dato[0] == 's') {
            printf("Good bye!\n");
            return 0;
        }
        else {
            decimal = atoi(dato);
        }

        //aqu� se crea la politica de reemplazo de p�ginas la variable count va almacenando los indices ocupados del tlb, cuando este se llena se libera el tlb y se reinicia la variable count 
        if (count == 9) {
            free(tlb);
            count = 0;
        }


        //En este for iteramos el TLB  en b�squeda de la direcci�n que se nos pidi� en el input, si se encuentra la variable estado se actualiza a true para indicar m�s abajo en la ejecuci�n que ruta seguir 
        // Y la variable indice almacena el lugar del TLB donde se encuentra la direcci�n, pero si no la encuentra se queda en -1
        for (i = 0; i < 10; i++) {
            if (tlb[i].dec == decimal) {
                //Al haber encontrado la direcci�n se para el conteo del tiempo para que a la hora de imprimirlo la funci�n calc_t haga el calculo
                clock_gettime(CLOCK_REALTIME, &fin);
                estado = true;
                indice = i;
                break;
            }
            else {
                estado = false;
                indice = -1;
            }
        }

        //Si en el for se encontr� la direcci�n, se accede a ella y se imprime la p�gina y el desplazamiento 
        // Esto significa que se lleg� a un tlb hit 
        if (estado == true && indice >= 0) {
            printf("Numero de pagina: %i\n", tlb[indice].pag);
            printf("Desplazamiento: %i\n", tlb[indice].des);
            printf("Tiempo de traducion: %0.8f segundos\n", calc_t(&inicio, &fin));

            printf("TLB: Hit\n");
        }
        //Si en el for no se encontr� la direcci�n buscada o se lleg� a un tlb miss se entra en esta parte de la ejecuci�n
        //Donde se realizan los c�lculos de la p�gina, el desplazamiento y se agrega al TLB
        else if (!estado && indice < 0) {
            //calculo de p�gina=    (direcci�n de memoria virtual/ tama�o de p�gina)
            division = decimal / tamano;
            pagina = (int)division;
            //calculo desplazamiento= (calculo p�gina-calculo p�gina entero)*tama�o de p�gina 
            desplazamiento = (division - pagina) * tamano;
            //se agregan, la direcci�n, pagina y desplazmiento a nuestro struct
            p.dec = decimal;
            p.pag = pagina;
            p.des = desplazamiento;
            //se aumenta el contador que mantiene siguiente indice d�sponible del tlb
            count++;
            // y aqu� se agrega al TLB1
            tlb[count] = p;
            //Cuando se añade la direcci�n traducida al TLB se para este conteo del tiempo para realizar el procedimiento del calculo con la funci�n |

            clock_gettime(CLOCK_REALTIME, &fin);
            printf("Numero de pagina: %i\n", tlb[count].pag);
            printf("Desplazamiento: %i\n", tlb[count].des);
            printf("Tiempo de traducion: %0.8f segundos\n", calc_t(&inicio, &fin));

            printf("TLB: Miss\n");
        }

    }

}
