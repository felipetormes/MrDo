#include <stdio.h>
#include <Windows.h>
#include "conio.h"
#include "conio.c"
#include <string.h>

#include <stdlib.h>

#define NUMLIN 23
#define NUMCOL 60

#define LINPATEA 16 //Linhas Patrocinio EA
#define COLPATEA 71 //Colunas Patrocinio EA

#define LINPATINF 16 // Linhas Patrocinio INF
#define COLPATINF 68 // Colunas Patrocinio INF

#define CIMA 72;
#define BAIXO 80
#define ESQUERDA 75
#define DIREITA 77

#define MRDO 227
#define NINHO 206
#define PAREDE 176
#define FRUTA 229
#define TIRO 250
#define VAZIO 32
#define FANTASMA 153

#define TAMHIGHSCORES 40
#define MAXSCORES 5

//prototipagem de funcoes
void mov(char mat[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *fp, FILE *continuar, int pos_fant[][2]);
int abre_le_matriz(char mat[NUMLIN][NUMCOL], FILE *fp, char nome[20]);
int abre_le_imprime_continuar(char mat[NUMLIN][NUMCOL], FILE *continuar, char continua[20], int score);
void patrocinio_ea(void);
void patrocinio_inf(void);
int menu();
void opcao_salvar(char mat[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *fp, FILE *continuar, int pos_fant[][2], int score);
void barreira_horizontal_cima(void);
void barreira_horizontal_baixo(void);
void imprime_matriz(char mat[NUMLIN][NUMCOL]);
void salvar_elementos_cenario(char mat[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2]);
int abre_le_matriz_fase_dois(char mat[NUMLIN][NUMCOL], FILE *fp);
void grava_highscore(int score, int op, char mat[NUMLIN][NUMCOL],int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *fp, FILE *continuar, int pos_fant[][2]);
void le_bin();
void inic_random();
void fase_2(char mat[NUMLIN][NUMCOL], FILE *fp, int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *continuar, int pos_fant[][2], int score);
void fim_jogo();
//int salva_posicao_fantasmas(char mat[NUMLIN][NUMCOL], COORDENADAS fantasmas[]);
//void continua_salvo(char mat_salvo[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *continuar);

/*void grava_arquivo(char *nome_arq, HIGHSCORE entradas[], int numscores);
void imprime_highscores(HIGHSCORE entradas[], int numentradas);*/
//-------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct //Struct para o Highscore
{
    char name[TAMHIGHSCORES];
    int score;
} HIGHSCORE;

typedef struct //Essa estrutura e as proximas duas estavam sendo usadas para o implemento dos fantasmas, mas tiveram pequenos bugs na hora de rodar o codigo
{
    int score;
    int ghosts;
} GameState;

typedef struct
{
    int y;
    int x;
    int lastDirection;  // 0 = cima, 1 = esquerda, 2 = baixo, 3 = direita
    char status;
} COORDENADAS;

int abre_le_matriz(char mat[NUMLIN][NUMCOL], FILE *fp, char nome[20])
{
    int i,j;
    char trash;//Trash usado para pegar o \n
    int erro;//flag usada para indicar quando a fase nao pode ser aberta por algum motivo aleatorio

    fp = fopen(nome, "r" );

    if(fp != NULL)
    {
        for(i=0; i<NUMLIN; i++)
        {
            for(j=0; j<NUMCOL; j++)
            {
                mat[i][j] = fgetc(fp);//cada posicao da matriz recebe um caracter do arquivo
            }
            trash = fgetc(fp); //Salva o \0 por causa da string
        }
    }
    else//indicarah o erro em caso de (fp == NULL)
    {
        printf("\n    Fase nao encontrada!\n");
        printf("\n Jogo nao poderah ser rodado!\n");
        erro = -1;//usado no retorno da flag erro
    }
    return erro;
}

int abre_le_matriz_fase_dois(char mat[NUMLIN][NUMCOL], FILE *fp)//Seriam os mesmos comentarios utilizados na funcao abre_le_matriz da fase1
{
    int i,j;
    char trash;
    int erro;

    fp = fopen("fase2.txt", "r" );

    if(fp != NULL)
    {
        for(i=0; i<NUMLIN; i++)
        {
            for(j=0; j<NUMCOL; j++)
            {
                mat[i][j] = fgetc(fp);
            }
            trash = fgetc(fp); //Salva o \0 por causa da string
        }
    }
    else
    {
        printf("\n    Fase 2 nao encontrada!\n");
        printf("\n Jogo nao poderah ser rodado!\n");
        erro = -1;
    }
    return erro;
}

int abre_le_imprime_continuar(char mat[NUMLIN][NUMCOL], FILE *continuar, char continua[20], int score)//quando eh retornado 2 pela op do menu vem para cah
{
    //Estava tentando arrumar a parte do score aparecer com numeros aleatorios ao lado do score normal.
//    gotoxy(67, 8); //Posicao na tela do score
//    textcolor(14);
//    printf("SCORE: %04d", score);//-1 para o score no momento em que nao salva e volta para o jogo nao somar 1

    int i,j;
    char trash;
    int erro;

    continuar = fopen("continuar.txt", "r" );
    //system("cls");
    if(continuar != NULL)
    {
        for(i=0; i<NUMLIN; i++)
        {
            for(j=0; j<NUMCOL; j++)
            {
                mat[i][j] = fgetc(continuar);
            }
            trash = fgetc(continuar); //Salva o \0 por causa da string
        }

    }
    else//caso a fase continuar.txt nao for encontrada ele fecha o jogo
    {
        gotoxy(20,13);
        textcolor(YELLOW);
        printf("===========================================");
        gotoxy(27,14);
        textcolor(YELLOW);
        printf("  Jogo salvo nao encontrado!!!");
        gotoxy(20,15);
        textcolor(YELLOW);
        printf("===========================================\n\n\n\n\n\n\n\n\n\n\n\n");
        erro = -1;
    }
    return erro;
}

//Função com sigla em minuscula e matriz com siglas em maiusculas
void patrocinio_ea(void)//Os patrocinios(INF e EA) sao feitos em forma de matriz e depois convertidas para caracteres ascii(quadradinho)
{
    int f, g;

    char patrocinio_EA[LINPATEA][COLPATEA] =
    {
        "eeeeeeeeeeeeeeeeeeeeeeeeooooooooooooooooooooooeeeeeeoeeeeeeeeeeeeeeeeee", //1
        "eeeeeeeeeeeeeeeeeeeeeeeeeoooooooooooooooooooooooeeeeeoooeeeeeeeeeeeeeee", //2
        "eeeeeeeeeeeeeeeeeeeeeeeeooooooooooooooooooooooooeeeeoooooeeeeeeeeeeeeee", //3
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeoooooooeeeeeeeeeeeee", //4
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeoooooooooeeeeeeeeeeee", //5
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeoooooooooooeeeeeeeeeee", //6
        "eeeeeeeeeeeeeeeeeeeoooooooooooooooooooooooeeeeeeooooooeooooooeeeeeeeeee", //7
        "eeeeeeeeeeeeeeeeeeoooooooooooooooooooooooeeeeeeooooooeeeooooooeeeeeeeee", //8
        "eeeeeeeeeeeeeeeeeoooooooooooooooooooooooeeeeeeooooooeeeeeooooooeeeeeeee", //9
        "eeeeeeeeeeeeeeeeoooooooooooooooooooooooeeeeeeooooooooeeoooooooooeeeeeee", //10
        "eeeeeeeeeeeeeeeeeeeoooooooeeeeeeeeeeeeeeeeeeooooooooeeoooooooooooeeeeee", //11
        "eeeeeeeeeeeeeeeeeeoooooooeeeeeeeeeeeeeeeeeeooooooooeeoooooooooooooeeeee", //12
        "eeeeeeeeeeeeeeeeeoooooooeeeeeeeeeeeeeeeeeeooooooooeeoooooooooooooooeeee", //13
        "eeeeeeeeeeeeeeeeoooooooooooooooooooooooooooeeeeeeeeeeeeeeeeeeeeeooooeee", //14
        "eeeeeeeeeeeeeeeooooooooooooooooooooooooooooeeeeeeeeeeeeeeeeeeeeeeooooee", //15
        "eeeeeeeeeeeeeeoooooooooooooooooooooooooooooeeeeeeeeeeeeeeeeeeeeeeeoooee", //16
    };

    for(f=0; f<=LINPATEA; f++)
    {
        for(g=0; g<=COLPATEA; g++)
        {
            switch(patrocinio_EA[f][g])
            {
            case 'e':
                printf(" ");
                break;
            case 'o':
                printf("%c", 178);
                break;
            default:
                printf(" ");
            }
        }
        printf("\n");
    }
}

void patrocinio_inf(void)
{
    int f,g;

    char patrocinio_INF[LINPATINF][COLPATINF] =
    {
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeefffffffeeee",
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeefffffffffff",
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffffffffffff",
        "eeeeeeeeeeeeeeeeeeeffffffeeeffffeeeeeeffffffffffeeeeeeeffffffffeeeee",
        "eeeeeeeeeeeeeeeeeeeffffffeeeffffffeeffffffffffffffeeeeefffffffeeeeee",
        "eeeeeeeeeeeeeeeeeeeffffffeeeffffffffffffffffffffffffeeeffffffffffffe",
        "eeeeeeeeeeeeeeeeeeeffffffeeeffffffffeeeeeeeeffffffffeeeffffffffffffe",
        "eeeeeeeeeeeeeeeeeeeffffffeeefffffffeeeeeeeeeefffffffeeefffffffeeeeee",
        "eeeeeeeeeeeeeeeeeeeffffffeeefffffffeeeeeeeeeefffffffeeefffffffeeeeee",
        "eeeeeeeeeeeeeeeeeeeffffffeeefffffffeeeeeeeeeefffffffeeefffffffeeeeee",
        "eeeeeeeeeeeeeeeeeeeffffffeeefffffffeeeeeeeeeefffffffeeefffffffeeeeee",
        "eeeeeeeeeeeeeeeeeeeffffffeeefffffffeeeeeeeeeefffffffeeefffffffeeeeee",
        "eeeeeeeeeeefffeeeeeffffffeeefffffffeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
        "eeeeeeeeefffffffeeeffffffeeefffffffeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
        "eeeeeeeeefffffffeeeffffffeeefffffffeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
        "eeeeeeeeeefffffeeeeffffffeeefffffffeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
    };

    for(f=0; f<=LINPATINF; f++)
    {
        for(g=0; g<=COLPATINF; g++)
        {
            switch(patrocinio_INF[f][g])
            {
            case 'e':
                printf(" ");
                break;
            case 'f':
                printf("%c", 178);
                break;
            default:
                printf(" ");
            }
        }
        printf("\n");
    }
}

int menu()
{

    _setcursortype(_NOCURSOR);
    char tecla;
    int i, op = 1, x = 29, y1 = 20, y2 = 17, y;


    //_______________________________________________________________________________________________
    for(i=0 ; i<4 ; i++) //PATROCINIOS DA ELETRONIC ARTS
    {
        gotoxy(3, 4); //posicao do centro da tela
        patrocinio_ea();
        printf("\n\t\t  Eletronic Arts, Patrocinadora Oficial Deste Jogo\n");
        Sleep(900);
        clrscr();
        Sleep(400);
    }

//________________________________________________________________________________________________

    for(i=0 ; i<4 ; i++) //PATROCINIOS DO INF
    {
        gotoxy(5, 4); //posicao 30,13 eh do centro da tela
        patrocinio_inf();
        printf("\n\t\t\tInstituto Oficial do Game MrDo!\n");
        Sleep(900);
        clrscr();
        Sleep(400);
    }

//________________________________________________________________________________________________


    while(tecla != 13) //Enquanto nao foi pressionado enter, fica piscando na tela o "pressione enter~" para entrar no jogo
    {
        if( kbhit( ) ) //Aguardando usuaria digitar uma tecla
            tecla = getch();
        gotoxy(30, 12); //posicao 30,13 eh do centro da tela
        printf("_____________________________\n");
        gotoxy(30, 14);
        printf(" Pressione ENTER para Comecar");
        gotoxy(30, 15);
        printf("_____________________________\n");
        Sleep(100);//efeito de "pisca-pisca" nessa e nas proximas duas linhas
        clrscr();
        Sleep(100);
    }

    gotoxy(14,8);
    textcolor(RED);
    printf("Pressione espaco para entrar em um das opcoes abaixo");

    gotoxy(19, 13); //posicao da barreira horizontal superior do menu
    textcolor(14);
    for(i=0; i<40; i++) //Imprime uma barra superior no menu
        printf("%c", 223);//Caracter de quadradinho

    gotoxy(19, 21);//posicao da barreira horizontal inferior do menu
    textcolor(14);
    for(i=0; i<40; i++)
        printf("%c", 220);

    gotoxy(36, 14);
    printf("Mr. Do!");

    gotoxy(32, 17);
    printf("Novo Jogo");

    gotoxy(29, 17);
    textcolor(3);
    printf("%c", MRDO);

    gotoxy(32, 18);
    textcolor(14);
    printf("Continuar");

    gotoxy(32, 19);
    printf("High Scores");

    gotoxy(32, 20);
    printf("Sair");

    do//A variável "op" verifica em qual opção o cursor está, sendo: 1 para "Novo jogo", 2 para "Continuar",
    {
        if( kbhit( ) )
        {
            tecla = getch( );
            tecla = toupper(tecla);
            Sleep(150); //delay do menu para mrdo nao andar muito rapido caso a tecla direcionar fique pressionada

            switch(tecla)
            {
            case 'W':
                if(op == 1)//Caso o caracter do Mr. Do esteja na primeira opção e a tecla "W" for pressionada, o caracter do Mr. Do eh impresso na posicao 4
                {
                    op = 4;
                    gotoxy(29, 17);
                    printf("%c", 255); //Printa vazio
                    gotoxy(29, 20);
                    textcolor(3);
                    printf("%c", MRDO);//Vai para outra posicao e printa o Mrdo
                }
                else if(op == 4 || op == 3 || op == 2)//Se o caracter do Mr. Do não estiver na primeira opção, sua posição y é decrementada
                {
                    gotoxy(x, y1);
                    printf("%c", 255);
                    y1--; //y1 é a variável y quando é movimentado para cima.
                    y2 = y1; //y2 é a variável y quando é movimentado para baixo.
                    gotoxy(x, y1); //y2 = y1 para que quando o caracter for movimentado para baixo o programa saiba qual a coordenada y.
                    textcolor(3);
                    printf("%c", MRDO);
                    op--; //Contador da opção é decrementado.
                    if  (y1 == 17)
                        y1 = 20;
                }
                break;
            case 'S':
                if(op == 4)//Caso o caracter do Mr. Do esteja na última opção e a tecla "S" for pressionada, o caracter do Mr. Do na opcao 1
                {
                    op = 1;
                    gotoxy(29, 20);
                    printf("%c", 255);
                    gotoxy(29, 17);
                    textcolor(3);
                    printf("%c", MRDO);
                }
                else if(op == 1 || op == 2 || op == 3)  //Se o caracter do Mr. Do não estiver na última opção, sua posição y é adicionada
                {
                    //e o Mr. Do é posto na opção de baixo.
                    gotoxy(x, y1);
                    gotoxy(x, y2);
                    printf("%c", 255);              //y2 é a variável y quando é movimentado para baixo.
                    y2++;                           //y1 é a variável y quando é movimentado para cima.
                    y1 = y2;                        //y1 = y2 para que quando o caracter for movimentado para baixo o programa saiba qual a coordenada y.
                    gotoxy(x, y2);
                    textcolor(3);
                    printf("%c", MRDO);
                    op++;                           //Contador da opção é adicionado.
                    if  (y2 == 20)
                        y2 = 17;
                }
                break;
            }
        }
    }
    while((op == 4 || op == 3 || op == 2 || op == 1) && tecla != 32);

    switch(op)//Dependendo da opção selecionada, a variável y recebe a coordenada y correspondente a opção
    {
    case 1:
        y = 17;
        break;
    case 2:
        y = 18;
        break;
    case 3:
        y = 19;
        break;
    case 4:
        y = 20;
        break;
    }

    while(x != 43)
    {
        gotoxy(x, y);
        printf("%c", 255);//Efeito que faz o Mr. Do "comer" a opção selecionada daqui ateh o Sleep(100)
        x++;
        gotoxy(x, y);
        textcolor(3);
        printf("%c", MRDO);
        Sleep(100);
    }
    textcolor(15);
    if(op == 2)
    {
        return 2;
    }

    else if(op == 4)
    {
        return 4;
    }
    else if (op == 3)
        return 3;

    clrscr();
    gotoxy(1, 1);
    return 1;
}

void opcao_salvar(char mat[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *fp, FILE *continuar, int pos_fant[][2], int score)
{
    int op;//essa opcao foi usada no menu
    char continua[20]="continuar.txt";//Nome do arquivo usado para carregar o jogo salvo
    char fase1[20]="fase1.txt";//Nome do arquivo usado para carregar o jogo inicial
    char resposta_salvamento;//caracter que vai receber as resposta no Menu Pause do Game

    gotoxy(65, 13);
    printf("Jogo Pausado   \n");//Espacos colocados ao lado de "Pausado" eh para que quando for digitado em cima de "Despausado" nao de erro de caracteres
    gotoxy(66, 15);
    printf("Salvar:");
    gotoxy(66, 16);
    printf("S - Sim:");
    gotoxy(66, 17);
    printf("N - Nao");
    gotoxy(66, 18);
    printf("Q - Sair");
    gotoxy(66, 20);
    printf(">>> ");
    scanf(" %c", &resposta_salvamento);

    resposta_salvamento = toupper(resposta_salvamento);//Transforma para maiuscula a resposta dada na pergunta de salvar

    switch(resposta_salvamento)
    {
    case 'S'://caso que salva os elementos da matriz num arquivo texto
    {
        salvar_elementos_cenario(mat,pos_fru, pos_ninho, pos_do); //Se a resposta for sim o jogo leva para a funcao onde salva tudo que estah na matriz atual no arquivo continuar.txt
        break;
    }
    case 'N'://caso para nao salvar e continuar o jogo
    {
        gotoxy(65, 13);
        printf("Jogo Desausado\n");
        Sleep(850);
        gotoxy(65, 13);
        printf("                   ");//os espacos abaixo sao usados para limpar o menu pause depois da escolha da resposta
        gotoxy(65, 14);
        printf("                   ");
        gotoxy(65, 15);
        printf("                   ");
        gotoxy(65, 16);
        printf("                   ");
        gotoxy(65, 17);
        printf("                   ");
        gotoxy(65, 18);
        printf("                   ");
        gotoxy(65, 19);
        printf("                   ");
        gotoxy(65, 20);
        printf("                   ");
        gotoxy(65, 21);
        printf("                   ");
        gotoxy(65, 22);
        printf("                   ");
        gotoxy(65, 23);
        printf("                   ");
        gotoxy(65, 24);
        printf("                   ");
        break;
    }
    case 'Q'://caso para sair
    {
        op = menu(); //chama o menu de novo e segue o jogo normalmente assim que escolhida a opcao que o usuario queira entrar no game
        if(op == 2)
        {
            system("cls");
            abre_le_imprime_continuar(mat, continuar, continua, score);
            imprime_matriz(mat);
        }
        else if(op == 1)
        {
            abre_le_matriz(mat, fp,fase1);
            system("cls");
            imprime_matriz(mat);
            mov(mat, pos_fru, pos_ninho, pos_do, fp, continuar, pos_fant);
        }
        else if(op == 3)
        {
            system("cls");//Opcao onde mostra o highscore
            le_bin();
        }
        else if(op == 4)
        {
            system("cls");
            gotoxy(20,13);
            textcolor(YELLOW);
            printf("===========================================");
            gotoxy(27,14);
            textcolor(YELLOW);
            printf("Jogo encerrado com sucesso!!!");
            gotoxy(20,15);
            textcolor(YELLOW);
            printf("===========================================\n\n\n\n\n\n\n\n\n\n\n\n");
            exit(EXIT_FAILURE);
        }


        break;
    }
    default://Caso para o usuario nao digitar nem S,N ou Q
        gotoxy(64, 22);
        kbhit();
        printf("Opcao Invalida!");
        gotoxy(65, 13);
        printf("Jogo Desausado\n");
        Sleep(850);
        gotoxy(65, 13);
        printf("                   ");
        gotoxy(65, 14);
        printf("                   ");
        gotoxy(65, 15);
        printf("                   ");
        gotoxy(65, 16);
        printf("                   ");
        gotoxy(65, 17);
        printf("                   ");
        gotoxy(65, 18);
        printf("                   ");
        gotoxy(65, 19);
        printf("                   ");
        gotoxy(65, 20);
        printf("                   ");
        gotoxy(65, 21);
        printf("                   ");
        gotoxy(65, 22);
        printf("                   ");
        gotoxy(65, 23);
        printf("                   ");
        gotoxy(65, 24);
        printf("                   ");
        break;
    }
}

void mov(char mat[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *fp, FILE *continuar, int pos_fant[][2])
{
    int cont_fruta_fase_2=0;
    int game_over = -1;//flag para o game over
    int op;//opcao que serah retornada pelo menu
    int cont_fruta = 32;//Usado no contador para passar para a proxima fase
    _setcursortype(_NOCURSOR);//Para nao ficar piscando o cursor enquanto o jogo eh executado
    char tecla, tecla_anterior = 'D';//tecla->movimentacao do MrDo, tecla_anterior->Usado para o tiro
    int x, y, k=0, i, j, score=0;//x,y->usados na matriz, k->contagem de frutas, i,j->Usados na matriz, score iniciando em zero
    int f=0; //Contador inicial de fantasmas
    int x_tiro, y_tiro;//Coordenadas do tiro
    int resposta_salvamento;//Usado na opcao_salvar

    for(i=0; i<NUMLIN; i++)
    {
        for(j=0; j<NUMCOL; j++)
        {
            if(mat[i][j] == 'f') //Varre a matriz em busca de frutas
            {
                pos_fru[k][0] = i;
                pos_fru[k][1] = j;
                k++; //Incrementa um em cada fruta encontrada
            }
            else if(mat[i][j] == 'i') //Varre a matriz em busca de fantasmas
            {
                pos_fant[f][0] = i;
                pos_fant[f][1] = j;
                f++; //Incrementa um a cada fantasma encontrado
            }
            else if(mat[i][j] == 'n') //Varre a matriz em busca do ninho
            {
                pos_ninho[0] = i;
                pos_ninho[1] = j;
            }

            else if(mat[i][j] == 'd') //Varre a matriz em busca do MrDo
            {
                pos_do[0] = i;
                pos_do[1] = j;
            }
        }
    }

    y = pos_do[0] + 2; //Soma-se dois devido a posicao da barreira horizontal
    x = pos_do[1] + 2; //Soma-se dois devido a posicao da barreira horizontal

    do
    {
        // gotoxy(65,5);
        // printf("%c", mat[x-1][y-1]); -IMPRIME CARACTER QUE O MRDO ESTAH PASSANDO

        if( kbhit( ) ) //Enquanto ha uma tecla que pode ser digitada
        {
            tecla = getch(); //Recebe o caracter em forma de char da tecla digitada via teclado
            tecla = toupper(tecla); //Transforma a tecla em maiusculas

            switch(tecla)
            {

            case 'C'://Chama creditos do jogo
                fim_jogo();
                break;

            case 'P'://Pausa o game quando digitado `P`
            {
                mat[y-2][x-2] = 'd';//Salva posicao atual do MrDo na matriz
                mat[pos_do[0]][pos_do[1]] = 'v';//Coloca vazio na posicao inicial do MrDo

                opcao_salvar(mat, pos_fru, pos_ninho, pos_do, fp, continuar, pos_fant, score);//Chamamento da funcao que salva os elementos do cenario
                //imprime_matriz(mat);
                break;
            }


            case 'W'://Mover MrDo para cima
                tecla_anterior = 'W';
                if(y!=2)
                {
                    //Sleep(200); //Velocidade do MrDo alterada para nao correr demais
                    if(pos_ninho[0] != y-2 || pos_ninho[1] != x-2)
                    {
                        gotoxy(x, y);
                        printf("%c", VAZIO);
                        y--;//Decremento na matriz porque a matriz do jogo eh invertida em Y
                        //gotoxy(5, 23);
                        //printf("%c", mat[y-2][x-2]);
                        mat[y-2][x-2] = 'v'; //Cada vez que o MrDo anda coloca-se vazio na posicao anterior
                        textcolor(3);
                        gotoxy(x, y);
                        printf("%c", MRDO); //Impressao do MrDo na tela
                    }
                    else//Condicao para quando o MrDo estiver em cima do ninho
                    {
                        gotoxy(x, y);
                        textcolor(8);
                        printf("%c", NINHO);
                        mat[y-2][x-2] = 'n';//Ao inves de printar vazio, printa o ninho
                        y--;
                        gotoxy(x, y);
                        textcolor(3);
                        printf("%c", MRDO);
                    }
                }
                break;

            /*As proximas direcoes tem a mesma logica utilizada da que quando o MrDo vai para cima, por isso os comentarios serao soh em partes que se diferencia do caso do personagem se mover para cima*/
            case 'S': //MrDo vai para baixo
                tecla_anterior = 'S';
                if(y!=24)
                {
                    //Sleep(200); //Velocidade do MrDo alterada para nao correr demais
                    if(pos_ninho[0] != y-2 || pos_ninho[1] != x-2)
                    {
                        gotoxy(x, y);
                        printf("%c", VAZIO);
                        y++;//Uma posicao abaixo na matriz do jogo
                        mat[y-2][x-2] = 'v';
                        textcolor(3);
                        gotoxy(x, y);
                        printf("%c", MRDO);
                    }
                    else
                    {
                        gotoxy(x, y);
                        textcolor(8);
                        printf("%c", NINHO);
                        mat[y-2][x-2] = 'n';
                        y++;
                        mat[y-2][x-2] = 'v';
                        gotoxy(x, y);
                        textcolor(3);
                        printf("%c", MRDO);
                    }
                }
                break;
            case 'A':
                tecla_anterior = 'A';
                if(x!=2)
                {
                    //Sleep(200); //Velocidade do MrDo alterada para nao correr demais
                    if(pos_ninho[0] != y-2 || pos_ninho[1] != x-2)
                    {
                        gotoxy(x, y);
                        printf("%c", VAZIO);
                        x--; //Uma posicao para o lado esquerdo da matriz
                        mat[y-2][x-2] = 'v';
                        textcolor(3);
                        gotoxy(x, y);
                        printf("%c", MRDO);
                    }
                    else
                    {

                        gotoxy(x, y);
                        textcolor(8);
                        printf("%c", NINHO);
                        mat[y-2][x-2] = 'n';
                        x--;
                        gotoxy(x, y);
                        textcolor(3);
                        printf("%c", MRDO);
                    }
                }
                break;

            case 'D':
                tecla_anterior = 'D';
                if(x!=61)
                {
                    //Sleep(200); //Velocidade do MrDo alterada para nao correr demais
                    if(pos_ninho[0] != y-2 || pos_ninho[1] != x-2)
                    {
                        gotoxy(x, y);
                        printf("%c", VAZIO);
                        x++; // Uma posicao para o lado direito da matriz
                        mat[y-2][x-2] = 'v';
                        textcolor(3);
                        gotoxy(x, y);
                        printf("%c", MRDO);
                    }
                    else
                    {
                        gotoxy(x, y);
                        textcolor(8);
                        printf("%c", NINHO);
                        mat[y-2][x-2] = 'n';
                        x++;
                        gotoxy(x, y);
                        textcolor(3);
                        printf("%c", MRDO);
                    }
                }
                break;

            case 32: // Caso em que a tecla ESPACO eh pressionada(Inicio do tiro)
                switch(tecla_anterior)//Recebe a ultima posicao em que o MrDo estava apontando
                {
                case 'W': //Caso a tecla anterior for "W", o tiro vai para cima.
                    x_tiro = x;
                    y_tiro = y - 1;
                    while(y_tiro != 1 && mat[y_tiro - 2][x_tiro - 2] != 'n' && mat[y_tiro - 2][x_tiro - 2] != 'p' && mat[y_tiro - 2][x_tiro - 2] != 'f'
                            && mat[y_tiro - 2][x_tiro - 2] != 'i')
                    {
                        textcolor(WHITE); //Cor branca para o tiro
                        gotoxy (x_tiro, y_tiro);//Coordenadas do tiro
                        printf("%c", 250);// Printa o tiro uma posicao por vez no intervalo de tempo de 30 milisegundos
                        Sleep(30);//Vekicidade do tiro
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);//Printa caminho
                        y_tiro--;//Incrementa uma posicao para cima no tiro
                    }
                    if(mat[y_tiro - 2][x_tiro - 2] == 'i') //Caso o tiro colida com um fantasma
                    {
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 42);//Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        Sleep(60);//explosao do tiro colidindo
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        mat[y_tiro-2][x_tiro-2] = 'v';//Imprime vazio onde estava o fantasma
                        printf("\a");//Som de um "beep" quando ocorre a colisao tiro-fantasma
                        for(i=0; i<10; i++)//for que percorre o numero de fantasma ateh as posicoes do mesmo serem iguais a do tiro
                        {
                            if(pos_fant[i][0] == (y_tiro-2) && pos_fant[i][1] == (x_tiro-2))
                            {
                                pos_fant[i][0] = 'v';
                                pos_fant[i][1] = 'v';
                            }
                        }
                        score += 10; //Incrementa 10 pontos a cada tiro acertado no fantasma
                        gotoxy(67, 8); //Posicao na tela do score
                        textcolor(14);
                        printf("SCORE: %04d", score);//Impressao do score na tela
                    }
                    else//Caso o tiro colida em outra coisa
                    {
                        gotoxy (x_tiro, y_tiro+1);
                        printf("%c", 42);
                        Sleep(60);//explosao do tiro colidindo, 60->Velocidade da explosao do tiro
                        gotoxy (x_tiro, y_tiro+1);
                        printf("%c", 32);
                        printf("\a"); //Beep
                    }
                    break;
                /*As proximas direcoes do tiro seguem o mesmo algoritmo de quando ele for para cima, mudando soh as coordenadas X,Y*/
                case 'S': //Caso a tecla anterior for "S", o tiro vai para baixo.
                    x_tiro = x;
                    y_tiro = y + 1;
                    while(y_tiro != 25 && mat[y_tiro - 2][x_tiro - 2] != 'n' && mat[y_tiro - 2][x_tiro - 2] != 'p' && mat[y_tiro - 2][x_tiro - 2] != 'f' && mat[y_tiro - 2][x_tiro - 2] != 'i')
                    {
                        textcolor(WHITE);
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 250);
                        Sleep(30);
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        y_tiro++; //Tiro para baixo
                    }
                    if(mat[y_tiro - 2][x_tiro - 2] == 'i')
                    {
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 42);               //Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        Sleep(60);//explosao do tiro colidindo
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        mat[y_tiro-2][x_tiro-2] = 'v';
                        printf("\a");
                        for(i=0; i<10; i++)
                        {
                            if(pos_fant[i][0] == (y_tiro-2) && pos_fant[i][1] == (x_tiro-2))
                            {
                                pos_fant[i][0] = 'v';
                                pos_fant[i][1] = 'v';
                            }
                        }
                        score += 10;
                        gotoxy(67, 8); //Posicao na tela do score
                        textcolor(14);
                        printf("SCORE: %04d", score);
                    }
                    else
                    {
                        gotoxy (x_tiro, y_tiro-1);
                        printf("%c", 42);               //Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        Sleep(60);//explosao do tiro colidindo
                        gotoxy (x_tiro, y_tiro-1);
                        printf("%c", 32);
                        printf("\a");
                    }
                    break;

                case 'A': //Caso a tecla anterior for "A", o tiro vai para o lado esquerdo.
                    x_tiro = x - 1;
                    y_tiro = y;
                    while(x_tiro != 1 && mat[y_tiro - 2][x_tiro - 2] != 'n' && mat[y_tiro - 2][x_tiro - 2] != 'p' && mat[y_tiro - 2][x_tiro - 2] != 'f'
                            && mat[y_tiro - 2][x_tiro - 2] != 'i')
                    {
                        textcolor(WHITE);
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 250);
                        Sleep(30);
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        x_tiro--; //Tiro para o lado esquerdo
                    }
                    if(mat[y_tiro - 2][x_tiro - 2] == 'i')
                    {
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 42);               //Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        Sleep(60);//explosao do tiro colidindo
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        mat[y_tiro-2][x_tiro-2] = 'v';
                        printf("\a");
                        for(i=0; i<10; i++)
                        {
                            if(pos_fant[i][0] == (y_tiro-2) && pos_fant[i][1] == (x_tiro-2))
                            {
                                pos_fant[i][0] = 'v';
                                pos_fant[i][1] = 'v';
                            }
                        }
                        score += 10;
                        gotoxy(67, 8); //Posicao na tela do score
                        textcolor(14);
                        printf("SCORE: %04d", score);
                    }
                    else
                    {
                        gotoxy (x_tiro+1, y_tiro);
                        printf("%c", 42);
                        Sleep(60); //Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        gotoxy (x_tiro+1, y_tiro);
                        printf("%c", 32);
                        printf("\a");
                    }
                    break;

                case 'D': //Caso a tecla anterior for "D", o tiro vai para o lado direito.
                    x_tiro = x + 1;
                    y_tiro = y;
                    while(x_tiro != 62 && mat[y_tiro - 2][x_tiro - 2] != 'n' && mat[y_tiro - 2][x_tiro - 2] != 'p' && mat[y_tiro - 2][x_tiro - 2] != 'f'
                            && mat[y_tiro - 2][x_tiro - 2] != 'i')
                    {
                        textcolor(WHITE);
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 250);
                        Sleep(30);
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        x_tiro++;//Tiro para a direita
                    }
                    if(mat[y_tiro - 2][x_tiro - 2] == 'i')
                    {
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 42);               //Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        Sleep(60); //explosao do tiro colidindo
                        gotoxy (x_tiro, y_tiro);
                        printf("%c", 32);
                        mat[y_tiro-2][x_tiro-2] = 'v';
                        printf("\a");
                        for(i=0; i<10; i++)
                        {
                            if(pos_fant[i][0] == (y_tiro-2) && pos_fant[i][1] == (x_tiro-2))
                            {
                                pos_fant[i][0] = 'v';
                                pos_fant[i][1] = 'v';
                            }
                        }
                        score += 10;
                        gotoxy(67, 8); //Posicao na tela do score
                        textcolor(14);
                        printf("SCORE: %04d", score);
                    }
                    else
                    {
                        gotoxy (x_tiro-1, y_tiro);
                        printf("%c", 42);
                        Sleep(60); //Efeito que coloca um arterisco quando o tiro encosta em alguma barreira, simulando uma explosão.
                        gotoxy (x_tiro-1, y_tiro);
                        printf("%c", 32);
                        printf("\a");
                        break;
                    }
                }
                break;
            }
            //} - do -32
            for(i=0; i<32; i++)
            {
                if(pos_fru[i][0] == y-2 && pos_fru[i][1] == x-2 )
                {
                    score += 50;
                    mat[y-2][x-2] = 'v';
                    pos_fru[i][0] = 'v';
                    pos_fru[i][1] = 'v';
                    gotoxy(67, 8); //Posicao na tela do score
                    textcolor(14);
                    printf("SCORE: %04d", score);
                    cont_fruta--;
                }
            }


            for(i=0; i<10; i++)
            {
                if(pos_fant[i][0] == y-2 && pos_fant[i][1] == x-2) //Quando acontecer a colisao de MrDo e fantasmas, vai para a funcao onde grava o highscore atual e o nome do jogador
                {
                    grava_highscore(score, op, mat, pos_fru, pos_ninho, pos_do,fp, continuar, pos_fant);
                }
            }

            if(cont_fruta == 0) //Quando as frutas chegarem a zero eh a hora de ir para a fase 2
            {
                clrscr();//Limpa a tela por 3 decimos de segundo
                Sleep(300);
                gotoxy(35,17);
                textcolor(YELLOW);
                printf("Fase 2!");
                Sleep(650);//Pisca uma vez por 650 milisegundos
                clrscr();
                fase_2(mat, fp, pos_fru, pos_ninho, pos_do, continuar, pos_fant, score);
            }
        }
    }
    while(tecla != 27);
}

void barreira_horizontal_cima(void) //Funcao que imprime uma barreira superior na matriz do game
{
    int k;
    for(k=1; k<=62; k++)//62-> Largura da matriz
    {
        textcolor(GREEN);
        printf("%c", 220); //Quadradinho
    }
    printf("\n");
}

void barreira_horizontal_baixo(void) //Funcao que imprime a barreira inferior na matriz do game
{
    int k;
    for(k=1; k<=62; k++)//62-> Largura da matriz
    {
        textcolor(GREEN);
        printf("%c", 223); //Quadradinho

    }
    printf("\n");//Pula uma linha
}

void imprime_matriz(char mat[NUMLIN][NUMCOL])
{
    int i, j, k;
    int score;

    barreira_horizontal_cima();

    for(i=0; i<NUMLIN; i++)
    {
        printf("%c", 219); //Imprime um quadradinho a esquerda em cada linha
        for(j=0; j<NUMCOL; j++)
        {
            switch(mat[i][j])//Switch para decidir os caracteres a serem printados na matriz
            {
            case 'p':
                textcolor(2);
                printf("%c", PAREDE);
                break;
            case 'v':
                printf("%c", VAZIO);
                break;
            case 'i':
                textcolor(WHITE);
                printf("%c", FANTASMA);
                break;
            case 'd':
                textcolor(3);
                printf("%c", MRDO);
                break;
            case 'n':
                textcolor(8);
                printf("%c", NINHO);
                break;
            case 'f':
                textcolor(4);
                printf("%c", FRUTA);
                break;
            }
        }
        printf("%c", 219); //Imprime um quadradinho à direita da linha
        printf("\n"); //vai para a proxima linha para imprimir um quadradinho de cada lado novamente ateh chegar no final da matrzi
    }
    barreira_horizontal_baixo();

    //Imprimir score no inicio do jogo
    gotoxy(67, 8); //Posicao na tela do score
    textcolor(14);
    printf("SCORE: %04d", (score)-1);//-1 para o score no momento em que nao salva e volta para o jogo nao somar 1

}

void salvar_elementos_cenario(char mat[NUMLIN][NUMCOL], int pos_fru[][2], int pos_ninho[2], int pos_do[2])
{
    int x, y;
    int i, j;
    char n='\n';
    FILE *arq;//Ponteiro para ler o arquivo

    arq = fopen("continuar.txt", "w"); //Abrir o arquivo texto "continuar" para escrita

    if(arq != NULL)//Caso o arquivo nao exista
    {
        for(i=0; i<NUMLIN; i++)
        {
            for(j=0; j<NUMCOL; j++)
            {
                fprintf(arq, "%c", mat[i][j]); //fprintf(fp, formatos, lista de variáveis)
            }

            fprintf(arq,"%c",'\n'); //Precisa printar uma quebra de linha
        }

        {
            gotoxy(65, 23);
            printf("Arquivo salvo");
            gotoxy(65, 24);
            printf("Com sucesso!");
            gotoxy(65, 13);
            printf("Jogo Despausado\n");

            Sleep(850);
            gotoxy(65, 13);
            printf("                   ");
            gotoxy(65, 14);
            printf("                   ");
            gotoxy(65, 15);
            printf("                   ");
            gotoxy(65, 16);
            printf("                   ");
            gotoxy(65, 17);
            printf("                   ");
            gotoxy(65, 18);
            printf("                   ");
            gotoxy(65, 19);
            printf("                   ");
            gotoxy(65, 20);
            printf("                   ");
            gotoxy(65, 21);
            printf("                   ");
            gotoxy(65, 22);
            printf("                   ");
            gotoxy(65, 23);
            printf("                   ");
            gotoxy(65, 24);
            printf("                   ");
        }
        fclose(arq);
    }
}

void grava_highscore(int score, int op, char mat[NUMLIN][NUMCOL],int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *fp, FILE *continuar, int pos_fant[][2])
{
    system("cls");
    gotoxy(35,16);
    textcolor(YELLOW);
    printf("Game Over!");
    Sleep(1000);


    HIGHSCORE highscore[5];//Declaracao da estrutura highscore de tipo HIGHSCORE criado como global com uso de typedef
    FILE *high_bin; //Ponteiro para escrever no arquivo binario
    char continua[20]="continuar.txt";
    int i; //usar no contador do for para percorrer o highscore
    system("cls");
    gotoxy(35,18);
    gotoxy(6, 10);
    printf("Perdedor no jogo, digite seu nome: ");
    gets(highscore[0].name);
    highscore[0].score = 0;
//        }

    if(!(high_bin = fopen("highscores.bin", "wb")))//Abre o arquivo binario para escrita
    {
        printf("Erro na abertura do arquivo binario!\n");
    }
    else
    {
        for(i=0; i<5; i++)
        {
            fwrite(&highscore[i], sizeof(HIGHSCORE), 1, high_bin);
        }
    }

//    }
    op = menu();//Recebendo o retorno da funcao menu
    if(op == 2)
    {
        system("cls");
        abre_le_matriz(mat, fp,continua);//Chama a funcao que faz o continua do game
    }
    else if(op == 1)//Novo jogo
    {
        abre_le_matriz(mat, fp,"fase1.txt");
        system("cls");
        imprime_matriz(mat);
        mov(mat, pos_fru, pos_ninho, pos_do, fp, continuar, pos_fant);
    }
//        else
//            if(op == 3)
//            {
//             mostrar_highscores();
//            }
    else if(op == 4)//Sair do game
    {
        system("cls");
        gotoxy(20,13);
        textcolor(YELLOW);
        printf("===========================================");
        gotoxy(27,14);
        textcolor(YELLOW);
        printf("Jogo encerrado com sucesso!!!");
        gotoxy(20,15);
        textcolor(YELLOW);
        printf("===========================================\n\n\n\n\n\n\n\n\n\n\n\n");
        exit(EXIT_FAILURE);
    }
    fclose(high_bin);//Fechando o arquivo binario
}

void le_bin()//Para ler o arquivo binario highscores.bin
{
    HIGHSCORE highscore[5];
    HIGHSCORE name[TAMHIGHSCORES];
    FILE *ftp;
    int i;

    ftp = fopen("highscores.bin", "rb");//Abre o arquivo highscores.bin para leitura

    for(i=0; i<5; i++)
    {
        fread(&highscore[i], sizeof(HIGHSCORE), 1, ftp);
        printf("%d\n", highscore[i].score);//Le pontuacao da struct
        printf("%s\n", highscore[i].name);//Le nome da struct
    }
}

void inic_random() //Funcao randomica que estava sendo usada para os fantasmas
{
    long ultime;
    time(&ultime);
    srand((unsigned) ultime);
}

int salva_posicao_fantasmas(char mat[NUMLIN][NUMCOL], COORDENADAS fantasmas[])//Funcao que estava sendo usada para salvar as posicoes a cada movimento dos fantasmas
{
    int x, y, cont = 0;

    for(y = 0; y < NUMLIN; y++)
    {
        for(x = 0; x < NUMCOL; x++)
        {
            if (mat[y][x] == 'g')
            {
                fantasmas[cont].y = y;
                fantasmas[cont].x = x;
                cont ++;
            }
        }
    }
    return cont;
}

int teste_cross(char mat[NUMLIN][NUMCOL], COORDENADAS fantasmas[], int g)//Funcao usada para quando o fantasma estah num cruzamento
{
    int x, y;

    y = fantasmas[g].y;
    x = fantasmas[g].x;

    if(mat[y-2][x] == 'v' && mat[y-2][x-2] == 'v' &&  mat[y][x-1] == 'v' || mat[y+1][x] == 'v' && mat[y-1][x] == 'v' &&  mat[y][x+1] == 'v' || mat[y][x+1] == 'v' && mat[y][x-1] == 'v' &&  mat[y+1][x] == 'v' || mat[y][x+1] == 'v' && mat[y][x-1] == 'v' &&  mat[y-1][x] == 'v' || mat[y+1][x] == 'p' && mat[y][x+1] == 'v' || mat[y+1][x] == 'p' && mat[y][x-1] == 'v' || mat[y-1][x] == 'p' && mat[y][x+1] == 'v' || mat[y-1][x] == 'p' && mat[y][x-1] == 'v')
        return 1;

    else
        return 0;
}

void fase_2(char mat[NUMLIN][NUMCOL], FILE *fp, int pos_fru[][2], int pos_ninho[2], int pos_do[2], FILE *continuar, int pos_fant[][2], int score)
{
    int i;
    int x,y;
    int cont_fruta_fase_2=32;

    abre_le_matriz_fase_dois(mat, fp);//Vai para a funcao onde abre o arquivo da fase 2 do game
    system("cls");//OUtra funcao limpa a tela

    imprime_matriz(mat);//Funcao para ler a matriz

    gotoxy(69, 1);
    textcolor(14);
    printf("Mr. Do!");

    mov(mat, pos_fru, pos_ninho, pos_do, fp, continuar, pos_fant);//Tem que chamar a funcao mover tambem aqui


}

void fim_jogo()
{

    system("cls");
    int x, y;

    _setcursortype(_NOCURSOR);

    textcolor(8);
    gotoxy(40, 7);          //Imprime o ninho.
    printf("%c", NINHO);

    for(y=25; y>12; y--)
    {
        textcolor(7);
        gotoxy(39, y);
        printf("Fim");
        Sleep(150);         //Faz a palavra "Fim" subir.
        gotoxy(39, y);
        printf("    ");
    }
    gotoxy(39, y);
    printf("Fim");
    Sleep(1000);

    for(x=1; x<20; x++)
    {
        textcolor(3);
        gotoxy(x, 12);
        printf("%c", MRDO);     //Move o Mr. Do para a direita até uma certa posição.
        Sleep(100);
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(x, 12);
    printf("%c", MRDO);

    for(x=21; x<40; x++)
    {
        textcolor(7);
        gotoxy(x, 12);
        printf("%c", TIRO);
        Sleep(50);              //O Mr. Do atira na letra "F".
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(39, 12);
    printf("%c", 42);
    Sleep(60);                  //Letra "F" explode.
    gotoxy(39, 12);
    printf("%c", VAZIO);

    for(x=21; x<41; x++)
    {
        gotoxy(x, 12);
        printf("%c", TIRO);
        Sleep(50);              //Mr. Do atira na letra "I".
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(40, 12);
    printf("%c", 42);
    Sleep(60);                  //Letra "I" explode.
    gotoxy(40, 12);
    printf("%c", VAZIO);
    for(x=21; x<42; x++)
    {
        gotoxy(x, 12);
        printf("%c", TIRO);
        Sleep(50);              //Mr. Do atira na letra "M".
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(41, 12);
    printf("%c", 42);
    Sleep(60);                  //Letra "M" explode.
    gotoxy(41, 12);
    printf("%c", VAZIO);

    for(x=20; x<45; x++)
    {
        textcolor(3);
        gotoxy(x, 12);
        printf("%c", MRDO);
        gotoxy(x-10, 12);           // Depois de apagar a palavra "Fim", um fantasma persegue o Mr. Do.
        textcolor(7);
        printf("%c", FANTASMA);
        Sleep(100);
        gotoxy(x, 12);
        printf("%c", VAZIO);
        gotoxy(x-10, 12);
        printf("%c", VAZIO);
    }

    textcolor(3);
    gotoxy(x, 12);
    printf("%c", MRDO);

    for(x=35; x<38; x++)
    {
        textcolor(7);
        gotoxy(x, 12);
        printf("%c", FANTASMA);
        Sleep(100);                 //Mr. Do para e o fantasma anda mais um pouco.
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(x, 12);
    printf("%c", FANTASMA);

    for(x=44; x>38; x--)
    {
        gotoxy(x, 12);
        printf("%c", TIRO);
        Sleep(100);             //Mr. Do atira e mata o fantasma.
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(x, 12);
    printf("%c", 42);
    Sleep(60);                  //Fantasma explode.
    gotoxy(x, 12);
    printf("%c", VAZIO);

    for(x=45; x<62; x++)
    {
        textcolor(3);
        gotoxy(x, 12);
        printf("%c", MRDO);
        Sleep(100);             //Mr. Do anda para a direita até uma certa posição.
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(x, 12);
    printf("%c", MRDO);

    Sleep(200);
    gotoxy(27, 12);                     // Impressão do nome "Felipe Barbosa Tormes".
    textcolor(11);
    printf("Felipe Barbosa Tormes");

    Sleep(1000);

    for(x=62; x>17; x--)
    {
        textcolor(3);
        gotoxy(x, 12);
        printf("%c", MRDO);
        Sleep(100);                 //Mr. Do "come" o nome.
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(x, 12);
    printf("%c", MRDO);

    Sleep(200);

    gotoxy(24, 12);
    textcolor(11);                              // Impressão do nome Levindo Gabriel Taschetto Neto.
    printf("Levindo Gabriel Taschetto Neto");

    Sleep(1000);

    for(x=17; x<62; x++)
    {
        textcolor(3);
        gotoxy(x, 12);
        printf("%c", MRDO);
        Sleep(100);                 // Mr. Do "come" o nome.
        gotoxy(x, 12);
        printf("%c", VAZIO);
    }

    gotoxy(x, 12);
    printf("%c", MRDO);

    for(y=12; y>7; y--)
    {
        gotoxy(62, y);
        printf("%c", MRDO);
        Sleep(100);             //Mr. Do sobe até uma certa posição.
        gotoxy(62, y);
        printf("%c", VAZIO);
    }

    gotoxy(62, y);
    printf("%c", MRDO);

    for(x=62; x>40; x--)
    {
        gotoxy(x, 7);
        printf("%c", MRDO);
        Sleep(100);             //Mr. Do anda para a esquerda até chegar no ninho.
        gotoxy(x, 7);
        printf("%c", VAZIO);
    }

    textcolor(8);
    gotoxy(x, 7);
    printf("%c", NINHO);

    exit(EXIT_FAILURE);
}

int main()
{
    int tecla;//Daqui ateh a linha 1286 eh declaracoes
    int score = 0;
    FILE *fp;
    FILE *continuar;
    char tecla_anterior;
    int x_tiro, x, y_tiro, y;
    int pos_fru[32][2];
    int pos_fant[10][2];
    int pos_do[2];
    char continua[20]="continuar.txt";
    char fase1[20]="fase1.txt";
    int op;
    int pos_ninho[2];
    char mat[NUMLIN][NUMCOL];
    int seguir=-1; //flag para caso o arquivo nao existe o jogo pare

    HIGHSCORE entradas[MAXSCORES];
    char nome_arquivo[TAMHIGHSCORES];
    char novo_nome[TAMHIGHSCORES];

    op = menu();//Chamando a funcao menu com retorno
    if(op == 2)//Continuar
    {
        system("cls");
        seguir = abre_le_imprime_continuar(mat, continuar, continua, score);
    }

    else if(op == 4) //Encerrar game
    {
        system("cls");
        gotoxy(20,13);
        textcolor(YELLOW);
        printf("===========================================");
        gotoxy(27,14);
        textcolor(YELLOW);
        printf("Jogo encerrado com sucesso!!!");
        gotoxy(20,15);
        textcolor(YELLOW);
        printf("===========================================\n\n\n\n\n\n\n\n\n\n\n\n");
        exit(EXIT_FAILURE);
    }
    else if(op == 3)//Chama Highscores
    {
        system("cls");
        le_bin();
        Sleep(8000);
        //main();

        op = menu();
        if(op == 2)
        {
            system("cls");
            abre_le_imprime_continuar(mat, continuar, continua, score);
            imprime_matriz(mat);
        }
        else if(op == 1)
        {
            abre_le_matriz(mat, fp,fase1);
            system("cls");
            imprime_matriz(mat);
            mov(mat, pos_fru, pos_ninho, pos_do, fp, continuar, pos_fant);
        }
        else if(op == 3)
        {
            system("cls");
            le_bin();
        }
        else if(op == 4)
        {
            system("cls");
            gotoxy(20,13);
            textcolor(YELLOW);
            printf("===========================================");
            gotoxy(27,14);
            textcolor(YELLOW);
            printf("Jogo encerrado com sucesso!!!");
            gotoxy(20,15);
            textcolor(YELLOW);
            printf("===========================================\n\n\n\n\n\n\n\n\n\n\n\n");
            exit(EXIT_FAILURE);
        }

    }
    else
        seguir = abre_le_matriz(mat, fp,fase1);//Chamar a matriz enquanto o seguir estiver ativo

    if(seguir != -1)//Se seguir for diferente de menos 1, imprime a matriz novamente
    {
        imprime_matriz(mat);
        gotoxy(69, 3);
        textcolor(14);
        printf("Mr. Do!");

        gotoxy(67, 8);
        textcolor(14);
        printf("SCORE: 0000");

        mov(mat, pos_fru, pos_ninho, pos_do, fp, continuar, pos_fant);

        //salvar_elementos_cenario(mat, pos_fru, pos_ninho, pos_do, continuar);
    }

    return 0;
}
