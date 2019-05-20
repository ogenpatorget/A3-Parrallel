#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MASTER 0

int main(int argc, char* argv[]) {
	if (argc!=3){
		printf("\n Usage %s input file, output file \n",argv[0]);
		return -1;
	}

	int rank,size,n,chunk;
	MPI_Status status;
	MPI_Datatype rowtype;
	MPI_Datatype coltype;
    MPI_Datatype matrixtype;



	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	/*Initialize the matrices*/
	double** A;
	double** B;
	double** C;
	int* send_counts;
	int* displs;

	/*Read the input file and create the matrices A and B*/
	if(rank==MASTER){
		FILE *input=fopen(argv[1],"r");
		fscanf(input,"%d",&n);

        A=(double **)malloc(n*sizeof(double*));
        B = (double **)malloc(n*sizeof(double*));
        C=(double **)malloc(n*sizeof(double*));
	    for(int i=0;i<n;i++){
            A[i] = (double*)malloc(n*sizeof(double));
            B[i] = (double*)malloc(n*sizeof(double));
            C[i] = (double*)malloc(n*sizeof(double));
		}
		
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                fscanf(input,"%lf",&A[i][j]);			
            }
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                fscanf(input,"%lf",&B[i][j]);			
            }
        }
            fclose(input);

	}
	
	MPI_Scatter(&n,1,MPI_INT,&n,1,MPI_INT,MASTER,MPI_COMM_WORLD);

	chunk  = n/size;
	double** block_A = (double **)malloc(n*sizeof(double*));	
	double** block_C = (double **)malloc(n*sizeof(double*));
	for(int i=0;i<n;i++){
		block_A[i] = (double*)malloc(chunk*sizeof(double));
		block_C[i] = (double*)malloc(chunk*sizeof(double));
		}

	if(rank!=MASTER){
		B = (double **)malloc(n*sizeof(double*));
		for(int i=0;i<n;i++){
			B[i] = (double*)malloc(n*sizeof(double));
		}
	}

	//MPI_Scatter(B,n*n,MPI_DOUBLE,B,n*n,MPI_DOUBLE,MASTER,MPI_COMM_WORLD);

   /* if(rank == 1){
        printf("B: %lf \n", B[0][0]);
    }*/
	/*Print the input matrices*/
	//if(rank==1){
	/*for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%lf ",A[i][j]);			
		}
			printf("\n ");	
	}*/
	/*		printf("\n ");	
			printf("\n ");	

	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%lf ",B[i][j]);			
		}
			printf("\n ");	
	}*/


//	}


	/*partition A row wise and B column wise*/	
	MPI_Type_vector(chunk,n,n, MPI_DOUBLE, &rowtype);
	MPI_Type_vector(n,chunk,n, MPI_DOUBLE, &coltype);
    MPI_Type_vector(n,n,1,MPI_DOUBLE, &matrixtype);
	MPI_Type_commit(&rowtype);
	MPI_Type_commit(&coltype); 
    MPI_Type_commit(&matrixtype); 



	if(rank==MASTER){
		for(int i=1; i<size;i++){
			MPI_Send(&A[i*chunk][0], 1,rowtype,i, 111,MPI_COMM_WORLD);
            MPI_Send(&B[0][0],1,matrixtype,i,333,MPI_COMM_WORLD);
    	}
        
        

        /*for (int i=0;i<chunk; i++) {
            for (int j=0;j<n;j++) {
                for(int k=0;k<n;k++){
                    C[i][j]+= A[i][k]*B[k][j];
                }
            }
        }*/

        C[0][0]=19;
        C[0][1]=22;


        for(int i=1;i<size;i++){
            MPI_Recv(&C[i*chunk][0],1,rowtype, i, 222, MPI_COMM_WORLD, &status);
        }
	}
    else{
        MPI_Recv(&block_A[0][0],1,rowtype, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[0][0],1,matrixtype, 0, 333, MPI_COMM_WORLD, &status);
        
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                printf("%lf ",B[i][j]);			
            }
            printf("\n");
        }

        //printf("first: %lf second: %lf \n", block_A[0][0], block_A[0][1]);
        for (int i=0; i<chunk; i++) {
            for (int j=0;j<n;j++) {
                for(int k=0;k<n;k++){
                    printf("Test print %lf \n", B[k][j]);
                    block_C[i][j] += block_A[i][k]*B[k][j];
                    
                }
            }
        }
        //block_C[0][0]=43;
        //block_C[0][1]=50;

	    MPI_Send(&block_C[0][0], 1,rowtype,0, 222,MPI_COMM_WORLD);	

	}
                      
	if(rank==MASTER){
        printf("\n ");
        printf("\n ");
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                printf("%lf ",C[i][j]);			
            }
                printf("\n ");	
        }
	}




	MPI_Finalize();
	return 0;
}