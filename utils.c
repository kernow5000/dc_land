
#include <kos.h>
#include <dc_land/dc_land.h>



//general useful functions


// our crappy sleep function, as Usleep does _nothing_

void DC_sleep(int m) {
  int i =0;
  while(i<m*12000*2) i++;
};





