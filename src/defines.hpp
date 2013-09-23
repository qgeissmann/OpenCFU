//#include "defines.hpp"
//Should be in all headers in all headers!
#ifndef DEFINES_H
#define DEFINES_H

#include <assert.h>
#include <unistd.h>
#include "config.h"

#ifdef DBG_MESSAGE
#include <iostream>
#define DEV_INFOS(X) std::cerr<<"file:"<<__FILE__<<"\tline:"<<__LINE__<<"\t=>"<<X<<std::endl
//#define DEV_INFOS(X) ;
#else
#define DEV_INFOS(X) ;
#endif // DBG_MESSAGE


#define OCFU_THR_NORM 0
#define OCFU_THR_INV 1
#define OCFU_THR_BILAT 2

#define MODE_STANDBY 0
#define MODE_IMG 1

#define PROCESSOR_WORKING_SIGNAL 0
#define PROCESSOR_IDLE_SIGNAL 1
//#define PROCESSOR_HAS_REWORKED_SIGNAL 2



#define MASK_TYPE_NONE 0
#define MASK_TYPE_FILE 1
#define MASK_TYPE_DRAW 2
#define MASK_TYPE_AUTO 3

#define MASK_TOOL_3P_CIRCLE 0
#define MASK_TOOL_CONV_POLYGON 1

//GUI:
#define DISPLAY_MIN_MARGIN 10

#endif // NDEVEL


#define MY_PI 3.1416

