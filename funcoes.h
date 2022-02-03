#ifndef _funcoes_h
#define _funcoes_h
#define MAX_BARALHO 52

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tristto.h"
#include <windows.h>
#include <conio.h>
#include <pthread.h>
/*REGISTROS*/
typedef struct Cartas{
    char naipe;
    int num;
}Cartas;

typedef struct Mao{
    int valor;
    Cartas cartas[7];
}Mao;
/*1 TIGHT 2 LOOSE 3 AGRESSIVE*/
typedef struct Jogadores{
    Mao mao;

    int estilo;
    char nome[15];
    int fichas;
    int pos_rel; //1 dealer 2 small blind 3 big blind
    int bol_ativo;
    int wins;
}Jogadores;

/*ENUMS*/
typedef enum naipes{Copas,Ouros,Paus,Espadas}tipo_naipes;

typedef enum maos{Alta,Par,Double_Par,Trinca,Sequencia,Flush,Full_House,
                Quadra,Str_Flush,Roy_Flush}tipo_maos;

typedef enum cartas{Dois,Tres,Quatro,Cinco,Seis,Sete,Oito,Nove,Dez,
                    Valete,Rainha,Rei,Az}tipo_cartas;


/*FUNÇÕES*/

/*DO MENU*/

int menu();

int instrucoes();

Jogadores* parametros(int* num_players,int* qnt_maos,int* vlr_blnd,int* vlr_fichas);

int estatisticas();

void simulacao(Jogadores* jogadores,int num_players,int qnt_maos,int vlr_blnd,
               int vlr_fichas);

void prime_msg();

void fechar();

/*DE ANÁLISE DOS JOGOS*/
int atribui_num_naipe(char naipe);

int desempate(Mao* Mao1,Mao* Mao2,int vlr);

int checa_valor_mao(Mao *mao);

int checa_Roy_Flush(Mao *mao);

int checa_Str_Flush(Mao *mao);

int checa_Quadra(Mao *mao);

int checa_Full_House(Mao *mao);

int checa_Flush(Mao *mao);

int checa_Sequencia(Mao *mao);

int checa_Trinca(Mao *mao);

int checa_Double_Par(Mao *mao);

int checa_Par(Mao *mao);

/*DE JOGABILIDADE*/
void embaralha(Cartas *baralho);

void distribui_cartas(int* cont_baralho,Cartas* baralho,
                      Jogadores* jogador,
                      int n,Cartas* mesa);

//TYPE SERIA 1 PARA PRIMEIRA RONDA E 0 PARA QUALQUER OUTRA
void apostas(int type,Jogadores* jogadores,int num_players,
             int vlr_blnd,
             int* pot,int dealer);

//2 PARA FOLD 3 PARA COBRIR A APOSTA
int jogada_por_perfil(int estilo);
/*DA INTERFACE GRÁFICA*/
void print_nomes(Jogadores* jogadores,int num_players);

int print_carta(int x, int y,int numero, int naipe);

void print_tela(int num_players, Jogadores* jogadores,Cartas* mesa,int vlr_blnd,int dealer);

void print_dial();

/*AUXILIARES*/
int tam_center_str(char *str);  //RETORNA UMA POSIÇÃO CENTRALIZADA PARA INÍCIO DA STRING

int tam_center_dial(char* str); // POSIÇÃO CENTRALIZADA PARA O A CAIXA DE DIÁLOGO

int atribui_num_naipe(char naipe);

char atribui_naipe(int num);

void atribui_pos(Jogadores* jogadores,int num_players,
                 int dealer);

Jogadores* set_values(int* num_players,int* qnt_maos,int* vlr_blnd,int* vlr_fichas);

#endif
