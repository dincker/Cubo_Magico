#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
/*#ifdef _OPENMP
 #include <omp.h>
 #define TRUE 1
 #define FALSE 0
#else
 #define omp_get_thread_num() 0
 #define omp_get_num_threads() 1
#endif
#define N 24
int tid, nthr; // identificador del thread y numero de threads
int i, A[N];
*/
//#define DECBIN(n,i) ((n&(1<<i))?1:0)
using namespace std;
void LlenarFichero();
int contarLineas();
int sumaDiagonal(int matriz[3][3]);
int sumaHorizontal(int matriz[3][3]);
int sumaVertical(int matriz[3][3]);
void transformar();
int chunksize,offset,dest,tag1,tag2,source;

int main(int argc,char* argv[])
{







    //transformar();
    int rank, size;
    //mpi_comm
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //cout << "La tarea MPI ha comenzado ... \ n"<< rank<<endl;

    MPI_Status status;


    


    if (rank == 0){

    	string * datos = NULL;
    //int rank,size;
    //argc=2;
    //omp_set_num_threads(2);
    //#pragma omp parallel for

    /*for(int i=0; i<100;i++){
        cout << i << endl;
    }*/
    int matriz[3][3];

    //cout << "Hello world!: "<<rank << endl;
    //LlenarFichero();
    cout<<"entra"<<endl;
    int lineas=contarLineas();
    cout<<lineas<<endl;
    cout<<"sale"<<endl;
    datos = new string[lineas];
    //string datos[lineas];

    ifstream fin("arreglo.txt");
    //ofstream salida;
    string cubo;
 cout<<"chao"<<endl;
    if(fin.is_open()){

        for(int i = 0; i<lineas;i++){
            getline(fin,cubo);
            datos[i]=cubo;
        }

    }else{
        cout << "error al abrir .txt" << endl;
    }
    /*for(int i=0;i<lineas;i++){
        cout << datos[i] <<endl;
    }*/

    chunksize=(lineas/size);
    //cout << size<<endl;
    
    offset==chunksize;
    for (dest=1; dest<size; dest++) {
    MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
    MPI_Send(&datos[offset], chunksize, MPI_FLOAT, dest, tag2, MPI_COMM_WORLD);
    MPI_Send(&lineas, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
    printf("Sent %d elements to task %d offset= %d\n",chunksize,dest,offset);
    offset = offset + chunksize;
    }


    tag2 = 1;
    tag1 = 2;

    int tam;

    char*cadena;
    for(int i=rank*chunksize;i<lineas;i++){

            //tamaño del string
            string lin =datos[i];
            tam=lin.size();
            //cout<<lin<<endl;
            //cout<<tam<<endl;
            //transformar string a char

            cadena = (char *)lin.c_str();
            //char cadena_aux[tam-3];
            //llena la matriz auxiliar
            //for(int i=1; i<tam-2;i++){
              //  cadena_aux[i-1]=cadena[i];
            //}
            int contador=0,inicio=0,fin,fila=0,columna=0;
            //rrecorre el arreglo con los datos del txt
            for(int i=0;i<tam;i++){
                //en el caso de que el arreglo en la posicion 0 tenga un corchete el inicia lo aumenta en 1
                if(cadena[i]=='['){
                    inicio=i+1;
                }
                if(cadena[i]==',' or cadena[i]==';' or cadena[i]==']'){
                    //la varible fin la guarda con el valor anterior de donde encontro un ;-,-]
                    fin=i-1;
                    //crea un char para almacenar cada numero del cubo magico
                    char numeros[fin-inicio-1];
                    //posicion sirve para apuntar a la posicion 0 del arreglo numeros
                    int posicion=0;
                    //for para guardar los numero encontrador en el text file y pasarlos a numeros
                    for(int a=inicio;a<=fin;a++){
                        numeros[posicion]=cadena[a];
                        posicion++;
                    }
                    //Asigna el numero almacenado en numeros a una posicion de la matriz que contiene el cuadrado
                    matriz[columna][fila]=atoi(numeros);
                    //reinicia la variable posicion para poder encontrar el siguiente numero
                    posicion=0;
                    //contador++;
                    //fila es para moverse en la matriz del cuadrado
                    fila++;
                    //cambia el valor de inicio a la posicion siguiente de encontrar un ;-,
                    inicio=i+1;
                    //si la fila es 3 reinicia la fila y ahumenta en 1 la columna
                    if(fila==3){
                        fila=0;
                        columna++;
                    }
                    //si la columna es 3 la vuelve a 0 para la siguiente linea del txt
                    if(columna==3){
                        columna=0;
                    }
                }
                //pos++;
            }
            //guarda el valor que retorna la funcion en una variable
            int v=sumaVertical(matriz);
            int h=sumaHorizontal(matriz);
            int d1=sumaDiagonal(matriz);
            //compara los resultados de la suma diagonal, verical y horizontal. Si son iguales es un cubo magico
            if(v==h and v==d1){

                cout<<"Cubo magico"<<endl;
                ofstream salida("CuadradosMagicos.txt",ios::app);
                //salida.open("Auxiliar".c_str(),ios::app);
                salida<<datos[i]<<endl;
                //cout << "Cubo Magico"<<endl;
                //system("pause");
                salida.close();
            }
        }



    }
    if (rank>0) {
        source = 0;
        MPI_Recv (& offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, & status);
        MPI_Recv (& datos [offset], chunksize, MPI_FLOAT, source, tag2,MPI_COMM_WORLD, & status);
        MPI_Recv (& lineas, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, & status);

        tag2 = 1;
    tag1 = 2;

    int tam;

    char*cadena;
    for(int i=rank*offset;i<lineas;i++){

            //tamaño del string
            string lin =datos[i];
            tam=lin.size();
            //cout<<lin<<endl;
            //cout<<tam<<endl;
            //transformar string a char

            cadena = (char *)lin.c_str();
            //char cadena_aux[tam-3];
            //llena la matriz auxiliar
            //for(int i=1; i<tam-2;i++){
              //  cadena_aux[i-1]=cadena[i];
            //}
            int contador=0,inicio=0,fin,fila=0,columna=0;
            //rrecorre el arreglo con los datos del txt
            for(int i=0;i<tam;i++){
                //en el caso de que el arreglo en la posicion 0 tenga un corchete el inicia lo aumenta en 1
                if(cadena[i]=='['){
                    inicio=i+1;
                }
                if(cadena[i]==',' or cadena[i]==';' or cadena[i]==']'){
                    //la varible fin la guarda con el valor anterior de donde encontro un ;-,-]
                    fin=i-1;
                    //crea un char para almacenar cada numero del cubo magico
                    char numeros[fin-inicio-1];
                    //posicion sirve para apuntar a la posicion 0 del arreglo numeros
                    int posicion=0;
                    //for para guardar los numero encontrador en el text file y pasarlos a numeros
                    for(int a=inicio;a<=fin;a++){
                        numeros[posicion]=cadena[a];
                        posicion++;
                    }
                    //Asigna el numero almacenado en numeros a una posicion de la matriz que contiene el cuadrado
                    matriz[columna][fila]=atoi(numeros);
                    //reinicia la variable posicion para poder encontrar el siguiente numero
                    posicion=0;
                    //contador++;
                    //fila es para moverse en la matriz del cuadrado
                    fila++;
                    //cambia el valor de inicio a la posicion siguiente de encontrar un ;-,
                    inicio=i+1;
                    //si la fila es 3 reinicia la fila y ahumenta en 1 la columna
                    if(fila==3){
                        fila=0;
                        columna++;
                    }
                    //si la columna es 3 la vuelve a 0 para la siguiente linea del txt
                    if(columna==3){
                        columna=0;
                    }
                }
                //pos++;
            }
            //guarda el valor que retorna la funcion en una variable
            int v=sumaVertical(matriz);
            int h=sumaHorizontal(matriz);
            int d1=sumaDiagonal(matriz);
            //compara los resultados de la suma diagonal, verical y horizontal. Si son iguales es un cubo magico
            if(v==h and v==d1){

                cout<<"Cubo magico"<<endl;
                ofstream salida("CuadradosMagicos.txt",ios::app);
                //salida.open("Auxiliar".c_str(),ios::app);
                salida<<datos[i]<<endl;
                //cout << "Cubo Magico"<<endl;
                //system("pause");
                salida.close();
            }
        }


        //mysum = update (offset, chunksize, taskid);

        // * Enviar mis resultados a la tarea maestra * /
        //dest = MASTER;
        //MPI_Send (& offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
        //MPI_Send (& data [offset], chunksize, MPI_FLOAT, MASTER, tag2, MPI_COMM_WORLD);

        //MPI_Reduce (& mysum, & sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}

void transformar(){
    string nfin="matrices.txt";
    char cubo;
    ifstream fin;
    fin.open(nfin.c_str());

    if(!fin.fail()){
        fin.get(cubo);
        ofstream salida("Arreglo.txt",ios::app);
        while(!fin.eof()){
            //cout<< cubo<<endl;


            //salida.open("Auxiliar".c_str(),ios::app);

            if(cubo!=']'){
                salida<<cubo;
            }else{
                salida<<cubo<<":";
            }
            fin.get(cubo);
            //cout << "Cubo Magico"<<endl;
            //system("pause");

        }salida.close();


}
fin.close();
}


void LlenarFichero(){
    //Crear fichero
    ofstream fout("Cuadrados.txt");
    srand(time(NULL));
    //escribir en fichero
    for(int i=0;i<1000000;i++){
        fout << "["<<(1 + rand() % 100);
        fout<<","<<(1 + rand() % 100);
        fout<<","<<(1 + rand() % 100);
        fout<<";"<<(1 + rand() % 100);
        fout<<","<<(1 + rand() % 100);
        fout<<","<<(1 + rand() % 100);
        fout<<";"<<(1 + rand() % 100);
        fout<<","<<(1 + rand() % 100);
        fout<<","<<(1 + rand() % 100);
        fout<<"]:"<<endl;
    }
    fout.close();
}
int contarLineas(){
    ifstream fin("arreglo.txt");
    string cubo;
    int lineas=0;
    if(fin.is_open()){
        while(getline(fin,cubo)){


            lineas++;

        }

    }else{
        cout << "error al abrir Cuadrados.txt" << endl;
    }
    fin.close();
    return lineas;

}

int sumaVertical(int matriz[3][3]){
    //Horizonte magico
    int suma=0,aux=0,cont=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<matriz[i][j]<<" ";
            suma=suma+matriz[j][i];
        }
        if(aux==0){
            aux=suma;
            suma=0;
        }else{
            if(aux==suma){
                cont=cont+1;
                suma=0;
            }else{
                suma=0;
            }
        }
        cout<<endl;
    }
    if(cont==2){
        cout << "Vertical magico"<<endl;
        system("pause");
        return aux;
    }else{
        return 0;
    }
}

//funcion para comprobar que las sumas verticales sean iguales
int sumaHorizontal(int matriz[3][3]){
    //Horizonte magico
    int suma=0,aux=0,cont=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            //cout<<matriz[i][j]<<" ";
            suma=suma+matriz[i][j];
        }
        if(aux==0){
            aux=suma;
            suma=0;
        }else{
            if(aux==suma){
                cont=cont+1;
                suma=0;
            }else{
                suma=0;
            }
        }
        //cout<<endl;
    }
    if(cont==2){

        cout << "horizonte magico"<<endl;
        system("pause");
        return aux;
    }else{
        return 1;
    }

}


//funcion para comprobar que las sumas diagonales sean iguales
int sumaDiagonal(int matriz[3][3]){
    int suma=0,aux=0,suma2=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(i==j){
                suma=suma+matriz[i][j];
            }
            aux=3-1;
            if(i+j==aux){
                suma2=suma2+matriz[i][j];
            }
        }
    }
    if(suma==suma2){
        return suma;
    }else{
        return 2;
    }
}
