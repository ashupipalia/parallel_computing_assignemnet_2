# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <unistd.h>

/*
compile command-mpicc -o a7 a7.c


run command- mpirun -n 2 ./a7

*/



/*
working-
iittp_barrier-
concept of Tree Barrier to achieve High degree of parallelism. 
This application assumes that the total number of nodes will a power of 2 (like 2, 4 , 8 etc.).
assume there is total 4 processes and then n0,n1,n2,n3 
then n3 ---> n1 and n2 ---> n0 and n1 ---> n0 after this 
step process zero knows that all the process reached the 
barrier then process n0 broadcast to all other nodes.
*/

void iittp_barrier(int pid,int np){

	double temp = log(np)/log(2);
	int value=temp;
	int k=value;
	int i=0;
	int j=0; 
	int it=0;
	for(i=k-1;i>=0;i--)
	{
		it=2-i;
		for(j=pow(2,i);j<pow(2,i+1);j++)
		{		 
			if(pid==j){
				MPI_Send(&it,1,MPI_INT,j-(int)pow(2,i),it,MPI_COMM_WORLD);
			}
			if(pid== j-(int)pow(2,i) ){
				MPI_Status status;
				int data;
				MPI_Recv(&data,1,MPI_INT,j,it,MPI_COMM_WORLD,&status);
			}	 
		 }
	}
	
	i=0;
	j=0;
	it=0;

	for(i=0;i<=k-1;i++)
	{
		 it=i;
		 for(j=0;j<=pow(2,i)-1;j++)
		 {
			if(pid==j){
			 	MPI_Send(&it,1,MPI_INT,j+(int)pow(2,i),it,MPI_COMM_WORLD);
			}
			if(pid==j+(int)pow(2,i) )
			{
				MPI_Status status;
				int temp1;
				MPI_Recv(&temp1,1,MPI_INT,j,it,MPI_COMM_WORLD,&status);
				printf("\nreceiving message from %d to %d and message is %d \n",j,j+(int)pow(2,i),temp1);
			}	 
		 }
	}
 }

int main(int argc, char *argv[]){
 
 	int pid, np;
 	MPI_Init (&argc, &argv);
 	MPI_Comm_rank (MPI_COMM_WORLD, &pid);
 	MPI_Comm_size (MPI_COMM_WORLD, &np); 
 	
	printf( "Before Barrier, process no %d \n", pid);
    	sleep(3);
    	
    	iittp_barrier(pid,np);
	sleep(1);
	printf( "After Barrier, process no %d \n", pid);
	
	MPI_Finalize();
	return 0;
}


