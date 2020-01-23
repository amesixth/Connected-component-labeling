#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>

using namespace std;
using namespace cv;

// g++ $(pkg-config --cflags --libs opencv4) -std=c++11  main.cpp -o main
// "/Users/Karla/Documents/Computo-Paralelo/Proyecto-Secuencial/images/Lenna.png"

int main(int, char**) {

    cout << "Please enter an image filename : ";
    string img_addr;
    cin >> img_addr;

    Mat image = imread(img_addr, IMREAD_GRAYSCALE);
    if(!image.data) {
        cout << "Error: the image wasn't correctly loaded." << endl;
        return -1;
    }

    list <int> labels;
    labels.push_back(0);

    list <vector<int>> unions;

    //Declarar la matriz
    int **matriz;
    matriz = (int**)malloc(image.rows*sizeof(int*));

    for(int i=0;i<image.rows;i++){
        matriz[i] = (int*)malloc(image.cols*sizeof(int));
        for(int j=0;j<image.cols;j++){
            matriz[i][j]=0;
        }
    }
    int background = image.at<uchar>(0,0);

    // *** FIRST PASS ***

    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++) {

            if(image.at<uchar>(i,j) == background){
                matriz[i][j] = 0;
                continue;
            }
            
            if(i==0){
                if(j==0){
                    matriz[i][j]=labels.back();
                    
                }else{
                    if(matriz[i][j-1]==0){
                        labels.push_back(labels.back()+1);
                        matriz[i][j]=labels.back();
                        
                    }
                    else{
                        matriz[i][j]=matriz[i][j-1];
                        
                    }
                }
            }else{
                if(j==0){
                    if(matriz[i-1][j]==0){
                        labels.push_back(labels.back()+1);
                        matriz[i][j]=labels.back();
                        
                    }else{
                        matriz[i][j]=matriz[i-1][j];
                        
                    }
                }else{
                    int left = matriz[i][j-1];
                    int up = matriz[i-1][j];

                    if(left==0 && up==0){
                        labels.push_back(labels.back()+1);
                        matriz[i][j]=labels.back();
                        
                    }else if(left==0){
                        matriz[i][j]=up;
                        
                    }else if(up == 0){
                        matriz[i][j]=left;
                        
                    }else if(left == up){
                        matriz[i][j]=up;
                        
                    }else{
                        //Union de regiones
                        int auxMin = min(left,up);
                        int auxMax = max(left,up);
                        matriz[i][j] = auxMin;
                        vector<int> aux = {auxMax,auxMin};
                        unions.push_back(aux);
                    }
                }
            }
        }
    }

    // *** ORDENAR LAS UNIONES (MENOR A MAYOR) *** Elimina repetidos ***
    unions.sort(); unions.unique();

    // *** MAYOR A MENOR ***
    list <vector<int>> listAux;
    for (list <vector<int>>::iterator it=unions.begin(); it != unions.end(); ++it){
        listAux.push_front(*it);
    }
    unions = listAux;
    
    // *** SECOND PASS ***
    // *** REEMPLAZO ***

    for(int i = 0; i < image.rows ; i++){
        for(int j = 0; j < image.cols ; j++){
            if(matriz[i][j]!=0){
                //Buscar en las uniones
                for (list <vector<int>>::iterator it=unions.begin(); it != unions.end(); ++it){
                    vector<int> aux = *it;
                    if(aux.at(0)==matriz[i][j]){
                        matriz[i][j] = aux.at(1);
                    }
                }
            }
        }
    }

    list<int> regiones;

    for(int i = 0; i < image.rows ; i++){
        for(int j = 0; j < image.cols ; j++){
            if(matriz[i][j]!=0){
                regiones.push_back(matriz[i][j]);
            }
        }
    }

    regiones.sort(); regiones.unique();

    Mat output;
    image.copyTo(output);

    for(int i = 0; i < image.rows ; i++){
        for(int j = 0; j < image.cols ; j++){
            if(matriz[i][j]!=0){
                for(list <int>::iterator it=regiones.begin();it !=regiones.end();it++){
                    if(*it == matriz[i][j]){
                        output.at<uchar>(i,j) = *it * 20;
                    }
                }
            }else{
                output.at<uchar>(i,j) = background;
            }
        }
    }

    cv::imshow("Connected Component", output);
    cv::imshow("Original", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}