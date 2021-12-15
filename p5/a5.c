# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>


/*
compile command-mpicc -o a5 a5.c


run command- mpirun -n 2 ./a5 <array_size>

*/

int main(int argc, char* argv[]){

	int arr_len,global_sum;
	arr_len = atoi(argv[1]);
	int vector1[arr_len];
	int vector2[arr_len];
	
	//Initialize array with random values
	srand(time(0));
	for(int i=0; i<arr_len; i++){
		vector1[i]=rand()%10+1;
		vector2[i]=rand()%10+1;
	}

	int size,rank;
	MPI_Status status;
	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//equally divided
	int d_size = arr_len/size;
	int start = rank*d_size;
	int end;
	if(rank<size-1){
		end = ((rank+1)*d_size);
	}
	else{
		end = arr_len;
	}
	
	//int end = (rank+1)*d_size;
	long local_var = 0;
	
	//determine local sum
	for(int i=start; i<end; i++){
		local_var += vector1[i]*vector2[i];
		//printf("%d, %d\n",vector1[i],vector2[i]); 
		
	}
	
	//send local sum to master process then it will add into the master variable
	if(rank!=0){
		MPI_Send(&local_var, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);								
	}
	else{
		long temp1 = local_var;
		for(int i=1; i<size; i++){
			long temp;			
			MPI_Recv(&temp, 1, MPI_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			temp1 += temp;
		}
		printf("Total Sum: %ld\n", temp1);	
	}

	MPI_Finalize();
	return 0;
}
