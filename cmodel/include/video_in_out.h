#include "video_unit.h"

int gen_rgb_memory_bin(char *file_in);
u16 **Malloc2D_uchr(int wt, int ht, u8 init);
void write_nv12(char *file_out,struct img_t *img_y,
                        struct img_t *img_u,struct img_t *img_v);
void read_nv12(char *file_in,struct img_t *img_y,
                        struct img_t *img_u,struct img_t *img_v);