/* File : gd-eg1.c */
#include <gd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include "genome.h"
#include "image.h"
#include "file.h"

#define LINE1 "#Target File: doku.jpg"
#define LINE2 "#Segments: 23X23"
#define LINE3 "#Segments Chosen: Random"
#define LINE4 "#Gene Pool Size: 640"
#define LINE5 "#Parent Pool Size: 320"
#define LINE6 "#Offspring Pool Size: 320"
#define LINE7 "#Iteration: 2,500"
#define LINE8 "#Parents: Multiple"
#define LINE9 "#Group(s): 1"
#define LINE10 "#Mutation Probability: 0.001"
#define LINE11 "#Iteration   Fitness"

#define DAT_FILE "graphPlot.dat"

#define MAX_GENE_POOL 640 /* should be a multiple of 2 */
#define MAX_PARENT_POOL 320
#define MAX_ITERATION 2500
        
#define MUTATE_PERCENT  0.1099 /* Keep Mutation rate very low, this is seen to imcrease the rate of Fitness, and faster Convergence */

using namespace std;

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
	//ofstream outfile ("10Par10Chi1.dat"); // create file store fitness data
	ostringstream oss;
	oss << LINE1 << endl << LINE2 << endl << LINE3 << endl << LINE4 << endl << LINE5 << endl;
	oss << LINE6 << endl << LINE7 << endl << LINE8 << endl << LINE9 << endl << LINE10 << endl << LINE11 << endl;
	datFile outFile(DAT_FILE);
	outFile.init(oss.str());
	//outfile << "#Generation   " << "Fitness" << endl;
	
	time_t time_now;
	time_now = time(NULL);

	printf(" Time = %d\n", (int ) time_now);

	srand((int)time_now);

	image pic(IMG_FILE);

	im = gdImageCreateFromJpeg(pic.fp);

	bin_image bin_img(im->sx, im->sy);
	bin_img.load(im->tpixels);

	//bin_img.pixelize();

	// Load the target genome
	genome target_genome(im->sx, im->sy);
	target_genome.load_from_image(im->tpixels);

	genome_operator g_op;
	genome *gene_pool[MAX_GENE_POOL];
	//vector <genome> gene_pool;

        g_op.create_genome_pool(gene_pool, MAX_GENE_POOL, im->sx, im->sy);
        

        //g_op.pixelize(&target_genome);

	//for (int iteration = 0; iteration < MAX_ITERATION; iteration++)
        int iteration = 0;
        //while(1)
	while(iteration <= MAX_ITERATION)
	{
		/* Calculate fitness */
		g_op.calc_fitness_of_pool(gene_pool, MAX_GENE_POOL, &target_genome);

		/* Sort pool based on fitness */
		g_op.sort_gene_pool(gene_pool, MAX_GENE_POOL);

#define DEBUG1
#ifdef DEBUG1
                /* Draw the best genome every 100 cycles */
                if ((iteration % 1) == 0) {
                	g_op.pixelize(gene_pool[PARENT1]);

			/* Pack the fitness and generation data as a output stringstream and send it to dat file  */			
			ostringstream ss;
			ss << iteration << "	" << gene_pool[PARENT1]->get_fitness();
			outFile.append(ss.str());

                	printf("Generation = %d        Fitness = %d    X = %d    Y = %d\n",iteration,gene_pool[PARENT1]->get_fitness(),im->sx,im->sy);

			/* Calculate fitness */
			//g_op.calc_fitness_of_pool(gene_pool, MAX_GENE_POOL, &target_genome);
			/* Sort pool based on fitness */
			//g_op.sort_gene_pool(gene_pool, MAX_GENE_POOL);

			//g_op.backupGenePool(gene_pool, MAX_GENE_POOL);
                }

		/* Crossover the top 2 parents to get children genome */
                //g_op.crossover_genepool(gene_pool, MAX_GENE_POOL, MUTATE_PERCENT);

		/* Crossover the top n parents to get children genome */
                g_op.crossover_genepool(gene_pool, MAX_GENE_POOL, MAX_PARENT_POOL, MUTATE_PERCENT);

		iteration++;
#endif
	}

        //g_op.pixelize(gene_pool[PARENT1]);
        //g_op.pixelize(&target_genome);

	/* Calculate fitness */
	g_op.calc_fitness_of_pool(gene_pool, MAX_GENE_POOL, &target_genome);//, genePoolOnFile);


	/* Sort pool based on fitness */
	g_op.sort_gene_pool(gene_pool, MAX_GENE_POOL);

	//g_op.backupGenePool(gene_pool, MAX_GENE_POOL);
	gdImageDestroy(im);
	outFile.close();
}


