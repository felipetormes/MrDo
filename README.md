# MrDo
Game desenvolvido em C.

## Comandos
Para comandar os movimentos do MrDo na tecla são usadas as teclas: W(Cima), S(Baixo), A(Esquerda), D(Direita);
Para pausar o jogo deve ser pressionada a tecla ESPAÇO;
Os outros comandos são informados ao longo do game.

## Estruturas
typedef struct (Implementada)
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

## Funções:
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
