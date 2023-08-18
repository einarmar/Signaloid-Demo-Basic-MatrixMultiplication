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
#include <iostream>
#include <fstream>
#include <cstring>
#include <string_view>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <sstream>
#include <memory>
#include <unistd.h>

#if WITH_SIGNALOID
#include <uxhw.h>
#endif

using namespace std;

class MatrixMultiplier {
public:
    static constexpr size_t DefaultMatrixRows = 8;
    static constexpr size_t DefaultMatrixCols = 8;

    MatrixMultiplier(int argc, char* argv[]) {
        parseArguments(argc, argv);
        loadMatrices();
    }

    void run() {
        multiplyMatrices();
        printResult();
    }

private:
    size_t rowsA = DefaultMatrixRows;
    size_t colsA = DefaultMatrixCols;
    size_t rowsB = DefaultMatrixCols;
    size_t colsB = DefaultMatrixRows;

    string_view arrayAIdentifier = "gaussian/input-A";
    string_view arrayBIdentifier = "gaussian/input-B";

    vector<vector<double>> matrixA;
    vector<vector<double>> matrixB;
    vector<vector<double>> result;

    void parseArguments(int argc, char* argv[]) {
        int c;
        while ((c = getopt(argc, argv, "a:b:m:n:x:y:h")) != -1) {
            switch (c) {
                case 'a':
                    arrayAIdentifier = optarg;
                    break;
                case 'b':
                    arrayBIdentifier = optarg;
                    break;
                case 'm':
                    rowsA = parseInt(optarg, "Option '-m'");
                    if (rowsA < 1 || rowsA > 8) {
                        cerr << "Option -m should be between 1 and 8, not " << rowsA << endl;
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'n':
                    colsA = parseInt(optarg, "Option '-n'");
                    if (colsA < 1 || colsA > 8) {
                        cerr << "Option -n should be between 1 and 8, not " << colsA << endl;
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'x':
                    rowsB = parseInt(optarg, "Option '-x'");
                    if (rowsB < 1 || rowsB > 8) {
                        cerr << "Option -x should be between 1 and 8, not " << rowsB << endl;
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'y':
                    colsB = parseInt(optarg, "Option '-y'");
                    if (colsB < 1 || colsB > 8) {
                        cerr << "Option -y should be between 1 and 8, not " << colsB << endl;
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'h':
                    usage(cout);
                    exit(EXIT_SUCCESS);
                default:
                    cerr << "arguments malformed" << endl;
                    usage(cerr);
                    exit(EXIT_FAILURE);
            }
        }
    }

    void loadMatrices() {
        matrixA.resize(rowsA, vector<double>(colsA));
        matrixB.resize(rowsB, vector<double>(colsB));

        cout << "A =" << endl;
        cout.precision(6);
		for (size_t i = 0; i < rowsA; i++)
		{
			for (size_t j = 0; j < colsA; j++)
			{
				loadDoubleDistFromPathTemplate(matrixA[i][j], arrayAIdentifier, i, j);
                cout << matrixA[i][j] << '\t';
			}
            cout << endl;
		}

        cout << "B =" << endl;
        cout.precision(6);
		for (size_t i = 0; i < rowsB; i++)
		{
			for (size_t j = 0; j < colsB; j++)
			{
				loadDoubleDistFromPathTemplate(matrixB[i][j], arrayBIdentifier, i, j);
                cout << matrixB[i][j] << '\t';
			}
            cout << endl;
		}
    }

    void multiplyMatrices() {
        result.resize(rowsA, vector<double>(colsB, 0.0));

        for (size_t i = 0; i < rowsA; i++) {
            for (size_t j = 0; j < colsB; j++) {
                for (size_t k = 0; k < colsA; k++) {
                    result[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
    }


	void loadDoubleDistFromPathTemplate(double& var, const string_view& arg0, size_t arg1, size_t arg2) {
		stringstream pathTemplate;
		pathTemplate << arg0 << "-" << arg1 << "-" << arg2 << ".txt";
		loadDoubleDistFromPath(var, pathTemplate.str());
	}

	void loadDoubleDistFromPath(double& var, const string_view& filename) {
		ifstream inp(filename.data());
		int sampleCount;

		if (!inp) {
			cerr << "Error: could not open input file: " << filename << endl;
			exit(EXIT_FAILURE);
		}
		inp >> sampleCount;

		vector<double> samples(sampleCount);
		for (int i = 0; i < sampleCount; i++) {
			inp >> samples[i];
		}
#if WITH_SIGNALOID
    	var = UxHwDoubleDistFromSamples(samples.data(), sampleCount);
#else
        var = 0.2342323341; // dummy value
#endif
	}

    void printResult() {
        cout << "Result =" << endl;
        cout.precision(6);
        for (const auto& row : result) {
            for (const double& val : row) {
                cout << val << '\t';
            }
            cout << endl;
        }
    }

    int parseInt(const char* arg, const char* opt) {
        errno = 0;
        int i = static_cast<int>(strtol(arg, nullptr, 10));
        if (errno != 0) {
            perror(opt);
            exit(EXIT_FAILURE);
        }
        return i;
    }

    void usage(ostream& out) {
        out << "matrix-multiplication -a <file prefix for A> -m <A rows> -n <A columns> -b <file "
               "prefix for B> -x <B rows> -y <B columns>" << endl
            << "\t-a <file prefix for A>: default is \"gaussian/input-A\"" << endl
            << "\t-m <A rows>:            default is 8" << endl
            << "\t-n <A columns>:         default is 8" << endl
            << "\t-b <file prefix for B>: default is \"gaussian/input-B\"" << endl
            << "\t-x <B rows>:            default is 8" << endl
            << "\t-y <B columns>:         default is 8" << endl;
    }
};

int main(int argc, char* argv[]) {
    MatrixMultiplier multiplier(argc, argv);
    multiplier.run();

    return 0;
}
