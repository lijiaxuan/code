#ifndef STORE_H
#include "driver/sd/file.h"
#include "module/pid/pid_global.h"
#include "module/GPS/GPS_global.h"
#include "module/brake/brake_global.h"
#include "module/steer/steer_global.h"

void str_init(void);
void str_save(void);
void str_close(void);

#define STORE_H
#endif
