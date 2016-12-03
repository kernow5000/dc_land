// stuff for vmu SCREEN handling

#include <kos.h>
#include <dc_land/vmu.h>


// load an icon
void vmu_icon_init(const char *vmu_icon) {
	int x, y, xi, xb;
	uint8 bitmap[48*32/8];
	uint8 addr;

	addr = maple_first_vmu(); // this can do all 4 in 1.1.8 i think
	if (!addr) return;

	memset(bitmap, 0, 48*32/8);
	for (y=0; y<32; y++)
		for (x=0; x<48; x++) {
			xi = x / 8;
			xb = 0x80 >> (x % 8);
			if (vmu_icon[(31-y)*48+(47-x)] == '.')
				bitmap[y*(48/8)+xi] |= xb;
		}
	
	vmu_draw_lcd(addr, bitmap); // commit to screen
}


void vmu_icon_clear() {
   // hmm, i could copy half of the code above, but instead
   vmu_icon_init(NULL);
   // :D
}
