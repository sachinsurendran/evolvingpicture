//#define IMG_FILE "avi.jpg"
//#define IMG_FILE "jolla.jpg"
#define IMG_FILE "doku.jpg"
//#define IMG_FILE "dracis.jpg"
//#define IMG_FILE "nursery.jpg"


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
image(const char *filename);
~image();
FILE *fp;
int open(const char *filename);
void close(FILE *fp);
};

image::image(const char *filename)
{
	fp = fopen(filename, "r");
}

image::~image()
{
	fclose(fp);
}

int image::open(const char *filename)
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

void image::close(FILE *fp)
{
	fclose(fp);
}
