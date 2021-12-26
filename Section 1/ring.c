#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, size, left, right, msgLeft, msgRight, tag, lastLeftTag, lastRightTag, count;
    double time, maxtime;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    time = MPI_Wtime();
    
    /* To get left and right neighbouring nodes of the current processor
to fix the case of 0 to the right of the last node and last node to the left of the 0 node */
    left = (rank + size - 1) % size;
    right = (rank + 1) % size;

    /*The assignment of the initial values of the messages*/
    msgRight = rank - 2 * rank;
    msgLeft = rank;
 
    tag = rank * 10;
    count = 0;

    /** The objective here is to:
	- Send to the left the message "msgLeft"
 	- Receive from the right the message "msgLeft"
     **/

    for (int i = 0; i < size; i++) {
        MPI_Send(&msgLeft, 1, MPI_INT, left, tag, MPI_COMM_WORLD);
        MPI_Recv(&msgLeft, 1, MPI_INT, right, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (size - 1 != i) {
            msgLeft = msgLeft + rank;
            lastLeftTag = status.MPI_TAG;
        }
        count++;
        

    /** The objective here is to:
        - Send to the right the message "msgRight"
        - Recieve from the left the message "msgRight"
     **/

        MPI_Send(&msgRight, 1, MPI_INT, right, tag, MPI_COMM_WORLD);
        MPI_Recv(&msgRight, 1, MPI_INT, left, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        if (size - 1 != i) {
            msgRight = msgRight - rank;
            lastRightTag = status.MPI_TAG;
        }
        count++;
    }

    printf("\n\n I am process %d and I have received %d messges. My final messages have tag (%d, %d) and value (%d, %d)\n", rank, count, lastRightTag, lastLeftTag, msgLeft, msgRight);

    MPI_Barrier(MPI_COMM_WORLD);
    time = MPI_Wtime() - time;

    // To find the maximum time among all the processors:

    MPI_Reduce(&time, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}