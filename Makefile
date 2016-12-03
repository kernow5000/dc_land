# KallistiOS 1.1.7 library
#
# libdc_land Makefile

# DC LAND FOR KOS

OBJS = 2d.o menu.o pad.o utils.o maple_devices.o vmu.o sound.o strings.o tile.o

myall: $(OBJS)
	rm -f $(KOS_BASE)/lib/libdc_land.a
	$(KOS_AR) rcs $(KOS_BASE)/lib/libdc_land.a $(OBJS)

include $(KOS_BASE)/Makefile.prefab


