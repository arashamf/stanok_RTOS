
#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes --------------------------------------------------------------------------//
#include "main.h"

// Exported types -------------------------------------------------------------------//

//Private defines ------------------------------------------------------------------//
#define KEY_BOUNCE_TIME 			50 				// время дребезга в мс
#define KEY_AUTOREPEAT_TIME 	100 			// время автоповтора в мс
#define COUNT_REPEAT_BUTTON 	5

// Prototypes ---------------------------------------------------------------------//
uint16_t scan_keys (void);
uint16_t scan_keys_long (uint16_t );
uint8_t scan_buttons_GPIO (uint16_t );
#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H__ */

