#indef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>

void Button_Init(void);
void Button_Update(void);
bool Button_IsPressed(void);

#endif 

