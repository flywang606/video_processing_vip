#include <math.h>
#include "video_unit.h"

static int mirror(int x, int min, int max){
    while(x < min || x >= max){
        if(x < min) x = min + (min - x - 1);
        if(x >= max) x = max + (max - x - 1);
    }

    return x;
}

static float SQR(float x){
    return x * x;
}

double img_ssim(struct img_t *img1, struct img_t *img2)
{
    float c1 = SQR(0.01f * 255.0f);
    float c2 = SQR(0.03f * 255.0f);

    int m = 16;

    float Sum = 0.0f;

    int mx, my;
    int y, x;
    int dy, dx;

    int height=img1->ht;
    int width=img1->wt;

    if(img1->ht != img2->ht || img1->wt != img2->wt)

    {
        printf("image size unmatched\n");
        return -1;
    }

    for(y=0;y<height;y++){
        for(x=0;x<width;x++){

            float ave1 = 0.0f, ave2 = 0.0f; // 平均
            float var1 = 0.0f, var2 = 0.0f; // 分散
            float cov = 0.0f; // 共分散

            for(dy=-m;dy<=m;dy++){
                for(dx=-m;dx<=m;dx++){
                    mx = mirror(x + dx, 0, width);
                    my = mirror(y + dy, 0, height);

                    ave1 += img1->data[my][mx];
                    ave2 += img2->data[my][mx];
                }
            }
            ave1 /= SQR(m * 2.0f + 1.0f);
            ave2 /= SQR(m * 2.0f + 1.0f);

            for(dy=-m;dy<=m;dy++){
                for(dx=-m;dx<=m;dx++){
                    mx = mirror(x + dx, 0, width);
                    my = mirror(y + dy, 0, height);

                    var1 += SQR(img1->data[my][mx] - ave1);
                    var2 += SQR(img2->data[my][mx] - ave2);
                    cov += (img1->data[my][mx] - ave1) * (img2->data[my][mx] - ave2);
                }
            }
            var1 /= SQR(m * 2.0f + 1.0f);
            var2 /= SQR(m * 2.0f + 1.0f);
            cov /= SQR(m * 2.0f + 1.0f);

            Sum += ((2.0f * ave1 * ave2 + c1) * (2.0f * cov + c2)) / ((SQR(ave1) + SQR(ave2) + c1) * (var1 + var2 + c2));
        }
    }

    return Sum / (width * height);
}

double img_psnr(struct img_t *img1, struct img_t *img2)
{
    int h, w;
    double MSE;
    double psnr;

    if(img1->ht != img2->ht || img1->wt != img2->wt)
    {
        printf("image size unmatched\n");
        return -1;
    }

    MSE=0;
    for(h=0; h<img1->ht; h++)
    {
        for(w=0; w<img1->wt; w++)
        {
            MSE += (img1->data[h][w]-img2->data[h][w])*(img1->data[h][w]-img2->data[h][w]);
        }
    }

    MSE /= img1->ht*img1->wt;

    if(MSE==0.0)
        return 99.0;

    psnr=10*log10(255*255/MSE);

    return psnr;
}


double img_snr_gain(struct img_t *pimg_orig,struct img_t *pimg_noise,
                    struct img_t *pimg_denoise)
{
    int i,j;
    u16 height,width;
    u32 sub_orig_noise,sub_orig_denoise;

    height = pimg_orig->ht;
    width = pimg_orig->wt;

    sub_orig_noise = 0;
    sub_orig_denoise = 0;
    
    for(j=0;j < height;j++)
    {
        for(i=0;i < width;i++)
        {
            sub_orig_noise += ((pimg_orig->data[j][i]-pimg_noise->data[j][i])*(pimg_orig->data[j][i]-pimg_noise->data[j][i]));
            sub_orig_denoise +=((pimg_orig->data[j][i]-pimg_denoise->data[j][i])*(pimg_orig->data[j][i]-pimg_denoise->data[j][i]));
        }
    }

    return 10*log10(sub_orig_noise/sub_orig_denoise);
}
