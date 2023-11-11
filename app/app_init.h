/*
 * app_init.h
 *
 *  Created on: Dec 27, 2019
 *      Author: axel
 */

#ifndef APP_INIT_H_
#define APP_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//application initialisation
void app_init();

void button_up();
void button_down();
#ifdef __cplusplus
}
#endif

#endif /* APP_INIT_H_ */
