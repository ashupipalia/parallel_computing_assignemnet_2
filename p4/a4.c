# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>

/*compile command-mpicc -o a4 a4.c

run command- mpirun -n <# of proccesor> ./a4 <array_size> <option number> <buffersize>

option=1 block distribution
option=2 cyclic distribution
option=3 block cyclic distribution
buffer size for option=3*/

int main(int argc, char* argv[]){

	int arr_len,global_sum,option;
	arr_len = atoi(argv[1]);
	option = atoi(argv[2]);
	int buffer_size;
	
	int inputarr[arr_len];
	
	int size,rank;
	MPI_Status status;
	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int local_sum=0;
	if(option==1){//block distribution
		int d_size = arr_len/size;
		int start = rank*d_size;
		int end;
		if(rank<size-1){
			end = ((rank+1)*d_size)-1;
		}
		else{
			end = arr_len-1;
		}
		int array[d_size];
		int counter = end-start+1;
		for(int i=0; i<counter; i++){
			array[i] = start+i;
			//printf("%d \n",array[i]);
			local_sum += array[i];
		}
	}
	else if(option==2){//cyclic distribution
		int d_size = arr_len/size;
		int array[d_size];
		int j=0;
		for(int i=rank;i<arr_len;i+=size){
			array[j] = i;
			//printf("%d \n",array[j]);
			local_sum += array[j++];
		}
		
	}
	else if(option==3){//block cyclic distribution
		buffer_size = atoi(argv[3]);
		int d_size = arr_len/size;
		int array[d_size];
		int counter=0;
		
		for(int i=rank*buffer_size; i<arr_len; i+=size*buffer_size){
			for(int j=0;j<buffer_size;j++){
				array[counter] = i+j;
				local_sum += array[counter]; 	
				counter++;
			}
		}
	}
	//send local sum to master process then it will add into the master variable
	if(rank!=0){
		MPI_Send(&local_sum, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);							
	}
	else{
		long temp1 = local_sum;
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
