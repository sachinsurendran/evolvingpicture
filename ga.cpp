/* File : gd-eg1.c */
#include <gd.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "genome.h"

//#define IMG_FILE "unseen.jpg"
#define IMG_FILE "doku.jpg"
//#define IMG_FILE "penguin-org.jpeg"


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

#define MAX_GENE_POOL 10 /* should be a multiple of 2 */
#define MAX_ITERATION 100000000
#define MUTATE_PERCENT 5

#ifdef OLD_CODE
	genome chld1(im->sx, im->sy);
	genome chld2(im->sx, im->sy);
	genome chld3(im->sx, im->sy);
	genome chld4(im->sx, im->sy);
        genome chld5(im->sx, im->sy);
        genome chld6(im->sx, im->sy);
        genome chld7(im->sx, im->sy);
        genome chld8(im->sx, im->sy);
        genome chld9(im->sx, im->sy);
        genome chld10(im->sx, im->sy);
#endif  /* OLD_CODE */
	genome_operator g_op;
	genome *gene_pool[MAX_GENE_POOL];

        g_op.create_genome_pool(gene_pool, MAX_GENE_POOL, im->sx, im->sy);


#ifdef OLD_CODE
        gene_pool[0] =  new genome(im->sx, im->sy);

	gene_pool[0] = &chld1;

	gene_pool[1] = &chld2;
	gene_pool[2] = &chld3;
	gene_pool[3] = &chld4;
        gene_pool[4] = &chld5;
        gene_pool[5] = &chld6;
        gene_pool[6] = &chld7;
        gene_pool[7] = &chld8;
        gene_pool[8] = &chld9;
        gene_pool[9] = &chld10;

	/* Randomize Gene pool */
	for (int indx = 0; indx < MAX_GENE_POOL; indx++)
	{
		gene_pool[indx]->randomize_genome();
		g_op.calculate_fitness(gene_pool[indx], &target_genome);
                g_op.pixelize(gene_pool[indx]);
                printf("\n %d----------------------------------------------------------------------------\n\n", indx);

	}

#endif /* OLD_CODE */

        //g_op.pixelize(&target_genome);

	for (int iteration = 0; iteration < MAX_ITERATION; iteration++)
	{

		/* Sort pool based on fitness */
		g_op.sort_gene_pool(gene_pool, MAX_GENE_POOL);

		/* Crossover the top 2 parents to get children genome */
                g_op.crossover_genepool(gene_pool, MAX_GENE_POOL, MUTATE_PERCENT);
#ifdef MANUAL_CROSSOVER
		g_op.crossover(gene_pool[PARENT1], gene_pool[PARENT2],
		              gene_pool[CHILD1], gene_pool[CHILD2]);
                g_op.crossover(gene_pool[PARENT1], gene_pool[PARENT2],
                        gene_pool[CHILD3], gene_pool[CHILD4]);

                g_op.crossover(gene_pool[PARENT1], gene_pool[PARENT2],
                        gene_pool[CHILD5], gene_pool[CHILD6]);
                g_op.crossover(gene_pool[PARENT1], gene_pool[PARENT2],
                        gene_pool[CHILD7], gene_pool[CHILD8]);

		/* Mutate the children */
                gene_pool[PARENT1]->mutate(0);
                gene_pool[PARENT2]->mutate(0);


		gene_pool[CHILD1]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD2]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD3]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD4]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD5]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD6]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD7]->mutate(MUTATE_PERCENT);
		gene_pool[CHILD8]->mutate(MUTATE_PERCENT);

#endif /* MANUAL_CROSSOVER */

		/* Calculate fitness */
		g_op.calc_fitness_of_pool(gene_pool, MAX_GENE_POOL, &target_genome);

#ifdef DEBUG1
                /* Draw the best genome every 100 cycles */
                if ((iteration % 100) == 0)
                    g_op.pixelize(gene_pool[PARENT1]);
#endif
	}

        g_op.pixelize(gene_pool[PARENT1]);
        //g_op.pixelize(&target_genome);

	gdImageDestroy(im);
}


