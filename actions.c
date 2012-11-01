/******************************************************************************
 *
 *            Curso PICMINAS de Microcontroladores (PIC18F4550)
 *
 ******************************************************************************
 * Nome do Arquivo:     actions.c
 * Dependencias:    	Veja a seção INCLUDES abaixo
 * Processador:       	PIC18F4550
 * Opção de Clock:		HS 20MHz (externo) - CPU:48MHz
 * Compilador:        	C18 v3.20 ou superior
 * Empresa:         	Ruhan Bidart <ruhan@2xt.com.br>
 *
 * Plataforma:			Kit PICMINAS v3.0
 *						
 * Descrição:			Acoes dos "objetos" estarao aqui.
 * 
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include "const.h"
#include "my_xlcd.h"
#include "actions.h"
#include <stdio.h>
#include <timers.h>
#include <delays.h>
#include <math.h>

/** V A R I A V E I S  G L O B A I S ******************************************/
unsigned char barra_up_debounce = 1;
unsigned char barra_down_debounce = 1;
unsigned char preescaler_fake_timer1 = 0;


/** F U N C O E S ************************************************************/
static void jogar(void); 
static void placar(void); 
static unsigned char verificar_colisao(unsigned char); 
static void CGRAM_cache_escrever(void);

/**
 * Acoes da BOLA
 **/
// Verifica a proxima posicao da bola 
void bola_proxima_posicao(void) 
{
	// Horizontal
	if(bola.direcao_x == ESQUERDA) {		
		// Trocamos a direcao pois alcancou o maximo
		if(bola.x-- == BOLA_MIN_X) {
			bola.direcao_x = DIREITA;
			
			// Verificamos se foi gol ou nao
			verificar_colisao(0);
		}
	} else {
		// Trocamos a direcao pois alcancou o maximo
		if(bola.x++ == BOLA_MAX_X) {
			bola.direcao_x = ESQUERDA;
			
			// Verificamos se foi gol ou nao
			verificar_colisao(1);
		}	
	}
	
	// Vertical
	if(bola.direcao_y == CIMA) {
		// Trocamos a direcao pois alcancou o maximo
		if(bola.y-- == BOLA_MIN_Y) {
			bola.direcao_y = BAIXO;
		}	
	} else {
		// Trocamos a direcao pois alcancou o maximo
		if(bola.y++ == BOLA_MAX_Y) {
			bola.direcao_y = CIMA;
		}	
	}
	bola.desenhar();
}

void bola_desenhar(void) s
{	
	// Posicao do caracter inicial utilizado na bola
	unsigned short int x_char_pos = (bola.x/5) + BOLA_COLUNA_1;
	unsigned short int y_char_pos = bola.y > 7 ? 0x40 : 0x00;
	// Posicao do Y relativa ao caracter
	unsigned char x_rel_pos = (bola.x%5) - (bola.direcao_x == ESQUERDA ? 1 : 0);
	unsigned char y_rel_pos = bola.y - (y_char_pos > 0 ? 7 : 0);
	unsigned char i, j;

	// Verificando as condicoes em que teremos que digitar 2 caracteres
	// para formar a bola.
	// Verificando a condicao em X
	// Limpando a bola anterior
	if((x_char_pos + y_char_pos) != bola.cache_limpar) bola.limpar();

	bola.cache_limpar = x_char_pos + y_char_pos;
//XLCD_posicionar(0x04);
//XLCD_escrever_string(itoa(x_char_pos));
//XLCD_escrever_string(itoa(y_char_pos));


	// ** Caso nao ocorra nenhuma colisao basta fazer um desenho
	// em 1 caracter.
	// Criando o caractere
	for(i=0; i<8; i++) {
		if(i >= y_rel_pos && i <= y_rel_pos+1) { 
			// TODO: fazer de uma forma melhor, talvez com a formula abaixo
			// Invertemos o valor, pois aqui 0 -> 16 | 1 -> 8 | 2 -> 4 | 3 -> 2 | 4 -> 1
			//XLCD_escrever_byte(pow(2, (x_rel_pos-4)*-1) + pow(2, (x_rel_pos-3)*-1)); 
			if(x_rel_pos == 0) {
				CGRAM_cache[2][i] = 0b00011000; 
			} else if(x_rel_pos == 1) {
				CGRAM_cache[2][i] = 0b00001100; 
			} else if(x_rel_pos == 2) {
				CGRAM_cache[2][i] = 0b00000110; 
			} else if(x_rel_pos == 3) {
				CGRAM_cache[2][i] = 0b00000011; 
			} else if(x_rel_pos == 4) {
				// Nao deve cair aqui, pois a validacao acima 
				// deveria ter pego estes casos
			}
		} else {
			CGRAM_cache[2][i] = 0b00000000; 	
		}
	}

	CGRAM_cache_escrever();


	// Escrevendo efetivamente
	XLCD_posicionar(x_char_pos + y_char_pos);
	XLCD_escrever_byte(0x02);
}

// "Limpa" a posicao anterior da bola para desenhar outra
// TODO: fazer verificacoes se mudou de espaco de caracter, caso nao 
// tenha modificado nao eh necessario faze-lo
void bola_limpar(void) 
{
	if(bola.cache_limpar != -1) {
		XLCD_posicionar(bola.cache_limpar);
		XLCD_escrever_byte(0x20);
	}
}

// Calcula a direcao da bola logo apos uma colisao
void bola_calcular_direcao(void) 
{
	if(barras.y-1 == bola.y || barras.y == bola.y) {
		bola.direcao_y = CIMA;
	} else {
		bola.direcao_y = BAIXO;
	}
}

void bola_iniciar(void) 
{
	// Atribuindo funcoes ao "objeto"
	bola.proxima_posicao = &bola_proxima_posicao;
	bola.desenhar = &bola_desenhar;	
	bola.limpar = &bola_limpar;	
	bola.calcular_direcao = &bola_calcular_direcao;	
	bola.cache_limpar = -1;
	
    // A bola vai comecar do meio indo para a direita acima
	bola.direcao_x = DIREITA;
	bola.direcao_y = CIMA;
	bola.x = 0;
	bola.y = 0;
	bola.desenhar();
}
	

// --------------------------------------------------------------------------------

/**
 * Acoes da BARRA
 **/
// Verifica a proxima posicao da barra dependendo do botao que esta 
// sendo clicado. O Botao1 indica para cima e o Botao2 para baixo
// ja a desenhando
void barra_proxima_posicao(void) 
{
		// *** BOTAO UP
	    // Utilizacao de debounce para o botao 1
	    if(barra_up_debounce == 1) {
	        if(BARRA_UP == PRESSIONADO) {
	            // Feito o debounce identificando ainda botao pressionado
				LED_AMARELO = 1;
				barra_up_debounce = 0;								
				if(barras.y > 0) {
					// Limpando as barras pois vai mudar de linha
					if(barras.y == 6) barras.limpar(1);
					barras.y--;
					barras.desenhar();
				}	            
	        } else {
	            // Caso tenha soltado o botao antes de dar o tempo de 
				barra_up_debounce = 0;				
				LED_AMARELO = 0;
	        }
	    } else {
			LED_AMARELO = 0;
	        if(BARRA_UP == PRESSIONADO && barra_up_debounce != 2) {
		        WriteTimer0(INICIO_TMR0);
	            // O Timer vai fazer o debounce ligando o flag `botao_1_debounce`
				barra_up_debounce = 2;
		    }
	    }

	    // *** BOTAO DOWN
	    if(barra_down_debounce == 1) {
	        if(BARRA_DOWN == PRESSIONADO) {
				LED_VERMELHO = 1;	
				barra_down_debounce = 0;								
				if(barras.y < 11) {
					// Limpando as barras pois vai mudar de linha
					if(barras.y == 5) barras.limpar(0);
					barras.y++;
					barras.desenhar();
				}
	        } else {
				LED_VERMELHO = 0;
	            // Caso tenha soltado o botao antes de dar o tempo de 
				barra_down_debounce = 0;				
	        }
	    } else {
			LED_VERMELHO = 0;
	        if(BARRA_DOWN == PRESSIONADO && barra_down_debounce != 2) {
		        WriteTimer1(INICIO_TMR1);
	            // O Timer vai fazer o debounce ligando o flag `botao_1_debounce`
				barra_down_debounce = 2;
		    }
	    }
}

// Limpa o caractere da barra, escrevendo um espaco vazio
void barra_limpar(unsigned char linha) 
{
	if(linha == 0) {
		// Barra Esquerda
		XLCD_posicionar(0x00);
		XLCD_escrever_byte(0x20);
		
		// Barra Direita
		XLCD_posicionar(0x00 + BARRA_DIREITA_X);
		XLCD_escrever_byte(0x20);
	} else {
		// Barra Esquerda
		XLCD_posicionar(0x40);
		XLCD_escrever_byte(0x20);

		// Barra Direita		
		XLCD_posicionar(0x40 + BARRA_DIREITA_X);
		XLCD_escrever_byte(0x20);
	}	
	// Volta a posicao para a anterior, lembrando que ao escrever algo
	// o cursor vai para frente
	//XLCD_posicionar(barra_y);
}


// Veja que a barra tem que ser desenhada e diversas posicoes 
// dependendo do seu valor de X.
// TODO: melhorar performance da escrita na CGRAM do caracter a ser utilizado
// utilizando um range de posicoes.
void barra_desenhar(void) 
{
	unsigned char i=0;
	unsigned char barra_y = barras.y%6;
	
	// ** Fazemos dois desenhos de barras para evitar fazer o entrelacamento entre
	// o desenho da barra e da bola quando ocorrer um choque. Desta forma ira 
	// aparentar um choque entre bola e barra naturalmente
	// Encontramos o desenho da barra esquerda
	for(i=0; i<8; i++) {
		if(i<barra_y || i>(barra_y+2)) {
			CGRAM_cache[0][i] = 0b00000000; 
		} else {
			CGRAM_cache[0][i] = 0b00000011; 
		}
	}	

	// Encontramos o desenho da direita
	for(i=0; i<8; i++) {
		if(i<barra_y || i>(barra_y+2)) {
		 	CGRAM_cache[1][i] = 0b00000000; 
		} else {
			CGRAM_cache[1][i] = 0b00011000; 
		}
	}	

	CGRAM_cache_escrever();

	// Barra Esquerda	
	XLCD_posicionar(barras.y >= 6 ? 0x40 : 0x00);
	XLCD_escrever_byte(0x00);
	
	// Barra Direita
	XLCD_posicionar(barras.y >= 6 ? (0x40 + BARRA_DIREITA_X) : (0x00 + BARRA_DIREITA_X));
	XLCD_escrever_byte(0x01);
}

void barra_iniciar(unsigned char y) 
{
    // Atribuindo as funcoes ao "objeto"
	barras.proxima_posicao = &barra_proxima_posicao;
	barras.desenhar = &barra_desenhar;
	barras.limpar = &barra_limpar;

	// X deve ser menor do que 16 e Y menor do que 12
	//barra->x = x;
	barras.y = y;
	barras.desenhar();
}

// --------------------------------------------------------------------------------

/**
 * Acoes do Jogo
 **/

// Verifica se houve uma colisao entre a bola e a barra
// retornando:
//	NAO_COLIDIU - Se nao houve colisao e esta distante
//  COLIDIU     - Se houve colisao
//  GOL         - Se nao houve colisao e passou pela barra
// Alem de verificar as colisoes ela define para qual lado a 
// bola ira apos uma possivel colisao.
static unsigned char verificar_colisao(unsigned char barra) {
	// Barra da esquerda
	if(barra == 0) {
		// Pelos locais onde esta funcao eh chamada neste momento nos ja sabemos que colidiu
//		if(bola.x == BOLA_MIN_X) {
			if(bola.y >= barras.y-1 && bola.y <= barras.y + 3) {
				bola.calcular_direcao();
				return COLIDIU;
			} else {
				gols_na_esquerda++;
				placar();
				return GOL;
			}
//		} else {
//			return NAO_COLIDIU;
//		}
	} else {
		// Barra da direita
		// Pelos locais onde esta funcao eh chamada neste momento nos ja sabemos que colidiu
		//if((bola.x/5) == BARRA_DIREITA_X) {
			if(bola.y >= barras.y-1 && bola.y <= barras.y + 3) {
				bola.calcular_direcao();
				return COLIDIU;
			} else {
				gols_na_direita++;
				placar();
				return GOL;
			}
		//} else {
			return NAO_COLIDIU;
		//}
	}
}

// Escreve o placar do jogo e zera caso alguem venca
static void placar(void) 
{
	char buffer[4];
	unsigned short char i;

	// Mostra a vitoria dependendo da quantidade de gols
	// e reinicia o jogo
	if(gols_na_esquerda > GOLS_MAX || gols_na_direita > GOLS_MAX) {
		XLCD_limpar();
		XLCD_posicionar(0x00);
		if(gols_na_esquerda > GOLS_MAX) {
			XLCD_escrever_rom_string("Jogador 1 vence!");
		} else {
			XLCD_escrever_rom_string("Jogador 2 vence!");
		}
		// Delay para possibilitar a leitura da frase
		for(i=0; i<10; i++) Delay10KTCYx(100000);
		
		// Reinicia o jogo
    	_asm goto _startup _endasm
	}
	
	// Escrevendo o placar efetivamente
	sprintf(buffer, "%dx%d", gols_na_esquerda, gols_na_direita);
	XLCD_posicionar(0x40 + 13);
	XLCD_escrever_string(buffer);
}	

// Da os dados iniciais para se jogar o jogo
void iniciar_jogo(void) 
{	
	// Mostramos o placar inicial
	placar();
	
	// Apontamos a cada "objeto" as funcoes que ele deve conter
	barras.iniciar = &barra_iniciar;
	barras.iniciar(0);	

	bola.iniciar = &bola_iniciar;
	bola.iniciar();
	
	jogar();
}

// Loop infinito que faz as verificacoes dos estados correntes do jogo
static void jogar(void) 
{
	while(1) 
	{
		barras.proxima_posicao();	
		bola.proxima_posicao();
	}
}


/**
 * Funcoes auxiliares (TODO: colocar isto em outro arquivo - utils)
 **/
// Escreve na CGRAM os caracteres que estao no cache
static void CGRAM_cache_escrever(void)
{
	unsigned char register i, j;
 	XLCD_CGRAM();
	// ** Fazemos dois desenhos de barras para evitar fazer o entrelacamento entre
	// o desenho da barra e da bola quando ocorrer um choque. Desta forma ira 
	// aparentar um choque entre bola e barra naturalmente
	// Encontramos o desenho da barra esquerda
	for(i=0; i<3; i++) {
		for(j=0; j<8; j++) {
			XLCD_escrever_byte(CGRAM_cache[i][j]); 
		}
	}
}