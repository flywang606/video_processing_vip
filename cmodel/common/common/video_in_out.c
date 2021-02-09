#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "video_in_out.h"


//rgb888
int gen_rgb_memory_bin(char *file_in)
{
	//ppm to *.bin
	FILE *fp;
	char buffer[16];
	int c,w,h,i=0;
	u32 image_w,image_h,color_depth;
	
	fp = fopen(file_in,"rb");
	if(fp == NULL)
	{
	    printf("open error\n");
		return -1;
	}
	
	if(!fgets(buffer,sizeof(buffer),fp))
	{
		return -1;
	}
	
	if((buffer[0] != 'P')||(buffer[1] != '6'))
	{
		return -1;
	}
	
	c = getc(fp);
	while(c == '#')
	{
		while(getc(fp) != '\n');
			c = getc(fp);
	}
    
	ungetc(c,fp);
	fscanf(fp,"%d %d",&image_w,&image_h);
	//
	fscanf(fp,"%d",&color_depth);
	
	u8 *data=(u8 *)malloc(image_w*image_h*3*sizeof(u8));
	
	fread(data,sizeof(u8),image_w*image_h*3,fp);
	//data[i] data[i+1] data[i+2] r g b
#ifdef HDL_SIM
    FILE *hdl_fp;
    hdl_fp = fopen("./../IMG_BIN.bin","wb");
    if(hdl_fp == NULL)
	{
	    printf("hdl open error\n");
		return -1;
	}
    fwrite(data,sizeof(u8),image_w*image_h*3,hdl_fp);
    fclose(hdl_fp);
#endif
    
	free(data);
	fclose(fp);
	
	return 0;
}

//yuv422(yu16) yuv420(nv12)
//output:y u v 3 binary
void gen_yuv_memory_bin(char *file_in,int isyu16)
{
	
}

void read_nv12(char *file_in,struct img_t *img_y,
                        struct img_t *img_u,struct img_t *img_v)
{
    FILE *fp;
    u16 width = img_y->wt;
    u16 height = img_y->ht;
    u8 *data_y=(u8 *)malloc(width*height*sizeof(u8));
    u8 *data=(u8 *)malloc(width*height/2*sizeof(u8));
    int i,j,m;
    
    fp = fopen(file_in,"rb");
	if(fp == NULL)
	{
	    printf("open error\n");
		//return -1;
	}

    //fread(&(img_y->data[0][0]),sizeof(u8),width*height,fp);
    fread(data_y,sizeof(u8),width*height,fp);
    fread(data,sizeof(u8),width*height/2,fp);

    m=0;
    for(j = 0;j < height;j++)
    {
        for(i = 0;i < width;i++)
        {
            img_y->data[j][i] = data_y[m++];
        
        }
    }
    
    m=0;
    for(j = 0;j < height/2;j++)
    {
        for(i = 0;i < width/2;i++)
        {
            img_u->data[j][i] = data[m++];
            img_v->data[j][i] = data[m++];
        }
    }
    free(data);
    fclose(fp);
}
                        
void write_nv12(char *file_out,struct img_t *img_y,
                        struct img_t *img_u,struct img_t *img_v)
{
    int i,j,m;
    u16 width = img_y->wt;
    u16 height = img_y->ht;
    u8 *data=(u8 *)malloc(width*height*3/2*sizeof(u8));
    FILE *fp;
    fp = fopen(file_out,"wb");
    if(fp == NULL)
    {
        printf("open error\n");
        return ;
    }

    m=0;
    for(j = 0;j < height;j++)
    {
        for(i = 0;i < width;i++)
        {
            data[m++] = img_y->data[j][i]&0xff;
        }
    }

    for(j = 0;j < height/2;j++)
    {
        for(i = 0;i < width/2;i++)
        {
            data[m++] = img_u->data[j][i]&0xff;
            data[m++] = img_v->data[j][i]&0xff;
        }
    }
    assert(m!=(width*height*3/2-1));
    fwrite(data,sizeof(u8),width*height*3/2,fp);
    
    free(data);
    fclose(fp);
}

u16 **Malloc2D_uchr(int wt, int ht, u8 init)
{
    int h,w;
    u16 **x;

    x=(u16 **)malloc(sizeof(void *)*ht);
    if(x==NULL){
        printf("malloc failed (%s,%i)\n",__FILE__,__LINE__);
    }
    x[0]=(u16 *)malloc(sizeof(u16)*ht*wt);
    if(x[0]==NULL){
        printf("malloc failed (%s,%i)\n",__FILE__,__LINE__);
    }

    for(h=1;h<ht;h++){
        x[h]=x[h-1]+wt;
    }
    for(h=0;h<ht;h++){
        for(w=0;w<wt;w++){
            x[h][w]=init;
        }
    }
    return x;
}

