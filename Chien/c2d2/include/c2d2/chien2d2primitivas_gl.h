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

#ifndef C2D2PRIMITIVASGL_H
#define C2D2PRIMITIVASGL_H

// Fun��o que inicia a biblioteca
bool C2D2PGL_Inicia();
// Algoritmo para desenhar um vetor de pontos na tela
void C2D2PGL_Pontos(C2D2P_Ponto pontos[], unsigned int nPontos, unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar uma linha reta, baseado em duas coordenadas no plano e uma cor em RGB
void C2D2PGL_Linha(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar um ret�ngulo, baseado em duas coordenadas no plano e uma cor em RGB
void C2D2PGL_Retangulo(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar um ret�ngulo pintado, baseado em duas coordenadas no plano e uma cor em RGB
void C2D2PGL_RetanguloPintado(int x1,int y1,int x2,int y2, unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar um ret�ngulo pintado, baseado em duas coordenadas no plano e uma cor em RGB
void C2D2PGL_RetanguloPintadoAlfa(int x1,int y1,int x2,int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char alfa);
// Algoritmo para desenhar um c�rculo, baseado em uma coordenada no plano, o raio e uma cor em RGB
void C2D2PGL_Circulo(int x,int y,int raio, unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar um c�rculo pintado, baseado em uma coordenada no plano, o raio e uma cor em RGB
void C2D2PGL_CirculoPintado(int x,int y,int raio, unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar uma elipse, baseado no centro da elipse, a rela��o "a" e "b", mais uma cor RGB
void C2D2PGL_Elipse(int xc, int yc, int va, int vb, unsigned char r, unsigned char g, unsigned char b);
// Algoritmo para desenhar uma elipse pintada, baseado no centro da elipse, a rela��o "a" e "b", mais uma cor RGB
void C2D2PGL_ElipsePintada(int xc, int yc, int va, int vb, unsigned char r, unsigned char g, unsigned char b);


#endif
