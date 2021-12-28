#include <iostream>
#include <string>
#include "mpi.h"

int main(int argc, char **argv) {
    int rank, size;
    double time, maxtime, mintime, avgtime;
    MPI_Status status;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

/*to define the dimensions of matrices and submatrices
 std::stoi >> string to int*/ 
    int x = std::stoi(argv[1]); 
    int y = std::stoi(argv[2]);
    int z = std::stoi(argv[3]);

    int x_submatrix = std::stoi(argv[4]);
    int y_submatrix = std::stoi(argv[5]);
    int z_submatrix = std::stoi(argv[6]);

    /*allocating the matrices dynamically*/
    double* matrix_1 = new double[x * y * z];
    double* matrix_2 = new double[x * y * z];
    double* matrix_3 = new double[x * y * z];
    double* matrix_1_submatrix = new double[x_submatrix * y_submatrix * z_submatrix];
    double* matrix_2_submatrix = new double[x_submatrix * y_submatrix * z_submatrix];
    double* matrix_3_submatrix = new double[x_submatrix * y_submatrix * z_submatrix];

    /*number of elements oin the submatrix*/
    int elements_submatrix= x_submatrix * y_submatrix * z_submatrix;

    /*to create the 3D matrix values by a random generator*/
    if (rank == 0) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                for (int k = 0; k < z; k++) {
                    *(matrix_1 + i * y * z + j * z + k) = (double)rand() / RAND_MAX * 100;
                    *(matrix_2 + i * y * z + j * z + k) = (double)rand() / RAND_MAX * 100;
                }
            }
        }
    }
/*to synchronize all the processors to test time*/
    MPI_Barrier(MPI_COMM_WORLD);
    time = MPI_Wtime();
/*to distribute the submatrix1 and submatrix 2 to the processors*/
    MPI_Scatter(matrix_1, elements_submatrix, MPI_DOUBLE, matrix_1_submatrix, elements_submatrix, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix_2, elements_submatrix, MPI_DOUBLE, matrix_2_submatrix, elements_submatrix, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /*to add the 2 submatrix*/
    for (int i = 0; i < x_submatrix; i++) {
        for (int j = 0; j < y_submatrix; j++) {
            for (int k = 0; k < z_submatrix; k++) {
                *(matrix_3_submatrix + i * y_submatrix * z_submatrix + j * z_submatrix + k) = *(matrix_1_submatrix + i * y_submatrix * z_submatrix + j * z_submatrix + k) + *(matrix_2_submatrix + i * y_submatrix * z_submatrix + j * z_submatrix + k);
            }
        }
    }
    /*to collect the added values*/
    MPI_Gather(matrix_3_submatrix; , elements_submatrix; , MPI_DOUBLE, matrix_3, elements_submatrix, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    time = MPI_Wtime() - time;

    /*to collect the time obtained*/
    MPI_Reduce(&time, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time, &mintime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
     
    /*to check if the values calculated above are correct*/
    if (rank == 0) {
        float sum1 = 0;
        float sum2 = 0;
        float sum3 = 0;
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                for (int k = 0; k < z; k++) {
                    sum1 = sum1 + *(matrix_1 + i * y * z + j * z + k);
                    sum2 = sum2 + *(matrix_2 + i * y * z + j * z + k);
                    sum3 = sum3 + *(matrix_3 + i * y * z + j * z + k);
                }
            }
        }
        avgtime = avgtime / size;
        std::cout << "[" << x << ", " << y << ", " << z << "] ---> " << 24 << "*[" << x_submatrix << ", " << y_submatrix << ", " << z_submatrix << "]" << std::endl;
        std::cout << "MAX: " << maxtime << " MIN: " << mintime << " AVG: " << avgtime << std::endl;
        std::cout << "SUM M1: " << sum1 << "\nSUM M2: " << sum2 << "\nSUM M1 + M2: " << sum3 << std::endl;
    }
    /*to free up the memory allocaated on heap*/
    delete[] matrix_1;
    delete[] matrix_2;
    delete[] matrix_3;
    delete[] matrix_1_submatrix;
    delete[] matrix_2_submatrix;;
    delete[] matrix_3_submatrix;;

    MPI_Finalize();

    return 0;
}