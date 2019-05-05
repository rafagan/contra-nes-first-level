/*

Chien2D v2.0 OpenGL Render

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
	Data: 12/06/2008

	Hist�rico de altera��es:

	Data       Programador      Altera��o
	========== ================ ======================================================================
	23/01/2010 Fabio Binder     Corrigido bug no qual a colis�o com paredes n�o funcionava para mapa 
	                            sem gravidade e personagem andando para cima.
	29/06/2010 Fabio Binder		inserido o par�metro qtde_animacoes em ATOR_CarregaAtorEstatico

**/

#include <stdlib.h>
#include <math.h>

#include <c2d2/chien2d2.h>
#include <c2d2/chien2d2primitivas.h>
#include <c2d2/chien2d2mapa.h>
#include <c2d2/chienaudio2.h>
#include <c2d2/ator.h>

#define ATOR_ANG_RAD	0.017453292519943295769236907684886
#define ATOR_RAD_ANG	57.295779513082320876798154814105

// Prot�tipo de fun��es exclusivas deste m�dulo

bool FILA_InsereFilaEvento(FilaEventos *f, Evento *evt);
bool FILA_RemoveFilaEvento(FilaEventos *f, Evento *evt);
void FILA_LimpaFilaEvento(FilaEventos *f);
// Fun��o que calcula se um bounding box (ator) est� dentro de uma �rea (tela ou mapa)
bool ATOR_DentroArea(Ator *a, int xref, int yref, int largura, int altura);

// O vetor de dados dos atores
DadosAtor vetorAtores[ATOR_MAX_TIPOS];
// Indica se j� iniciou o sistema ou n�o
bool ATOR_iniciado = false;
// A fila de eventos do jogo
FilaEventos filaJogo;

bool ATOR_Inicia()
{
	if(ATOR_iniciado)
		return false;
	// Zera os dados dos atores
	for(int i=0;i>ATOR_MAX_TIPOS;i++)
	{
		// Zera o spriteset
		vetorAtores[i].spriteset=0;
		// zera o n�mero de sons
		vetorAtores[i].numSons=0;		
	}
	// Indica que iniciou
	ATOR_iniciado = true;
	return true;
}

void ATOR_Encerra()
{
	// Se o ator est� em mem�ria, apaga os seus dados
	for(int i=0;i<ATOR_MAX_TIPOS;i++)
		if(vetorAtores[i].spriteset)
			ATOR_DescarregaAtor(i);
	ATOR_iniciado = false;
}

// Fun��o para registrar um ator no sistema. A fun��o recebe os dados do ator e deve 
// indicar qual o tipo (c�digo) �nico. A fun��o retorna falso se n�o conseguir carregar
// os recursos, se o ator j� tiver sido registrado ou se exceder o limite de atores 
// carregados.
bool ATOR_CarregaAtorEstatico(unsigned int tipo, char *spriteset, int lsprite, 
							  int asprite, int xref, int yref, int largura, int altura,
							  Animacao *animacoes, bool rotaciona,
							  char **sons, unsigned int numSons, 
							  bool (*atualiza)(Ator* a, unsigned int mapa),
							  int qtde_animacoes)
{
	// Testa se o ator n�o existe
	if(vetorAtores[tipo].spriteset)
	{
		printf("Ator %i ja registrado no sistema. \n", tipo);
		return false;
	}
	// Carrega o spriteset
	vetorAtores[tipo].spriteset=C2D2_CarregaSpriteSet(spriteset, lsprite, asprite);
	if(!vetorAtores[tipo].spriteset)
	{
		printf("Erro ao carregar o ator %i. Nao conseguiu carregar o spriteset %s, de dimensoes %ix%i.\n", tipo, spriteset, lsprite, asprite);
		return false;
	}
	// Carrega os sons do ator
	for(unsigned int i=0;i<numSons;i++)
	{
		vetorAtores[tipo].sons[i] = CA2_CarregaEfeito(sons[i]);
		// Falhou ao carregar o som? N�o � um erro cr�tico , mas pode ocorrer
		if(!vetorAtores[tipo].sons[i])
			printf("Erro ao carregar o ator %i. Nao conseguiu carregar o efeito sonoro %s.\n", tipo, sons[i]);
		/*	// Apaga o spriteset
			C2D2_RemoveSpriteSet(vetorAtores[tipo].spriteset);
			vetorAtores[tipo].spriteset=0;
			// apaga os sons que j� foram carregados
			for(int x=0;x<i;x++)
				CA2_RemoveEfeito(vetorAtores[tipo].sons[x]);
			return false;
		}*/
	}
	// Se carregou, guarda quantos sons foram armazenados
	vetorAtores[tipo].numSons=numSons;
	// Enfim, armazena as anima��es
	memcpy(vetorAtores[tipo].animacoes, animacoes, sizeof(Animacao)*qtde_animacoes);
	// O tamanho do sprite
	vetorAtores[tipo].lsprite = lsprite;
	vetorAtores[tipo].asprite = asprite;
	// O bounding box do ator
	vetorAtores[tipo].xref = xref;
	vetorAtores[tipo].yref = yref;
	vetorAtores[tipo].altura = altura;
	vetorAtores[tipo].largura = largura;
	// Cria os pontos extremos (usados mais tarde), sentido hor�rio a partir do ponto direito superior
	vetorAtores[tipo].x[0]=-largura/2-xref;
	vetorAtores[tipo].y[0]=-altura/2-yref;
	vetorAtores[tipo].x[1]=(lsprite-xref-largura)+largura/2;
	vetorAtores[tipo].y[1]=-altura/2-yref;
	vetorAtores[tipo].x[2]=(lsprite-xref-largura)+largura/2;
	vetorAtores[tipo].y[2]=(asprite-yref-altura)+altura/2;
	vetorAtores[tipo].x[3]=-largura/2-xref;
	vetorAtores[tipo].y[3]=(asprite-yref-altura)+altura/2;
	// Calcula as diagonais
	vetorAtores[tipo].diag[0] = sqrt(pow((double)vetorAtores[tipo].x[0],2)+pow((double)vetorAtores[tipo].y[0],2));
	vetorAtores[tipo].diag[1] = sqrt(pow((double)vetorAtores[tipo].x[1],2)+pow((double)vetorAtores[tipo].y[1],2));
	vetorAtores[tipo].diag[2] = sqrt(pow((double)vetorAtores[tipo].x[2],2)+pow((double)vetorAtores[tipo].y[2],2));
	vetorAtores[tipo].diag[3] = sqrt(pow((double)vetorAtores[tipo].x[3],2)+pow((double)vetorAtores[tipo].y[3],2));
	// Calcula os �ngulos
	vetorAtores[tipo].angulos[0] = 180.0 - asin(-vetorAtores[tipo].y[0]/vetorAtores[tipo].diag[0])*ATOR_RAD_ANG;
	vetorAtores[tipo].angulos[1] = asin(-vetorAtores[tipo].y[1]/vetorAtores[tipo].diag[1])*ATOR_RAD_ANG;
	vetorAtores[tipo].angulos[2] = 360.0 - asin(vetorAtores[tipo].y[2]/vetorAtores[tipo].diag[2])*ATOR_RAD_ANG;
	vetorAtores[tipo].angulos[3] = 180.0 + asin(vetorAtores[tipo].y[3]/vetorAtores[tipo].diag[3])*ATOR_RAD_ANG;
	// Rotaciona ou n�o
	vetorAtores[tipo].rotaciona = rotaciona;
	// A fun��o que atualiza
	vetorAtores[tipo].atualiza=atualiza;
	// Indica que est� tudo ok
	return true;
}

// Fun��o para eliminar um ator no sistema. 
bool ATOR_DescarregaAtor(unsigned int tipo)
{
	// Apaga o spriteset
	if(vetorAtores[tipo].spriteset)
		C2D2_RemoveSpriteSet(vetorAtores[tipo].spriteset);
	vetorAtores[tipo].spriteset=0;
	// Apaga os sons da mem�ria
	for(unsigned int i=0;i<vetorAtores[tipo].numSons;i++)
		if(vetorAtores[tipo].sons[i])
			CA2_RemoveEfeito(vetorAtores[tipo].sons[i]);
	vetorAtores[tipo].numSons=0;
	return true;
}

// Fun��o que cria um novo ator, de um tipo especificado pelo programador.
// O ator � criado na posi��o x,y indicadas, com um bounding box descrito por xref,yref 
// e largura,altura. O ator come�a na posi��o indicada, no estado ATOR_NASCENDO, 
// velocidade 0, dire��o 0 e olhando para 0. Nesta situa��o o ator n�o � desenhado e o 
// c�digo do programador deve coloc�-lo em um estado v�lido.
//
// Em especial, no estado ATOR_NASCENDO, o c�digo do usu�rio deve preencher as informa��es
// da estutura de dados do ator.
//
// A fun��o retorna o ponteiro do ator criado. Caso a aloca��o falhe, retorna 0.
//
Ator* ATOR_CriaAtor(unsigned int tipo, double x, double y, int direcao)
{
	// O tipo do ator � v�lido?
	if(!vetorAtores[tipo].spriteset)
		return 0;
	// O ator que ser� retornado;
	Ator *ator = (Ator*)malloc(sizeof(Ator));
	if(ator==0)
		return 0;
	// Preenche os dados b�sicos
	ator->tipo=tipo;
	ator->x=x;
	ator->y=y;
	ator->vidas=0;
	ator->energia=0;
	ator->direcao=direcao;
	ator->numAnimacao=0;
	ator->quadroAtual=0;
	ator->tempoQuadro=0;
	ator->olhandoPara=0;
	ator->velocidade=0;
	ator->invulneravel=false;
	ator->gravidade=true;
	ator->solido=true;
	ator->naTela=false;
	ator->impulsoVertical=0;
	ator->quadroQueda=0;
	for(int i=0;i<ATOR_MAX_TEMPORIZADORES;i++)
		ator->temporizadores[i]=0;
	// Limpa a fila de eventos
	FILA_LimpaFilaEvento(&ator->fila);
	// Limpa a pilha de estados
	// Prepara o estado inicial
	ator->estado.estado=ATOR_NASCENDO;
	// Inicializa vari�vel de usu�rio
	ator->aux = 0.0;
	// Retorna o ator recem-criado
	return ator;

}

// Fun��o que desloca um ator na tela/cen�rio e o anima.
void ATOR_AplicaEstado(Ator *a, unsigned int mapa, unsigned int larguratela, 
						unsigned int alturatela)
{
	//Testa se � v�lido
	if(a->estado.estado == ATOR_NASCENDO || vetorAtores[a->tipo].spriteset == 0)
		return;
	// Cria um evento tempor�rio
	Evento ev;
	// Verifica os temporizadores
	for(int i=0;i<ATOR_MAX_TEMPORIZADORES;i++)
		if(a->temporizadores[i]>0)
			if(!(--a->temporizadores[i]))
			{
				ev.tipoEvento = EVT_TEMPO;
				ev.subtipo = i;
				ATOR_EnviaEvento(a, &ev);
			}
	// Verifica se est� na tela ou fora dela
	bool naTela=true;
	// Verifica se est� no mapa ou fora dele
	bool noMapa=true;
	// qual a coordenada do mapa (se ela existir)
	int xmapa=0, ymapa=0;
	// qual a dimens�o do mapa
	int lmapa=0, amapa=0;
	// O ajuste da gravidade
	double dyGravidade=0;
	// Indica se aplica ou n�o a gravidade
	bool aplicaGravidade = false;
	if(mapa!=0)
	{
		C2D2M_PosicaoXY(mapa, &xmapa, &ymapa);
		C2D2M_DimensoesMapa(mapa, &lmapa, &amapa);
		noMapa = ATOR_DentroArea(a, 0, 0, lmapa, amapa);
		// Calcula a dist�ncia percorrida com a gravidade
		if(a->gravidade)
		{
			aplicaGravidade = C2D2M_CalculaQueda(mapa, a->quadroQueda, &dyGravidade);
			// Atualiza o quadro de queda
			a->quadroQueda++;
		}
	}

	// Caso ocorra UMA das situa��es, est� fora da tela no momento
	//naTela = ATOR_DentroArea(a, xmapa, ymapa, larguratela, alturatela);
	// Desloca o ator no cen�rio se a velocidade for maior que 0
	if(a->velocidade>0 || aplicaGravidade || a->impulsoVertical!=0)
	{
		// Calcula o deslocamento do personagem baseado na velocidade e dire��o
		double dx = a->velocidade*cos(a->direcao * ATOR_ANG_RAD);
		double dy = -a->velocidade*sin(a->direcao * ATOR_ANG_RAD);
		if(dx < 0.01 && dx > -0.01)
			dx=0;
		if(dy < 0.01 && dy > -0.01)
			dy=0;
		// Calcula o impulso do personagem
		double dyImpulso = 0;
		if(a->impulsoVertical!=0)
			dyImpulso=-(a->impulsoVertical*1.0/60.0);
		// Ajusta o impulso com a gravidade se a gravidade for maior
		if(dyGravidade+dyImpulso>=0 && a->impulsoVertical!=0)
		{
			dyGravidade=0;
			dyImpulso=0;
			a->impulsoVertical=0;
			a->quadroQueda=0;
			ev.tipoEvento=EVT_TOPO_PULO;
			ATOR_EnviaEvento(a, &ev);
		}
		// ajusta o dy com a gravidade e o impulso
		dy+=dyGravidade+dyImpulso;
		// Se o mapa existir, verifica se colide nele antes
		if(mapa !=0 && a->solido)
		{
			// Calcula o deslocamento inteiro para verificar colis�es com paredes
			int dxi=0;
			if(dx>0)
				dxi = (int)ceil(dx);
			else if(dx <0)
				dxi = -(int)ceil(abs(dx));
			int dyi=0;
			if(dy>0)
				dyi = (int)ceil(dy);
			else if(dy < 0)
				dyi = -(int)ceil(abs(dy));
			int olddxi=dxi, olddyi=dyi;
			//if(dx<0)
			//	dxi*=-1;
			
			//printf("A: dxi: %i, dx: %lf\n", dxi, dx);
			//if(dy<0)
			//	dyi*=-1;

			C2D2M_AjustaDeslocamento(mapa, (int)ceil(a->x-(vetorAtores[a->tipo].largura/2)), (int)ceil(a->y-vetorAtores[a->tipo].altura/2), 
				vetorAtores[a->tipo].largura, vetorAtores[a->tipo].altura, &dxi, &dyi, aplicaGravidade);
			//printf("B: dxi: %i, dxi: %lf\n", dxi, dx);
			// Cada vez que colide com uma parede abaixo, deve zerar o tempo de queda
			// Ajusta se necess�rio (se colidiu na horizontal)
			if(dxi != olddxi)
			{
				// Envia o evento se colidiu � direita
				if(dx > 0)
				{
					ev.tipoEvento = EVT_COLIDIU_PAREDE;
					ev.subtipo = SUBEVT_COLIDIU_PAREDE_DIR;
					ATOR_EnviaEvento(a, &ev);
				}
				else if(dx < 0)
				{
					ev.tipoEvento = EVT_COLIDIU_PAREDE;
					ev.subtipo = SUBEVT_COLIDIU_PAREDE_ESQ;
					ATOR_EnviaEvento(a, &ev);
				}
				// Se era igual a zero o dx, ainda assim colide ao ajustar na rampa.
				else 
				{
					if(dxi>0)
					{
						// Aqui bateu numa parede inclinada � esquerda
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_ESQ;
					}
					else
					{
						// foi uma parede inclinada � direita
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_DIR;
					}
					ATOR_EnviaEvento(a, &ev);
					// Se estava indo para baixo, indica que bateu numa parede para baixo
					if(dy>0)
					{
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_BAIXO;
						a->quadroQueda=0;
					}
					else
					{
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_CIMA;
						a->quadroQueda=0;
						a->impulsoVertical=0;
					}
					ATOR_EnviaEvento(a, &ev);
				}
				dx = dxi;
			}
			// Ajusta se necess�rio se colidiu na vertical
			if(dyi != olddyi)
			{
				// Envia o evento se colidiu abaixo
				if(dy > 0)
				{
					ev.tipoEvento = EVT_COLIDIU_PAREDE;
					ev.subtipo = SUBEVT_COLIDIU_PAREDE_BAIXO;
					ATOR_EnviaEvento(a, &ev);
					a->quadroQueda=0;
				}
				else if(dy < 0)
				{
					ev.tipoEvento = EVT_COLIDIU_PAREDE;
					ev.subtipo = SUBEVT_COLIDIU_PAREDE_CIMA;
					ATOR_EnviaEvento(a, &ev);
					a->quadroQueda=0;
					a->impulsoVertical=0;
				}
				// Se era igual a zero o dx, ainda assim colide ao ajustar na rampa.
				else 
				{
					if(dyi>0)
					{
						// Aqui bateu numa parede inclinada acima (empurrou para baixo)
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_CIMA;
						a->quadroQueda=0;
						a->impulsoVertical=0;
					}
					else
					{
						// foi uma parede inclinada em baixo (joga para cima)
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_BAIXO;
						a->quadroQueda=0;
					}
					ATOR_EnviaEvento(a, &ev);
					// Se estava indo para a direita, indica que foi bateu numa parede para a direita
					if(dx>0)
					{
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_DIR;
					}
					else
					{
						ev.tipoEvento = EVT_COLIDIU_PAREDE;
						ev.subtipo = SUBEVT_COLIDIU_PAREDE_ESQ;
					}
					ATOR_EnviaEvento(a, &ev);
				}
				dy = dyi;
			}
		}
		// Desloca
		a->x+=dx;
		a->y+=dy;
		if(a->quadroQueda==2 && a->gravidade && a->impulsoVertical==0)
		{
			ev.tipoEvento = EVT_COMECOU_CAIR;
			ATOR_EnviaEvento(a, &ev);
		}
	}

	// Verifica se saiu ou entrou na tela no eixo. Testa a posi��o atual para ver se es� fora da tela
	// e compara com o caso anterior (vari�vel naTela)

	if(ATOR_DentroArea(a, xmapa, ymapa, larguratela, alturatela))
	{
		// Se cai na confi��o verdadeira, � porqu� ap�s movimentar ele est� fora da tela.
		// Se estava na tela antes, manda o evento de sa�da
		if(!a->naTela)
		{
			ev.tipoEvento=EVT_FOCO_TELA;
			ATOR_EnviaEvento(a, &ev);
			a->naTela=true;
		}
	}
	else
	{
		// Se cai na condi��o falsa, � porqu� o personagem est� na tela. 
		// Se antes disso estava fora, manda o evento
		if(a->naTela)
		{
			ev.tipoEvento=EVT_PERDE_FOCO_TELA;
			ATOR_EnviaEvento(a, &ev);
			a->naTela=false;
		}
	}
	// Determina se saiu do mapa
	if(mapa != 0)
	{
		// Est� dentro ou fora do mapa?
		if(ATOR_DentroArea(a, 0, 0, lmapa, amapa))
		{
			// Se n�o estava antes, entrou no mapa
			if(!noMapa)
			{
				ev.tipoEvento = EVT_ENTROU_MAPA;
				ATOR_EnviaEvento(a, &ev);
			}
		}
		// Se estava antes no mapa, determina em qual dire��o ele saiu (podem ser m�ltiplas)
		else if(noMapa)
		{
			ev.tipoEvento = EVT_SAIU_FORA_MAPA;
			if(a->x<0)
			{
				ev.subtipo = SUBEVT_SAIU_FORA_MAPA_ESQ;
				ATOR_EnviaEvento(a, &ev);
			}
			else if(a->x>=lmapa)
			{
				ev.subtipo = SUBEVT_SAIU_FORA_MAPA_DIR;
				ATOR_EnviaEvento(a, &ev);
			}
			// Envia o evento
			
			if(a->y<0)
			{
				ev.subtipo = SUBEVT_SAIU_FORA_MAPA_CIMA;
				ATOR_EnviaEvento(a, &ev);
			}
			else if(a->y>=amapa)
			{
				ev.subtipo = SUBEVT_SAIU_FORA_MAPA_BAIXO;
				ATOR_EnviaEvento(a, &ev);
			}
		}
	}
	// Atualiza a anima��o. Se chegar no limite, troca o quadro
	if(++a->tempoQuadro >= (int)vetorAtores[a->tipo].animacoes[a->numAnimacao].tempo)
	{
		// Zera o tempo do quadro
		a->tempoQuadro = 0;
		// Muda para o pr�ximo quadro
		a->quadroAtual = (a->quadroAtual+1)%vetorAtores[a->tipo].animacoes[a->numAnimacao].numQuadros;
		// Se reiniciou a anima��o, envia um evento
		if(a->quadroAtual == 0 && vetorAtores[a->tipo].animacoes[a->numAnimacao].numQuadros > 1)
		{
			ev.tipoEvento = EVT_RECOMECOU_ANIMACAO;
			ev.subtipo = a->numAnimacao;
			ATOR_EnviaEvento(a, &ev);
		}
	}
}

// Fun��o que envia um evento para o personagem e o coloca na sua fila. A fun��o retorna
// true se conseguiu adicionar o evento e false caso contr�rio.
bool ATOR_EnviaEvento(Ator *a, Evento *evt)
{
	return FILA_InsereFilaEvento(&a->fila, evt);
}

// Fun��o que pega o pr�ximo evento do personagem. Ela recebe o ponteiro do evento que ir�
// receber a c�pia do evento e indica se o mesmo foi recuperado com sucesso atrav�s do valor
// booleano de retorno.
bool ATOR_ProximoEvento(Ator *a, Evento *evt)
{
	return FILA_RemoveFilaEvento(&a->fila, evt);
}

// Fun��o que limpa a fila de eventos de um ator
void ATOR_LimpaEvento(Ator *a)
{
	FILA_LimpaFilaEvento(&a->fila);
}

// Fun��o que troca o estado do ator para o novo estado, indicando se o estado atual deve ser
// preservado na pilha (true) ou n�o (false).
void ATOR_TrocaEstado(Ator *a, unsigned int estado, bool pilha)
{
	// Testa se deve empilhar o estado atual antes de mudar

	// Troca de estado e prepara o in�cio
	a->estado.estado=estado;
	a->estado.subestado=ESTADO_INICIO;
}

// Fun��o que troca o estado do ator para o novo estado, indicando se o estado atual deve ser
// preservado na pilha (true) ou n�o (false).
void ATOR_TrocaAnimacao(Ator *a, unsigned int numAnim)
{
	// Indica a nova animacao
	a->numAnimacao=numAnim;
	// Zera os cotnadores e marcadores
	a->quadroAtual=0;
	a->tempoQuadro=0;
}


// Fun��o que troca o estado do ator para o topo da pilha. Caso a pilha esteja vazia, o estado
// atual n�o � trocado
void ATOR_DesempilhaEstado(Ator *a)
{
}

// Fun��o para desenhar um ator, baseado no mapa e posi��o inicial do desenho na tela
void ATOR_Desenha(Ator *a, unsigned int idMapa, int xtela, int ytela)
{
	// Os pontos para um sprite rotacionado
	static int xrot[4], yrot[4];
	//Verifica se � v�lido
	if(!vetorAtores[a->tipo].spriteset || a->estado.estado==ATOR_NASCENDO || a->estado.estado==ATOR_MORTO || a->estado.estado==ATOR_ENCERRADO)
		return;
	// Aonde vai desenhar o ator na tela
	int x = (int)(a->x+xtela);
	int y = (int)(a->y+ytela);
	// Testa se tem um mapa v�lido para ajustar as coordenadas do mapa na tela
	if(idMapa != 0)
	{
		// recupera a coordenada inicial do mapa
		int xmapa, ymapa;
		C2D2M_PosicaoXY(idMapa, &xmapa, &ymapa);
		x-=xmapa;
		y-=ymapa;
	}
	// Se n�o rotaciona o ator, desenha direto
	if(!vetorAtores[a->tipo].rotaciona)
		C2D2_DesenhaSprite(vetorAtores[a->tipo].spriteset, 
			vetorAtores[a->tipo].animacoes[a->numAnimacao].quadros[a->quadroAtual],
			// As coordenadas s�o somadas ao ponto de refer�ncia no vetor de atores
			x+vetorAtores[a->tipo].x[0],
			y+vetorAtores[a->tipo].y[0]);
	else
	{
		// Aqui desenha o sprite rotacionado
		// Calcula os pontos extremos originais em rela��o ao centro de gravidade do sprite
		xrot[0]=(int)(x+vetorAtores[a->tipo].diag[0]*cos(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[0])));
		xrot[1]=(int)(x+vetorAtores[a->tipo].diag[1]*cos(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[1])));
		xrot[2]=(int)(x+vetorAtores[a->tipo].diag[2]*cos(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[2])));
		xrot[3]=(int)(x+vetorAtores[a->tipo].diag[3]*cos(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[3])));
		yrot[0]=(int)(y-vetorAtores[a->tipo].diag[0]*sin(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[0])));
		yrot[1]=(int)(y-vetorAtores[a->tipo].diag[1]*sin(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[1])));
		yrot[2]=(int)(y-vetorAtores[a->tipo].diag[2]*sin(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[2])));
		yrot[3]=(int)(y-vetorAtores[a->tipo].diag[3]*sin(ATOR_ANG_RAD*(a->olhandoPara+vetorAtores[a->tipo].angulos[3])));
		C2D2_DesenhaSpriteEfeito(vetorAtores[a->tipo].spriteset,
			vetorAtores[a->tipo].animacoes[a->numAnimacao].quadros[a->quadroAtual],
			xrot, yrot, 255, 255, 255, 255);
	}
	// DEsenha o bounding box
// 	C2D2P_Retangulo(x-vetorAtores[a->tipo].largura/2, y-vetorAtores[a->tipo].altura/2,
// 		x+vetorAtores[a->tipo].largura/2, y+vetorAtores[a->tipo].altura/2, 255, 0, 0);
}

// fun��o para centralizar a posi��o do mapa em fun��o de um personagem
void ATOR_CentraMapa(Ator *a, unsigned int idMapa, int lJanela, int aJanela)
{
	// Recuper as dimens�es do mapa
	int lmapa, amapa;
	C2D2M_DimensoesMapa(idMapa, &lmapa, &amapa);
	// Calcula a posi��o do mapa
	int xm = (int)a->x - lJanela/2;
	int ym = (int)a->y - aJanela/2;
	// Ajusta para que fique sempre dentro da tela
	if(xm < 0)
		xm=0;
	else if(xm >= lmapa-lJanela)
		xm = lmapa-lJanela-1;

	if(ym < 0)
		ym=0;
	else if(ym >= amapa-aJanela)
		ym = amapa-aJanela-1;

	// Posiciona de acordo com a posi��o do personagem
	C2D2M_PosicionaMapa(idMapa, xm, ym);
}

// Fun��o para atualizar um ator
bool ATOR_Atualiza(Ator *a, unsigned int mapa)
{
	// O ator � v�lido
	if(a == 0)
		return false;
	// O ator foi registrado
	if(vetorAtores[a->tipo].spriteset==0)
		return false;
	// Retorna o valor da atualiza��o
	return vetorAtores[a->tipo].atualiza(a, mapa);
}

// Fun��o para testar a colis�o entre dois personagens
bool ATOR_ColidiuAtores(Ator *a, Ator *b)
{
	if(a==0 || b==0)
		return false;
	// Se algum for invulner�vel, n�o colide
	if(a->invulneravel || b->invulneravel)
		return false;
	// Se algum estiver inativo ou nascendo, n�o colide
	if(a->estado.estado == ATOR_MORTO || a->estado.estado == ATOR_NASCENDO || a->estado.estado == ATOR_ENCERRADO)
		return false;
	if(b->estado.estado == ATOR_MORTO || b->estado.estado == ATOR_NASCENDO || b->estado.estado == ATOR_ENCERRADO)
		return false;
	// Testa a colis�o do bounding box
	int dx, dy;
	dx = (int)(a->x+vetorAtores[a->tipo].x[0]+vetorAtores[a->tipo].xref - (b->x+vetorAtores[b->tipo].x[0]+vetorAtores[b->tipo].xref));
	dy = (int)(a->y+vetorAtores[a->tipo].y[0]+vetorAtores[a->tipo].yref - (b->y+vetorAtores[b->tipo].y[0]+vetorAtores[b->tipo].yref));
	if( (dx>=0 && dx<vetorAtores[b->tipo].largura) || (dx<0 && abs(dx)<vetorAtores[a->tipo].largura))
		if( (dy>=0 && dy<vetorAtores[b->tipo].altura) || (dy<0 && -dy<vetorAtores[a->tipo].altura))
			if(C2D2_ColidiuSprites(vetorAtores[a->tipo].spriteset, 
					vetorAtores[a->tipo].animacoes[a->numAnimacao].quadros[a->quadroAtual], 
					(int)(a->x+vetorAtores[a->tipo].x[0]), (int)(a->y+vetorAtores[a->tipo].y[0]),
					vetorAtores[b->tipo].spriteset, 
					vetorAtores[b->tipo].animacoes[b->numAnimacao].quadros[b->quadroAtual], 
					(int)(b->x+vetorAtores[b->tipo].x[0]), (int)(b->y+vetorAtores[b->tipo].y[0])))
				{
					Evento e;
					e.tipoEvento = EVT_COLIDIU_PERSONAGEM;
					// indica o tipo do personagem b para o personagem a
					e.subtipo = b->tipo;
					e.x = (int)a->x;
					e.y = (int)a->y;
					ATOR_EnviaEvento(a, &e);
					// indica o tipo do personagem a para o personagem b
					e.subtipo = a->tipo;
					e.x = (int)b->x;
					e.y = (int)b->y;
					ATOR_EnviaEvento(b, &e);
					// retorna verdade
					return true;
				}
	return false;
}

// Fun��o para testar a colis�o de um personagem com o cen�rio
bool ATOR_ColidiuBlocoCenario(Ator *a, unsigned int idMapa, unsigned int codBloco)
{
	if(a==0)
		return false;
	// Por default, n�o colide
	return C2D2M_ColidiuBlocoCenario(idMapa, (int)(a->x-vetorAtores[a->tipo].largura/2), 
		(int)(a->y-vetorAtores[a->tipo].altura/2), vetorAtores[a->tipo].largura, 
		vetorAtores[a->tipo].altura, codBloco);
}

// Fun��o para tocar um efeito sonoro de um ator
int ATOR_TocaEfeito(Ator *a, unsigned int id, int posicao)
{
	// Verifica se o ator e o �udio existem
	if(vetorAtores[a->tipo].spriteset)
		if(id < vetorAtores[a->tipo].numSons)
			if(vetorAtores[a->tipo].sons[id])
				return CA2_TocaEfeito(vetorAtores[a->tipo].sons[id], posicao);
	return -1;
}

// Fun��o para tocar um efeito sonoro de um ator em loop
//
// Data: 30/01/2010
//
int ATOR_TocaEfeitoLoop(Ator *a, unsigned int id, int posicao, int loop)
{
	// Verifica se o ator e o �udio existem
	if(vetorAtores[a->tipo].spriteset)
		if(id < vetorAtores[a->tipo].numSons)
			if(vetorAtores[a->tipo].sons[id])
				return CA2_TocaEfeitoLoop(vetorAtores[a->tipo].sons[id], posicao, loop);
	return -1;
}


// Fun��o para tocar o efeito sonoro de um ator em fun��o de sua posi��o na tela
int ATOR_TocaEfeitoTela(Ator *a, unsigned int id, unsigned int mapa)
{
	int xref=0, yref=0;
	if(mapa!=0)
		C2D2M_PosicaoXY(mapa, &xref, &yref);
	// Verifica se o ator e o �udio existem
	if(vetorAtores[a->tipo].spriteset)
		if(id < vetorAtores[a->tipo].numSons)
			if(vetorAtores[a->tipo].sons[id])
				return CA2_TocaEfeitoTela(vetorAtores[a->tipo].sons[id], (int)(a->x-xref));
	return -1;
}

// Fun��o para enviar um evento para o jogo. Retorna verdadeiro se conseguiu enviar o
// evento. Falso caso contr�rio
bool ATOR_EnviaEventoJogo(Evento *evt)
{
	return FILA_InsereFilaEvento(&filaJogo, evt);
}

// Fun��o para pegar um evento do jogo. Se o evento foi recuperado, retorna true. Caso
// contr�rio, retorna falso.
bool ATOR_ProximoEventoJogo(Evento *evt)
{
	return FILA_RemoveFilaEvento(&filaJogo, evt);
}


// Use para limpar a fila de eventos do jogo
void ATOR_LimpaEventoJogo()
{
	FILA_LimpaFilaEvento(&filaJogo);
}


// fila de eventos

bool FILA_InsereFilaEvento(FilaEventos *f, Evento *evt)
{
	// Testa se � v�lida a fila
	if(f == 0)
		return false;
	// Testa se tem espa�o
	if(f->tamanho >= ATOR_MAX_FILA)
		return false;
	// Insere
	memcpy(&f->fila[(f->inicio+f->tamanho)%ATOR_MAX_FILA], evt, sizeof(Evento));
	// aumenta o tamanho da fila
	f->tamanho++;
	return true;
}

bool FILA_RemoveFilaEvento(FilaEventos *f, Evento *evt)
{
	// Testa se � v�lida a fila
	if(f == 0)
		return false;
	// Testa se tem evento na fila
	if(f->tamanho == 0)
		return false;
	// Copia
	memcpy(evt, &f->fila[f->inicio], sizeof(Evento));
	// move o in�cio
	f->inicio = (f->inicio+1)%ATOR_MAX_FILA;
	// diminui o tamanho da fila
	f->tamanho--;
	return true;
}

void FILA_LimpaFilaEvento(FilaEventos *f)
{
	if(f==0)
		return;
	f->tamanho=0;
	f->inicio=0;
}

// Fun��o que calcula se um bounding box (ator) est� dentro de uma �rea (tela ou mapa)
//
// Data: 01/09/2008
//
bool ATOR_DentroArea(Ator *a, int xref, int yref, int largura, int altura)
{
	// Se um dos extremos laterais est� fora da �rea na horizontal, retorna falso
	if(a->x-xref+vetorAtores[a->tipo].x[2] < 0 || a->x-xref+vetorAtores[a->tipo].x[0]>largura)
		return false;
	if(a->y-yref+vetorAtores[a->tipo].y[2] < 0 || a->y-yref+vetorAtores[a->tipo].y[0]>altura)
		return false;
	// Caso contr�rio, est� dentro da �rea
	return true;
}

// Fun��o para dar um impulso de mandeira correta ao jogador
//
// Data: 09/09/2008
//
void ATOR_Impulsiona(Ator *a, double impulso)
{
	if(a==0)
		return;
	// Modifica o personagem para o impulso
	a->impulsoVertical = impulso;
	a->quadroQueda=0;
}
