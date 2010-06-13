#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
//#include <string>
#include <sstream>

#define SUCCESS 1
#define FAIL    0

#define MAX_CHAR 17

#define SEGMENT_X 23
#define SEGMENT_Y 23


#define RANGE 1000
#define MUTATION_PROPABLITY 0.0901 /* 50 percent */
#define MUTATION_FACTOR (RANGE * MUTATION_PROPABLITY) 

#define WORST_FITNESS 0xffff

using namespace std;

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
    {16,'#'},\
    {MAX_CHAR,'X'}};




class genome{
    private:
        unsigned int fitness;
        unsigned int x,y;
        char *genome_ptr;

    public:
        friend class genome_operator;

        genome(int max_x, int max_y);
        ~genome();
        int get_fitness();
        void set_fitness(int );
        void randomize_genome();
        void mutate();
        void mutate(float percent);
        void load_from_image(int **img_ptr);
};


genome::genome(int max_x, int max_y)
{
    x = max_x;
    y = max_y;
    fitness = 65530;
    genome_ptr = (char *) malloc(max_x * max_y);
}

genome::~genome()
{
    free(genome_ptr);
    printf("Destroying Genome\n");
}

void genome::load_from_image(int **img_ptr)
{
    int i,j;

    if(genome_ptr) {
        for(i=0; i<y; i++){
            for(j=0; j<x; j++)
            {
                *(genome_ptr + (x*i) + j) = (char ) ((img_ptr[i][j])/ 1000000);
                //printf("Value = %d\n", (char ) ((img_ptr[i][j])/ 1000000));
            }
        }
    } else {
        printf(" Target Genome Memory not initialised\n");
    }
}


void genome::randomize_genome()
{
    int i,j;
    
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            *(genome_ptr+ (x*i) + j) = (rand() % MAX_CHAR);
        }
    }
}

void genome::mutate()
{
    int i,j;

    for(i=0; i<x; i++)
        for(j=0; j<y; j++)
        {
            if((rand() % RANGE) <  MUTATION_FACTOR)
            {
                *(genome_ptr+ (x*i) + j) = (rand() % MAX_CHAR);
            }
        }
}

void genome::mutate(float percent)
{
    int i,j;

    for(i=0; i<y; i++)
        for(j=0; j<x; j++)
        {
            if( (rand() % RANGE) < ((percent * RANGE)/100) )
            {
                *(genome_ptr+ (x*i) + j) = (rand() % MAX_CHAR);
            }
        }
}

int genome::get_fitness()
{
    return fitness;
}

void genome::set_fitness(int new_fitness)
{
    fitness = new_fitness;
}

/*
 *
 * GENOME OPERATOR:
 *
 */

class genome_operator{

    public:
	void create_genome_pool(genome *head[], int gene_pool_size, int gene_X_size, int gene_Y_size);
	void create_genome_pool(genome *head[][10], int group_size, int gene_pool_size, int gene_X_size, int gene_Y_size);
	void create_genome_pool(vector <genome*> *head, int gene_pool_size, int gene_X_size, int gene_Y_size);
	void calc_fitness_of_pool(genome *head[], int gene_pool_size, genome *target_genome);
	void calc_fitness_of_pool(vector <genome*> *head, int gene_pool_size, genome *target_genome);
        void calculate_fitness(genome *some_genome, genome *target_genome);
        //void calculate_fitness(genome some_genome, genome *target_genome);
        void pixelize(genome *some_genome);
        void sort_gene_pool(genome *head[], int gene_pool_size);
        void sort_gene_pool(vector <genome*> *head, int gene_pool_size);
        void crossover_genepool(genome *head[], int gene_pool_size, float mutation_percent);
        void crossover_genepool(genome *head[], int gene_pool_size, int parent_pool_size, float mutation_percent);
        void crossover_genepool(vector <genome*> *head, int gene_pool_size, int parent_pool_size, float mutation_percent);
        void crossover(genome *parent_gen1,
                genome *parent_gen2,
                genome *child_gen1,
                genome *child_gen2);
	void crossover(genome *head[], int gene_pool_size, int parent_pool_size);
	void crossover(vector <genome*> *head, int gene_pool_size, int parent_pool_size);
	void segmentSliceCopy_2D(genome *parent_gen,
                        genome *child_gen,
			int x1, int x2,
			int y1, int y2);
	void backupGenePool(genome *head[], int gene_pool_size);
};

void genome_operator::create_genome_pool(genome *head[], int gene_pool_size, int gene_X_size, int gene_Y_size)
{
    for (int count=0; count < gene_pool_size; count++)
    {
        head[count] = new genome(gene_X_size, gene_Y_size);
        head[count]->randomize_genome();
    }
}

void genome_operator::create_genome_pool(genome *head[][10], int group_size, int gene_pool_size, int gene_X_size, int gene_Y_size)
{
    for (int i=0; i < group_size; i++)
    {
    	for (int j=0; j < gene_pool_size; j++)
    	{
        	head[i][j] = new genome(gene_X_size, gene_Y_size);
        	head[i][j]->randomize_genome();
    	}
    }
}

void genome_operator::create_genome_pool(vector <genome*> *head, int gene_pool_size, int gene_X_size, int gene_Y_size)
{
    genome *temp_genome;
    //genome temp_gene(gene_X_size, gene_Y_size);
    for (int count=0; count < gene_pool_size; count++)
    {
        temp_genome = new genome(gene_X_size, gene_Y_size);
        //genome temp_genome(gene_X_size, gene_Y_size);
        int i;
    	//cin >> i;
        head->push_back(temp_genome);
        //cout << count << ": " << head->at(count).get_fitness() << endl;
        //cout << "Exiting iteration: " << head->size() << endl;
        //cin >> i;
    }
    
    for (int count=0; count < head->size(); count++)
    {
        //cout << count << ": " << head->at(count)->get_fitness() << endl;
        head->at(count)->randomize_genome();
        //std::cout << "Line: " << count << std::endl;
    }
    
}


void genome_operator::calc_fitness_of_pool(genome *head[], int gene_pool_size, genome *target_genome)
{
    int indx;
    
    for (indx = 0; indx < gene_pool_size; indx++)
    {
    	calculate_fitness(head[indx], target_genome);
    }
    //printf("---------------------------------\n");
}

void genome_operator::calc_fitness_of_pool(vector <genome*> *head, int gene_pool_size, genome *target_genome)
{
    int indx;
    
    for (indx = 0; indx < gene_pool_size; indx++)
    {
    	calculate_fitness(head->at(indx), target_genome);
    }
    //printf("---------------------------------\n");
}

void genome_operator::calculate_fitness(genome *some_genome, genome *target_genome)
{
    int max_x = some_genome->x;
    int max_y = some_genome->y;
    int fitness = 0;
    int x, y;


    for (x = 0; x < max_y; x++)
    {
        for (y = 0; y < max_x; y++)
        {
            int diff = *(some_genome->genome_ptr + (x * max_x) + y) - *(target_genome->genome_ptr + (x * max_x) + y);

            if (diff > 0)
                fitness += diff;
            else
                fitness -= diff;

	    //*genePoolOnFile << " " << (int)*(some_genome->genome_ptr + (x * max_x) + y);
        }
    }
    some_genome->set_fitness(fitness);
    //*genePoolOnFile << endl;
    //printf("fitness = %d\n", fitness);
}

/*void genome_operator::calculate_fitness(genome some_genome, genome *target_genome)
{
    int max_x = some_genome.x;
    int max_y = some_genome.y;
    int fitness = 0;
    int x, y;

    for (x = 0; x < max_y; x++)
    {
        for (y = 0; y < max_x; y++)
        {
            int diff = *(some_genome.genome_ptr + (x * max_x) + y) - *(target_genome->genome_ptr + (x * max_x) + y);

            if (diff > 0)
                fitness += diff;
            else
                fitness -= diff;
        }
    }
    some_genome.set_fitness(fitness);
}*/

void genome_operator::sort_gene_pool(genome *head[], int gene_pool_size)
{
    genome *temp_genome_ptr;
    int indx, loop_within;

    //for (indx = 0; indx < gene_pool_size/2; indx++)
    for (indx = 0; indx < gene_pool_size-1; indx++)
    {
        for (loop_within = 0; loop_within < (gene_pool_size-1); loop_within++)
        {
            if (head[loop_within]->get_fitness() > head[loop_within+1]->get_fitness())
            {
                temp_genome_ptr = head[loop_within];
                head[loop_within] = head[loop_within+1];
                head[loop_within+1] = temp_genome_ptr;
            }
        }
    }
//#define DEBUG
#ifdef DEBUG
    /* Print fitness of best parent */
    printf("---------------------------------\n");
    printf(" Best Fitness [%d]  = %d\n", 0, head[0]->get_fitness());
    /* Print fitness of worst genome */
    printf(" Worst Fitness [%d] = %d\n", 0, head[5]->get_fitness());
    printf("---------------------------------\n");
#endif

    /*for (indx = 0; indx < gene_pool_size; indx++)
    {
	printf("%d: %d\n",indx,head[indx]->get_fitness());
    }
    scanf("%d",&indx);*/

}

struct compairClass {
  bool operator() (genome a, genome b) { return (a.get_fitness() <= b.get_fitness()); }
} compair;

bool myCompair(genome* a, genome* b) { return (a->get_fitness() < b->get_fitness()); }

void genome_operator::sort_gene_pool(vector <genome*> *head, int gene_pool_size)
{
    sort(head->begin(),head->end(),myCompair);
    
    /*for (int i = 0; i < gene_pool_size; i++)
    {
	printf("%d: %d\n",i,head->at(i)->get_fitness());
    }
    int indx;
    scanf("%d",&indx);*/
}


void genome_operator::pixelize(genome *some_genome)
{
    unsigned char indx = 0;
    unsigned char i,j;
    int x = some_genome->x;
    int y = some_genome->y;

    printf("Pixelize\n");

    if (some_genome->genome_ptr) 
    {
        for (i=0; i<y; i++) {
            for (j=0; j<x; j++){
                //printf(" some_genome->genome_ptr[%d][%d] = %d\n", i, j, (char) (* (some_genome->genome_ptr + (x*i) + j)));
                indx = 0;
                while(data[indx++].val != (char) (* (some_genome->genome_ptr + (x*i) + j)) && (indx <= MAX_CHAR) )
                {
                    ;
                }
                //printf("%02d ", indx-1);
                if (indx == MAX_CHAR)
                    printf("%c", data[MAX_CHAR -1].c);
                else
                    printf("%c", data[indx-1].c);

            }
            printf("\n");
        }
    }
}


void genome_operator::crossover_genepool(genome *head[], int gene_pool_size, float mutation_percent)
{
    for(int indx = 2; indx < gene_pool_size; indx +=  2)
    {
        /* Crossover parent1 & 2 and produce all the children */
        crossover(head[0], head[1],
                head[indx], head[indx+1]);

        head[indx]->mutate(mutation_percent);
        head[indx+1]->mutate(mutation_percent);
    }
}

void genome_operator::crossover_genepool(genome *head[], int gene_pool_size, int parent_pool_size, float mutation_percent)
{
    /* Multi Parent Crossover */
    crossover(head, gene_pool_size, parent_pool_size);

    for(int i=parent_pool_size; i<gene_pool_size; i++)
    {
    	head[i]->mutate(mutation_percent);
    }
}

void genome_operator::crossover_genepool(vector <genome*> *head, int gene_pool_size, int parent_pool_size, float mutation_percent)
{
    /* Multi Parent Crossover */
    crossover(head, gene_pool_size, parent_pool_size);

    for(int i=parent_pool_size; i<gene_pool_size; i++)
    {
    	head->at(i)->mutate(mutation_percent);
    }
}

/* Crossover with Two Parents */
void genome_operator::crossover(genome *parent_gen1,
                        genome *parent_gen2,
                        genome *child_gen1,
                        genome *child_gen2)
{
    int x=0,y=0,value,temp;
    int max_x = parent_gen1->x;
    int max_y = parent_gen1->y;
    vector <int> crossover_X;
    vector <int> crossover_Y;
    
    /* Push '0' as the first value for both X and Y */
    crossover_X.push_back(0); x++;
    crossover_Y.push_back(0); y++;
    
    /* Get the crossover limits */
    while(x < SEGMENT_X)
    {
	do
	{
	    value = rand() % max_x;
	}while(find(crossover_X.begin(), crossover_X.end(), value) != crossover_X.end());
	crossover_X.push_back(value);
	x++;
    }
    
    while(y < SEGMENT_Y)
    {
	do
	{
	    value = rand() % max_y;
	}while(find(crossover_Y.begin(), crossover_Y.end(), value) != crossover_Y.end());
	crossover_Y.push_back(value);
	y++;
    }

    /* Push max_x and max_y to X and Y respectively, as the last value */
    crossover_X.push_back(max_x);
    crossover_Y.push_back(max_y);

    /* Sort crossover points in X and Y */
    sort (crossover_X.begin(), crossover_X.end());
    sort (crossover_Y.begin(), crossover_Y.end());
    
    /* Performing crossover operation by copying alternate 
       segments of parent1 and parent2 to child1 and child2*/
    for(y = 0; y < SEGMENT_Y; y++)
    {
	for(x = 0; x < SEGMENT_X; x++)
	{
	    if(((y*SEGMENT_X)+x)%2 == 0)
	    {
	    	segmentSliceCopy_2D(parent_gen1,child_gen1,crossover_X[x],crossover_X[x+1],crossover_Y[y],crossover_Y[y+1]);
	    	segmentSliceCopy_2D(parent_gen2,child_gen2,crossover_X[x],crossover_X[x+1],crossover_Y[y],crossover_Y[y+1]);

	    }
	    else
	    {
	    	segmentSliceCopy_2D(parent_gen1,child_gen2,crossover_X[x],crossover_X[x+1],crossover_Y[y],crossover_Y[y+1]);
	    	segmentSliceCopy_2D(parent_gen2,child_gen1,crossover_X[x],crossover_X[x+1],crossover_Y[y],crossover_Y[y+1]);

	    }
	}
    }
}

/* Crossover with Multiple Parents */
void genome_operator::crossover(genome *head[], int gene_pool_size, int parent_pool_size)
{
    int x=0,y=0,value,temp;
    int max_x = head[0]->x;
    int max_y = head[0]->y;
    vector <int> crossover_X;
    vector <int> crossover_Y;
    
    /* Push '0' as the first value for both X and Y */
    crossover_X.push_back(0); x++;
    crossover_Y.push_back(0); y++;
    
    /* Get the crossover limits */
    while(x < SEGMENT_X)
    {
	do
	{
	    value = rand() % max_x;
	}while(find(crossover_X.begin(), crossover_X.end(), value) != crossover_X.end());
	crossover_X.push_back(value);
	x++;
    }
    
    while(y < SEGMENT_Y)
    {
	do
	{
	    value = rand() % max_y;
	}while(find(crossover_Y.begin(), crossover_Y.end(), value) != crossover_Y.end());
	crossover_Y.push_back(value);
	y++;
    }

    /* Push max_x and max_y to X and Y respectively, as the last value */
    crossover_X.push_back(max_x);
    crossover_Y.push_back(max_y);

    /* Sort crossover points in X and Y */
    sort (crossover_X.begin(), crossover_X.end());
    sort (crossover_Y.begin(), crossover_Y.end());
    
    /* Performing crossover operation by randomly selecting and copying a segment from one of the parent genome to child genome*/
    for(int i=parent_pool_size; i<gene_pool_size; i++) //Crossing over 0-parent_pool_size genome to replace the rest of the genome in the pool.
    {
	for(y = 0; y < SEGMENT_Y; y++)
    	{
	    for(x = 0; x < SEGMENT_X; x++)
	    {
		/* For each segment position of each child genome, randomly select a parent genome */
		segmentSliceCopy_2D(head[rand() % parent_pool_size],head[i],crossover_X[x],crossover_X[x+1],crossover_Y[y],crossover_Y[y+1]);
	    }
	}
    }
}

/* Crossover with Multiple Parents */
void genome_operator::crossover(vector <genome*> *head, int gene_pool_size, int parent_pool_size)
{
    int x=0,y=0,value,temp;
    int max_x = head->at(0)->x;
    int max_y = head->at(0)->y;
    vector <int> crossover_X;
    vector <int> crossover_Y;
    
    /* Push '0' as the first value for both X and Y */
    crossover_X.push_back(0); x++;
    crossover_Y.push_back(0); y++;
    
    /* Get the crossover limits */
    while(x < SEGMENT_X)
    {
	do
	{
	    value = rand() % max_x;
	}while(find(crossover_X.begin(), crossover_X.end(), value) != crossover_X.end());
	crossover_X.push_back(value);
	x++;
    }
    
    while(y < SEGMENT_Y)
    {
	do
	{
	    value = rand() % max_y;
	}while(find(crossover_Y.begin(), crossover_Y.end(), value) != crossover_Y.end());
	crossover_Y.push_back(value);
	y++;
    }

    /* Push max_x and max_y to X and Y respectively, as the last value */
    crossover_X.push_back(max_x);
    crossover_Y.push_back(max_y);

    /* Sort crossover points in X and Y */
    sort (crossover_X.begin(), crossover_X.end());
    sort (crossover_Y.begin(), crossover_Y.end());
    
    /* Performing crossover operation by randomly selecting and copying a segment from one of the parent genome to child genome*/
    for(int i=parent_pool_size; i<gene_pool_size; i++) //Crossing over 0-parent_pool_size genome to replace the rest of the genome in the pool.
    {
	for(y = 0; y < SEGMENT_Y; y++)
    	{
	    for(x = 0; x < SEGMENT_X; x++)
	    {
		/* For each segment position of each child genome, randomly select a parent genome */
		segmentSliceCopy_2D(head->at(rand() % parent_pool_size),head->at(i),crossover_X[x],crossover_X[x+1],crossover_Y[y],crossover_Y[y+1]);
	    }
	}
    }
}

void genome_operator::segmentSliceCopy_2D(genome *parent_gen,
                        genome *child_gen,
			int x1, int x2,
			int y1, int y2)
{
    int pixel_x,pixel_y;
    int max_x = parent_gen->x;

    for(pixel_y = y1; pixel_y < y2; pixel_y++)
    {
	for(pixel_x = x1; pixel_x < x2; pixel_x++)
	{
	    *(child_gen->genome_ptr + (pixel_y * max_x) + pixel_x) 
                = *(parent_gen->genome_ptr + (pixel_y * max_x) + pixel_x);
	}
    }
}

void genome_operator::backupGenePool(genome *head[], int gene_pool_size)
{
    ofstream genePoolOnFile[gene_pool_size];
    int max_x = head[0]->x; 
    int max_y = head[0]->y;
    for(int i=0;i<gene_pool_size;i++)
    {
	stringstream ss;
	ss << i+1 << ".gnm";
	string str(ss.str());
	genePoolOnFile[i].open(str.c_str());
	for(int y=0; y<max_y; y++)
	{
	    for(int x=0; x<max_x; x++)
	    {
		genePoolOnFile[i] << (int)*(head[i]->genome_ptr + (y * max_x) + x) << " ";
	    }
	}
	genePoolOnFile[i].close();
    }
}









