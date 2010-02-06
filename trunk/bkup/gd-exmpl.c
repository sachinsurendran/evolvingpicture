
typedef struct pixel {
    int val;
    char c;
}pixel_t;

pixel_t data[]={{0, ' '},\
    {1, '`'},\
    {2, '.'},\
    {3, ','},\
    {4, '-'},\
    {5, '~'},\
    {6, ':'},\
    {7, '"'},\
    {8, '!'},\
    {9, 'i'},\
    {10,'1'},\
    {11,'L'},\
    {12,'S'},\
    {13,'H'},\
    {14,'B'},\
    {15,'Q'},\
    {16,'#'}};



/* File : gd-eg1.c */
#include <gd.h>
#include <stdio.h>
//#define IMG_FILE "unseen.jpg"
#define IMG_FILE "doku.jpg"

inline char pixelize(int i) {
    int indx =0;
    while(data[indx++].val != i)
    {;}
    return data[indx-1].c;
}

int main() {
	gdImagePtr im; //declaration of the image
	FILE *in; //output file
	int black,white,x,y;

/*	im = gdImageCreate(100,100); //create an image, 100 by 100 tpixels

	black = gdImageColorAllocate(im, 0, 0, 0); // allocate black color
	white = gdImageColorAllocate(im, 255, 255, 255);	// allocate white color	
	gdImageLine(im, 0, 0,100,100, white); // draw a line using the allocated white color.*/

	in = fopen(IMG_FILE, "r"); //open a file
        im = gdImageCreateFromJpeg(in);


	//gdImageJpeg(im, in, -1); //write the image to the file using the default quality setting

	/* be good, clean up stuff */
        printf("im.sx =%d im.sy=%d colorsTotal=%d\n",
                im->sx,
                im->sy,
                im->colorsTotal);
        char pixel;

        for(y=0; y<im->sy; y++){
            for(x=0; x<im->sx; x++) {
                //printf("%3d", 167 - im->tpixels[x][y]/100000);
               pixel= pixelize( im->tpixels[y][x]/1000000);
               printf("%c",pixel ); 
            }
            printf("\n");

        }


	fclose(in); 
	gdImageDestroy(im);
}


