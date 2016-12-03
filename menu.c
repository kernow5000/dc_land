
#include <kos.h>
#include <dc_land/dc_land.h>



struct a_menu *new_menu (char *title) {
  struct a_menu *menu = (struct a_menu *) malloc (sizeof (struct a_menu));
  int i;

  strcpy (menu -> title, title);

  for (i = 0; i < MAX_OPTIONS; i++)
    strcpy (menu -> options[i], "");
  
  menu -> num_of_options = 0;

  return menu;
}


int add_option (struct a_menu *menu, char *caption) {

  if (menu -> num_of_options == MAX_OPTIONS)
    return -1;

  strcpy (menu -> options [menu -> num_of_options], caption);
  menu -> num_of_options++;

  return (menu -> num_of_options - 1);    //returns the array posn of the caption
}


int display_menu (struct a_menu *menu, int x, int y, int box_flag) {
  int i, y2 = y, button = 300, current = 0;

  //if (box_flag == 1) {
    //find the longest line in the options, draw the box that big   8>
    //height = 40 + (num_of_options * 24)
    //box (x, y, 
  //}

  v_drawstring (x, y, menu -> title);
  y += 36;

  for (i = 0; i < menu -> num_of_options; i++) {
    v_drawstring (x, y, menu -> options[i]);
    y += 24;
  }

  y = y2 + 48;

  v_circle (x - 10, y, 8, 0, 255, 0);

  
  while ( (button != btnX) && (button != btnSTART) ) {
    DC_sleep(200);
    button = get_button ();
	
    switch (button) {
    case dpadUP: {
      if (current > 0) {
	v_circle (x - 10, y, 8, 0, 0, 0);
	y -= 24;
	v_circle (x - 10, y, 8, 0, 255, 0);
	current --;
      }
      else {
	v_circle (x - 10, y, 8, 0, 0, 255);
	DC_sleep (60);
	v_circle (x - 10, y, 8, 0, 255, 0);
      }
      break;
    }
    case dpadDOWN: {
      if (current < (menu -> num_of_options - 1)) {
	v_circle (x - 10, y, 8, 0, 0, 0);
	y += 24;
	v_circle (x - 10, y, 8, 0, 255, 0);
	current ++;
      }
      else {
	v_circle (x - 10, y, 8, 0, 0, 255);
	DC_sleep (60);
	v_circle (x - 10, y, 8, 0, 255, 0);
      }
      break;
    }
    default: {
      v_circle (x - 10, y, 8, 0, 0, 255);
      DC_sleep (60);
      v_circle (x - 10, y, 8, 0, 255, 0);
      break;
    }
    }
  }  //end of while (button != btnX)
  
  if (button == btnSTART) {   //a way out! other than choosing.. for dev purposes
    v_drawstring (x + 300, y,  ":debug: bing!");
    return -1;
  }
  
  return current;
}

