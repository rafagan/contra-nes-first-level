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

#ifndef CHIEN2D2_INTERNO_H
#define CHIEN2D2_INTERNO_H

#include <c2d2/chien2d2.h>

// indica se a Chien2D 2 j� foi inicializada. Por default, n�o foi ainda.
extern bool inicializado;
// A tela principal
extern SDL_Surface *screen;
// O vetor com os sprite sets
extern C2D2_SpriteSet sprites[C2D2_MAX_SPRITESET];
// Finalmente, o vetor com as fontes
extern C2D2_Fonte fontes[C2D2_MAX_FONTES];
// Indica se o sistema est� em shutdown ou n�o
extern bool c_shutdown;
// O vetor de teclas utilizadas
extern C2D2_Botao teclas[C2D2_MAXTECLAS];
// O mouse do sistema
extern C2D2_Mouse mouse;



// Aten��o desenvolvedor: n�o utilize as seguintes fun��es, limitadas a uso interno!
// Fun��o para zerar os dados da Chien2D 2
void reset();
// Fun��o auziliar para pontos transparentes
bool PontoTransparente(SDL_Surface *surface, int x, int y);

// Fun��es relacionadas com as bitmasks dos sprites

// Fun��o para criar uma bit mask para um sprite
C2D2_BitMask* C2D2_CriaBitMask(int largura, int altura);
// Fun��o para remover uma bitmask de um sprite
void C2D2_RemoveBitMask(C2D2_BitMask *bmask);
// Fun��o para criar o vetor de bitmasks de um spriteset
C2D2_BitMask** C2D2_CriaVetorBitMasks(SDL_Surface *imagem, int spx, int spy, int limagem, int aimagem);
// Fun��o para setar um bit do bitmask
void C2D2_SetBit(C2D2_BitMask *bmask, int x, int y, int val);
bool C2D2_GetBit(C2D2_BitMask *bmask, int x, int y);
// Fun��o para preencher o vetor de bitmasks de um spriteset
void C2D2_PreencheVetorBitMasks(SDL_Surface *imagem, int spx, int spy, int limagem, int aimagem, C2D2_BitMask** bmask);
// Fun��o para testar a colis�o entre dois bitmasks, dado um offset x e y da segunda em rela��o � primeira
int C2D2_SobrepoeBitMasks(C2D2_BitMask *a, C2D2_BitMask *b, int offx, int offy);

#endif
