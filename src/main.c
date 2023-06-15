/*
 *	Authored 2020, James Timothy Meech.
 *	Authored 2021, Orestis Kaparounakis.
 *	Authored 2022, Damien Zufferey.
 *
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *	*	Redistributions of source code must retain the above
 *		copyright notice, this list of conditions and the following
 *		disclaimer.
 *	*	Redistributions in binary form must reproduce the above
 *		copyright notice, this list of conditions and the following
 *		disclaimer in the documentation and/or other materials
 *		provided with the distribution.
 *	*	Neither the name of the author nor the names of its
 *		contributors may be used to endorse or promote products
 *		derived from this software without specific prior written
 *		permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	POSSIBILITY OF SUCH DAMAGE.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uxhw.h>
#include <unistd.h>

typedef enum
{
	kBenchmarkDefaultMatrixRows = 8,
	kBenchmarkDefaultMatrixCols = 8,
} BenchmarkDefaults;

double loadDoubleDistFromPath(double * var, const char * filename);
void   printMatrix(
	  double m[kBenchmarkDefaultMatrixRows][kBenchmarkDefaultMatrixCols],
	  size_t rows,
	  size_t cols);
void loadDoubleDistFromPathTemplate(double * var, const char * arg0, size_t arg1, size_t arg2);
int  parseInt(char * arg, const char * opt);
void usage(FILE *);

int
main(int argc, char *  argv[])
{
	size_t rowsA = kBenchmarkDefaultMatrixRows;
	size_t colsA = kBenchmarkDefaultMatrixCols;
	size_t rowsB = kBenchmarkDefaultMatrixCols;
	size_t colsB = kBenchmarkDefaultMatrixRows;

	size_t identifierLength = 0;
	char * arrayAIdentifier = "gaussian/input-A";
	char * arrayBIdentifier = "gaussian/input-B";

	double matrixA[kBenchmarkDefaultMatrixRows][kBenchmarkDefaultMatrixCols];
	double matrixB[kBenchmarkDefaultMatrixRows][kBenchmarkDefaultMatrixCols];
	double result[kBenchmarkDefaultMatrixRows][kBenchmarkDefaultMatrixCols];

	int c;

	while ((c = getopt(argc, argv, "a:b:m:n:x:y:h")) != -1)
	{
		switch (c)
		{
		case 'a':
			identifierLength = strlen(optarg);
			arrayAIdentifier = (char *)malloc(identifierLength + 1);
			if (arrayAIdentifier == NULL)
			{
				fprintf(stderr, "memory allocation failed.\n");
				exit(EXIT_FAILURE);
			}
			strncpy(arrayAIdentifier, optarg, identifierLength + 1);
			break;
		case 'b':
			identifierLength = strlen(optarg);
			arrayBIdentifier = (char *)malloc(identifierLength + 1);
			if (arrayAIdentifier == NULL)
			{
				fprintf(stderr, "memory allocation failed.\n");
				exit(EXIT_FAILURE);
			}
			strncpy(arrayBIdentifier, optarg, identifierLength + 1);
			break;
		case 'm':
			rowsA = parseInt(optarg, "Option '-m'");
			if (rowsA < 1 || rowsA > 8)
			{
				fprintf(stderr, "Option -m should be between 1 and 8, not %u\n", rowsA);
				exit(EXIT_FAILURE);
			}
			break;
		case 'n':
			colsA = parseInt(optarg, "Option '-n'");
			if (colsA < 1 || colsA > 8)
			{
				fprintf(stderr, "Option -n should be between 1 and 8, not %u\n", colsA);
				exit(EXIT_FAILURE);
			}
			break;
		case 'x':
			rowsB = parseInt(optarg, "Option '-x'");
			if (rowsB < 1 || rowsB > 8)
			{
				fprintf(stderr, "Option -x should be between 1 and 8, not %u\n", rowsB);
				exit(EXIT_FAILURE);
			}
			break;
		case 'y':
			colsB = parseInt(optarg, "Option '-y'");
			if (colsB < 1 || colsB > 8)
			{
				fprintf(stderr, "Option -y should be between 1 and 8, not %u\n", colsB);
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			usage(stdout);
			exit(EXIT_SUCCESS);
		default:
			fprintf(stderr, "arguments malformed\n");
			usage(stderr);
			exit(EXIT_FAILURE);
		}
	}

	if (colsA != rowsB)
	{
		fprintf(stderr, "number of columns in A must match the rows of B.\n");
		exit(EXIT_FAILURE);
	}

	/*
	 *	Load matrix A
	 */
	for (size_t i = 0; i < rowsA; i++)
	{
		for (size_t j = 0; j < colsA; j++)
		{
			loadDoubleDistFromPathTemplate(&matrixA[i][j], arrayAIdentifier, i, j);
		}
	}

	/*
	 *	Load matrix B
	 */
	for (size_t i = 0; i < rowsB; i++)
	{
		for (size_t j = 0; j < colsB; j++)
		{
			loadDoubleDistFromPathTemplate(&matrixB[i][j], arrayBIdentifier, i, j);
		}
	}

	printf("A = \n");
	printMatrix(matrixA, rowsA, colsA);
	printf("B = \n");
	printMatrix(matrixB, rowsA, colsB);

	for (size_t i = 0; i < rowsA; i++)
	{
		for (size_t j = 0; j < colsB; j++)
		{
			result[i][j] = 0;
		}
	}

	for (size_t i = 0; i < rowsA; i++)
	{
		for (size_t j = 0; j < colsB; j++)
		{
			for (size_t k = 0; k < colsA; k++)
			{
				result[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}

	printf("result =\n");
	printMatrix(result, rowsA, colsB);

	return 0;
}

int
parseInt(char * arg, const char * opt)
{
	errno = 0;
	int i = (int)strtol(arg, NULL, 10);
	if (errno != 0)
	{
		perror(opt);
		exit(EXIT_FAILURE);
	}
	return i;
}

void
printMatrix(double m[kBenchmarkDefaultMatrixRows][kBenchmarkDefaultMatrixCols], size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			printf("%lf", m[i][j]);
			if (j != cols - 1)
			{
				printf("\t");
			}
		}
		printf("\n");
	}
}

void
loadDoubleDistFromPathTemplate(double * var, const char * arg0, size_t arg1, size_t arg2)
{
	const char * pathTemplate = "%s-%d-%d.txt";
	char *       pathFilled;
	size_t       needed = 0;

	needed = snprintf(NULL, 0, pathTemplate, arg0, arg1, arg2) + 1;
	pathFilled = malloc(needed);
	if (pathFilled == NULL)
	{
		fprintf(stderr, "Error: could not allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	snprintf(pathFilled, needed, pathTemplate, arg0, arg1, arg2);

	loadDoubleDistFromPath(var, pathFilled);

	free(pathFilled);
	return;
}

double
loadDoubleDistFromPath(double *var, const char *filename)
{
	FILE *inp;
	int sampleCount;
	double *samples;
	double returnValue;

	inp = fopen(filename, "r");
	if (inp == NULL)
	{
		fprintf(stderr, "Error: could not open input file: %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fscanf(inp, "%d\n", &sampleCount);

	printf("Number of samples from %s: %d\n", filename, sampleCount);

	samples = calloc(sampleCount, sizeof(double));
	if (samples == NULL)
	{
		fprintf(stderr, "Error: could not allocate memory for samples.\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < sampleCount; i++)
	{
		fscanf(inp, "%lf\n", &samples[i]);
	}

	if (var == NULL)
	{
		returnValue = UxHwDoubleDistFromSamples(samples, sampleCount);
	}
	else
	{
		(*var) = UxHwDoubleDistFromSamples(samples, sampleCount);
	}

	if (fclose(inp) != 0)
	{
		fprintf(stderr, "Warning: could not close input file.\n");
	}
	free(samples);

	return returnValue;
}

void
usage(FILE * out)
{
	fprintf(out,
	        "matrix-multiplication -a <file prefix for A> -m <A rows> -n <A columns> -b <file "
	        "prefix for B> -x <B rows> -y <B columns>\n"
	        "\t-a <file prefix for A>: default is \"gaussian/input-A\"\n"
	        "\t-m <A rows>:            default is 8\n"
	        "\t-n <A columns>:         default is 8\n"
	        "\t-b <file prefix for B>: default is \"gaussian/input-B\"\n"
	        "\t-x <B rows>:            default is 8\n"
	        "\t-y <B columns>:         default is 8\n");
}
