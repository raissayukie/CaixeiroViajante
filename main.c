//
//  main.c
//  Caixeiro Viajante
//
//  Created by FELIPE SCHWAB e PEDRO PEREIRA on 15/6/16.
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
    char* nome;        //Matriz de nomes.
    
}Grafo;



Grafo *inicia_Grafo(int vertices, int n_conexoes)    // Função para inicializar o mapa das cidades.
{
    Grafo *mapa = (Grafo*)malloc(sizeof(Grafo));        // Alocação da memória.
    
    if( mapa != NULL)       // Verifica que o Mapa (Grafo) ainda nao existe.
    {
        int aux;
        mapa->n_nohs = vertices;
        
        mapa->grau = (int*) calloc(vertices, sizeof(int));     // Utilizou-se calloc pq inicialmente todos os nós tem grau zero
        
        mapa->conexoes = n_conexoes;
        
        mapa->nohs = (int**)malloc(vertices*sizeof(int*));              // Inicializa a matriz de nós.
        
        mapa->tamanho = (float**)malloc(vertices*sizeof(float*));       // Inicilaiza a matriz de tamanho das arestas.
        
        mapa->nome = (char*)malloc(vertices*sizeof(char*));
        
        for (aux = 0; aux < vertices; aux++)
        {
            mapa->nohs[aux] = (int*)malloc(n_conexoes*sizeof(int));         // Inicializa o numero maximo de conexoes por nó.
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

int InsereAresta(Grafo* mapa, int tam, int origem, int destino, int digrafo, char cidade)
{
    if (mapa == NULL) return 0;      // Verifica se o Grafo existe.
    
    if (origem < 0 || origem > mapa->n_nohs) return  0;        // Verifica se o nó de origem existe dentro do Grafo.
    
    if (destino < 0 || destino > mapa->n_nohs) return 0;       // Verifica se o nó de destino existe dentro do Grafo.
    
    mapa->nohs[origem][mapa->grau[origem]] = destino;       // Insere na ultima posicao da lista adjacente de Nós o valor do Destino.
    mapa->tamanho[origem][mapa->grau[origem]] = tam;       // Insere na ultima posicao da lista adjacente de Tamanho o valor do tamanho da aresta.
    
    mapa->grau[origem]++;        // Incrementa o grau do nó origem.
    
    if (digrafo == 1)
    {
        InsereAresta(mapa, tam, destino, origem, 0, cidade);        // Cosiderando que todo caminho que vai, volta. Precisamos
                                                                    // rodar duas vezes a funcao. Para criar as arestas de ida e volta.
        mapa->nome[origem] = cidade;
    }
    
    return 1;
    
}


void AddCidade (Grafo* Mapa, char comeco)
{
    char fim, aux;
    int origem, destino, tam, c;
    
    origem = tam = 0;
    destino = -1;
    
    for (int i = 0; i <= Mapa->n_nohs; i++)
    {
        if(Mapa->grau[i] == 0)
        {
            origem = i;
            break;
        }
    }
    
    printf("\n\t %c possui caminho para qual cidade? \n ", comeco);
    printf(">>");
    scanf("%c", &fim);
    getchar();
    
    for (int i = 0; i <= Mapa->nome[i]; i++)
    {
        if(Mapa->nome[i] == fim)
        {
            destino = i;
            break;
        }
    }
    
    if (destino != -1)
    {
        printf("\n\t Qual a distancia de %c para %c?\n ", comeco, fim);
        printf(">>");
        scanf("%d", &tam);
        getchar();
        
        c = InsereAresta(Mapa, tam, origem, destino, 1, comeco);
        
        if (c == 1)
        {
            printf("\n\n Deseja adicionar mais algum caminho partindo de %c? (S/N) ", comeco);
            scanf("%c", &aux);
            getchar();
            
            if (aux == 'S' || aux == 's')
            {
                AddCidade(Mapa, comeco);
            }
        }
    }
    
    else printf("\n\n ----- Cidades nao existentes -------");
    
    
}

int DeletaAresta(Grafo* mapa, int origem, int destino, int digrafo)
{
    int cont = 0;
    
    if (mapa == NULL) return 0;      // Verifica se o Grafo existe.
    
    if (origem < 0 || origem > mapa->n_nohs) return  0;        // Verifica se o nó de origem existe dentro do Grafo.
    
    if (destino < 0 || destino > mapa->n_nohs) return 0;       // Verifica se o nó de destino existe dentro do Grafo.
    
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

void DelAresta(Grafo* Mapa)
{
    char comeco, fim, aux;
    int origem, destino, tam;
    
    origem = tam = 0;
    destino = -1;

    printf("\n\t Informe o nome da cidade na qual o caminho a ser deletado comeca.\n ");
    printf(">>");
    scanf(" %c", &comeco);
    getchar();
    
    for (int i = 0; i <= Mapa->nome[i]; i++)
    {
        if(Mapa->nome[i] == comeco)
        {
            origem = i;
            break;
        }
    }
    
    printf("\n\t %c O caminho para qual cidade deve ser deletado? \n ", comeco);
    printf(">>");
    scanf(" %c", &fim);
    getchar();
    
    for (int i = 0; i <= Mapa->nome[i]; i++)
    {
        if(Mapa->nome[i] == fim)
        {
            destino = i;
            break;
        }
    }
    
    aux = DeletaAresta(Mapa, origem, destino, 1);
    
    if (aux == 1)
    {
        printf("\t\t\n ------- Caminho deletado com sucesso! ------- \n\n");
    }
    
    else printf("\t\t\n ------- Caminho nao existe! ------- \n\n");
    

}

void MostraGrafo(Grafo *mapa)
{
    if (mapa != NULL)
    {
        for (int i = 0; i <= mapa->n_nohs; i++)
        {
            for (int j = 0; j < mapa->grau[i]; j++)
            {
                printf("\n \n Caminho de %c (%d) para %c (%d) \n\t Distancia: \t %.2f \n ", mapa->nome[i], i, mapa->nome[mapa->nohs[i][j]],
                       mapa->nohs[i][j], mapa->tamanho[i][j]);
            }
            
        }
    }
}

void Menu (Grafo* Mapa)
{
    int op;
    char comeco, fim;
    int origem, destino, tam, aux;
    
    aux = origem = destino = tam = op = 0;
    comeco = 'Z';
    fim = comeco;
    
    while (op != 9)
    {
        printf("\n\n Escolha uma das opcoes abaixo: ");
        printf("\n\t 1- Adicionar cidade.");
        printf("\n\t 2- Deletar caminho.");
        printf("\n\t 3- Mostra cidades.");
        printf("\n\t 4- Buscar menor caminho.");
        printf("\n\t 9- Sair.");
        printf("\n >>");
        scanf(" %d", &op);
        getchar();
    
        switch (op)
        {
            case 1:
                printf("\n\t Informe o nome cidade.\n ");
                printf(">>");
                scanf(" %c", &comeco);
                getchar();

                AddCidade(Mapa, comeco);
                break;
                
            case 2:
                DelAresta(Mapa);
                break;
                
            case 3:
                MostraGrafo(Mapa);
                break;
                
            case 4:
                break;
            
            case 9:
                printf(" \n\n\t -------- FIM DE PROGRAMA ------- \n\n");
                break;
                
            default:
                printf("\n\t ------- OPCAO INVALIDA ------ \n");
                break;
        }
    }
}


int main(int argc, const char * argv[])
{

    Grafo *Mapa = inicia_Grafo(15, 5);
    
    InsereAresta(Mapa, 0, 0, 1, 1, 'A');
    InsereAresta(Mapa, 1, 1, 2, 1, 'B');
    InsereAresta(Mapa, 2, 2, 3, 1, 'C');
    InsereAresta(Mapa, 3, 3, 0, 1, 'D');
    InsereAresta(Mapa, 4, 4, 0, 1, 'E');
    
    Menu(Mapa);
  
    deleta_Grafo(Mapa);
    
    return 0;
}

