//
//  main.c
//  Caixeiro Viajante
//
//  Created by Felipe Schwab on 6/15/16.
//  Copyright © 2016 Felipe Schwab. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct grafo
{
    int n_nohs;     // Numero de nohs do Grafo.
    int** nohs;      // Matriz de arestas do Grafo.
    int conexoes;      // Numero maximo de conexoes de uma cidades com outras cidades.
    float** tamanho;      // Matriz que indica o tamanho de cada aresta.
    int* grau;       // Grau do Grafo.
    
}Grafo;

Grafo *inicia_Grafo(int vertices, int n_conexoes)    // Função para inicializar o mapa das cidades.
{
    Grafo *mapa = (Grafo*)malloc(sizeof(Grafo));        // Alocação da memória.
    
    if( mapa != NULL)       // Verifica que o Mapa (Grafo) ainda nao existe.
    {
        int aux;
        mapa->n_nohs = vertices;
        
        mapa->grau = (int*) calloc(vertices, sizeof(int));     // Utilizou-se calloc pq inicialmente todos os nós tem grau zero
        
        mapa->nohs = (int**)malloc(vertices*sizeof(int*));       // Inicializa a matriz de nós.
        
        mapa->tamanho = (float**)malloc(vertices*sizeof(float*));       // Inicilaiza a matriz de tamanho das arestas.
        
        for (aux = 0; aux < vertices; aux++)
        {
            mapa->nohs[aux] = (int*)malloc(n_conexoes*sizeof(int));      // Inicializa o numero maximo de conexoes por nó.
            mapa->tamanho[aux] = (float*)malloc(n_conexoes*sizeof(float));  // Cria um vetor para cada nó com o tamanho de cada aresta.
        }
        
    }
    
    return mapa;
}

void deleta_Grafo (Grafo* mapa)
{
    if (mapa != NULL)
    {
        for (int i = 0; i < mapa->n_nohs; i++)
        {
            free(mapa->nohs[i]);
            free(mapa->tamanho[i]);
        }
        
        free(mapa->nohs);
        free(mapa->tamanho);
        free(mapa->grau);
        
        free(mapa);
    }
}

int InsereAresta(Grafo* mapa, int tam, int origem, int destino, int digrafo)
{
    if (mapa == NULL) return 0;      // Verifica se o Grafo existe.
    
    if (origem < 0 || origem >= mapa->n_nohs) return  0;        // Verifica se o nó de origem existe dentro do Grafo.
    
    if (destino < 0 || destino >= mapa->n_nohs) return 0;       // Verifica se o nó de destino existe dentro do Grafo.
    
    mapa->nohs[origem][mapa->grau[origem]] = destino;       // Insere na ultima posicao da lista adjacente de Nós o valor do Destino.
    mapa->tamanho[origem][mapa->grau[origem]] = tam;       // Insere na ultima posicao da lista adjacente de Tamanho o valor do tamanho da aresta.
    
    mapa->grau[origem]++;        // Incrementa o grau do nó origem.
    
    if (digrafo == 1)
    {
        InsereAresta(mapa, tam, destino, origem, 0);        // Cosiderando que todo caminho que vai, volta. Precisamos
                                                            // rodar duas vezes a funcao. Para criar as arestas de ida e volta.
    }
    
    return 1;
    
}

int DeletaAresta(Grafo* mapa, int origem, int destino, int digrafo)
{
    int cont = 0;
    
    if (mapa == NULL) return 0;      // Verifica se o Grafo existe.
    
    if (origem < 0 || origem >= mapa->n_nohs) return  0;        // Verifica se o nó de origem existe dentro do Grafo.
    
    if (destino < 0 || destino >= mapa->n_nohs) return 0;       // Verifica se o nó de destino existe dentro do Grafo.
    
    while (cont < mapa->grau[origem] && mapa->nohs[origem][cont] != destino)
    {
        cont++;
    }
    
    if (cont == mapa->grau[origem]) { return 0; }       // Se o contador for igual ao grau de origem
                                                        //ao sair do loop a aresta Destino nao foi encontrada.
    mapa->grau[origem]--;
    mapa->nohs[origem][cont] = mapa->nohs[origem][mapa->grau[origem]];      // Como a ordem nao importa. Para otimizar a exclusao o ultimo
    mapa->tamanho[origem][cont] = mapa->tamanho[origem][mapa->grau[origem]];// elemento eh posicionado no lugar do elemento que foi excluido.
    
    if (digrafo == 1)
    {
        DeletaAresta(mapa, destino, origem, 0);        // Cosiderando que todo caminho que vai, volta. Precisamos
                                                            // rodar duas vezes a funcao. Para criar as arestas de ida e volta.
    }
    
    return 1;
}

void MostraGrafo(Grafo *mapa)
{
    if (mapa != NULL)
    {
        for (int i = 0; i < mapa->n_nohs; i++)
        {
            for (int j = 0; j < mapa->grau[i]; j++)
            {
                printf("\n \n Vertice: %d \n Conexoes: \t\t %d \n Distancia: \t\t\t\t %.2f \n", i, mapa->nohs[i][j], mapa->tamanho[i][j]);
            }
            
        }
    }
}

int main(int argc, const char * argv[])
{

    Grafo *gr = inicia_Grafo(5, 5);
    InsereAresta(gr, 0, 0, 1, 1);
    InsereAresta(gr, 1, 1, 3, 1);
    InsereAresta(gr, 2, 1, 2, 1);
    InsereAresta(gr, 3, 2, 4, 1);
    InsereAresta(gr, 4, 3, 0, 1);
    InsereAresta(gr, 5, 3, 4, 1);
    InsereAresta(gr, 6, 4, 1, 1);

    free(gr);
    
    return 0;
}
