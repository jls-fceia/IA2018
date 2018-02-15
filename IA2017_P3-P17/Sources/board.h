/*
 * board.h
 *
 *  Created on: 2/5/2017
 *      Author: jose
 */

#ifndef SOURCES_BOARD_H_
#define SOURCES_BOARD_H_

#include <time.h>
#include "MKL46Z4.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define LED_ROJO_TOGGLE		GPIOE_PTOR |= (1 << 29)
#define LED_ROJO_OFF		GPIOE_PSOR |= (1 << 29)
#define LED_ROJO_ON			GPIOE_PCOR |= (1 << 29)

#define LED_VERDE_TOGGLE	GPIOD_PTOR |= (1 << 5)
#define LED_VERDE_OFF		GPIOD_PSOR |= (1 << 5)
#define LED_VERDE_ON		GPIOD_PCOR |= (1 << 5)

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

/** \brief inicialización del hardware
 **
 **/
void board_init(void);

/** \brief Devuelve estado del pulsador sw1
 **
 ** \return 1: si el pulsdor está apretado
 **         0: si el pulsador no está apretado
 **/
int8_t pulsadorSw1_get(void);

/** \brief Devuelve estado del pulsador sw3
 **
 ** \return 1: si el pulsdor está apretado
 **         0: si el pulsador no está apretado
 **/
int8_t pulsadorSw3_get(void);

/**
 **	\brief Retardo calculado en milisegundos
 **
 **/
void ret_milis( int milis );
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif



#endif /* SOURCES_BOARD_H_ */
