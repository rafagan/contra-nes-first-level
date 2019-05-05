/*
Copyright 2008-2010, Paulo Vinicius Wolski Radtke (pvwradtke@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/**

	Data		Programador	Altera��o
	=====================================================
	13/08/2010	Paulo VW Radtke	Fun��es para mudar uma camada do mapa e para pegar a camada de marcas.


**/


#ifndef C2D2MAPA_H
#define C2D2MAPA_H

// Defines espec�ficos para mapas do Mappy

// O n�mero m�ximo de camadas do mapa (0 � mais inferior, 8 � a mais em cima)
#define C2D2M_MAX_CAMADAS	8
// O n�mero m�ximo de azulejos animados (estamos sendo conservadores)
#define C2D2M_MAX_ANISTR	256
// O n�mero m�ximo de sequ�ncias de anima��o (conservador)
#define C2D2M_MAX_ANIMSEQ	2048
// O n�mero m�ximo de mapas utilizados
#define C2D2M_MAX_MAPAS		32
#define C2D2M_TAM_PATH		2048	// Tamanho do path de um arquivo completo em bytes


// Constantes para blocos marcadores no mapa
enum {C2D2M_INICIO, C2D2M_FIM, C2D2M_CHECK, 
// Os tipos de blocos de parede
C2D2M_SOLIDO, 
// blocos com inclina��o de 45� e 22,5�
C2D2M_SOLIDO45, C2D2M_SOLIDO22A, C2D2M_SOLIDO22B,
// blocos com inclina��o de 135� e 157,5� 
C2D2M_SOLIDO135, C2D2M_SOLIDO157A, C2D2M_SOLIDO157B,
// blocos com inclina��o de 225� e 202,5�
C2D2M_SOLIDO225, C2D2M_SOLIDO202A, C2D2M_SOLIDO202B,
// blocos com inclina��o de 315� e 337,5�
C2D2M_SOLIDO315, C2D2M_SOLIDO337A, C2D2M_SOLIDO337B,

// Os tipos de escada
C2D2M_ESCADAV, C2D2M_ESCADA_SD, C2D2M_ESCADA_SE, 
C2D2M_MORTE,  C2D2M_PROG};

// These structs and defines are from the original MAPPY source files
// � Robin Burrows <rburrows@bigfoot.com>
// http://www.tilemap.co.uk

// DEfines dos tipos de anima��o
#define AN_END -1
#define AN_NONE 0
#define AN_LOOPF 1
#define AN_LOOPR 2
#define AN_ONCE 3
#define AN_ONCEH 4
#define AN_PPFF 5
#define AN_PPRR 6
#define AN_PPRF 7
#define AN_PPFR 8
#define AN_ONCES 9


// Defines para os tipos de header
enum {CABECALHO_IGNORADO, CABECALHO_FORM, CABECALHO_FMAP, CABECALHO_MPHD, 
        CABECALHO_ANDT, CABECALHO_BODY, CABECALHO_LYR };

// Tipos para o mapa

// Header do arquivo do MAPPY
typedef struct {	/* Map header structure */
/* char M,P,H,D;	4 byte chunk identification. */
/* long int mphdsize;	size of map header. */
char mapverhigh;	/* map version number to left of . (ie X.0). */
char mapverlow;		/* map version number to right of . (ie 0.X). */
char lsb;		/* if 1, data stored LSB first, otherwise MSB first. */
char maptype;	/* 0 for 32 offset still, -16 offset anim shorts in BODY added FMP0.5*/
short int mapwidth;	/* width in blocks. */
short int mapheight;	/* height in blocks. */
short int reserved1;
short int reserved2;
short int blockwidth;	/* width of a block (tile) in pixels. */
short int blockheight;	/* height of a block (tile) in pixels. */
short int blockdepth;	/* depth of a block (tile) in planes (ie. 256 colours is 8) */
short int blockstrsize;	/* size of a block data structure */
short int numblockstr;	/* Number of block structures in BKDT */
short int numblockgfx;	/* Number of 'blocks' in graphics (BODY) */
unsigned char ckey8bit, ckeyred, ckeygreen, ckeyblue; /* colour key values added FMP0.4*/
/* info for non rectangular block maps added FMP0.5*/
short int blockgapx, blockgapy, blockstaggerx, blockstaggery;
short int clickmask, pillars;
} MPHD;

// Estrutura da anima��o
typedef struct { /* Animation control structure */
signed char antype;	/* Type of anim, AN_? */
signed char andelay;	/* Frames to go before next frame */
signed char ancount;	/* Counter, decs each frame, till 0, then resets to andelay */
signed char anuser;	/* User info */
//long 
int ancuroff;	/* Points to current offset in list */
//long 
int anstartoff;	/* Points to start of blkstr offsets list, AFTER ref. blkstr offset */
//long 
int anendoff;	/* Points to end of blkstr offsets list */
} ANISTR;

typedef struct
{
    char tipo[4];
    int tamanho;
}CabecalhoBloco;

typedef struct
{
    char tipo[4];
    int tamanho;
    char subtipo[4];
}CabecalhoArquivo;

typedef struct C2D2M_Mapa
{
	// Indica se o mapa foi inicializado ou n�o
	bool inicializado;
    // O apelido da fonte. �nico no sistema
    char apelido[C2D2M_TAM_PATH];
    // Quantos elementos usam esse mapa. Elimina quando chega a zero
    int usuarios;
    // A largura e altura do mapa em BLOCOS
    int largura, altura;
	// A largura e altura do mapa em pixels (�til pra evitar multiplica��es excessivas)
	int larguraPixel, alturaPixel;
    // O tamanho do spriteset usado (deve bater com o arquivo utilizado)
    int dimensaoBlocoH, dimensaoBlocoV;
    // AS camadas do mapa. 0 � a camada de base e 7 � a mais em cima
	short int *camadas[C2D2M_MAX_CAMADAS];
	// A velocidade de cada camada. Se for zero, a camada est� desabilitada
	int vCamadas[C2D2M_MAX_CAMADAS];
    //   As estruturas de anima��o, s�o referenciadas como n�meros negativos
    // nas camadas
    ANISTR estrutAnimacao[C2D2M_MAX_ANISTR];
    //   As sequ�ncias de anima��o, referenciadas na estrutAnimacao
    //long 
	int seqAnimacao[C2D2M_MAX_ANIMSEQ];
	// O spriteset utilizado pelo mapa
	unsigned int idSpriteset;
	// As coordenadas x,y do ponto de refer�ncia do mapa
	int x,y;
	// A camada que serve como marcador de cen�rio
	int camadaMarcas;
	// O �ndice do bloco inicial das marcas
	int offbloco;
	
	// Campos para a busca do pr�ximo bloco
	
	// Indica que est� buscando o proximo
	bool buscaProximo;
	// Indica qual o bloco que est� buscando
	int blocoProcurado;
	// Indica em qual posi��o achou o �ltimo bloco
	int posicaoUltimo;

    // A gravidade do jogo. Default � zero para que o personagem n�o caia em mapas top view
    double gravidade;
    // O limite da velocidade de queda em pixels por segundo (simula atrito)
    double maxgravidade;

}C2D2M_Mapa;


// Fun��es para manipular os mapas

// Fun��o que inicia a Chien2D 2 Mapa
bool C2D2M_Inicia();
// Fun��o que encerra a Chien2D 2 Mapa
void C2D2M_Encerra();

//Fun��o que carrega uma mapa no formato Mappy
unsigned int C2D2M_CarregaMapaMappy(const char *arquivo_mapa, const char *arquivo_spriteset);
// Remove os dados alocados do mapa e marca como n�o inicializado
void C2D2M_RemoveMapa(unsigned int id);
// Fun��o para procurar um mapa pelo nome na mem�ria
unsigned int C2D2M_ProcuraMapa(const char *apelido);
// Fun��o para ver as dimens�es do mapa (retorna true se o mapa � v�lido, false caso contr�rio)
bool C2D2M_DimensoesMapa(unsigned int idMapa, int *largura, int *altura);
// Posiciona o mapa nas coordenadas x,y indicadas
void C2D2M_PosicionaMapa(unsigned int idMapa, int x, int y);
// Recupera a posi��o do mapa
bool C2D2M_PosicaoXY(unsigned int idMapa, int *x, int *y);
// Fun��o para dar velocidade a uma camada do mapa
void C2D2M_VelocidadeCamadaMapa(unsigned int idMapa, int camada, int velocidade);
// Fun��o para indicar a camada de marcas do mapa e o n�mero do bloco inicial de marcas no cen�rio
bool C2D2M_CamadaMarcas(unsigned int idMapa, int camada, int offbloco);
// Anima os blocos animados do mapa. Atualiza os quadros, reseta e etc e tal
void C2D2M_AnimaMapa(unsigned int id);
// Ajusta o deslocamento de um boundingbox no mapa. Recebe o mapa aonde o deslocamento
// se realiza, a posi��o do bounding box e o quanto deve mover no eixo x e y. O booleano
// gravidade indica se h� gravidade, situa��o na qual deslocamentos verticais para baixo
// e cima n�o implicam em deslocamento no eixo x. 
//
//  Os par�metros s�o o identificador do mapa (idMapa), as coordenadas do bounding box
// (x,y), as dimens�es do bounding box (largura), os ponteiros para o deslocamento 
// (dx e dy) e se a gravidade est� em a��o.
void C2D2M_AjustaDeslocamento(unsigned int idMapa, int xorig, int yorig, 
							  int larg, int alt, int *dx, int *dy, bool gravidade);

// Testa se um bounding box colide com um bloco qualquer de cen�rio.
bool C2D2M_ColidiuBlocoCenario(unsigned int idMapa, int x, int y, int larg, int alt, int bloco);

// Para um mapa, calcula a velocidade de queda de um corpo de acordo com o tempo total da queda
// expresso em quadros (60 quadros por segundo)
bool C2D2M_CalculaQueda(unsigned int idMapa, int quadrosQueda, double *vqueda);

// Fun��o para desenhar uma camada do mapa na tela a partir das coordenadas do mapa (x,y na estrutura), dentro da janela especificada
// por xtela,ytela e as dimens�es largura e altura.
void C2D2M_DesenhaCamadaMapa(unsigned int idMapa, int camada, int xtela, int ytela, int largura, int altura);        
// fun��o para buscar as coordenadas da primeira ocorr�ncia de um bloco de marca dentro de um mapa
bool C2D2M_PrimeiroBlocoMarca(unsigned int idMapa, int bloco, int *x, int *y);
// fun��o para buscar as coordenadas da pr�xima ocorr�ncia de um bloco de marca dentro de um mapa. Usar apenas AP�S C2D2M_PrimeiroBlocoMarca.
bool C2D2M_ProximoBlocoMarca(unsigned int idMapa, int *x, int *y);

// Fun��o que indica a acelera��o de gravidade do mapa, em pixels por segundo, e a velocidade m�xima de gravidade para simular atrito
bool C2D2M_GravidadeMapa(unsigned int idMapa, double gravidade, double maxgravidade);

// M�todo para verificar se o cabe�alho do FMP/FMA � correto
bool C2D2M_VerificaCabecalhoArquivo(CabecalhoArquivo *cabecalho);
// M�todo para verificar o tipo do cabe�alho. Retorna um dos tipos enumerados
int C2D2M_TipoBloco(CabecalhoBloco *cabecalho);

// Fun��o que recupera a camazda de marcas
unsigned int C2D2M_PegaCamadaMarcas(unsigned int idMapa);
// Fun��o para mudar o valor de um bloco em uma camada espec�fica
void C2D2M_AlteraBloco(unsigned int idMapa, int camada, int x, int y, unsigned int valor);


#endif
