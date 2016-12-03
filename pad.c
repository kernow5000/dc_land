// pad reading stuff, hardcoded for one pad only boo
// 
// kernow 

#include <kos.h>
#include <dc_land/pad.h>

int get_button () {
  static uint8 mcont = 0;
  cont_cond_t cond;

  if (!mcont) {
    mcont = maple_first_controller();
    // lookkk
    if (!mcont) {
      printf("No controllers attached\r\n");
      return 1;
    }
  }

  /* Check for start on the controller */
  if (cont_get_cond(mcont, &cond)) {
    printf("Error getting controller status\r\n");
    return 1;
  }
    

  if (!(cond.buttons & CONT_DPAD_UP))
    return dpadUP;
  else if (!(cond.buttons & CONT_DPAD_DOWN))
      return dpadDOWN;
  else if (!(cond.buttons & CONT_DPAD_LEFT))
    return dpadLEFT;
  else if (!(cond.buttons & CONT_DPAD_RIGHT))
    return dpadRIGHT;


  if (!(cond.buttons & CONT_START))
    return btnSTART;
  else if (!(cond.buttons & CONT_A))
    return btnA;
  else if (!(cond.buttons & CONT_B))
    return btnB;
  else if (!(cond.buttons & CONT_X))
    return btnX;
  else if (!(cond.buttons & CONT_Y))
    return btnY;


  return 0;
}
