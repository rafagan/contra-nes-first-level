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
	Chien2D v2.0 Ator

	Data: 2008

	Hist�rico de altera��es:

	Data       Programador      Altera��o
	========== ================ ======================================================================
	13/05/2010 Fabio Binder     Inserido campo double aux em tagAtor 

**/

#ifndef ATOR_H
#define ATOR_H

// O maior n�mero de quadros permitido em um anima��o
#define ATOR_MAX_QUADROS		32
// O maior n�mero de anima��es diferentes permitidos para um personagem
#define ATOR_MAX_ANIMACOES		64
// O maior n�mero de efeitos sonoros que um personagem pode possuir
#define ATOR_MAX_SONS			40
// O n�mero m�ximo de estados na pilha
#define	ATOR_MAX_PILHA			16
// O n�mero m�ximo de eventos na fila
#define	ATOR_MAX_FILA			32
// O n�mero m�ximo de atores no sistema
#define ATOR_MAX_TIPOS			128
// O n�mero m�ximo de temporizadores
#define ATOR_MAX_TEMPORIZADORES	8
// O n�mero m�ximo de vari�veis autilizares
#define ATOR_MAX_AUX			15

// Enumera��o para estados do ator
enum {ATOR_NASCENDO, ATOR_MORTO, ATOR_ENCERRADO, ATOR_PROG_INI};

// Enumara��o para os sub-estados do estado de um ator
enum {ESTADO_INICIO, ESTADO_RODANDO, ESTADO_PROG_INI};

// Enumera��o para os tipos de eventos que o engine envia
enum {
		EVT_NULO,					// Evento nulo, retornado quando a fila est� vazia								->ok!
		EVT_TEMPO,					// Evento que indica que um temporizador estourou
		// Eventos de foco de tela
		EVT_FOCO_TELA,				// Indica que o ator entrou dentro do foco da cena			(programador do jogo, usa fun��o do engine)
		EVT_PERDE_FOCO_TELA,		// Indica que o ator saiu do foco da cena					(programador do jogo, usa fun��o do engine)
		// Eventos de foco do mapa
		EVT_SAIU_FORA_MAPA_BAIXO,	// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte de baixo		->ok!
		EVT_SAIU_FORA_MAPA_CIMA,	// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte de cima		->ok!
		EVT_SAIU_FORA_MAPA_ESQ,		// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte da esquerda	->ok!
		EVT_SAIU_FORA_MAPA_DIR,		// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte de direita	->ok!
		// Eventos de colis�o com cen�rio (gerados automaticamente)
		EVT_COLIDIU_PAREDE_ESQ,		// Indica que colidiu com uma parede � esquerda									->ok!
		EVT_COLIDIU_PAREDE_DIR,		// Indica que colidiu com uma parede � direita									->ok!
		EVT_COLIDIU_PAREDE_CIMA,	// Indica que colidiu com uma parede acima										->ok!
		EVT_COLIDIU_PAREDE_BAIXO,	// Indica que colidiu com uma parede abaixo										->ok!
		// Eventos de foco do mapa
		EVT_SAIU_FORA_MAPA,			// Indica que o ator saiu COMPLETAMENTE fora do mapa ->ok!
		EVT_ENTROU_MAPA,			// Indica que o ator entrou no mapa
		// Eventos de colis�o com cen�rio (gerados automaticamente)
		// Verificar o subtipo do evento
		EVT_COLIDIU_PAREDE,
		EVT_TOPO_PULO,				// Indoca que chegou ao m�ximo que o personagem pode pular						->ok!
		EVT_COMECOU_CAIR,			// Indica que o personagem come�ou a cair										->ok!
		// Eventos de colis�o com atores
		EVT_COLIDIU_PERSONAGEM,		// Indica que colidiu com um personagem						(programador do jogo)
		EVT_COLIDIU_ARMADILHA,		// Indica que colidiu com um armadilha colocada no cen�rio	(programador do jogo)
		// Eventos do direcional
		EVT_PRESSIONOU_ESQ,			// Indica que o direcional da esquerda foi pressionado		(programador do jogo)
		EVT_PRESSIONOU_DIR,			// Indica que o direcional da direita foi pressionado		(programador do jogo)
		EVT_PRESSIONOU_CIMA,		// Indica que o direcional para cima foi pressionado		(programador do jogo)
		EVT_PRESSIONOU_BAIXO,		// Indica que o direcional para baixo foi pressionado		(programador do jogo)
		EVT_LIBEROU_ESQ,			// Indica que o direcional da esquerda foi liberado			(programador do jogo)
		EVT_LIBEROU_DIR,			// Indica que o direcional da direita foi liberado			(programador do jogo)
		EVT_LIBEROU_CIMA,			// Indica que o direcional para cima foi liberado			(programador do jogo)
		EVT_LIBEROU_BAIXO,			// Indica que o direcional para baixo foi liberado			(programador do jogo)
		// Eventos dos bot�es
		EVT_PRESSIONOU_BOTAO1,		// Indica que pressionou o bot�o 1							(programador do jogo)
		EVT_PRESSIONOU_BOTAO2,		// Indica que pressionou o bot�o 2							(programador do jogo)
		EVT_PRESSIONOU_BOTAO3,		// Indica que pressionou o bot�o 3							(programador do jogo)
		EVT_PRESSIONOU_BOTAO4,		// Indica que pressionou o bot�o 4							(programador do jogo)
		EVT_PRESSIONOU_BOTAO5,		// Indica que pressionou o bot�o 5							(programador do jogo)
		EVT_PRESSIONOU_BOTAO6,		// Indica que pressionou o bot�o 6							(programador do jogo)
		EVT_LIBEROU_BOTAO1,			// Indica que pressionou o bot�o 1							(programador do jogo)
		EVT_LIBEROU_BOTAO2,			// Indica que pressionou o bot�o 2							(programador do jogo)
		EVT_LIBEROU_BOTAO3,			// Indica que pressionou o bot�o 3							(programador do jogo)
		EVT_LIBEROU_BOTAO4,			// Indica que pressionou o bot�o 4							(programador do jogo)
		EVT_LIBEROU_BOTAO5,			// Indica que pressionou o bot�o 5							(programador do jogo)
		EVT_LIBEROU_BOTAO6,			// Indica que pressionou o bot�o 6							(programador do jogo)
		// Evento de pausa do jogo
		EVT_PRESSIONOU_PAUSA,		// Indica que pressionou o bot�o de pausa do jogo			(programador do jogo)
		EVT_LIBEROU_PAUSA	,		// Indica que liberou o bot�o de pausa do jogo			(programador do jogo)
		// Eventos de anima��o
		EVT_RECOMECOU_ANIMACAO,		// Indica que recome�ou a anima��o											->ok!
		// Eventos especiais de fase
		EVT_FIM_FASE,				// Indica que acabou a fase
		EVT_CHECKPOINT,				// Indica que chegou a um checkpoint
		// Eventos especiais
		EVT_CRIA_PERSONAGEM,		// Indica para o jogo que deve criar um personagem
		EVT_PLACAR,					// Indica para o jogo que um aumento no placar do jogador
		EVT_POSICAO,				// Evento para o personagem que indica uma nova posi��o a um personagem
		// In�cio dos eventos do jogo em si
		EVT_JOGO_PROG
};

// Subtipos de eventos
enum
{
		SUBEVT_COLIDIU_PAREDE_ESQ,				// Indica que colidiu com uma parede � esquerda									->ok!
		SUBEVT_COLIDIU_PAREDE_DIR,				// Indica que colidiu com uma parede � direita									->ok!
		SUBEVT_COLIDIU_PAREDE_CIMA,				// Indica que colidiu com uma parede acima										->ok!
		SUBEVT_COLIDIU_PAREDE_BAIXO,			// Indica que colidiu com uma parede abaixo										->ok!
		SUBEVT_COLIDIU_PERSONAGEM_CIMA,			// Indica que colidiu com um personagem						(programador do jogo)
		SUBEVT_COLIDIU_PERSONAGEM_ESQUERDA,		// Indica que colidiu com um personagem						(programador do jogo)
		SUBEVT_COLIDIU_PERSONAGEM_DIREITA,		// Indica que colidiu com um personagem						(programador do jogo)
		SUBEVT_COLIDIU_PERSONAGEM_BAIXO,		// Indica que colidiu com um personagem						(programador do jogo)
		SUBEVT_SAIU_FORA_MAPA_BAIXO,			// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte de baixo		->ok!
		SUBEVT_SAIU_FORA_MAPA_CIMA,				// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte de cima		->ok!
		SUBEVT_SAIU_FORA_MAPA_ESQ,				// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte da esquerda	->ok!
		SUBEVT_SAIU_FORA_MAPA_DIR,				// Indica que o ator saiu COMPLETAMENTE fora do mapa para a parte de direita	->ok!
		SUBEVT_FIM_FASE_VITORIA,				// Indica que o jogador venceu a fase
		SUBEVT_FIM_FASE_DERROTA					// Indicaque o jogador perdeu a fase

};

// Estrutura de dados para uma anima��o do personagem
typedef struct tagAnimacao
{
	// O n�mero total de quadros da anima��o
	unsigned int numQuadros;
	// O tempo entre os quadros
	unsigned int tempo;
	// A sequ�ncia de quadros desta anima��o
	unsigned int quadros[ATOR_MAX_QUADROS];
}Animacao;

// Estrutura que define o estado do personagem
typedef struct tagEstadoAtor
{
	// O c�digo estado atual
	unsigned int estado;
	// O subestado
	unsigned int subestado;
}EstadoAtor;

// A pilha de estados do personagem
typedef struct tagPilhaEstados
{
	int tamanho;
	EstadoAtor pilha[ATOR_MAX_PILHA];
}PilhaEstados;

// A estrutura do evento
typedef struct tagEvento
{
	// O tipo do evento
	unsigned int tipoEvento;
	// O subtipo, pode ser o c�digo de um personagem, por exemplo (opcional)
	unsigned int subtipo;
	// A posi��o aonde o evento ocorreu (opciona)
	int x, y; 
	// Um valor extra, como uma dire��o ou pontua��o (opcional)
	int valor;
}Evento;

// A fila de eventos do personagem
typedef struct tagFilaEventos
{
	int tamanho;
	int inicio;
	Evento fila[ATOR_MAX_FILA];
}FilaEventos;

// Estrutura para representar um ator
typedef struct tagAtor
{
	// O tipo do ator
	unsigned int tipo;
	// coordenadas x,y do ator
	double x, y;
	// N�mero de vidas
	unsigned int vidas;
	// Energia (se for o caso)
	unsigned int energia;
	// A velocidade do personagem
	double velocidade;
	// a direcao na qual o personagem se desloca
	int direcao;
	// direcao para a qual o personagem olha
	int olhandoPara;
	// O c�digo da anima��o (posi��o no vetor)
	unsigned int numAnimacao;
	// informa��o da anima��o atual
	int quadroAtual, tempoQuadro;
	// Os temporizadores do personagem
	unsigned long int temporizadores[ATOR_MAX_TEMPORIZADORES];
	// AS vari�veis auxiliares inteiras
	int aux_int[ATOR_MAX_AUX];
	// As vari�veis auxiliares reais
	double aux_real[ATOR_MAX_AUX];
	// Indica se o personagem est� invulner�vel ou n�o
	bool invulneravel;
	// Indica se o personagem � sujeito � gravidade (default) ou n�o
	bool gravidade;
	// Indica se o personagem est� limitado pelo cen�rio (default) ou n�o
	bool solido;
	// O impulso que o personagem recebe 
	double impulsoVertical;
	// O tempo de uma queda em quadros
	int quadroQueda;
	// Indica se est� em foco na tela ou n�o
	bool naTela;

	// RESERVADO: O estado atual do ator
	EstadoAtor estado;
	// RESERVADO: Fila de eventos do personagem
	FilaEventos fila;
	// RESERVADO: pilha de estados do personagem
	PilhaEstados pilha;

	// Liberado para utiliza��o pelo programador, n�o utilizado pela Chien
	double aux;
}Ator;

// Estrutura de dados para os dados de um ator
typedef struct tagDadosAtor
{
	// As anima��es do personagem
	Animacao animacoes[ATOR_MAX_ANIMACOES];
	// O vetor de efeitos sonoros
	unsigned int sons[ATOR_MAX_SONS];
	// O n�mero de sons guardados
	unsigned int numSons;
	// O spriteset do personagem. Se zero, indica que o ator n�o foi iniciado.
	unsigned int spriteset;
	// Dimens�es do sprite
	unsigned int lsprite, asprite;
	// Ponto de refer�ncia do bounding box do ator
	int xref, yref;
	// Dimens�es do bounding box do ator
	int largura, altura;
	// Vetor de pontos extremos do sprite
	int x[4], y[4];
	// Vetor de diagonais do sprite
	double diag[4];
	// Vetor de �ngulos das diagonais do sprite
	double angulos[4];
	// Indica se o ator deve ter a imagem rotacionada de acordo com olhandopara
	bool rotaciona;
	// A fun��o que atualiza o personagem no cen�rio
	bool (*atualiza)(Ator* a, unsigned int mapa);
}DadosAtor;

// Prot�tipos das fun��es

// Fun��o que inicia o sistema de atores. Retorna verdade se iniciou corretamente, ou
// retorna falso se n�o conseguir.
bool ATOR_Inicia();

// Fun��o que encerra o sistema de atores.
void ATOR_Encerra();

// Fun��o para registrar um ator no sistema. A fun��o recebe os dados do ator e deve 
// indicar qual o tipo (c�digo) �nico. A fun��o retorna falso se n�o conseguir carregar
// os recursos, se o ator j� tiver sido registrado ou se exceder o limite de atores 
// carregados.
// Um dado importante � a fun��o que atualiza o personagem, que deve ser escrita pelo
// programador de acordo com a assinatura proposta.
//
bool ATOR_CarregaAtorEstatico(unsigned int tipo, char *spriteset, int lsprite, 
							  int asprite, int xref, int yref, int largura, int altura,
							  Animacao *animacoes, bool rotaciona,
							  char **sons, unsigned int numSons, 
							  bool (*atualiza)(Ator* a, unsigned int mapa),
							  int qtde_animacoes=ATOR_MAX_ANIMACOES);

// Fun��o para eliminar um ator no sistema. 
bool ATOR_DescarregaAtor(unsigned int tipo);

// Fun��o que cria um novo ator, de um tipo especificado pelo programador.
// O ator � criado na posi��o x,y indicadas. 
// O ator come�a na posi��o indicada, no estado ATOR_NASCENDO, 
// velocidade 0, dire��o 0 e olhando para 0. Nesta situa��o o ator n�o � desenhado e o 
// c�digo do programador deve coloc�-lo em um estado v�lido.
//
// A fun��o retorna o ponteiro do ator criado. Caso a aloca��o falhe, retorna 0.
//
Ator* ATOR_CriaAtor(unsigned int tipo, double x, double y, int direcao);

// Fun��o que desloca um ator na tela/cen�rio e o anima.
void ATOR_AplicaEstado(Ator *a, unsigned int mapa, unsigned int larguratela, 
						unsigned int alturatela);

// Fun��o que envia um evento para o personagem e o coloca na sua fila. A fun��o retorna
// true se conseguiu adicionar o evento e false caso contr�rio.
bool ATOR_EnviaEvento(Ator *a, Evento *evt);

// Fun��o que pega o pr�ximo evento do personagem. Ela recebe o ponteiro do evento que ir�
// receber a c�pia do evento e indica se o mesmo foi recuperado com sucesso atrav�s do valor
// booleano de retorno.
bool ATOR_ProximoEvento(Ator *a, Evento *evt);

// Fun��o que limpa a fila de eventos de um ator
void ATOR_LimpaEvento(Ator *a);

// Fun��o que troca o estado do ator para o novo estado, indicando se o estado atual deve ser
// preservado na pilha (true) ou n�o (false).
void ATOR_TrocaEstado(Ator *a, unsigned int estado, bool pilha);

// Fun��o que troca o estado do ator para o novo estado, indicando se o estado atual deve ser
// preservado na pilha (true) ou n�o (false).
void ATOR_TrocaAnimacao(Ator *a, unsigned int numAnim);

// Fun��o que troca o estado do ator para o topo da pilha. Caso a pilha esteja vazia, o estado
// atual n�o � trocado
void ATOR_DesempilhaEstado(Ator *a);

// Fun��o para desenhar um ator, baseado no mapa e posi��o inicial do desenho na tela
void ATOR_Desenha(Ator *a, unsigned int idMapa, int xtela, int ytela);

// fun��o para centralizar a posi��o do mapa em fun��o de um personagem
void ATOR_CentraMapa(Ator *a, unsigned int idMapa, int lJanela, int aJanela);

// Fun��o para atualizar um ator
bool ATOR_Atualiza(Ator *a, unsigned int mapa);

// Fun��o para testar a colis�o entre dois personagens
bool ATOR_ColidiuAtores(Ator *a, Ator *b);
// Fun��o para testar a colis�o de um personagem com o cen�rio
bool ATOR_ColidiuBlocoCenario(Ator *a, unsigned int idMapa, unsigned int codBloco);

// Fun��o para tocar um efeito sonoro de um ator
int ATOR_TocaEfeito(Ator *a, unsigned int id, int posicao);
// Fun��o para tocar um efeito sonoro de um ator em loop
int ATOR_TocaEfeitoLoop(Ator *a, unsigned int id, int posicao, int loop);

// Fun��o para tocar o efeito sonoro de um ator em fun��o de sua posi��o na tela
int ATOR_TocaEfeitoTela(Ator *a, unsigned int id, unsigned int mapa);

// Fun��o para enviar um evento para o jogo. Retorna verdadeiro se conseguiu enviar o
// evento. Falso caso contr�rio
bool ATOR_EnviaEventoJogo(Evento *evt);

// Fun��o para pegar um evento do jogo. Se o evento foi recuperado, retorna true. Caso
// contr�rio, retorna falso.
bool ATOR_ProximoEventoJogo(Evento *evt);

// Use para limpar a fila de eventos do jogo
void ATOR_LimpaEventoJogo();

// Fun��o para dar um impulso de mandeira correta ao jogador
void ATOR_Impulsiona(Ator *a, double impulso);

#endif
