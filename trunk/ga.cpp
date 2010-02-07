/* File : gd-eg1.c */
#include <gd.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "genome.h"

//#define IMG_FILE "unseen.jpg"
//#define IMG_FILE "doku.jpg"
//#define IMG_FILE "penguin-org.jpeg"
#define IMG_FILE "sachin.jpg"


inline char pixelize(int i) {
	int indx =0;
	while(data[indx++].val != i)
	{;}
	return data[indx-1].c;
}


class bin_image {
private:
char *bin_img_ptr;
int x,y;
public:
~bin_image();
bin_image(int x, int y);
void pixelize();
void load(int **img_ptr);
};

bin_image::bin_image(int max_x, int max_y)
{
	printf("x = %d y=%d\n", max_x, max_y);
	bin_img_ptr =(char *) malloc(max_x * max_y);
	x = max_x;
	y = max_y;
	printf("x = %d y=%d\n", x, y);
}

bin_image::~bin_image()
{
	free(bin_img_ptr);
}

void bin_image::pixelize()
{
	unsigned char indx = 0;
	unsigned char i,j;

	printf("Pixelize\n");

	if (bin_img_ptr)
	{
		for (i=0; i<x; i++) {
			indx = 0;
			for (j=0; j<y; j++){
				//printf(" bin_img_ptr[%d][%d] = %d\n", i, j, (char) (* (bin_img_ptr + (x*i) + j)));
				while(data[indx++].val != (char) (*(bin_img_ptr + (x*i) + j)) )
				{;}
				printf("%c", data[indx-1].c);
			}
			printf("\n");
		}
	}
}

void bin_image:: load(int **img_ptr){

	int i,j;

	if(bin_img_ptr) {

		for(i=0; i<x; i++){
			for(j=0; j<y; j++)
			{
				*(bin_img_ptr+(x*i)+j) = (char ) ((img_ptr[i][j])/ 1000000);
				//printf("Value = %d\n", (char ) ((img_ptr[i][j])/ 1000000));
			}
		}
	}
}





class image {
private:
char *filename;

public:
image(char *filename);
~image();
FILE *fp;
int open(char *filename);
void close(FILE *fp);
};


int image::open(char *filename)
{
	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error opening file\n");
		return FAIL;
	} else {
		printf("Opened file %s\n", filename);
		return SUCCESS;
	}
}

image::image(char *filename)
{
	fp = fopen(filename, "r");
}

image::~image()
{
	fclose(fp);
}



void image::close(FILE *fp)
{
	fclose(fp);
}

#define PARENT1 0
#define PARENT2 1
#define CHILD1  2
#define CHILD2  3
#define CHILD3  4
#define CHILD4  5
#define CHILD5  6
#define CHILD6  7
#define CHILD7  8
#define CHILD8  9

int main() {
	gdImagePtr im; //declaration of the image
	int black,white,x,y;
	time_t time_now;

	time_now = time(NULL);

	printf(" Time = %d\n", (int ) time_now);

	srand((int)time_now);

/*	im = gdImageCreate(100,100); //create an image, 100 by 100 tpixels

        black = gdImageColorAllocate(im, 0, 0, 0); // allocate black color
        white = gdImageColorAllocate(im, 255, 255, 255);	// allocate white color
        gdImageLine(im, 0, 0,100,100, white); // draw a line using the allocated white color.*/

	image pic(IMG_FILE);

	im = gdImageCreateFromJpeg(pic.fp);

	bin_image bin_img(im->sx, im->sy);
	bin_img.load(im->tpixels);

	//bin_img.pixelize();

	// Load the target genome
	genome target_genome(im->sx, im->sy);
	target_genome.load_from_image(im->tpixels);

#define MAX_GENE_POOL 20 /* should be a multiple of 2 */
#define MAX_ITERATION 100000000
        
#define MUTATE_PERCENT  0.1099 /* Keep Mutation rate very low, this is seen to imcrease the rate of Fitness, and faster Convergence */

	genome_operator g_op;
	genome *gene_pool[MAX_GENE_POOL];

        g_op.create_genome_pool(gene_pool, MAX_GENE_POOL, im->sx, im->sy);
        

        //g_op.pixelize(&target_genome);

	//for (int iteration = 0; iteration < MAX_ITERATION; iteration++)
        int iteration = 0;
        while(1)
	{
		/* Sort pool based on fitness */
		g_op.sort_gene_pool(gene_pool, MAX_GENE_POOL);

		/* Crossover the top 2 parents to get children genome */
                g_op.crossover_genepool(gene_pool, MAX_GENE_POOL, MUTATE_PERCENT);

		/* Calculate fitness */
		g_op.calc_fitness_of_pool(gene_pool, MAX_GENE_POOL, &target_genome);
#define DEBUG1
#ifdef DEBUG1
                /* Draw the best genome every 100 cycles */
                if ((iteration++ % 100) == 0) {
                    g_op.pixelize(gene_pool[PARENT1]);
                    printf("Fitness = %d\n", gene_pool[PARENT1]->get_fitness());
                }

#endif
	}

        g_op.pixelize(gene_pool[PARENT1]);
        //g_op.pixelize(&target_genome);

	gdImageDestroy(im);
}


