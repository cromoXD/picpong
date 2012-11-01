/******************************************************************************
 *
 *            Curso PICMINAS de Microcontroladores (PIC18F4550)
 *
 ******************************************************************************
 * Nome do Arquivo:     actions.h
 * Dependencias:    	Veja seção INCLUDES
 * Processador:       	PIC18F4550
 * Opção de Clock:		HS 20MHz (externo) - CPU: 48MHz	
 * Compilador:        	C18 v3.20 ou superior
 * Empresa:         	Ruhan Bidart <ruhan@2xt.com.br>
 *
 * Plataforma:			Kit PICMINAS v3.0
 *
 * Autor:				Ruhan Bidart <ruhan@2xt.com.br>
 *
 * Data:				04/06/2010
 *
 * Descrição:			
 *
 *****************************************************************************/
#ifndef __PC_ACTIONS_H
#define __PC_ACTIONS_H

// Marcadores de colisao
#define NAO_COLIDIU 0
#define COLIDIU     1
#define GOL         2

// Marcadores de direcao
#define CIMA        0
#define BAIXO       1
#define DIREITA     0
#define ESQUERDA    1

/** I N C L U D E S **********************************************************/

// Perceba que Y para baixo cresce, ao contrario da nocao dos eixos cartesianos

struct barra {
	//unsigned char x;
	unsigned char y;
	void (*proxima_posicao)(void);
	void (*iniciar)(unsigned char y);
	void (*desenhar)(void);
	void (*limpar)(unsigned char linha);
};

struct bola {
	short int x;
	short int y;	
	unsigned char direcao_x;	// DIREITA | ESQUERDA
	unsigned char direcao_y;	// CIMA | BAIXO
	// Ultimas posicoes utilizadas que devem ser limpas antes de escrever a proxima posicao da bola
	signed char cache_limpar;
	void (*proxima_posicao)(void);
	void (*calcular_direcao)(void);
	void (*iniciar)(void);
	void (*desenhar)(void);
	void (*limpar)(void);
};

static struct barra barras; // Barras
//static struct barra barraD; // Barra da direita
static struct bola bola; // Bola

// Marcador para os gols
static unsigned char gols_na_esquerda=0, gols_na_direita=0;

// Marcador utilizado para se fazer o debounce do botao 1 por interrupcao
extern unsigned char barra_up_debounce;	// 0 - Nao fazendo debounce
                                        // 1 - Tempo de Debounce esperado, botao ativo
                                        // 2 - Aguardando o debounce

// Marcador utilizado para se fazer o debounce do botao 2 por interrupcao
extern unsigned char barra_down_debounce; // 0 - Nao fazendo debounce
                                          // 1 - Tempo de Debounce esperado, botao ativo
                                          // 2 - Aguardando o debounce
// Para a barra_down que utiliza o Timer1 temos que aumentar o prescaler utilizando uma variavel
// uma vez que o maximo a que o Timer1 alcanca eh 1:8
extern unsigned char preescaler_fake_timer1;

// Cache para a CGRAM, sempre utilizado ao escrever algum valor nela
static unsigned char CGRAM_cache[3][8];

// Inicia o jogo
extern void iniciar_jogo(void);
 
// Para reiniciar o jogo quando alguem vencer
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#endif

