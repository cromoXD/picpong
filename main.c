/******************************************************************************
 *
 *            Curso PICMINAS de Microcontroladores (PIC18F4550)
 *
 ******************************************************************************
 * Nome do Arquivo:     main.c
 * Dependencias:    	Veja a se��o INCLUDES abaixo
 * Processador:       	PIC18F4550
 * Op��o de Clock:		HS 20MHz (externo) - CPU:48MHz
 * Compilador:        	C18 v3.20 ou superior
 * Empresa:         	PICMINAS - Axoon - UFMG
 *
 * Plataforma:			Kit PICMINAS v3.0
 * Pr�tica:	 			Pratica 7.1 - Uso da biblioteca my_xlcd.h
 *						
 * Descri��o:	Usar a biblioteca de usuario my_xlcd.h para escrever no display 
 *				alfanumerico 16x2. 
 *				Fazer um projeto que ao pressionar um bot�o (Bot�o_1 ou Bot�o_2),
 *				escreve no Display de LCD qual o bot�o est� pressionado no momento
 *				Usando as fun��es da biblioteca string.h para manipular as
 *				strings do seu firmware
 *				Dicas:
 *				 - Para escrever no display utilize a biblioteca de usuario my_xlcd.h
 *				 - Lembre-se dos modificadores "rom" e "ram" que informam ao compilador
 *				C18 se as vari�veis devem ser alocadas na m�moria de programa(FLASH) ou
 *				mem�ria de dados(SDRAM), respectivamente.
 *				A bibliotecas padr�o C18, string.h tem varias fun��es para manipula��o 
 *				de strings e a convers�o de valores decimais em string, usa-las facilitar� na 
 *				implemmenta��o do projeto.
 *				 - Informa��es sobre as bibliotecas se encontram na propria pasta 
 *				da mesma, por exemplo, C:\MCC18\doc\hlpC18Lib.chm
 * 
 *****************************************************************************/


/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <stdlib.h>
#include <string.h>
#include <timers.h>
#include "my_xlcd.h"	// Biblioteca com de rotinas para usar o display LCD no Kit AxoonPIC v3.0
#include "config.h"
#include "const.h"
#include "actions.h"

/** D E F I N E S ************************************************************/


/** V A R I A V E I S   G L O B A I S ****************************************/
extern unsigned char barra_up_debounce;
extern unsigned char barra_down_debounce;


/**  P R O T O T I P O S   P R I V A D O S ***********************************/
void tratar_high_interrupt(void);
void tratar_low_interrupt(void);


/** F U N C O E S ************************************************************/

/******************************************************************************
 * Funcao:    	void main(void)
 * Entrada:     Nenhuma (void)
 * Sa�da:      	Nenhuma (void)
 * Descri��o:   Fun��o principal do programa. O termo "main" significa principal,
 *				pois esta � a primeira a ser chamada ap�s o	reset do 
 *				microcontrolador.
 *				Todas as outras fun��es sao chamadas a partir da "main".
 *****************************************************************************/

void main(void)
{
	configurar_sistema();
	iniciar_jogo();	
}


/** I N T E R R U P C O E S **************************************************/
// As interrupcoes aqui sao utilizadas para fazer o debounce dos botoes 1 e 2
// ATEN��O NA SINTAXE de declara��o com #pragma interrupt = Alta prioridade
#pragma interrupt tratar_high_interrupt
void tratar_high_interrupt(void)
{
	// Verifica se o motivo da chamada da interrup��o foi o estouro(overflow) do Timer0
	if(INTCONbits.TMR0IF)
	{
		//Re-carrega valor inicial do contador do Timer0
		WriteTimer0(INICIO_TMR0);

        if(barra_up_debounce == 2) barra_up_debounce = 1;

		//Limpa flag da interrup��o do Timer0
		INTCONbits.TMR0IF = 0;
	}
}


#pragma interruptlow tratar_low_interrupt
void tratar_low_interrupt(void)
{
	// Verifica se o motivo da chamada da interrup��o foi o estouro(overflow) do Timer0
	if(PIR1bits.TMR1IF)
	{
		if(++preescaler_fake_timer1 >= 2) {
			//Re-carrega valor inicial do contador do Timer0
			WriteTimer1(INICIO_TMR1);
	
	        if(barra_down_debounce == 2) barra_down_debounce = 1;
			
			preescaler_fake_timer1 = 0;
		}
		//Limpa flag da interrup��o do Timer0
		PIR1bits.TMR1IF = 0;
	}// end tratamento da interrup��o do Timer0 (INTCONbits.TMR0IF)
}


/** V E C T O R   R E M A P P I N G ******************************************/

// ATEN��O: COPIAR ESTA SE��O DO CODIGO PARA TODO ARQUIVO "main.c" DOS PROJETOS QUE
// UTILIZAM O BOOTLOADER PARA GRAVA��O IN-CIRCUIT.

// prot�tipo usado pelo compilador C18
extern void _startup (void);        // See c018i.c in your C18 compiler dir

// Aloca��o da fun��o de tratamento do "reset" da aplica��o principal 
// no endere�o 0x800 da mem�ria de programa
#pragma code REMAPPED_RESET_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code  	// Diretiva que retorna a aloca��o dos endere�os 
				// da mem�ria de programa para seus valores padr�o
/** F I M  D A  S E � � O  D E   V E C T O R   R E M A P P I N G *************/


/** I N T E R R U P T V E C T O R   R E M A P P I N G ****************************/

// Aloca��o da fun��o de tratamento das interrup��es de ALTA prioridade
// no endere�o 0x808 da mem�ria de programa.
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x000808
void _high_ISR (void)
{
    _asm goto tratar_high_interrupt _endasm
}

// Aloca��o da fun��o de tratamento das interrup��es de BAIXA prioridade
// no endere�o 0x818 da mem�ria de programa
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = 0x000818
void _low_ISR (void)
{
    _asm goto tratar_low_interrupt _endasm
}

#pragma code  	// Diretiva que retorna a aloca��o dos endere�os
				// da mem�ria de programa para seus valores padr�o

/** EOF main.c ***************************************************************/
