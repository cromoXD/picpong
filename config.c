/******************************************************************************
 *
 *            Curso PICMINAS de Microcontroladores (PIC18F4550)
 *
 ******************************************************************************
 * Nome do Arquivo:     config.c
 * Dependencias:    	Veja a seção INCLUDES abaixo
 * Processador:       	PIC18F4550
 * Opção de Clock:		HS 20MHz (externo) - CPU:48MHz
 * Compilador:        	C18 v3.20 ou superior
 * Empresa:         	Ruhan Bidart <ruhan@2xt.com.br>
 *
 * Plataforma:			Kit PICMINAS v3.0
 *						
 * Descrição:	Configuracoes serao mantidas aqui.
 * 
 *****************************************************************************/


/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <timers.h>
#include "my_xlcd.h"
#include "const.h"
#include "config.h"


/** F U N C O E S ************************************************************/
void configurar_interrupcoes(void);


/******************************************************************************
 * Funcao:     	void ConfiguraSistema(void)
 * Entrada:    	Nenhuma (void)
 * Saída:      	Nenhuma (void)
 * Descrição:  	ConfiguraSistema é a rotina de configuração principal do projeto.
 *				Seu objetivo é configurar as portas de I/O e os demais periféricos
 *				do microcontrolador para que operem da maneira desejada no projeto.  
 *****************************************************************************/

void configurar_sistema (void)
{
	// Desabilita todas as 13 portas analógicas ANx (portas do conversor A/D) assim
	//as mesmas fucionam como pinos digitais
	ADCON1 |= 0x0F;

	// Configura LED's do Kit PICMINAS: saida digital
	TRISCbits.TRISC2=0; 	// RC2: saida digital - LED_VERDE
	TRISDbits.TRISD1=0; 	// RD1: saida digital - LED_VERMELHO
	TRISDbits.TRISD0=0; 	// RD0: saida digital - LED_AMARELO
	
	LED_VERDE=0;
	LED_AMARELO=0;
	LED_VERMELHO=0;
	
	// Configura Botões do Kit PICMINAS: entradas digitais
	TRISEbits.TRISE1=1; 	// RE1: entrada digital - BOTAO_1
	TRISEbits.TRISE2=1; 	// RE2: entrada digital - BOTAO_2

    configurar_interrupcoes();

	//Configura os pinos que são usados pelo LCD 16x2 alfa-numerico
	XLCD_iniciar();
	XLCD_limpar();
}

void configurar_interrupcoes(void)
{
    RCONbits.IPEN=1;		// Habilita prioridade de interrupção
	
	INTCONbits.GIE=0; 		// Desabilita TODAS as interrupções

	INTCONbits.PEIE=1;		// Habilita interrupção de periféricos

	// ** Para o TIMER 0
	// Configura interrupção do Timer0 para que ocorra um estouro (overflow) a cada 180 ms
	OpenTimer0(T0_16BIT &			// Timer0 usando 16 bits
			   T0_SOURCE_INT &	// Fonte de clock interna, o mesmo da CPU
			   T0_PS_1_64 &		// Prescaler = 1:64, ou seja, divide o clock da CPU por 64
			   TIMER_INT_ON		// Habilita interrupção e inicia a contagem do Timer0
			   );

	WriteTimer0(INICIO_TMR0);	// Escreve o valor correspondente ao atraso de um segundo
								// nos registradores do Timer0: TMR0L e TMR0H
								
	INTCON2bits.TMR0IP = 1;		// Seleciona ALTA prioridade para interrupção do Timer0
	INTCONbits.TMR0IF = 0;    	// Garantir flag inicial da interrupção em zero.

	// ** Para o TIMER 1
    // Configura interrupção do Timer0 para que ocorra um estouro (overflow) a cada 100 ms
	OpenTimer1(T1_16BIT_RW &	// Timer0 usando 16 bits
			   T1_SOURCE_INT &	// Fonte de clock interna, o mesmo da CPU
			   T1_PS_1_8 &		// Prescaler = 1:8, ou seja, divide o clock da CPU por 8, este eh o max para o Timer1
			   TIMER_INT_ON		// Habilita interrupção e inicia a contagem do Timer0
			   );	

	WriteTimer1(INICIO_TMR1);	// Escreve o valor correspondente ao atraso de um segundo
								// nos registradores do Timer1: TMR1L e TMR1H
	
	IPR1bits.TMR1IP = 0;        // Seleciona BAIXA prioridade para interrupção

	PIR1bits.TMR1IF = 0;        // Garantir flag inicial da interrupção em zero.
	
	INTCONbits.GIE=1; 			// Habilita todas as interrupcoes
}