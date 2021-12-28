# Section 1 : Compile-Run guide

These are the command lines needed to compile and run the 2 codes provided in this section on OpenMPI:

## Code 1: ring.c

This code implements in C an MPI program using P processors on a ring. 

1- Use the following commands to load the onenmpi module once in the correct directory:
```
module load openmpi-4.1.1+gnu-9.3.0
```

2- Use the following command to compile this C code:
```
mpicc ./ring.c -o ring.x
```

3- Use the following command to run the code:
```
mpirun -np P ./ring.x
```
    
#Note:


P= number of processors, which has been varied in order to compare performances for different number of processors


## Code 2: matrix.cpp

This code implements in C++ a simple 3D matrix-matrix addition in parallel using a 1D, 2D and 3D distribution of data using virtual topology. 24 processors was used in all cases.

1- Use the following commands to load the openMPI module once you are in the corect directory:
```
module load openmpi-4.1.1+gnu-9.3.0
```

2- Use the following command to compile this C code:
```
mpicpp ./matrix.cpp -o matrix.x
```

3- Use the following command to run the code :
#Generally: 
```
mpirun -np 24 ./matrix.x "X" "Y" "Z" "x" "y" "z"
```

#Note: 

Different matrices with different sizes were tested
For example:

```
mpirun -np 24 ./matrix.x "2400" "100" "100" "100" "100" "100"
mpirun -np 24 ./matrix.x "1200" "200" "100" "100" "200" "50"
mpirun -np 24 ./matrix.x "800" "300" "100" "400" "100" "25"
```
 Where:
 
 - X: x-axis size of the big matrix
 - Y: y-axis size of the big matrix
 - Z: z-axis size of the big matrix
 - x: x-axis size of the submatrix
 - y: y-axis size of the submatrix
 - z: z-axis size of the submatrix

 
