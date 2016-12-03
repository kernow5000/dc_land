// sound stuff, stolen from dan below :(
// kernow 02/11/02



#include <kos.h>

#include <dc_land/dc_land.h>
#include <dc_land/sound.h> // not needed really, just being safe
#include <dc_land/s3mplay.h>

void s3m_play(char *s3m) {

   file_t f;
   int len;
   uint8 *song;
   
   // open the s3m
   f = fs_open(s3m, O_RDONLY);
   len = fs_total(f);
   
   // map it
   song = fs_mmap(f);

   spu_disable();
   
   // load it
   spu_memload(0x10000, song, len);
   spu_memload(0, s3mplay, sizeof(s3mplay));
   
   // play it
   spu_enable();
   
   fs_close(f);
}


void s3m_stop() {
// stop the sound, useful for starting a different tune, etc

   sound_stop();

}


void sound_stop() {
   
   // just a nice wrapper
   spu_disable();
   
}




