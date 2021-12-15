# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>

//compile command--->mpicc -o a2 a2.c

//run command---> mpirun -n <# of processor> ./a2 <array_size>

int main(int argc, char* argv[]){

	int arr_len,global_sum;
	arr_len = atoi(argv[1]);
	
	int rank, size; 
	MPI_Status status;
	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int inputarr[arr_len];
	
	/*//Initialize array with random values
	srand(time(0));
	for(int i=0; i<arr_len; i++){
		inputarr[i] = rand()%10 + 1; 
	}*/
	
	printf("Process %d Elements before: \n", rank);

	//Initialize array with random values
	srand(rank+time(0));
	for(int i=0; i<arr_len; i++){
		inputarr[i] = rand()%50;
		printf("%d ", inputarr[i]); 
	}
	printf("\n");
	
	//Buffer array to store results
	int res[arr_len]; 


	if(size==0){
    		MPI_Send(inputarr, arr_len, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		MPI_Recv(res, arr_len, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		}
  	else if(rank==size-1){
    		MPI_Send(inputarr, arr_len, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(res, arr_len, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		}
 	else{
    		MPI_Send(inputarr, arr_len, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		MPI_Recv(res, arr_len, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		}

	printf("Process %d Elements after: \n", rank);

	for(int i=0; i<arr_len; i++){
		printf("%d ", res[i]); 
	}
	printf("\n");
	
	MPI_Finalize(); 
	return 0;
}
