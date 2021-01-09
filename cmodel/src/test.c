#include <stdio.h>
#include "video_in_out.h"

int main()
{
	char *file="./../origin_image/lena_64_64.ppm";
    gen_rgb_memory_bin(file);
}