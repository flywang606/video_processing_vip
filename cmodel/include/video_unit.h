#ifndef VIDEO_UNIT_H
#define VIDEO_UNIT_H
#include <stdio.h>
#include "typedef.h"

#define CLIP(a,b,c) (((a>=b)&&(b>=c))||((c>=b) && (b>=a)))?b:((((b>=a) && (a>=c))||((c>=a) && (a>=b)))?a:c)

struct img_t
{
	u16 wt;
	u16 ht;
	u16 **data;
};

struct img_yv12_t
{
	u16 wt;
	u16 ht;
	u16 **y;
	u16 **u;
	u16 **v;
};

struct img_nv12_t
{
	u16 wt;
	u16 ht;
	u16 **y;
	u16 **uv;
};

#endif
