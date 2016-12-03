// maple stuff
// 22/12/01
// 
// this is really old, we need a new way of doing this, but still using pad_exists();
// multi controllers and vmu also needed


#include <kos.h>
#include <dc_land/dc_land.h>


void detect_maple()
{
  // this doesnt do anything anymore
   
  //if( keyboard_exists() )
  //  v_drawstring(vram_s+40*640+20, 640 ,0, "<*> Keyboard found");
  
  //if( pad_exists() )
  //  v_drawstring(vram_s+60*640+20, 640 ,0, "<*> Pad found"); 

  //if( vmu_exists() )
  //  v_drawstring(vram_s+80*640+20, 640 ,0, "<*> VMU found"); 
  
}



extern uint8 keyboard_exists()
{
    return 0;
}

uint8 pad_exists()
{
  static uint8 cont = 0;

   // hardcoded to one pad, for now
  cont = maple_first_controller();
  if(cont) 
    {
      return cont;
    }
  else
    return 0;
   // need to do two controllers, then four
}


uint8 vmu_exists()
{
      return 0;
};
