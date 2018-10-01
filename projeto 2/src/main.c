#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define gray_size 256
#define size_metrics 24
#define max_size_ilbp 512
#define size_random_set 25
#define grass_path "../DataSet/grass/grass_"
#define asphalt_path "../DataSet/asphalt/asphalt_"

int col = 0;
int row = 0;
int correct = 0;
int f_positive = 0;
int f_negative = 0;	

char *getFileFormat(char *, int , const char*);
int *getFile(char *);
int *calcILBP(int *, int, int);
int getLowestBin(int);
void Debug(int , int);
void average(double**, double**);
void declarationSet(double *, double *, double *);
void normalizedVector(double **, int);
void euclidianDistance(double *, double *, double **, int);
void *separateTest(double *);
void *separateTraining(double *);
double Max(double *, int);
double Min(double *, int);
double *glcmMatrix(int *, int, int, int);
double *glcmDirection(int *, int *, int, int, int);
double **getDescriptorFile(char *);

int main(int argc, char **argv){

	double **grass, *grass_test, *grass_training;

	grass = getDescriptorFile(grass_path);

	separateTraining(grass_training);

	double **asphalt, *asphalt_test, *asphalt_training;

	asphalt = getDescriptorFile(asphalt_path);

	printf("%lf", *grass_training);
	// grass_training = separateTraining(grass);

	// printf("%lf", *grass_training);
	// char *fp = getFileFormat(grass_path, 1, ".txt");

	// int *matrix = getFile(fp);

	// double * test = glcmMatrix(matrix, row, col, gray_size);
	//ilbp = calcILBP(matrix, row, col);

	//teste = getFileFormat();
	//separateGroup(test, train);
	return 0;
}

int *getFile(char *path)
{
	FILE *fp;

	fp = fopen(path, "r");

	if(fp == NULL)
	{
		printf("\n\nError: Fail to open file!\n\n");
		exit(1);
	}

	if(row == 0)
	{
		char ch;
		int count = 1;

		while(feof(fp) == 0)
		{
			ch = fgetc(fp);

			if(count && ch == ';')
				col++;

			if(ch == '\n')
			{
				row++;
				count = 0;
			}
		}
		col += 1;

		printf("\n\nlin: %d col: %d\n\n", row, col);
	}

	rewind(fp);

	int *matrix = (int *) calloc(row*col ,sizeof(int));// Give "free" later
	if (matrix == NULL) {
		printf("\nError: cannot alocate memory\n");
		exit(1);
	}

	for(int i = 0; i <= row; i++)
	{
		for(int j = 0; j <= col; j++)
			fscanf(fp, "%d;", matrix + i*col + j);

	}

	fclose(fp);

	return matrix;
}

char *getFileFormat(char *path, int id, const char *postfix){
	
	char *fileformat = (char *) calloc((strlen(path) + strlen(postfix) + 12), sizeof(char));

	if (fileformat == NULL) {
		printf("\nError: cannot alocate memory\n");
		exit(1);
	}
	
	char buffer[12] = { 0 };
	sprintf(buffer, "%02d", id);

	strcpy(fileformat, path);
	strcat(fileformat, buffer);       
	strcat(fileformat, postfix);

	printf("\nFilename: %s\n", fileformat);

	return fileformat;
}

int *calcILBP(int *matrix, int row, int col)
{
	// [2][3][7] 	s(x) >= 0 set (1), s(x) < 0 set(0)
	// [1][5][9]	ilbp(gc) = sum(0 -> 8) of s(gp - avg(gc))2^p, that gp = variant pixel
	// [8][4][6]
	//   			and avg(gc) = central pixel avarage - > (g0 + g1 + ... gc)/9
	// 		|
	//		|

	// ilbp describer:  (2 + 3 + 7 + 1 + 5 + 9 + 8 + 4 + 6)/9 = 5
	// s(x) - > [0][0][1]
	// 			[0][1][1] -> 001110100 -> unsigned
	// 			[1][0][0]

	// 	find the lowest bit in rotation of pixel
	// 	ex: 001001000 - > rotate -> 000100100 -> rotate -> 000010010 -> 000001001 = 9
	
	unsigned int lowestBin = 0;
	int binary[9] = {0};
	double avg = 0;
	int decimal = 0;
	
	int *ilbp = (int *) calloc(max_size_ilbp, sizeof(int)); //max size of ilbp is 2^9 - 1, that 9 is the amount pixel of submatrix;

	if (ilbp == NULL) {
		printf("\nError: cannot alocate memory\n");
		exit(1);
	}

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			// setting the initial value of matrix as center
			int submatrix[3][3];

			submatrix[0][0] = (i == 0 || j == 0) ? 0 : *(matrix + ((i - 1) * col) +(j - 1));
			submatrix[0][1] = (i == 0) ? 0 : *(matrix + ((i - 1) * col) + j);
			submatrix[0][2] = (i == 0 || j == (col - 1)) ? 0 : *(matrix + ((i - 1) * col) +(j + 1));
			submatrix[1][0] = (j == 0) ? 0 : *(matrix + (i * col) + (j - 1));
			submatrix[1][1] = *(matrix + (i * col) + j);
			submatrix[1][2] = (j == (col - 1)) ? 0 : *(matrix + (i * col) +(j + 1));
			submatrix[2][0] = (i == (row - 1) || j == 0) ? 0 : *(matrix + ((i + 1) * col) + (j - 1));
			submatrix[2][1] = (i == (row - 1) || j == 1) ? 0 : *(matrix + ((i + 1) * col) + j);
			submatrix[2][2] = (i == (row - 1) || j == (col - 1)) ? 0 : *(matrix + ((i + 1) * col) + (j + 1));
			

			for(int x = 0; x < 3; x++){
				for(int y = 0; y < 3; y++){
					avg += submatrix[x][y];
				}
			}

			avg = avg/9;

			binary[0] = (submatrix[0][0] >= avg) ? 1 : 0;
			binary[1] = (submatrix[0][1] >= avg) ? 1 : 0; 
			binary[2] = (submatrix[0][2] >= avg) ? 1 : 0; 
			binary[3] = (submatrix[1][0] >= avg) ? 1 : 0; 
			binary[4] = (submatrix[1][1] >= avg) ? 1 : 0; 
			binary[5] = (submatrix[1][2] >= avg) ? 1 : 0; 
			binary[6] = (submatrix[2][0] >= avg) ? 1 : 0; 
			binary[7] = (submatrix[2][1] >= avg) ? 1 : 0; 
			binary[8] = (submatrix[2][2] >= avg) ? 1 : 0;

			lowestBin = getLowestBin(*binary);

			ilbp[lowestBin] += 1; 
		}
	}
	//printf("%d\n", *ilbp);
	return ilbp;
}

int getLowestBin(int binary)
{
	// extracting the lowest binary
	int lowest = binary;
			
	for (int i = 0; i < 9; i++) {
		binary = (binary >> 1) | (binary << 8) & 512;
		
		if (binary < lowest){
			lowest = binary;
		}

	}
	return lowest;
}

double Max(double *concatenateVector, int size)
{
	size = size_random_set;
	double max = concatenateVector[0];

	for(int i = 0; i < size; i++) {
		if(*(concatenateVector + i) >= max){
			max = *(concatenateVector + i);
		}
	}

	return max;
}

double Min(double *concatenateVector, int size)
{
	size = size_random_set;
	double min = concatenateVector[0];
	
	for(int i = 0; i < size; i++) {
		if (*(concatenateVector + i) <= min){
			min = *(concatenateVector + i);
		}
	}
	return min; 
}

void normalizedVector(double **concatenateVector, int size)
{
	double vmax = Max(*concatenateVector, size);
	double vmin = Min(*concatenateVector, size);
	
	for(int i = 0; i < size; i++){
		*(*concatenateVector + i) = (*(*concatenateVector) - vmin) / (vmax - vmin);
	}
}

double *glcmDirection(int direction[2], int* matrix, int row, int col, int max_gray_level) {

	// 		1º to condition your REFERENCE DIRECTION: right, left, up, botton;
	// 		2º: Need to look, "What is the BIGGEST bit?; this bit "m" will be the size of the GLCM[m][m].
	// 		3º: "How many times does J happens on DIRECTION of I?",
	//
	//		Example:
	//		[1][2][4]	1º: the REFERENCE DIRECTION is right; 
	//  	[2][1][4]	2º: the size of the GLCM is 4, because [1][3] && [2][3] = 4, most BIGGEST bit;
	// 		[3][2][3]	3º: looking the GLCM position, you answer the question:
	// 					   	GLCM[1][1], How many times does j(1) happens on RIGHT of i(1)? = 0 -> GLCM[1][1] = 0;
	//						GLCM[1][2], How many times does j(2) happens on RIGHT of i(1)? = 2 => GLCM[1][2] = 1;
	//					 ...GLCM[4][4], How many times does j(4) happens on RIGHT of i(4)? = 0 => GLCM[4][4] = 0;
	//
	// 		Finally, GLCM is:
	// 		[0][1][0][1]
	//		[1][0][1][1]
	//		[0][1][0][0]
	//		[0][0][0][0]

	double energy = 0, contraste = 0, homog = 0;
	direction[0] -= 1;
	direction[1] -= 1;
	//Alocating memory for every direction
	double *glcm_matrix = (double*)calloc(max_gray_level * max_gray_level, sizeof(double)); // 2^8 = 256

	if(glcm_matrix == NULL){
		printf("\nError: cannot alocate memory\n");
		exit(1);
	}

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){

			// checking if matrix(x, y) == j and matrix(x, y on direction) == i
			// if != i (outside) continue;
			if(i + direction[0] < 0 || i + direction[0] >= row || 
			   j + direction[1] < 0 || j + direction[1] >= col)
			{
				continue;
			}

			// setting direction
			int n = matrix[(i * col) + j];
			int m = matrix[(i + direction[0]) * col + (j + direction[1])];

			glcm_matrix[(n * (max_gray_level-1)) + m] += 1;

		}
	}

	for(int i = 0; i < max_gray_level; i++){
   		for(int j = 0; j < max_gray_level; j++){
	      contraste += glcm_matrix[i * max_gray_level + j] * pow(abs(i - j),2);
		  energy += pow(glcm_matrix[i * max_gray_level + j], 2);
		  homog += (glcm_matrix[i * max_gray_level + j] /(1 + abs(i - j)));
	   }

	}
	double* metric = (double*)calloc(3, sizeof(double));

	metric[0] = contraste;
	metric[1] = energy;
	metric[2] = homog;

	free(glcm_matrix);
	return metric;
}

double *glcmMatrix(int *matrix, int row, int col, int max_gray_level) {
	double* glcm = (double*)calloc(24, sizeof(double));
	//We want to calculate glcm for every direction in a matrix.
	//We are putting x as a linear direction and y as a vertical direction,

	//Constant for putting all 24 metrics in GLCM;
	int m = 0;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(i == 1 && j == 1){
				continue;
			}
			int direction[2] = {i, j};

			double *metric = glcmDirection(direction, matrix, row, col, max_gray_level);

			for(int n = 0; n < 3; n++){
				glcm[m] = metric[n];
				m++;
			}
			
			free(metric);
		}
	}
	return glcm;

}

double **getDescriptorFile(char *datatype)
{
	char *path = (char *) calloc(255, sizeof(char));

	strcpy(path, datatype);

	if(path == NULL)
	{
		printf("\nError: cannot alocate memory!\n");
		exit(1);
	}

	double **img_describe = (double **) calloc(50, sizeof(double *));

	if(img_describe == NULL)
	{
		printf("\nError: cannot alocate memory!\n");
		exit(1);
	}

	for(int i = 1; i <= 50; i++)
	{
		char *filename = getFileFormat(path, i, ".txt");

		int *matrix = getFile(filename);

		int *ilbp = calcILBP(matrix, row, col);

		double *glcm = glcmMatrix(matrix, row, col, gray_size);

		double *describe = (double *) calloc(max_size_ilbp + size_metrics, sizeof(double));

		for(int j = 0; j < max_size_ilbp + size_metrics; j++)
		{
			if (j < max_size_ilbp)
			{
				describe[j] = (double)ilbp[j];
			}
			else
			{
				describe[j] = glcm[j - max_size_ilbp];
			}
		}

		normalizedVector(&describe, (max_size_ilbp + size_metrics));

		*img_describe = (describe + i);

		free(matrix);
		free(ilbp);
		free(glcm);
		free(filename);
	}

	return img_describe;
}

void *separateTest(double *test)
{

	int count, num;
	srand(time(NULL));


	//getting random files to test
	for(int i = 0; i <= 50; i++)
	{
		num = rand() % 50 + 1;
		count = 0;
		
		for(int j = 0; j <= 25; j++){
			if(*(test + i) == num)
				count++;
			else
			{
				continue;
			}
		}
		if (count > 0)
		{
			i--;
			continue;
		}
		*(test + i) = num;
	}
}

void *separateTraining(double *training){
	
	int count = 0, num;

	srand(time(NULL));

	for(int x = 0; x <= 50; x++)
	{

		num = rand() % 50 + 1;
		count = 0;

		for (int i = 0; i <= 25; i++)
		{
			if(*(training + x) == num)
				count++;
		}

		if (count > 0)
		{
			x--;
			continue;
		}

		*(training + x);
	} 
}


void average(double **training_set, double **concatenateVector){

	double average;
	for(int i = 0; i < 536; i++){
        for(int j = 0; j < 25; j++){
            
			average += *(*concatenateVector + i) + *(*(training_set + j) + i);
        }

    	average /= 25;
    }
}
void euclidianDistance(double *asphalt_descriptor, double* grass_descriptor, double** test_set, int base){
	//		Euclidian Distance:
	//		sqrt((x1-x0)^2+(y1-y0)^2
	// We need to calculate for the test set of images
	for(int i = 0; i < 25; i++){
		double distance_grass, distance_asphalt;
		double d_grass = 0, d_asphalt = 0;
		int result;
		
		for(int j = 0; j < 25; j++){
			d_grass += pow(grass_descriptor[i] - *(*(test_set + i) + j), 2);
			d_asphalt += pow(asphalt_descriptor[i] - *(*(test_set + i) + j), 2);
		}

		distance_asphalt = sqrt(d_asphalt);
		distance_grass = sqrt(d_grass);
	}
}

void Debug(int result, int base){
	if(result == base){
		correct++;
	}
	else{
		if(base == 1){
			f_positive++;
		}
		else{
			f_negative++;
		}
	}
}

void declarationSet(double *average_grass, double *average_asphalt, double *euclidianDistance){
	char metric[50] = {};
	double diference_average_grass;
	double diference_average_asphalt;
	double percentual_asphalt = 0, percentual_grass = 0;
	int total_grass = 0, total_asphalt = 0;
	
	for(int j = 0; j < 50; j++){
		diference_average_grass = 0;
		diference_average_asphalt = 0;
		diference_average_grass = euclidianDistance[j] - *average_grass;
		diference_average_asphalt = euclidianDistance[j] - *average_asphalt;

		if(diference_average_grass > diference_average_asphalt){
			metric[j] = 'g';
			percentual_grass++;
		}

		else if(diference_average_grass < diference_average_asphalt){
			metric[j] = 'a';
			percentual_asphalt++;
		}
	}

	printf("Número de Imagens Grama: %d\n", total_grass);
	printf("Percentual de Imagens Grama: %.2lf %% \n", percentual_grass = total_grass*100/50);

	printf("Número de Imagens Asfalto: %d\n", total_asphalt);
	printf("Percentual de Imagens Asfalto: %.2lf % \n", percentual_asphalt = total_asphalt*100/50);

	double total = percentual_asphalt + percentual_grass;

	for(int j=0;j<total;j++){
		
		if(metric[j]== 'g'){
			printf("Área segura para o pouso!\n");
		}
		
		if(metric[j]== 'a'){
			printf("Área não segura para pouso!\n");
		}
	}

}