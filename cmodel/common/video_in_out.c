
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
	while(c == "#")
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
void gen_yuv_memory_bin(FILE *fp)
{
	
}