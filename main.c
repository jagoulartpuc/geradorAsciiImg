#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int converteParaCinza(int red, int green, int blue) {
    int intensidade = 0.3 * red + 0.59 * green + 0.11 * blue;
    return intensidade;
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    int MatrizCinza[pic.width][pic.height];

    int fatorFixoPorEnquanto = 65;

    int numeroDePixelsPorCluster = 100/fatorFixoPorEnquanto;

    for(int i = 0; i< pic.width * pic.height; i++) {
        //printf("[%02d %02d %02d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
        //Converte pixel para cinza
        int pixel = converteParaCinza(pic.img[i].r, pic.img[i].g, pic.img[i].b);

        //Adiciona na matriz
        int posicaoAltura = i / pic.width;
        int posicaoLargura = i % pic.width;
        MatrizCinza[posicaoLargura][posicaoAltura] = pixel;
    }

    //Cria vetor reduzido com tamanho já calculado
    int matrizClusterizada[(pic.width/numeroDePixelsPorCluster)][(pic.height/numeroDePixelsPorCluster)];

    for(int i = 0; i< (pic.height/numeroDePixelsPorCluster); i++){
        for(int j = 0; j < (pic.width/numeroDePixelsPorCluster); j++){
                int contador = 0;
                int soma = 0;
                for(int k = (i*numeroDePixelsPorCluster); k < (i*numeroDePixelsPorCluster)+numeroDePixelsPorCluster; k++){
                    for(int l = (j*numeroDePixelsPorCluster); l < (j*numeroDePixelsPorCluster)+numeroDePixelsPorCluster; l++){
                        contador = contador + 1;
                        soma = soma + MatrizCinza[l][k];
                    }
                }
                matrizClusterizada[j][i] = (soma/contador);
                //printf("Linha: %d, Coluna: %d - Valor: %d \n", i, j, matrizClusterizada[j][i]);
        }
    }

    //Transformar nos caracteres correspondentes
    int j;
    char url[]="saida.html";
    FILE *arq;
    char matrizChar[(pic.width/numeroDePixelsPorCluster)][(pic.height/numeroDePixelsPorCluster)];
    arq=fopen(url, "w");
    fprintf(arq,
    "<html><head></head><body style=\"background:black;\" leftmargin=0 topmargin=0> <style> pre  {color: white;font-family: Courier;font-size: 2.5px;}</style><pre>");

    for(int i = 0; i< (pic.height/numeroDePixelsPorCluster); i++){
        for(j = 0; j < (pic.width/numeroDePixelsPorCluster); j++){
            if(matrizClusterizada[j][i] >= 0 && matrizClusterizada[j][i] <= 32){
                matrizChar[j][i] = '.';
            }
            else if(matrizClusterizada[j][i] > 32 && matrizClusterizada[j][i] <= 64){
                matrizChar[j][i] = ':';
            }
            else if(matrizClusterizada[j][i] > 64 && matrizClusterizada[j][i] <= 96){
                matrizChar[j][i] = 'c';
            }
            else if(matrizClusterizada[j][i] > 96 && matrizClusterizada[j][i] <= 128){
                matrizChar[j][i] = 'o';
            }
            else if(matrizClusterizada[j][i] > 128 && matrizClusterizada[j][i] <= 160){
                matrizChar[j][i] = '5';
            }
            else if(matrizClusterizada[j][i] > 160 && matrizClusterizada[j][i] <= 192){
                matrizChar[j][i] = 'x';
            }
            else if(matrizClusterizada[j][i] > 192 && matrizClusterizada[j][i] <= 224){
                matrizChar[j][i] = '#';
            }
            else if(matrizClusterizada[j][i] > 224 && matrizClusterizada[j][i] <= 255){
                matrizChar[j][i] = '@';
            }


            fputc(matrizChar[j][i], arq);
            fputc(matrizChar[j][i], arq);
        }


        fprintf(arq,"<br/>");
    }
    fprintf(arq,"</pre></body></html>");
    free(pic.img);
}
