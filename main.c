#include "funcoes.h"

int main(){
    int num_players,vlr_menu = 10,qnt_maos,
        vlr_blnd,ind_prm_set = 0,vlr_fichas;
    Jogadores *jogadores;
    inicializaTela("main.c");

    while(vlr_menu != 5){
        if(vlr_menu == 10)
            vlr_menu = menu();

        else if(vlr_menu == 1)
            vlr_menu = instrucoes();

        else if(vlr_menu == 2){
            jogadores = parametros(&num_players,&qnt_maos,&vlr_blnd,&vlr_fichas);
            ind_prm_set = 1;
            vlr_menu = 10;
        }
        else if(vlr_menu == 3){
            if(ind_prm_set == 0)
                jogadores = set_values(&num_players,&qnt_maos,&vlr_blnd,&vlr_fichas);

            simulacao(jogadores,num_players,qnt_maos,vlr_blnd,vlr_fichas);
            vlr_menu = 10;
        }
        else if(vlr_menu == 4){
            vlr_menu = estatisticas(jogadores,num_players);
        }
    }

    free(jogadores);
    jogadores = NULL;

    fechar();
    return 0;
}
