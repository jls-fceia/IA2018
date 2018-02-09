/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include "board.h"

#define LED1 1
#define LED2 2


static int i = 0, sec = 0, usec;
enum estados { INIT, GEN_SEC, PULSANDO, PERDISTE, FIN};
enum Eventos { SW1_ON, SW2_ON };

int secuencia[100];
int usr_sec[100];

enum estados estado_actual = INIT;
enum estados proximo_estado= INIT;

	/*
	 * Retardo(): delay primitivo
	 */
void Retardo( int ciclos ){
	int n;

	for( n = 0; n < ciclos; n++ ){
		;
	}
	return;
}

	/*
	 * GenSec(): genera una secuencia aleatoria de encendido de LEDs, sumando uno al anterior
	 */
void GenSec( void ){
	int a, sigte;

	a = rand() % 10 + 1;
	sigte = a > 5 ? LED1:LED2;
	secuencia[sec++] = sigte;
	return;
}

	/*
	 * RepSec(): reproduce la secuencia almacenada
	 */
void RepSec(void){
	int k;
	for( k = 0; k < 100; k++ )
		switch( secuencia[k] ){
		case 1:
			LED_ROJO_ON;
			Retardo( 1000 );
			LED_ROJO_OFF;
			break;

		case 2:
			LED_VERDE_ON;
			Retardo( 1000 );
			LED_VERDE_OFF;

		default:
			return;

		}
	return;
}

/*
 * CompSec(): compara la secuencia del usuario con la de la maquina, devuelve 0
 * 			  si son iguales, != 0 si no
 */
int CompSec(void){
	int l;
	for( l = 0; l < sec; l++ )
		if( secuencia[l] != usr_sec[l])
			return -1;

	return 0;
}

/*
 * Ganaste(): el usuario llego a reproducir una secuencia de 100!
 * 			  Parpadeo alternativo de leds
 */
void Ganaste(void){
	int n;
	for( n = 0; n < 20; n++ ){
		LED_ROJO_ON;
		Retardo( 100 );
		LED_ROJO_OFF;
		LED_VERDE_ON;
		Retardo( 100 );
		LED_VERDE_OFF;
	}
	return;
}

int main(void)
{

    /*
     * Inicializacion placa y estructuras de datos
     */

	int j;
	int8_t tp1, tp2;

	board_init();

	/*
     * Lazo principal: termina con la pulsacion simultanea de SW1 y SW3
     */
    for (;;) {

    	/*
    	 * Maquina de estados con switch
    	 */
    	switch( estado_actual ){
    	case INIT:
    		/*
    		 * Inicializar secuencia del kit y del usuario
    		 */
    		for( j = 0; j < 100; j++ ){
    			secuencia[j] = 0;
    			usr_sec[j] = 0;
    		}
    		sec = 0;
    		usec = 0;
    		proximo_estado = GEN_SEC;
    		break;

    	case GEN_SEC:
    		if( sec >= 100 ){
    			proximo_estado = INIT;
    			Ganaste();
    			break;
    		}

    		GenSec();
    		RepSec();
    		proximo_estado = PULSANDO;
    		break;

    	case PULSANDO:
    		do{
    			/*
    			 * Leer pulsadores
    			 */
    			tp1 = pulsadorSw1_get();
    			tp2 = pulsadorSw3_get();

    			if( tp1 != 0 && tp2 == 0)
    				usr_sec[ usec++ ] = LED1;
    			else
    				if( tp1 == 0 && tp2 != 0 )
    					usr_sec[ usec++ ] = LED2;
    				else {
    					proximo_estado = PERDISTE;
    					break;
    				}
    		} while( usec < sec);

    		/*
    		 * Comparar secuencia
    		 */
    		if( CompSec() == 0 )		// sec usuario == sec maquina
    			proximo_estado = GEN_SEC;
    		else
    			proximo_estado = PERDISTE;

    		break;

    	case PERDISTE:
    		for( i = 0; i < 20; i++ ){
    			LED_ROJO_TOGGLE;
    			Retardo( 1000 );
    		}
    		proximo_estado = FIN;
    		break;

    	case FIN:
    		return 0;
    	}

    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
