/******************************************************************************
 *
 *            Curso PICMINAS de Microcontroladores (PIC18F4550)
 *
 ******************************************************************************
 * Nome do Arquivo:     const.h
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
 * Data:				
 *
 * Descrição:			
 *
 *****************************************************************************/
#ifndef __PC_CONST_H
#define __PC_CONST_H

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>	// Biblioteca com as definições do PIC selecionado 
						//no projeto, neste caso, o modelo 18F4550.
// LED's do Kit PICMINAS:	saida digital
// atribuição:	0 = LED desligado
//				1 = LED ligado
#define LED_VERDE 		PORTCbits.RC2
#define LED_VERMELHO 	PORTDbits.RD1
#define LED_AMARELO 	PORTDbits.RD0

// ** Atribuicoes dos Botoes 
#define BARRA_UP		 PORTEbits.RE1  // Botao 1
#define BARRA_DOWN		 PORTEbits.RE2  // Botao 2
#define BARRA_DIREITA_X  12
#define PRESSIONADO      1
#define NAO_PRESSIONADO  0
#define DELAY_BOTAO      140 // tempo para se eliminar o ruido do clicar no botao

// ** Atribuicoes da bola
// Transformamos os caracteres que sobraram em um plano cartesiano limitado
// pelos numeros abaixo
#define BOLA_MIN_X    0
#define BOLA_MIN_Y    0
//#define BOLA_MAX_X    78
#define BOLA_MAX_X    53
#define BOLA_MAX_Y    14
#define BOLA_COLUNAS  11 // Se modificar aqui tem-se que modificar tudo acima refazendo os calculos
#define BOLA_COLUNA_1 1  // Primeira coluna onde a bola vai poder "andar"
#define GOLS_MAX      1  // Gols que o vencedor tem que fazer para vencer 
 
// ** Timers
//  LOAD_TIMER = (estouro do timer) - ( tempo desejado * ( (Clock CPU/4) * prescaler ) )  )
// Preescaler de 1:64, clock interno (48MHz) e tempo 180.
#define INICIO_TMR0	31786

//  LOAD_TIMER = (estouro do timer) - ( tempo desejado * ( (Clock CPU/4) * prescaler ) )  )
// Preescaler de 1:8, clock interno (48MHz) e tempo 180. Utilizamos aqui uma variavel para 
// aumentar o prescaler, pois neste caso (Timer1) o maximo eh 8.
#define INICIO_TMR1	0
 
#endif

