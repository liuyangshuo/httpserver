#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void conf_init(const char *path, const char *file);

#endif
