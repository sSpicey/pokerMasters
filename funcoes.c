
/* ==============================================ESTE DOCUMENTO DEFINE AS FUNÇÕES DECLARADAS NO "funcoes.h"===================================== */
#include "funcoes.h"

/*NAIPE = 0 -> ESPADAS =//= NAIPE = 1-> PAUS =//= NAIPE = 2 -> COPAS =//= NAIPE = 3 -> OUROS*/

/*FUNÇÕES AUXILIARES*/

//RETORNA O TAMANHO PARA CENTRALIZAR A STRING
int tam_center_str(char *str){
    return METADE_LARGURA_TELA - strlen(str)/2;
}

int tam_center_dial(char* str){
    return 51 - strlen(str)/2;
}
char atribui_naipe(int num){

    char ch;

    if(num == Espadas)
        ch = 'e';
    else if(num == Paus)
        ch = 'p';
    else if(num == Copas)
        ch = 'c';
    else if(num == Ouros)
        ch = 'o';

    return ch;
}
int atribui_num_naipe(char naipe){
    int num;

    if(naipe == 'c')
        num = Copas;
    else if(naipe == 'e')
        num = Espadas;
    else if(naipe == 'o')
        num = Ouros;
    else if(naipe == 'p')
        num = Paus;

    return num;
}

//ATRIBUI DEALER, SB E BB
void atribui_pos(Jogadores* jogadores,int num_players,int dealer){
    int i;

    if(num_players != 2){
        for(i=0;i<num_players;i++)
                jogadores[i].pos_rel = 0;

        if(dealer == num_players)
            dealer = 0;

        if(dealer == num_players - 2){
            jogadores[dealer].pos_rel = 1;
            jogadores[num_players - 1].pos_rel = 2;
            jogadores[0].pos_rel = 3;
        }
        else if(dealer == num_players - 1){
            jogadores[dealer].pos_rel = 1;
            jogadores[0].pos_rel = 2;
            jogadores[1].pos_rel = 3;
        }
        else{
            jogadores[dealer].pos_rel = 1;
            jogadores[dealer + 1].pos_rel = 2;
            jogadores[dealer + 2].pos_rel = 3;
        }
    }
    else{
        if(dealer == 1){
            jogadores[1].pos_rel = 1;
            jogadores[0].pos_rel = 2;
        }
        else if(dealer == 0){
            jogadores[0].pos_rel = 1;
            jogadores[1].pos_rel = 2;
        }
    }
}

/*FUNÇÃO QUE EMBARALHA AS CARTAS*/
void embaralha(Cartas *baralho){
    Cartas cartas_usadas[MAX_BARALHO];

    int i, j, num_naipe, num_carta,
        ind_teste, cont_usadas = 0;

    char naipe;

    srand(time(NULL)); //INICIALIZAÇÃO DA SEMENTE DE ALEATORIEDADE

    for(i=0;i<MAX_BARALHO;i++){
        num_naipe = rand()%4;
        num_carta = rand()%13;

        /*LOOP PARA TESTAR SE A CARTA JA FOI ATRIBUIDA*/
        do{
            ind_teste = 0;
            for(j=0;j<MAX_BARALHO;j++){
                if(cartas_usadas[j].num == num_carta && cartas_usadas[j].naipe == num_naipe){
                    ind_teste = 1;
                    num_naipe = rand()%4;
                    num_carta = rand()%13;
                    break;
                }
            }
        }while(ind_teste == 1);
        /*============================================*/

        naipe = atribui_naipe(num_naipe);

        baralho[i].num = num_carta;
        baralho[i].naipe = naipe;

        cartas_usadas[cont_usadas].num = num_carta;
        cartas_usadas[cont_usadas].naipe = num_naipe;

        cont_usadas++;

    }
}
/*FUNÇÕES PARA ANÁLISE DE MÃOS E VALORES DE JOGOS*/
/* FUNCAO DE DESEMPATE */
int desempate(Mao *Mao1,Mao *Mao2,int vlr)
{
    int i,j,aux=0,aux2=0;
    int forca1=0,forca2=0, kicker1=0, kicker2=0;
    if(vlr==8)                       //DESEMPATE NO CASO DE DOIS STRAIGHT FLUSH
    {
        for(i=0;i<7;i++){
            for(j=0;j<7;j++)          // LOOPA AS DUAS MAOS PROCURANDO AS CARTAS QUE FAZEM PARTE DO STRAIGHT FLUSH E SOMANDO SEUS VALORES EM UM INTEIRO AUX
            {
                if(Mao1->cartas[i].naipe==Mao1->cartas[j].naipe && Mao1->cartas[i].num==Mao1->cartas[j].num-1)
                    aux = aux + Mao1->cartas[i].num;
                if(Mao2->cartas[i].naipe==Mao2->cartas[j].naipe && Mao2->cartas[i].num==Mao2->cartas[j].num-1)
                    aux2 = aux2 + Mao2->cartas[i].num;
            }
        }
        if(aux>aux2)    //RETORNA 1 CASO A PRIMEIRA MAO TENHA A MAIOR SEQUENCIA E 2 SE A SEGUNDA MAO TIVER A MAIOR SEQUENCIA
            return 1;
        else
            return 2;
    }
    if(vlr==7)                     //DESEMPATE NO CASO DE DUAS QUADRAS
    {
        for(i=0;i<4;i++){
            for(j=0;j<7;j++) //COMPARA AS MAOS EM SI PARA DETERMINAR O VALOR DA QUADRA
            {
                if(Mao1->cartas[i].num==Mao1->cartas[j].num)
                    aux++;
                if(Mao2->cartas[i].num==Mao2->cartas[j].num)
                    aux2++;
                if(aux>=4 && aux2>=4){
                    forca1 = Mao1->cartas[i].num; //GUARDA O VALOR DA QUADRA DE CADA JOGADOR
                    forca2 = Mao2->cartas[i].num;
                    if(forca1>forca2)  // COMPARA A FORÇA DAS QUADRAS E DETERMINA O VENCEDOR
                        return 1;
                    if(forca2>forca1)
                        return 2;
                    if(forca1==forca2) // CASO AS DUAS QUADRAS TENHAM A MESMA FORÇA, DETERMINA O KICKER E COMPARA
                    {
                        for(i=0;i<7;i++) // VARRE AS MAOS PROCURANDO O KICKER
                        {
                            if(Mao1->cartas[i].num!=forca1)
                                if(Mao1->cartas[i].num>kicker1)
                                    kicker1 = Mao1->cartas[i].num;
                            if(Mao2->cartas[i].num!=forca2)
                                if(Mao2->cartas[i].num>kicker2)
                                    kicker2 = Mao2->cartas[i].num;
                        }
                        if(kicker1>kicker2) // COMPARA OS KICKERS E DETERMINA O VENCEDOR OU EMPATE
                            return 1;
                        if(kicker2>kicker1)
                            return 2;
                        if(kicker1==kicker2)
                            return 3;
                    }
                }
            }
            aux = 0;
            aux2 = 0;
        }
    }
    if(vlr==6) // DESEMPATE NO CASO FULL HOUSE
    {
        for(i=0;i<7;i++)
        {
            aux = 0;
            aux2 = 0;
            for(j=0;j<7;j++) // COMPARA AS MAOS EM SI PARA DETERMINAR A FORÇA DA TRINCA
            {
                if(Mao1->cartas[i].num==Mao1->cartas[j].num)
                    aux++;
                if(Mao2->cartas[i].num==Mao2->cartas[j].num)
                    aux2++;
                if(aux==3 && forca1==0)
                    forca1 = Mao1->cartas[i].num;     // GUARDA A FORCA DA TRINCA
                if(aux2==3 && forca2==0)
                    forca2 = Mao2->cartas[i].num;
            }
            if(forca1!=0 && forca2!=0)             // PARA O LOOP ASSIM Q A FORÇA DA TRINCA FOR DETERMINADA
            {
                if(forca1>forca2)
                    return 1;
                if(forca2>forca1)
                    return 2;
                if(forca1==forca2)    //NO CASO DE EMPATE ENTRE AS TRINCAS, DETERMINAR O PAR MAIS FORTE
                {
                    aux = aux2 = 0;
                    for(i=0;i<7;i++) // LOOP PARA DETERMINAR O PAR
                    {
                        for(j=0;j<7;j++)
                        {
                            if(Mao1->cartas[i].num!=forca1 && Mao1->cartas[j].num!=forca1 && Mao1->cartas[i].num==Mao1->cartas[j].num)
                                aux++;
                            if(Mao2->cartas[i].num!=forca1 && Mao2->cartas[j].num!=forca1 && Mao2->cartas[i].num==Mao2->cartas[j].num)
                                aux2++;
                        }
                        if(aux>=2)       //GUARDA A FORÇA DO PAR
                            forca1 = Mao1->cartas[i].num;
                        if(aux2>=2)
                            forca2 = Mao2->cartas[i].num;
                    }
                    if(forca1>forca2)
                        return 1;
                    if(forca2>forca1)
                        return 2;
                    if(forca1==forca2)
                        return 3;
                }
            }
        }

    }
    if(vlr==5) //DESEMPATE NO CASO FLUSH
    {
        char naipe1 = ' ', naipe2 = ' ';
        int maior1 = 0, maior2 = 0;
        for(i=0;i<3;i++){
                aux = 0;
                aux2 = 0;
            for(j=0;j<7;j++){   // VARRE AS MAOS BUSCANDO O NAIPE DO FLUSH
                if(Mao1->cartas[i].naipe == Mao1->cartas[j].naipe)
                    aux++;
                if(Mao2->cartas[i].naipe == Mao2->cartas[j].naipe)
                    aux2++;
            }
            if(aux>=5 && naipe1 == ' '){  // QUANDO O NAIPE FOR DETERMINADO, VARRE A MAO BUSCANDO A CARTA DE MAIOR VALOR PRESENTE NO FLUSH
                naipe1 = Mao1->cartas[i].naipe;
                for(i=0;i<7;i++)
                {
                    if(Mao1->cartas[i].naipe == naipe1 && Mao1->cartas[i].num>maior1)
                        maior1 = Mao1->cartas[i].num;
                }
            }
            if(aux2>=5 && naipe2 == ' '){
                naipe2 = Mao2->cartas[i].naipe;
                for(i=0;i<7;i++)
                {
                    if(Mao2->cartas[i].naipe == naipe2 && Mao2->cartas[i].num>maior2)
                        maior2 = Mao2->cartas[i].num;
                }
            }
            if(maior1!=0 && maior2!=0)  // COMPARA AS MAIORES CARTAS DO FLUSH PARA DETERMINAR A MAO VENCEDORA
            {
                if(maior1>maior2)
                    return 1;
                if(maior2>maior1)
                    return 2;
                if(maior1==maior2)
                    return 3;
            }
        }
    }
    if(vlr==4) //DESEMPATE NO CASO SEQUENCIA
    {

    }
    if(vlr==3) //DESEMPATE NO CASO TRINCA
    {
        for(i=0;i<7;i++)
        {
            aux = 0;
            aux2 = 0;
            for(j=0;j<7;j++) // VARRE AS MÃOS PROCURANDO A TRINCA
            {
                if(Mao1->cartas[i].num==Mao2->cartas[j].num)
                    aux++;
                if(Mao2->cartas[i].num==Mao2->cartas[j].num)
                    aux2++;
            }
            if(aux>=3 && forca1 == 0)  // AO DETERMINAR A TRINCA, GUARDA SEU VALOR
                forca1 = Mao1->cartas[i].num;
            if(aux2>=3 && forca2 == 0)
                forca2 = Mao2->cartas[i].num;
            if(forca1 != 0 && forca2 != 0) // SE AS DUAS TRINCAS FOREM DETERMINADAS, COMPARA-SE O VALOR DAS DUAS
            {
                if(forca1>forca2)
                    return 1;
                if(forca2>forca1)
                    return 2;
                if(forca1==forca2) // CASO AS TRINCAS TENHAM O MESMO VALOR, DETERMINA-SE O KICKER
                {
                    for(i=0;i<7;i++)
                    {
                        if(Mao1->cartas[i].num != forca1)
                            if(Mao1->cartas[i].num > kicker1)
                                kicker1 = Mao1->cartas[i].num;
                        if(Mao2->cartas[i].num != forca2)
                            if(Mao2->cartas[i].num > kicker2)
                                kicker2 = Mao2->cartas[i].num;
                    }
                    if(kicker1>kicker2) // COMPARA OS KICKERS DE CADA MAO P/ DETERMINAR O VENCEDOR
                        return 1;
                    if(kicker2>kicker1)
                        return 2;
                    if(kicker1==kicker2) // CASO OS KICKERS TENHAM A MESMA FORCA, DETERMINA-SE O SEGUNDO KICKER
                    {
                        int v = kicker1;
                        kicker1 = 0;
                        kicker2 = 0;
                        for(i=0;i<7;i++)
                        {
                            if(Mao1->cartas[i].num != forca1 && Mao1->cartas[i].num != v)
                                if(Mao1->cartas[i].num>kicker1)
                                    kicker1 = Mao1->cartas[i].num;
                            if(Mao2->cartas[i].num != forca2 && Mao2->cartas[i].num != v)
                                if(Mao2->cartas[i].num>kicker2)
                                    kicker2 = Mao2->cartas[i].num;
                        }
                        if(kicker1>kicker2) // COMPARA-SE OS KICKERS_2 DE CADA MAO E DETERMINA O VENCEDOR OU EMPATE
                            return 1;
                        if(kicker2>kicker1)
                            return 2;
                        if(kicker1==kicker2)
                            return 3;
                    }


                }
            }
        }
    }
    if(vlr==2) //DESEMPATE NO CASO DOUBLE PAR
    {
        int par1 = 0, par2 = 0;
        for(i=0;i<7;i++)
        {
            for(j=0;j<7;j++)  //LOOP PARA DETERMINAR OS POSSIVEIS PARES
            {
                if(Mao1->cartas[i].num==Mao1->cartas[j].num)
                    aux++;
                if(Mao2->cartas[i].num==Mao2->cartas[j].num)
                    aux2++;
            }
            if(aux==2)
            {                                        // GUARDA O MAIOR PAR DE CADA MAO
                forca1 = Mao1->cartas[i].num;
                if(par1<forca1)
                    par1=forca1;
            }
            if(aux2==2)
            {
                forca2 = Mao2->cartas[i].num;
                if(par2<forca2)
                    par2=forca2;
            }
            aux = 0;
            aux2 = 0;
        }
        if(par1>par2)
            return 1;                     // COMPARA OS MAIORES PARES E DETERMINA O VENCERDOR
        if(par2>par1)
            return 2;
        if(par1==par2)
        {
            int v = par1;
            par1 = 0;
            par2 = 0;
            forca1 = 0;
            forca2 = 0;
            for(i=0;i<7;i++)  // NO CASO DE EMPATE NO MAIOR PAR, PROCURA O SEGUNDO PAR
            {
                for(j=0;j<7;j++)
                {
                    if(Mao1->cartas[i].num==Mao1->cartas[j].num && Mao1->cartas[i].num!= v)
                        aux++;
                    if(Mao2->cartas[i].num==Mao2->cartas[j].num && Mao2->cartas[i].num!= v)
                        aux2++;
                }
                if(aux==2)
                {
                    forca1 = Mao1->cartas[i].num;
                    if(par1<forca1)
                        par1 = forca1;
                }
                if(aux2++)
                {
                    forca2 = Mao2->cartas[i].num;
                    if(par2<forca2)
                        par2 = forca2;
                }

            }
            if(par1>par2)  // COMPARA O SEGUNDO PRA DE CADA MÃO E DETERMINA O VENCEDOR
                return 1;
            if(par2>par1)
                return 2;
            if(par1==par2)      // SE AINDA ASSIM FOREM IGUAIS, LOOPA BUSCANDO OS KICKERS DE CADA MAO
            {
                int v2 = par1;
                for(i=0;i<7;i++)
                {
                    if(Mao1->cartas[i].num!=v && Mao1->cartas[i].num!=v2)
                        if(Mao1->cartas[i].num > kicker1)
                            kicker1 = Mao1->cartas[i].num;
                    if(Mao2->cartas[i].num!=v && Mao2->cartas[i].num!=v2)
                        if(Mao2->cartas[i].num > kicker2)
                            kicker2 = Mao2->cartas[i].num;
                }
                if(kicker1>kicker2)  // COMPARA OS KICKERS DE CADA MÃO E DETERMINA O VENCEDOR OU EMPATE
                    return 1;
                if(kicker2>kicker1)
                    return 2;
                if(kicker1==kicker2)
                    return 3;
            }
        }

    }
    if(vlr==1) // DESEMPATE NO CASO 'PAR'
    {
        for(i=0;i<7;i++)
        {
            for(j=0;j<7;j++)  //LOOP PARA DETERMINAR OS PARES
            {
                if(Mao1->cartas[i].num==Mao1->cartas[j].num)
                    aux++;
                if(Mao2->cartas[i].num==Mao2->cartas[j].num)
                    aux2++;
            }
            if(aux==2)                     // GUARDA O PAR DE CADA MAO
                forca1 = Mao1->cartas[i].num;
            if(aux2==2)
                forca2 = Mao2->cartas[i].num;
            aux = 0;
            aux2 = 0;
        }
        if(forca1>forca2)
            return 1;                     // COMPARA OS PARES E DETERMINA O VENCERDOR
        if(forca2>forca1)
            return 2;
        if(forca1==forca2)
        {
            for(i=0;i<7;i++)  // NO CASO DE EMPATE ENTRE OS PARES, DETERMINA-SE O KICKER
            {
                if(Mao1->cartas[i].num!=forca1)
                {
                    if(Mao1->cartas[i].num>kicker1)
                        kicker1 = Mao1->cartas[i].num;
                }
                if(Mao2->cartas[i].num!=forca2)
                {
                    if(Mao2->cartas[i].num>kicker2)
                        kicker2 = Mao2->cartas[i].num;
                }
            }
            if(kicker1>kicker2)
                return 1;
            if(kicker2>kicker1)               //COMPARA-SE OS KICKERS E DETERMINA O VENCEDOR
                return 2;
            if(kicker1 == kicker2)
            {
                int v = kicker1;
                kicker1 = 0;
                kicker2 = 0;
                for(i=0;i<7;i++)  // NO CASO DE EMPATE ENTRE OS KICKERS, DETERMINA-SE O SEGUNDO KICKER
                {
                    if(Mao1->cartas[i].num!=forca1 && Mao1->cartas[i].num!=v)
                    {
                        if(Mao1->cartas[i].num>kicker1)
                            kicker1 = Mao1->cartas[i].num;
                    }
                    if(Mao2->cartas[i].num!=forca2 && Mao2->cartas[i].num!=v)
                    {
                        if(Mao2->cartas[i].num>kicker2)
                            kicker2 = Mao2->cartas[i].num;
                    }
                }
                if(kicker1>kicker2)
                    return 1;
                if(kicker2>kicker1)               //COMPARA-SE OS KICKERS E DETERMINA O VENCEDOR
                    return 2;
                if(kicker1 == kicker2)
                {
                    int v2 = kicker1;
                    kicker1 = 0;
                    kicker2 = 0;
                    for(i=0;i<7;i++)  // NO CASO DE EMPATE ENTRE OS SEGUNDOS KICKERS, DETERMINA-SE O TERCEIRO KICKER
                    {
                        if(Mao1->cartas[i].num!=forca1 && Mao1->cartas[i].num!=v && Mao1->cartas[i].num!=v2)
                        {
                            if(Mao1->cartas[i].num>kicker1)
                                kicker1 = Mao1->cartas[i].num;
                        }
                        if(Mao2->cartas[i].num!=forca2 && Mao2->cartas[i].num!=v && Mao2->cartas[i].num!=v2)
                        {
                            if(Mao2->cartas[i].num>kicker2)
                                kicker2 = Mao2->cartas[i].num;
                        }
                    }
                    if(kicker1>kicker2) // COMPARA O TERCEIRO KICKER E DETERMINA O VENCEDOR OU EMPATE
                        return 1;
                    if(kicker2>kicker1)
                        return 2;
                    if(kicker1==kicker2)
                        return 3;
                }
            }
        }
    }
    if(vlr==0) // CASO CARTA ALTA
    {

    }
}
int checa_valor_mao(Mao *mao){
    int vlr;

    if(checa_Roy_Flush(mao))
        vlr = 9;
    else if(checa_Str_Flush(mao))
        vlr = 8;
    else if(checa_Quadra(mao))
        vlr = 7;
    else if(checa_Full_House(mao))
        vlr = 6;
    else if(checa_Flush(mao))
        vlr = 5;
    else if(checa_Sequencia(mao))
        vlr = 4;
    else if(checa_Trinca(mao))
        vlr = 3;
    else if(checa_Double_Par(mao))
        vlr = 2;
    else if(checa_Par(mao))
        vlr = 1;
    else
        vlr = 0;

    return vlr;

}
int checa_Roy_Flush(Mao *mao){
    int i=0,j=0, aux=0;
    char naipe1 = 0, naipe2 = 2;
    for(i=0;i<7;i++){
        if(mao->cartas[i].num==12)
            naipe1 = mao->cartas[i].naipe;
        else if(mao->cartas[i].num==11)
            naipe2 = mao->cartas[i].naipe;
        if(naipe1 == naipe2)
        {
            for(i=12;i>7;i--)
            {
                for(j=0;j<7;j++)
                {
                    if(mao->cartas[j].num == i && mao->cartas[j].naipe== naipe1)
                        aux++;
                }
            }
            if(aux==5)
                return 1;
        }
    }
    return 0;
}
int checa_Str_Flush(Mao *mao){
    int i, j,aux=0, aux2=0;
    char naipe = 'n';
    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++) // RODA A MÃO PROCURANDO O NAIPE Q APARECE 5 VEZES OU MAIS
        {
            if(mao->cartas[i].naipe == mao->cartas[j].naipe)
                aux++;
            if(aux>=5){
                naipe = mao->cartas[i].naipe;
                aux2 = aux;
            }
        }
        aux = 0;
    }
    if(naipe=='n')  //SE NÃO ATRIBUI NAIPE, É PORQUE NÃO HÁ 5 CARTAS DE MESMO NAIPE, LOGO SEM CHANCE DE STRAIGHT
        return 0;

    int menor_num = 20;
    for(i=0;i<7;i++){
        if(mao->cartas[i].naipe == naipe && mao->cartas[i].num<menor_num) //PROCURA A MENOR CARTA DO STRAIGHT
            menor_num = mao->cartas[i].num;
    }
    if(aux2 == 5) //SE HÁ APENAS 5 CARTAS DE NAIPES IGUAIS, ENTÃO O STRAIGHT FLUSH SÃO ELAS
    {
        for(i=0;i<7;i++)
        {
            if(mao->cartas[i].num == menor_num + 1 || mao->cartas[i].num == menor_num +2 || mao->cartas[i].num == menor_num + 3 || mao->cartas[i].num == menor_num+4 && mao->cartas[i].naipe == naipe)
                aux++;
            if(aux==4)
                return 1;
            else
                return 0;

        }
    }
    if(aux2>5) // SE HÁ MAIS QUE 5 CARTAS DE MESMO NAIPE, ENTÃO A MENOR PODE OU NÃO FAZER PARTE DO STRAIGHT FLUSH
    {
        for(i=0;i<7;i++) // TESTA O STRAIGHT COM A MENOR DAS CARTAS
        {
            if(mao->cartas[i].num == menor_num + 1 || mao->cartas[i].num == menor_num +2 || mao->cartas[i].num == menor_num + 3 || mao->cartas[i].num == menor_num+4 && mao->cartas[i].naipe == naipe)
                aux++;
            if(aux==4)
                return 1; //SE FOR STRAIGHT, BLZ
        }
        int s_menor = 20; // SE NÃO, DETERMINA A SEGUNDA MENOR E DE MESMO NAIPE DO STRAIGHT
        for(i=0;i<7;i++)
        {
            if(mao->cartas[i].num>menor_num && mao->cartas[i].naipe==naipe)
            {
                if(mao->cartas[i].num<s_menor)
                    s_menor = mao->cartas[i].num;
            }
        }
        for(i=0;i<7;i++) //TESTA O STRAIGHT COM A SEGUNDA MENOR CARTA
        {
            if(mao->cartas[i].num == s_menor + 1 || mao->cartas[i].num == s_menor +2 || mao->cartas[i].num == s_menor + 3 || mao->cartas[i].num == s_menor+4 && mao->cartas[i].naipe == naipe)
                aux++;
            if(aux==4)
                return 1; //SE DEU BOA, CABOU

        }
        return 0; // SE N, NÃO É STRAIGHT
    }
}
/* FUNÇÃO QUE CHECA A QUADRA */
int checa_Quadra(Mao *mao){
    int aux, j, i;
    for(j=0;j<3;j++){
            aux = 0;
        for(i=0;i<7;i++){
            if(mao->cartas[j].num == mao->cartas[i].num)
                aux++;
        }
        if(aux>=4)
            return 1;
    }
    return 0;
}
/* FUNÇÃO QUE CHECA O FULL HOUSE */
int checa_Full_House(Mao *mao){
    int aux, aux2 = 0, aux3=0, j, i;
    for(j=0;j<7;j++){
        aux = 0;
        for(i=0;i<7;i++){
            if(mao->cartas[j].num == mao->cartas[i].num)
                aux++;
        }
        if(aux==2)
            aux2++;
        if(aux==3)
            aux3++;
        if(aux2==2 && aux3>=3)
            return 1;
    }
    return 0;
}

/* FUNÇÃO QUE CHECA O FLUSH */
int checa_Flush(Mao *mao){
    int i,aux=0;
    char naipe;
    naipe = mao->cartas[0].naipe;
    for(i=1;i<7;i++){
        if(naipe != mao->cartas[i].naipe)
            aux++;
    }
    if(aux<=2)
        return 1;
    return 0;
}
int checa_Sequencia(Mao *mao){
    int i,j,menor_num,
        num,aux;
    menor_num = mao->cartas[0].num;

    for(i=1;i<7;i++){ // DETERMINA O MENOR NUMERO DA MÃO
        if(mao->cartas[i].num < menor_num)
            menor_num = mao->cartas[i].num;
    }
    num = 0;
    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++) // testa a sequencia
        {
            if(mao->cartas[i].num == menor_num + num)
            {
                aux++;
                num++;
            }
        }
    }
    if(aux>=5)
        return 1;
    else
        return 0;
}
/* FUNÇÃO QUE CHECA A TRINCA */
int checa_Trinca(Mao *mao){
    int j,i,aux;
    for(j=0;j<7;j++){
        aux = 0;
        for(i=0;i<7;i++){
            if(mao->cartas[j].num == mao->cartas[i].num)
                aux++;
        }
        if(aux>=3)
            return 1;
    }
    return 0;
}

/* FUNÇÃO QUE CHECA DOIS PARES */
int checa_Double_Par(Mao *mao){
    int j,i,aux,aux2=0;
    for(j=0;j<7;j++){
        aux = 0;
        for(i=0;i<7;i++){
            if(mao->cartas[j].num == mao->cartas[i].num)
                aux++;
        }
        if(aux==2)
            aux2++;
        if(aux2==2)
            return 1;
    }
    return 0;
}

/* FUNÇÃO QUE CHECA UM PAR */
int checa_Par(Mao *mao){
    int j,i,aux=0;
    for(j=0;j<7;j++){
        for(i=0;i<7;i++){
            if(mao->cartas[j].num == mao->cartas[i].num)
                aux++;
        }
        if(aux==2)
            return 1;
    }
    return 0;
}
/*FUNÇÕES DE JOGABILIDADE*/
void distribui_cartas(int *cont_baralho,Cartas *baralho,Jogadores *jogador,int n,Cartas *mesa){
    int i,j;
    /*FLOP*/
    /*DUAS CARTAS PARA CADA JOGADOR*/
    for(i=0;i<n;i++){
        for(j=0;j<2;j++){
            jogador[i].mao.cartas[j] = baralho[(*cont_baralho)];
            (*cont_baralho) = (*cont_baralho) + 1;
        }
    }
    /*=============================*/
    /*TRÊS CARTAS PARA A MESA*/
    for(i=0;i<3;i++){
        mesa[i] = baralho[*cont_baralho];
        *cont_baralho = *cont_baralho + 1;
    }
    /*=======================*/
    /*ATRIBUIR AS CARTAS DA MESA PARA CADA JOGADOR*/
    for(i=0;i<n;i++){
        for(j=2;j<5;j++){
            jogador[i].mao.cartas[j] = mesa[j-2];
        }
    }
    /*TURN*/
    /*UMA CARTA PARA A MESA*/
    mesa[3] = baralho[*cont_baralho];
    *cont_baralho = *cont_baralho + 1;

    /*A CARTA DA MESA PROS JOGADORES*/
    for(i=0;i<n;i++)
        jogador[i].mao.cartas[5] = mesa[3];
    /*RIVER*/

    /*UMA CARTA PARA A MESA*/
    mesa[4] = baralho[*cont_baralho];
    *cont_baralho = *cont_baralho + 1;

    /*A CARTA DA MESA PROS JOGADORES*/
    for(i=0;i<n;i++)
        jogador[i].mao.cartas[6] = mesa[4];

}
//2 PARA FOLD 3 PARA COBRIR A APOSTA
int jogada_por_perfil(int estilo){
    int num,x;
    srand(time(NULL));
    num = rand()%100 + 1;

    if(estilo == 1){
        if(num > 20)
            x = 2;
        else if(num < 20)
            x = 3;
    }
    else if(estilo == 2){
        if(num > 20)
            x = 3;
        else if(num < 20)
            x = 2;
    }
    else if(estilo == 3)
        x = 3;

    return x;
}
/*iNTERFACE GRÁFICA*/
void apostas(int type,Jogadores* jogadores,int num_players,int vlr_blnd,int* pot,int dealer){
    int i,sml_pos,big_pos,x,
        ind_raise,num,vlr_raise = 0;

    if(type == 1){
        for(i=0;i<num_players;i++){
            if(jogadores[i].pos_rel == 2)
                sml_pos = i;
            else if(jogadores[i].pos_rel == 3)
                big_pos = i;
        }


        tprintf(tam_center_dial(jogadores[sml_pos].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                "%s",jogadores[sml_pos].nome);
        tprintf(tam_center_dial("Paga SB de R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                "Paga SB de R$%d",vlr_blnd);
        jogadores[sml_pos].fichas = jogadores[sml_pos].fichas - vlr_blnd;
        (*pot) += vlr_blnd;

        Sleep(2000);

        if(num_players == 2){
            num = jogada_por_perfil(jogadores[dealer].estilo);

            if(num == 2){
                    print_dial();
                    tprintf(tam_center_dial(jogadores[dealer].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                            "%s",jogadores[dealer].nome);
                    tprintf(tam_center_dial("Folds."),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Folds.");
                    jogadores[dealer].bol_ativo = 0;
                    Sleep(2000);
                }
            else if(num == 3){
                    print_dial();
                    tprintf(tam_center_dial(jogadores[dealer].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                            "%s",jogadores[dealer].nome);
                    tprintf(tam_center_dial("Pagou  R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Pagou R$%d",vlr_blnd * 2);
                    jogadores[dealer].fichas -= vlr_blnd * 2;
                    *pot += vlr_blnd * 2;
                    Sleep(2000);
            }
        }
        if(num_players > 2){
            tprintf(tam_center_dial(jogadores[big_pos].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[big_pos].nome);
            tprintf(tam_center_dial("Paga BB de R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                    "Paga BB de R$%d",vlr_blnd * 2);

            jogadores[big_pos].fichas = jogadores[big_pos].fichas - vlr_blnd * 2;
            (*pot) += vlr_blnd*2;
            Sleep(2000);

            x = big_pos + 1;
            if(x == num_players)
                x = 0;

            for(i=0;i<num_players-2;i++){
                if(x == num_players)
                    x = 0;

                num = jogada_por_perfil(jogadores[x].estilo);

                //CHECK OU DESISTIR
                if(num == 2 && jogadores[x].bol_ativo == 1 ){
                    print_dial();
                    tprintf(tam_center_dial(jogadores[x].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                            "%s",jogadores[x].nome);
                    tprintf(tam_center_dial("Folds."),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Folds.");
                    jogadores[x].bol_ativo = 0;
                    Sleep(2000);
                }
                else if(num == 3 && jogadores[x].bol_ativo == 1){
                    print_dial();
                    tprintf(tam_center_dial(jogadores[x].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                            "%s",jogadores[x].nome);
                    tprintf(tam_center_dial("Pagou R$300"),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Pagou R$%d",vlr_blnd * 2);
                    jogadores[x].fichas -= vlr_blnd * 2;
                    *pot += vlr_blnd * 2;
                    Sleep(2000);
                }
                //COBRIR A APOSTA


                x++;
            }
        }
    }
    else if(type == 2){
        vlr_raise = rand()%50 + 1;

        ind_raise = dealer + 1;

        if((dealer + 1) == num_players)
            ind_raise = 0;

        while(jogadores[ind_raise].bol_ativo != 1){
            ind_raise++;
            if((dealer + 1) == num_players)
                ind_raise = 0;
        }

        //APOSTA NO INICIO

        if(jogadores[ind_raise].bol_ativo == 1){
            tprintf(tam_center_dial(jogadores[ind_raise].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[ind_raise].nome);
            tprintf(tam_center_dial("Apostou R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                    "Apostou R$%d",vlr_raise);
            jogadores[ind_raise].fichas -= vlr_raise;
            *pot += vlr_raise;
            Sleep(2000);

                x = ind_raise + 1;
                if(x == num_players)
                    x = 0;
            do{
                num = jogada_por_perfil(jogadores[x].estilo);

                if(num == 2 && jogadores[x].bol_ativo == 1 ){
                    print_dial();
                    tprintf(tam_center_dial(jogadores[x].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                            "%s",jogadores[x].nome);
                    tprintf(tam_center_dial("Folds."),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Folds.");
                    jogadores[x].bol_ativo = 0;
                    Sleep(2000);
                }
                else if(num == 3 && jogadores[x].bol_ativo == 1 ){
                    print_dial();
                    tprintf(tam_center_dial(jogadores[x].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                            "%s",jogadores[x].nome);
                    tprintf(tam_center_dial("Pagou R$300"),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Pagou R$%d",vlr_raise);
                    jogadores[x].fichas -= vlr_raise;
                    *pot += vlr_raise;
                    Sleep(2000);
                }

                x++;
                if(x == num_players)
                    x = 0;
            }while(x != ind_raise);
        }
        else{
            for(i=0;i<num_players;i++){
                if(jogadores[i].bol_ativo == 1){
                    tprintf(tam_center_dial(jogadores[i].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[i].nome);
                    tprintf(tam_center_dial("Apostou R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Apostou R$%d",vlr_raise);
                    jogadores[i].fichas -= vlr_raise;
                    *pot += vlr_raise;
                }
            }
                     x = ind_raise + 1;
                if(x == num_players)
                    x = 0;
                    do{
                        num = jogada_por_perfil(jogadores[x].estilo);

                        if(num == 2 && jogadores[x].bol_ativo == 1 ){
                            print_dial();
                            tprintf(tam_center_dial(jogadores[x].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                                    "%s",jogadores[x].nome);
                            tprintf(tam_center_dial("Folds."),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                                    "Folds.");
                            jogadores[x].bol_ativo = 0;
                            Sleep(2000);
                        }
                        else if(num == 3 && jogadores[x].bol_ativo == 1){
                            print_dial();
                            tprintf(tam_center_dial(jogadores[x].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                                    "%s",jogadores[x].nome);
                            tprintf(tam_center_dial("Pagou R$300"),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                                    "Pagou R$%d",vlr_raise);
                            jogadores[x].fichas -= vlr_raise;
                            *pot += vlr_raise;
                            Sleep(2000);
                        }

                        x++;
                        if(x == num_players)
                            x = 0;
                    }while(x != ind_raise);
                }
            }
            Sleep(2000);

            print_dial();
        }

//PRINTA UMA DETERMINADA CARTA NA TELA NA POSICAO (X,Y)
void print_nomes(Jogadores* jogadores,int num_players) {
    int i,x,y;
    print_dial();
    tprintf(LARGURA_TELA - 15,1,COR_TEXTO,COR_FUNDO,"MESA:");
    for(i=0;i<num_players;i++){
        //PARA PRINTAR OS NOMES DOS USUÁRIOS
        if(i == 4){
            x = 40;
            y = 1;
        }
        else if(i == 5){
            x = 40;
            y = 19;
        }
        else{
            x = 1;
            y = 1 + 18*i;
        }

        tprintf(x,y,COR_TEXTO,COR_FUNDO,"%s:",jogadores[i].nome);
        tprintf(x + 17,y,COR_TEXTO,COR_FUNDO,"R$%d",jogadores[i].fichas);

        if(jogadores[i].pos_rel == 1)
            tprintf(x + 15,y,CIANO_CLARO,COR_FUNDO,"D");
        else if(jogadores[i].pos_rel == 2)
            tprintf(x + 15,y,CIANO_CLARO,COR_FUNDO,"S");
        else if(jogadores[i].pos_rel == 3)
            tprintf(x + 15,y,CIANO_CLARO,COR_FUNDO,"B");

    }
}
int print_carta(int x, int y,int numero, int naipe)
{
    char cartaExemplo[15][10];
    int corCartaExemplo[15][10], corFundoCartaEx[15][10];
    int i, j;

    if(numero<0 || numero>13 || naipe<0 || naipe>3) //carta invalida
        return -1;

    //monta cores e fundo
    for(i=0;i<15;i++)
    {
        for(j=0;j<10;j++)
        {
            if(naipe==0 || naipe==1)
                corCartaExemplo[i][j] = VERMELHO_CLARO;
            else
                corCartaExemplo[i][j] = PRETO;

            corFundoCartaEx[i][j] = BRANCO;
            cartaExemplo[i][j] = ' ';
        }
    }

    //coloca os naipes no mapa
    cartaExemplo[2][1] = naipe + 3;
    cartaExemplo[12][8] = naipe + 3;

    if(numero<Dez) //se for numero
        cartaExemplo[1][1] = cartaExemplo[13][8] = numero+50;

    if(numero == Dez) //se for 10
    {
        cartaExemplo[1][1] = '1';
        cartaExemplo[1][2] = '0';
        cartaExemplo[13][8] = '0';
        cartaExemplo[13][7] = '1';
    }
    if(numero == Az) //se for Ás
        cartaExemplo[1][1] = cartaExemplo[13][8] = 'A';
    if(numero == Valete) //se for valete
        cartaExemplo[1][1] = cartaExemplo[13][8] = 'J';
    if(numero == Rainha) //se for dama
        cartaExemplo[1][1] = cartaExemplo[13][8] = 'Q';
    if(numero == Rei) //se for rei
        cartaExemplo[1][1] = cartaExemplo[13][8] = 'K';

    imprimeMapa(x,y,*cartaExemplo, *corCartaExemplo, *corFundoCartaEx, 15, 10);
    return 0;
}
void print_dial(){
    tprintf(DIALOG_X0 ,DIALOG_Y0-1,COR_DIALOG,COR_DIALOG,
            "                     ");
    tprintf(DIALOG_X0,DIALOG_Y0,COR_DIALOG,COR_DIALOG,
            "                     ");
    tprintf(DIALOG_X0,DIALOG_Y0 + 1,COR_DIALOG,COR_DIALOG,
            "                     ");
    tprintf(DIALOG_X0,DIALOG_Y0 + 2,COR_DIALOG,COR_DIALOG,
            "                     ");

}
void print_all_cartas(Jogadores* jogadores,int num_players){
    int i,j,x,y;

        //PRINTA O FLOP
    for(i=0;i<num_players;i++){
        for(j=0;j<2;j++){
            if(i == 4){
                x = 40;
                y = 1;
            }
            else if(i == 5){
                x = 40;
                y = 19;
            }
            else{
                x = 1;
                y = 1 + 18*i;
            }
                if(jogadores[i].bol_ativo == 1 ){
                    print_carta(x + 11*j,y+2,jogadores[i].mao.cartas[j].num,
                    atribui_num_naipe(jogadores[i].mao.cartas[j].naipe));
                }
                else if(jogadores[i].bol_ativo == 0 )
                    tprintf_v(x+ 11,y+2,VERMELHO_CLARO,COR_FUNDO,
                            "DESISTIU");
            }
    }
}
void print_tela(int num_players,Jogadores* jogadores,Cartas* mesa,int vlr_blnd,int dealer){

    int i,j,x,y,ind_wnr,
        pot = 0,soma = 0,maior = 0,
        ind_maior,num = 0;

    print_dial();


    print_nomes(jogadores,num_players);
    Sleep(2000);

    apostas(1,jogadores,num_players,vlr_blnd,&pot,dealer);


    limpa_tela(COR_FUNDO);
    print_nomes(jogadores,num_players);
        //PRINTA O FLOP
    print_all_cartas(jogadores,num_players);

    Sleep(2000);

    for(i=0;i<num_players;i++){
        if(jogadores[i].bol_ativo == 1){
            soma++;
            ind_wnr = i;
        }
    }
    if(soma != 1){
        tprintf(tam_center_dial("FLOP!"),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
            "FLOP!");

        for(i=0;i<3;i++)
            print_carta(LARGURA_TELA - 12,3 + 16*i,mesa[i].num,
            atribui_num_naipe(mesa[i].naipe));

        Sleep(2000);

        apostas(2,jogadores,num_players,vlr_blnd,&pot,dealer);

        limpa_tela(COR_FUNDO);
        print_nomes(jogadores,num_players);
        print_all_cartas(jogadores,num_players);
        for(i=0;i<3;i++)
            print_carta(LARGURA_TELA - 12,3 + 16*i,mesa[i].num,
            atribui_num_naipe(mesa[i].naipe));
        print_dial();
        Sleep(2000);

        soma = 0;
        ind_wnr = 0;

        for(i=0;i<num_players;i++){
            if(jogadores[i].bol_ativo == 1){
                soma++;
                ind_wnr = i;
            }
        }

        if(soma != 1){
            //TURN
            print_carta(LARGURA_TELA - 23,10,mesa[3].num,
            atribui_num_naipe(mesa[3].naipe));

            tprintf(tam_center_dial("TURN!"),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "TURN!");
            Sleep(2000);

            apostas(2,jogadores,num_players,vlr_blnd,&pot,dealer);
            limpa_tela(COR_FUNDO);
            print_carta(LARGURA_TELA - 23,10,mesa[3].num,
            atribui_num_naipe(mesa[3].naipe));
            for(i=0;i<3;i++)
                print_carta(LARGURA_TELA - 12,3 + 16*i,mesa[i].num,
                atribui_num_naipe(mesa[i].naipe));
            print_all_cartas(jogadores,num_players);
            print_nomes(jogadores,num_players);
            print_dial();
            Sleep(2000);

            soma = 0;
            ind_wnr = 0;

            for(i=0;i<num_players;i++){
                if(jogadores[i].bol_ativo == 1){
                    soma++;
                    ind_wnr = i;
                }
            }
            if(soma != 1){
                    //RIVER

                print_carta(LARGURA_TELA - 23,30,mesa[4].num,
                atribui_num_naipe(mesa[4].naipe));

                tprintf(tam_center_dial("RIVER!"),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                        "RIVER!");
                Sleep(2000);

                print_dial();

                apostas(2,jogadores,num_players,vlr_blnd,&pot,dealer);
                limpa_tela(COR_FUNDO);

                for(i=0;i<3;i++)
                    print_carta(LARGURA_TELA - 12,3 + 16*i,mesa[i].num,
                    atribui_num_naipe(mesa[i].naipe));

                print_carta(LARGURA_TELA - 23,10,mesa[3].num,
                atribui_num_naipe(mesa[3].naipe));

                print_carta(LARGURA_TELA - 23,30,mesa[4].num,
                atribui_num_naipe(mesa[4].naipe));

                print_all_cartas(jogadores,num_players);
                print_nomes(jogadores,num_players);

                //AVALIA MAIOR MAO PARA PREMIAR
                for(i=0;i<num_players;i++){
                    jogadores[i].mao.valor = checa_valor_mao(&jogadores[i].mao);
                }

                for(i=0;i<num_players;i++){
                    if(jogadores[i].bol_ativo == 1 && jogadores[i].mao.valor > maior){
                        maior = jogadores[i].mao.valor;
                        ind_maior = i;
                    }
                }
                for(j=0;j<num_players;j++){
                    if(jogadores[i].bol_ativo == 1 && jogadores[i].mao.valor == maior
                       && j != i)
                        num = desempate(&jogadores[j].mao,&jogadores[ind_maior].mao,&jogadores[ind_maior].mao);

                }
                if(num == 1){
                    if(j == 4){
                        x = 40;
                        y = 1;
                    }
                    else if(j == 5){
                        x = 40;
                        y = 19;
                    }
                    else{
                        x = 1;
                        y = 1 + 18*j;
                    }
                    limpa_tela(COR_FUNDO);
                    print_nomes(jogadores,num_players);
                    print_dial();
                    tprintf(tam_center_dial(jogadores[j].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[j].nome);
                    tprintf(tam_center_dial("Recebe R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Recebe R$%d",pot);
                    tprintf_v(x + 2,y + 2,AMARELO_CLARO,COR_FUNDO,"WINNER!");
                    Sleep(2000);

                    jogadores[j].fichas += pot;
                    jogadores[j].wins = jogadores[j].wins + 1;
                    pot = 0;
                }
                else if(num == 2 || num == 0){
                    if(ind_maior == 4){
                        x = 40;
                        y = 1;
                    }
                    else if(ind_maior == 5){
                        x = 40;
                        y = 19;
                    }
                    else{
                        x = 1;
                        y = 1 + 18*ind_maior;
                    }

                    limpa_tela(COR_FUNDO);
                    print_nomes(jogadores,num_players);
                    print_dial();

                    tprintf(tam_center_dial(jogadores[ind_maior].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[ind_maior].nome);
                    tprintf(tam_center_dial("Recebe R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                            "Recebe R$%d",pot);
                    tprintf_v(x + 2,y + 2,AMARELO_CLARO,COR_FUNDO,"WINNER!");
                    Sleep(2000);

                    jogadores[ind_maior].fichas += pot;
                    jogadores[ind_maior].wins = jogadores[ind_maior].wins + 1;
                    pot = 0;
                }
            }
            else{
                if(ind_wnr == 4){
                x = 40;
                y = 1;
            }
            else if(ind_wnr == 5){
                x = 40;
                y = 19;
            }
            else{
                x = 1;
                y = 1 + 18*ind_wnr;
            }
            limpa_tela(COR_FUNDO);
            print_nomes(jogadores,num_players);
            tprintf_v(x + 2,y + 2,AMARELO_CLARO,COR_FUNDO,"WINNER!");
            Sleep(2000);

            tprintf(tam_center_dial(jogadores[ind_wnr].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[ind_wnr].nome);
            tprintf(tam_center_dial("Recebe R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                    "Recebe R$%d",pot);

            jogadores[ind_wnr].fichas += pot;
            jogadores[ind_wnr].wins = jogadores[ind_wnr].wins + 1;
            pot = 0;
            }
        }
        else{
            if(ind_wnr == 4){
                x = 40;
                y = 1;
            }
            else if(ind_wnr == 5){
                x = 40;
                y = 19;
            }
            else{
                x = 1;
                y = 1 + 18*ind_wnr;
            }
            limpa_tela(COR_FUNDO);
            print_nomes(jogadores,num_players);
            tprintf_v(x + 2,y + 2,AMARELO_CLARO,COR_FUNDO,"WINNER!");
            Sleep(2000);
            tprintf(tam_center_dial(jogadores[ind_wnr].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                    "%s",jogadores[ind_wnr].nome);
            tprintf(tam_center_dial("Recebe R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                    "Recebe R$%d",pot);

            jogadores[ind_wnr].fichas += pot;
            jogadores[ind_wnr].wins = jogadores[ind_wnr].wins + 1;
            pot = 0;
        }
    }
    else{
        if(ind_wnr == 4){
                x = 40;
                y = 1;
            }
            else if(ind_wnr == 5){
                x = 40;
                y = 19;
            }
            else{
                x = 1;
                y = 1 + 18*ind_wnr;
            }
        limpa_tela(COR_FUNDO);
        print_nomes(jogadores,num_players);
        tprintf_v(x + 2,y + 2,AMARELO_CLARO,COR_FUNDO,"WINNER!");
        Sleep(2000);

        tprintf(tam_center_dial(jogadores[ind_wnr].nome),DIALOG_Y0,TXT_DIALOG,COR_DIALOG,
                "%s",jogadores[ind_wnr].nome);
        tprintf(tam_center_dial("Recebe R$  "),DIALOG_Y0 + 1,TXT_DIALOG,COR_DIALOG,
                "Recebe R$%d",pot);

        jogadores[ind_wnr].fichas += pot;
        jogadores[ind_wnr].wins = jogadores[ind_wnr].wins + 1;
        pot = 0;

    }

}
//TELA FINAL DO JOGO
void fechar(){
    limpa_tela(COR_FUNDO);

    tprintf(tam_center_str("Obrigado por utilizar o simulador de poker Texas Hold'em! VOLTE SEMPRE."),
            METADE_ALTURA_TELA,COR_TEXTO,COR_FUNDO,
            "Obrigado por utilizar o simulador de poker Texas Hold'em! VOLTE SEMPRE.");

    tprintf(tam_center_str("FEITO POR IGOR AUGUSTO AMARAL PEDROZO E GABRIEL LABRES MARTINI."),
            ALTURA_TELA - 2,COR_TEXTO,COR_FUNDO,
            "FEITO POR IGOR AUGUSTO AMARAL PEDROZO E GABRIEL LABRES MARTINI.");
    Sleep(2000);
}
void prime_msg(){
    /*O TAMANHO DE CENTRALIZAÇÃO PARA O MENU É O MESMO PARA TODOS OS TEXTOS*/
    int tam_opts = tam_center_str("1) Inicializar uma simulação");

    tprintf(tam_center_str("Bem vindo ao simulador de Poker Texas Hold'em!"),
            1,COR_TEXTO,COR_FUNDO,
            "Bem vindo ao simulador de Poker Poker Texas Hold'em!");

    tprintf(tam_center_str("Digite o numero correspondente:"),10,COR_TEXTO,COR_FUNDO,
            "Digite o numero correspondente:");

    tprintf(tam_opts,20,COR_TEXTO,COR_FUNDO,"1) Ler instrucoes");
    tprintf(tam_opts,22,COR_TEXTO,COR_FUNDO,"2) Definir parametros");
    tprintf(tam_opts,24,COR_TEXTO,COR_FUNDO,"3) Iniciar simulacao");
    tprintf(tam_opts,26,COR_TEXTO,COR_FUNDO,"4) Estatisticas da sessao");
    tprintf(tam_opts,28,COR_TEXTO,COR_FUNDO,"5) Sair do simulador");

}
int menu(){
    int op,i = 1;
    /*O TAMANHO DE CENTRALIZAÇÃO PARA O MENU É O MESMO PARA TODOS OS TEXTOS*/
    int tam_opts = tam_center_str("1) Inicializar uma simulação");

    limpa_tela(COR_FUNDO);

    prime_msg();

    do{
        if(i != 1){
            tprintf(tam_opts,34,COR_TEXTO,COR_FUNDO,"                    ");
            tprintf(tam_center_str("Numero nao permitido! Digite um número de 1 a 5!"),
                    30,COR_TEXTO,COR_FUNDO,
                    "Numero nao permitido! Digite um número de 1 a 5!");
        }
        tscanf(tam_opts,34,COR_TEXTO,COR_FUNDO,"%d",&op);
        i++;
    }while(op<1||op>5);

    return op;

}
int instrucoes(){
    limpa_tela(COR_FUNDO);

    tprintf(1,1,COR_TEXTO,COR_FUNDO,"Cada jogador comeca com duas cartas fechadas. Ha tres rodadas de cartas comunitarias.Estas sao distribuidas com a face virada ");
    tprintf(1,2,COR_TEXTO,COR_FUNDO,"para cima, para todos os jogadores usarem, com apostas apos cada rodada.");
    tprintf(1,4,COR_TEXTO,COR_FUNDO,"A melhor mao com cinco cartas, usando qualquer combinacao das cinco cartas comunitarias e as duas cartas da mao vence.");
    tprintf(1,5,COR_TEXTO,COR_FUNDO,"Abaixo esta um descricaoo mais abrangente do Texas Hold'em online.");
    tprintf(tam_center_str("BLINDS"),7,VERMELHO_CLARO,COR_FUNDO,"BLINDS");
    tprintf(1,9,COR_TEXTO,COR_FUNDO,"Cada mao de Texas Hold'em comeca com dois blinds. Os blinds sao apostas preliminares feitas por dois jogadores antes da ");
    tprintf(1,10,COR_TEXTO,COR_FUNDO,"distribuicao das cartas, para estimular a acao. Se nao houvesse nada em jogo, o primeiro jogador a agir nao teria nenhum motivo ");
    tprintf(1,11,COR_TEXTO,COR_FUNDO,"para fazer uma aposta. A posicao do dealer e indicada por um disco branco chamado de botao do dealer, ou simplesmente de ");
    tprintf(1,12,COR_TEXTO,COR_FUNDO,"botao. Esta seria a posicao do jogador que distribuiria as cartas, caso nao houvesse ninguem responsavel por isso. Antes da ");
    tprintf(1,13,COR_TEXTO,COR_FUNDO,"distribuicao das cartas, o jogador a esquerda do botao coloca um valor em fichas normalmente igual a metade da aposta minima ");
    tprintf(1,14,COR_TEXTO,COR_FUNDO,"do jogo (a small blind). O jogador seguinte a esquerda coloca uma quantidade de fichas igual a aposta minima do jogo (a big ");
    tprintf(1,15,COR_TEXTO,COR_FUNDO,"blind).");
    tprintf(1,16,COR_TEXTO,COR_FUNDO,"");
    tprintf(tam_center_str("PRIMEIRA RODADA"),18,VERMELHO_CLARO,COR_FUNDO,"PRIMEIRA RODADA");
    tprintf(1,20,COR_TEXTO,COR_FUNDO,"Apos o pagamento de blinds, o dealer distribui a primeira carta fechada e entao outra carta fechada, uma de cada vez, a cada ");
    tprintf(1,21,COR_TEXTO,COR_FUNDO,"jogador na mesa, comecando pelo small blind. Essas duas cartas iniciais sao chamadas de cartas fechadas. Suas cartas ");
    tprintf(1,22,COR_TEXTO,COR_FUNDO,"fechadas vao aparecer visiveis em sua tela, mas nao se preocupe. Voce e o unico que consegue ve-las. Apenas os versos das ");
    tprintf(1,23,COR_TEXTO,COR_FUNDO,"cartas fechadas dos outros jogadores aparecem na tela. Os demais jogadores possuem visualizacao similar e podem ver apenas ");
    tprintf(1,24,COR_TEXTO,COR_FUNDO,"suas proprias cartas fechadas.");
    tprintf(1,26,COR_TEXTO,COR_FUNDO,"Cada jogador começa com duas cartas, então cinco cartas são colocadas abertas no centro da mesa. Estas cartas comunitárias ");
    tprintf(1,27,COR_TEXTO,COR_FUNDO,"fazem parte da mao de todos os jogadores, dessa forma cada um tem acesso a sete cartas. Cada jogador tenta fazer a melhor ");
    tprintf(1,28,COR_TEXTO,COR_FUNDO,"mao de poker possivel usando cinco das sete cartas. Ja que o poker consiste de exatas cinco cartas, apenas as melhores cinco ");
    tprintf(1,29,COR_TEXTO,COR_FUNDO,"das sete jogam. Mesmo se nao tiver experiencia com o Hold'em, voce nao precisa se preocupar em saber quais sao as melhores ");
    tprintf(1,30,COR_TEXTO,COR_FUNDO,"cartas; o software escolhe automaticamente as melhores cinco cartas na hora de comparar as maos.");
    tprintf(1,32,COR_TEXTO,COR_FUNDO,"O Hold'em, assim como qualquer outra forma de poker, gira em torno de apostar. O Hold'em tem quatro rodadas de apostas. Os ");
    tprintf(1,33,COR_TEXTO,COR_FUNDO,"tamanhos das apostas dependem da estrutura do jogo, que no Hold'em tem tres possibilidades:");
    tprintf(1,35,COR_TEXTO,COR_FUNDO,"Jogo com Limite – no Limit Texas Hold'em, cada rodada de aposta tem uma aposta fixa.");
    tprintf(1,36,COR_TEXTO,COR_FUNDO,"Jogo Pot Limit - no Pot Limit Texas Hold'em, um jogador pode apostar qualquer quantia entre a aposta minima e o valor do pote.");
    tprintf(1,37,COR_TEXTO,COR_FUNDO,"Jogo No-limit - no No Limit Texas Hold'em um jogador pode apostar qualquer valor a partir da aposta mínima ate um");
    tprintf(1,38,COR_TEXTO,COR_FUNDO,"numero maximo das fichas que ele tem em sua frente.");
    tprintf(1,40,COR_TEXTO,COR_FUNDO,"As apostas na primeira rodada sempre comecam pelo jogador logo a esquerda do big blind. Essa posicaxo e chamada de 'inicial'. ");
    tprintf(1,41,COR_TEXTO,COR_FUNDO,"Como o primeiro jogador a agir, você tem tres opcoes. Voce pode:");
    tprintf(1,43,COR_TEXTO,COR_FUNDO,"Desistir");
    tprintf(1,44,COR_TEXTO,COR_FUNDO,"Pagar pelo minimo (limp)");
    tprintf(1,45,COR_TEXTO,COR_FUNDO,"Abrir um aumento");
    tprintf(1,47,COR_TEXTO,COR_FUNDO,"Voce escolhe a sua acao clicando na caixa de dialogo. Se desistir a qualquer momento da mao, suas cartas sao removidas do ");
    tprintf(1,48,COR_TEXTO,COR_FUNDO,"jogo e deixam de aparecer na tela. Voce ficara de fora ate a mao seguinte e nao disputara mais o pote. Se voce desistir, o jogador ");
    tprintf(1,49,COR_TEXTO,COR_FUNDO,"seguinte tera as mesmas opcoes. Se todos desistirem, incluindo o small blind, o pote e entregue ao big blind e a mao seguinte e ");
    tprintf(1,50,COR_TEXTO,COR_FUNDO,"iniciada.");
    tprintf(1,52,COR_TEXTO,COR_FUNDO,"Se você ou outro jogador apostar, cada participante a seguir tem tres opcoes:");
    tprintf(1,54,COR_TEXTO,COR_FUNDO,"Desistir.");
    tprintf(1,55,COR_TEXTO,COR_FUNDO,"Pagar, isto e, igualar a aposta antecedente.");
    tprintf(1,56,COR_TEXTO,COR_FUNDO,"Aumentar a aposta.");
    tprintf(1,58,COR_TEXTO,COR_FUNDO,"Cada jogador em sua vez tem as mesmas tres escolhas. Se alguem aumentar, todos os jogadores que desejam continuar na mao ");
    tprintf(1,59,COR_TEXTO,COR_FUNDO,"devem pagar ou aumentar a aposta. Quando a acao chegar aos blinds, eles terao as mesmas opcoes. Porem, como eles ja ");
    tprintf(1,60,COR_TEXTO,COR_FUNDO,"possuem fichas no potes, essas fichas contam como parte da aposta. De forma similar, o big blind, que já tem US$ 2 investidos ");
    tprintf(1,61,COR_TEXTO,COR_FUNDO,"no pote, entra por menos US$ 2. Se ninguem apostar ou aumentar ate a acao chegar ao big blind, ele tera duas opcoes. Ele pode ");
    tprintf(1,62,COR_TEXTO,COR_FUNDO,"decidir aumentar, e neste caso todos os jogadores ativos deverao decidir entre pagar, aumentar ou reaumentar ou desistir. O big ");
    tprintf(1,63,COR_TEXTO,COR_FUNDO,"blind tambem pode decidir pedir mesa e assim o flop sera mostrado. No linguajar do poquer, esta situacao tambem e conhecida ");
    tprintf(1,64,COR_TEXTO,COR_FUNDO,"como 'live blind' ('blind viva').");
    tprintf(1,67,COR_TEXTO,VERMELHO_ESCURO,"Aperte qualquer tecla para ir a pagina seguinte.");

    getchar();
    limpa_tela(COR_FUNDO);
    tprintf(tam_center_str("O FLOP"),1,VERMELHO_CLARO,COR_FUNDO,"O Flop");
    tprintf(1,3,COR_TEXTO,COR_FUNDO,"Apos o termino da rodada de apostas, isto e, quando todos ja tiveram a chance de desistir ou pagar a aposta total, o carteador ");
    tprintf(1,4,COR_TEXTO,COR_FUNDO,"vira tres cartas com a face para cima no centro da mesa. Essas primeiras tres cartas comunitarias sao chamadas de flop.");
    tprintf(1,5,COR_TEXTO,COR_FUNDO,"Entao a segunda rodada de apostas e realizada. Nesta rodada, as apostas comecam pelo primeiro jogador ativo (que ainda ");
    tprintf(1,6,COR_TEXTO,COR_FUNDO,"possui cartas) a esquerda do botao. Se o small blind continua na disputa, sera o primeiro jogador a agir, mesmo depois de ser o ");
    tprintf(1,7,COR_TEXTO,COR_FUNDO,"penultimo na primeira rodada de apostas. Apenas na primeira rodada (tambem chamada de pre-flop) as apostas nao comecam ");
    tprintf(1,8,COR_TEXTO,COR_FUNDO,"por ele. Em todas as rodadas a partir da segunda, o primeiro jogador possui duas opcoes:");
    tprintf(1,10,COR_TEXTO,COR_FUNDO,"Passar, isto e, nao apostar");
    tprintf(1,11,COR_TEXTO,COR_FUNDO,"Apostar, isto e, fazer uma aposta no limite apropriado para aquela rodada");
    tprintf(1,13,COR_TEXTO,COR_FUNDO,"Se ninguem apostar, cada jogador na sua vez tem as mesmas escolhas. Essa situacao e possivel em todas as rodadas, exceto na ");
    tprintf(1,14,COR_TEXTO,COR_FUNDO,"primeira. Em uma rodada sem apostas dizemos que todos 'deram check'.");
    tprintf(1,16,COR_TEXTO,COR_FUNDO,"Se alguem apostar, cada jogador a seguir tem as tres opcoes citadas anteriormente.");
    tprintf(1,18,COR_TEXTO,COR_FUNDO,"Um jogador que passa mantem as suas cartas. Se alguem apostar e a acao voltar ate um jogador que pediu mesa anteriormente ");
    tprintf(1,19,COR_TEXTO,COR_FUNDO,"na mesma rodada, ele tem tres opcoes. Desistir, pagar ou aumentar. Pedir mesa com a intencao de aumentar diante de uma ");
    tprintf(1,20,COR_TEXTO,COR_FUNDO,"aposta e uma estrategia comum no poker. Porem, sempre existe o risco de ninguem apostar e a estrategia nao funcionar.");
    tprintf(tam_center_str("O TURN"),22,VERMELHO_CLARO,COR_FUNDO,"O TURN");
    tprintf(1,24,COR_TEXTO,COR_FUNDO,"Apos o termino da segunda rodada de apostas, isto e, quando todos ja tiveram a chance de pedir mesa ou pagar a aposta total da ");
    tprintf(1,25,COR_TEXTO,COR_FUNDO,"rodada, o carteador vira mais uma carta com a face para cima no centro da mesa. Essa quarta carta comunitaria e chamada de turn.");
    tprintf(1,27,COR_TEXTO,COR_FUNDO,"Entao a terceira rodada de apostas e realizada. Mais uma vez, a acao comeca pelo primeiro jogador ativo a esquerda do botao. ");
    tprintf(1,28,COR_TEXTO,COR_FUNDO,"As apostas prosseguem de maneira identica a segunda rodada. Em um jogo limit, na terceira e quarta rodadas a aposta ");
    tprintf(1,29,COR_TEXTO,COR_FUNDO,"normalmente acontece em incrementos do dobro do tamanho das primeiras duas rodadas..");
    tprintf(tam_center_str("O RIVER"),31,VERMELHO_CLARO,COR_FUNDO,"O RIVER");
    tprintf(1,33,COR_TEXTO,COR_FUNDO,"Apos o final da terceira rodada de apostas, o carteador vira uma quinta e Ultima carta com a face para cima no centro da mesa. ");
    tprintf(1,34,COR_TEXTO,COR_FUNDO,"Essa ultima carta comunitaria e chamada de river.");
    tprintf(1,36,COR_TEXTO,COR_FUNDO,"Entao a quarta e última rodada de apostas e realizada. Mais uma vez, a acao comeca pelo primeiro jogador ativo à esquerda do ");
    tprintf(1,37,COR_TEXTO,COR_FUNDO,"botao. As apostas prosseguem de maneira idêntica às duas rodadas anteriores.");
    tprintf(tam_center_str("O SHOWDOWN"),39,VERMELHO_CLARO,COR_FUNDO,"O SHOWDOWN");
    tprintf(1,41,COR_TEXTO,COR_FUNDO,"Apos a quarta rodada de apostas, a mao chega ao momento conhecido como showdown. Os jogadores ativos restantes ");
    tprintf(1,42,COR_TEXTO,COR_FUNDO,"mostram suas cartas e a melhor mao, composta das melhores cinco cartas entre a combinacao de cada jogador de duas cartas ");
    tprintf(1,43,COR_TEXTO,COR_FUNDO,"da mao mais as cartas comunitarias vence. O proprietario da mao vencedora e premiado com o pote. Se houver um empate, o ");
    tprintf(1,44,COR_TEXTO,COR_FUNDO,"pote sera dividido igualmente entre os jogadores com a melhor mao.");
    tprintf(1,46,COR_TEXTO,COR_FUNDO,"Se a aposta ou aumento final nao for pago na ultima rodada, ou seja, se um jogador apostou ou aumentou e ninguem pagou, nao ");
    tprintf(1,47,COR_TEXTO,COR_FUNDO,"havera showdown. O software entregara o pote ao jogador que fez a aposta ou aumento final. O mesmo vale para qualquer outra ");
    tprintf(1,48,COR_TEXTO,COR_FUNDO,"rodada anterior. Se isso acontecer em rodadas anteriores, nenhuma outra carta sera distribuida e a mao terminara.");
    tprintf(1,50,COR_TEXTO,COR_FUNDO,"As vezes, um jogador ficara sem fichas disponíveis antes do final das apostas. Em casos assim, um ou mais potes adicionais sao ");
    tprintf(1,51,COR_TEXTO,COR_FUNDO,"criados, e o software entregara adequadamente as fichas de cada um deles. Quando um jogador esta em situacao de 'all in', ");
    tprintf(1,52,COR_TEXTO,COR_FUNDO,"havera um showdown mesmo se a aposta ou aumento final nao for pago.");
    tprintf(1,54,COR_TEXTO,COR_FUNDO,"Os jogadores normalmente nao mostram suas maos perdedoras. Porem, você tem o direito de ver as cartas de todos os ");
    tprintf(1,55,COR_TEXTO,COR_FUNDO,"adversarios ativos no momento do showdown.");
    tprintf(1,57,COR_TEXTO,COR_FUNDO,"Para determinar a mao vencedora, a combinacao das melhores cinco cartas às vezes incluí ambas as cartas da mao do jogador. ");
    tprintf(1,59,COR_TEXTO,COR_FUNDO,"Algumas vezes inclui apenas uma das cartas da mao do jogador. Algumas vezes, raramente, nenhuma das cartas da mao e ");
    tprintf(1,60,COR_TEXTO,COR_FUNDO,"usada. Em tal caso, a mesa contem alguma combinacao melhor do que qualquer mao que possa ser feita usando as cartas da ");
    tprintf(1,61,COR_TEXTO,COR_FUNDO,"mao de qualquer jogador. Isso e chamado jogar com a mesa. Quando todos os jogadores jogam com a mesa, o pote e dividido ");
    tprintf(1,62,COR_TEXTO,COR_FUNDO,"igualmente entre todos os jogadores restantes na mao neste ponto.");
    tprintf(1,65,COR_TEXTO,VERMELHO_ESCURO,"Aperte qualquer tecla para voltar ao menu...");

    getchar();
    return 10;
}
/* FUNÇÃO QUE PEDE OS PARAMETROS DO USUARIO*/
Jogadores* parametros(int *num_players,int *qnt_maos,int *vlr_blnd,int* vlr_fichas){
    int i_num = 1,
        i,num;
    Jogadores* jogadores;
    limpa_tela(COR_FUNDO);

    tprintf(tam_center_str("Digite a quantidade de jogadores (2 a 6) para a simulacao:"),1,COR_TEXTO,COR_FUNDO,
            "Digite a quantidade de jogadores(2 a 6) para a simulacao:");

    do{
        if(i_num != 1){
            tprintf(tam_center_str("Digite novamente! Numero de jogadores deve ser menor que sete e maior que 1"),
                    5,COR_TEXTO,COR_FUNDO,"                                                ");

            tprintf(tam_center_str("Digite novamente! Numero de jogadores deve ser menor que sete e maior que 1"),
                    3,COR_TEXTO,VERMELHO_CLARO,"%s",
                    "Digite novamente! Numero de jogadores deve ser menor que sete e maior que 1");
        }
        tscanf(tam_center_str("Digite a quantidade de jogadores (2 a 6) para a simulacao:"),
               5,COR_TEXTO,COR_FUNDO,"%d",num_players);
        i_num++;
    }while((*num_players)<2 || (*num_players)>6);




    jogadores = malloc(sizeof(Jogadores*) * (*num_players));
    if (jogadores == NULL) {
        tprintf(1,1,VERMELHO_CLARO,AMARELO_CLARO,"ERRO NA ALOCACAO!");
        exit (EXIT_FAILURE);
    }

    limpa_tela(COR_FUNDO);

    tprintf(tam_center_str("Selecao do perfil dos jogadores: "),
            1,COR_TEXTO,COR_FUNDO,
            "Selecao do perfil dos jogadores: ");
    tprintf(tam_center_str("Digite 1 para TIGHT, 2 para LOOSE ou 3 para AGRESSIVE."),
            3,COR_TEXTO,COR_FUNDO,
            "Digite 1 para TIGHT, 2 para LOOSE ou 3 para AGRESSIVE.");

    for(i=0;i<(*num_players);i++){
        //PERSONALIDADES DOS JOGADORES
        tprintf(tam_center_str("Insira a personalidade do %d jogador: "),
                10 + 8 * i,COR_TEXTO,COR_FUNDO,
                "Insira a personalidade do jogador %d :",i + 1);

        tscanf(tam_center_str("Insira a personalidade do %d jogador: "),
                12 + 8 * i,COR_TEXTO,COR_FUNDO,
                "%d",&num);

        while(num < 1 || num > 3){
            tprintf(tam_center_str("Insira a personalidade do    jogador: "),
                    12 + 8 * i,COR_TEXTO,VERMELHO_CLARO,
                    "Numero invalido! Tente novamente!");
            tprintf(tam_center_str("Insira a personalidade do    jogador: "),
                    13 + 8 * i,COR_TEXTO,COR_FUNDO,
                    "                                                       ");
            tscanf(tam_center_str("Insira a personalidade do    jogador: "),
                    13 + 8 * i,COR_TEXTO,COR_FUNDO,
                    "%d",&num);
            tprintf(tam_center_str("Insira a personalidade do    jogador: "),
                    13 + 8 * i,COR_TEXTO,COR_FUNDO,
                    "                                                   ");

            }
            jogadores[i].estilo = num;

        //NOMES DOS JOGADORES
        tprintf(tam_center_str("Insira a personalidade do    jogador: "),
                    14 + 8 * i,COR_TEXTO,COR_FUNDO,
                    "Insira o nome do jogador %d com ate 15 caracteres:",i + 1);
        tgets(tam_center_str("Insira a personalidade do    jogador: "),
                    15 + 8 * i,COR_TEXTO,COR_FUNDO,
                    jogadores[i].nome);
        while(strlen(jogadores[i].nome) > 15){
            tprintf(tam_center_str("Insira a personalidade do    jogador: "),
                    16 + 8 * i,COR_TEXTO,VERMELHO_CLARO,
                    "Nome invalido! Tente novamente!");
            tprintf(tam_center_str("Insira a personalidade do    jogador: "),
                    17 + 8 * i,COR_TEXTO,COR_FUNDO,
                    "                                                      ");
            tgets(tam_center_str("Insira a personalidade do    jogador: "),
                    17 + 8 * i,COR_TEXTO,COR_FUNDO,
                    jogadores[i].nome);
        }

    }

    limpa_tela(COR_FUNDO);

    //QUANTIDADE DE MAOS
    tprintf(tam_center_str("Insira a quantidade de maos a serem disputadas:"),
            1,COR_TEXTO,COR_FUNDO,
            "Insira a quantidade de maos a serem disputadas:");
    tscanf(tam_center_str("Insira a quantidade de maos a serem disputadas:"),
            2,COR_TEXTO,COR_FUNDO,
            "%d",qnt_maos);

    limpa_tela(COR_FUNDO);

    tprintf(tam_center_str("Insira a quantidade de maos a serem disputadas:"),
            1,COR_TEXTO,COR_FUNDO,
            "Insira o valor inicial da small blind:");
    tscanf(tam_center_str("Insira a quantidade de maos a serem disputadas:"),
            2,COR_TEXTO,COR_FUNDO,
            "%d",vlr_blnd);

    limpa_tela(COR_FUNDO);

    tprintf(tam_center_str("Insira a quantidade de fichas para cada jogador:"),
            1,COR_TEXTO,COR_FUNDO,
            "Insira a quantidade de fichas para cada jogador:");
    tscanf(tam_center_str("Insira a quantidade de fichas para cada jogador:"),
            2,COR_TEXTO,COR_FUNDO,
            "%d",vlr_fichas);

    return jogadores;
}

/*FUNÇÃO QUE INICIA A SIMULAÇÃO*/
void simulacao(Jogadores* jogadores,int num_players,int qnt_maos,int vlr_blnd,int vlr_fichas){

    int i,j,cont_baralho = 0,dealer;

    Cartas baralho[MAX_BARALHO];
    Cartas mesa[5];

    srand(time(NULL));
    dealer = rand()%num_players;


    //SETA O DEALER, SMALL BLIND E BIG BLIND
    atribui_pos(jogadores,num_players,dealer);

    for(i=0;i<num_players;i++){
        jogadores[i].fichas = vlr_fichas;
        jogadores[i].wins = 0;
    }


    for(i=0;i<qnt_maos;i++){

        for(j=0;j<num_players;j++)
            jogadores[j].bol_ativo = 1;

        limpa_tela(COR_FUNDO);


        print_dial();
        tprintf(tam_center_dial("Mao   "),DIALOG_Y0,
                TXT_DIALOG,COR_DIALOG,"Mao %d:",i + 1);
        Sleep(2000);

        embaralha(baralho);

        distribui_cartas(&cont_baralho,baralho,jogadores,num_players,mesa);

        print_tela(num_players,jogadores,mesa,vlr_blnd,dealer);


        cont_baralho = 0;
        dealer++;
        if(dealer == num_players)
            dealer = 0;
        //LIMPA O VETOR
        atribui_pos(jogadores,num_players,dealer);

        Sleep(2000);
    }

}

/*FUNÇÃO QUE IMPRIME AS ESTATÍSTICAS SALVAS EM UM ARQUIVO-TEXTO*/
int estatisticas(Jogadores* jogadores,int num_players){
    int i;
    FILE* fp;

    fp = fopen("stats.txt","w");

    for(i=0;i<num_players;i++){
        fprintf(fp,"O jogador %s ganhou um total de %d maos.\n",jogadores[i].nome,jogadores[i].wins);
        fprintf(fp,"Terminou o jogo com R$%d\n",jogadores[i].fichas);
    }
    return 5;
}

//CASO O USUÁRIO NÃO INSERIR OS PARÂMETROS, ESSA FUNÇÃO O FAZ
Jogadores* set_values(int* num_players,int* qnt_maos,int* vlr_blnd,int* vlr_fichas){
    int i;
    char ch;
    Jogadores* jogadores;
    srand(time(NULL));

    *num_players = rand()%5 + 2;
    *qnt_maos = rand()%10 + 1;
    *vlr_blnd = rand()%10 + 1;
    *vlr_fichas = (rand()%10 + 1)*100;

    jogadores = malloc(sizeof(Jogadores) * (*num_players));

    for(i=0;i<(*num_players);i++){
        jogadores[i].estilo = rand()%3 + 1;
        strcpy(jogadores[i].nome,"BOT ");

        ch = i + 49;

        jogadores[i].nome[4] = ch;
        jogadores[i].nome[5] = '\0';

    }

    return jogadores;
}
