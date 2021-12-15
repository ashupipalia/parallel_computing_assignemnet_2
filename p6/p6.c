# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>

int main(int argc, char* argv[]){

	int arr_len;
	arr_len = atoi(argv[1]);
	int inputarr[arr_len];
	int rec[arr_len];
	int it = atoi(argv[2]);

	int size,rank;
	
	srand(time(0));
	for(int i=0; i<arr_len; i++){
		inputarr[i] = rand()%10 + 1; 
		printf("P%d[%d]: %d\n", rank, size, inputarr[i]); 
	}
	
	
	MPI_Status status;
	MPI_Request request;
	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//Initialize array with random values
	
	if(rank==0){
		int token= rand()%size;
		MPI_Bcast(&token, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank!=0){
		MPI_Recv(&token, arr_len, MPI_INT,MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}


	while(it!=0){
	
		int next =0;	
		
			if (rank != 0) {
			    MPI_Recv(&buff_arr, arr_len, MPI_INT,, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			    printf("Process %d received token %d from process %d\n",rank, arr_data, token);
		} 
		else {
	    		// Set the token's value if you are process 0
	    		token = -1;
		}
		MPI_Send(&inputarr, arr_len, MPI_INT, token,0, MPI_COMM_WORLD);

		// Now process 0 can receive from the last process.
		if (world_rank == 0) {
		   	MPI_Recv(&token, 1, MPI_INT, size - 1, 0,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    	printf("Process %d received token %d from process %d\n",rank, token, size - 1);
		}
		it--;
	}
	
	MPI_Finalize();
	return 0;
}
