	/**
	 * \brief Simulador de juego "Simon says" con Maquina de estados finitos en FRDKL46Z
	 */

	/**
	 * Informatica Aplicada - Ingenieria Electronica
	 *            DSI - EIE - FCEIA
	 *
	 *         Practica 3, ejercicio 17
	 *                 2018
	 */

#include <stdlib.h>
#include "board.h"

#define LED1 1
#define LED2 2


static int i = 0, sec = 0, usec;

	/**
	 * <Estados y eventos de la MEF
	 */
enum estados { INIT, GEN_SEC, PULSANDO, PERDISTE, FIN};
enum Eventos { SW1_ON, SW2_ON };

	/**
	 * <Secuencias de la maquina y del usuario
	 */

int secuencia[100];
int usr_sec[100];

	/**
	 * <Inicializacion de la MEF
	 */

enum estados estado_actual = INIT;
enum estados proximo_estado= INIT;


	/**
	 * \brief GenSec(): genera una secuencia aleatoria de encendido de LEDs, sumando uno al anterior
	 */
void GenSec( void ){
	int a, sigte;

	a = rand() % 10 + 1;
	sigte = a > 5 ? LED1:LED2;
	secuencia[sec++] = sigte;
	return;
}

	/**
	 * \brief RepSec(): reproduce la secuencia almacenada
	 */
void RepSec(void){
	int k;
	for( k = 0; k < 100; k++ )
		switch( secuencia[k] ){
		case 1:
			LED_ROJO_ON;
			ret_milis( 1000 );
			LED_ROJO_OFF;
			break;

		case 2:
			LED_VERDE_ON;
			ret_milis( 1000 );
			LED_VERDE_OFF;

		default:
			return;

		}
	return;
}

/**
 * \brief CompSec(): compara la secuencia del usuario con la de la maquina, devuelve 0
 * 			  si son iguales, != 0 si no
 */
int CompSec(void){
	int l;
	for( l = 0; l < sec; l++ )
		if( secuencia[l] != usr_sec[l])
			return -1;

	return 0;
}

/**
 * \brief Ganaste(): el usuario llego a reproducir una secuencia de 100!
 * 			  Parpadeo alternativo de leds
 */
void Ganaste(void){
	int n;
	for( n = 0; n < 20; n++ ){
		LED_ROJO_ON;
		ret_milis( 100 );
		LED_ROJO_OFF;
		LED_VERDE_ON;
		ret_milis( 100 );
		LED_VERDE_OFF;
	}
	return;
}

	/**
	 * \brief Funcion main(): implementacion de la MEF
	 * @return
	 */
int main(void)
{

    /*
     * Inicializacion placa y estructuras de datos
     */

	int j;
	int8_t tp1, tp2;

	board_init();

	/**
     * <Lazo principal: termina con la pulsacion simultanea de SW1 y SW3
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

    			/**
    			 * < con ambos pulsadores se termina el juego
    			 */
    			if( tp1 == 1 && tp2 == 1)
    				return;

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

    		/**
    		 * < Comparar secuencia
    		 */
    		if( CompSec() == 0 )		// sec usuario == sec maquina
    			proximo_estado = GEN_SEC;
    		else
    			proximo_estado = PERDISTE;

    		break;

    	case PERDISTE:
    		for( i = 0; i < 20; i++ ){
    			LED_ROJO_TOGGLE;
    			ret_milis( 1000 );
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
