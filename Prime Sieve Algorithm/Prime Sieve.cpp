#include <iostream>
#include <mpi.h>
#include <math.h>
#include <vector>
using namespace std;
#define Master 0

int main(int argc, char *argv[])
{
    int *result;
    int *resultP;
    int Proc_num, rank;
    long long int Size, Size_per_proc, SizeR;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &Proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == Master)
    {
        cout << "Please enter the lenght of vector:  \n";
        cin >> Size;
        SizeR= round(sqrt(Size));
        Size_per_proc = round(Size/Proc_num);
        if (Size%Size_per_proc != 0)
        {
            result = (int *)malloc(sizeof(int) * (Size+(Size%Size_per_proc)));
        } else {
           result = (int *)malloc(sizeof(int) * Size);   
        }
        
        
        
        resultP = (int *)malloc(sizeof(int) * Size_per_proc);
        // vector<int> result(Size, 1);
        for (int i = 1; i <= Size; i++)
        {
            result[i] = 1;
        }
        


        MPI_Bcast(&Size_per_proc, 1, MPI_LONG_LONG_INT, Master, MPI_COMM_WORLD);
        MPI_Bcast(&SizeR, 1, MPI_LONG_LONG_INT, Master, MPI_COMM_WORLD);
        MPI_Scatter(result, Size_per_proc, MPI_INT, resultP, Size_per_proc, MPI_INT, Master, MPI_COMM_WORLD);
        cout << "SizeR =" << SizeR;
        cout << "\n Size_per_proc =" << Size_per_proc;
        
        int p;
        int offset = (rank + 1) * Size_per_proc;
        //cout << "\n offset =" << offset;
        //cout << "\n execution = \n";
        for (int i = 2; i <= SizeR; i++)
        {
            for (int j = 2; i * j < ((rank + 1) * Size_per_proc); j++)
            {
                p = i * j;
                //cout << " i and j is = " << i << " , " << j << " ; ";
                //cout << "P is = " << p;
                p = p % Size_per_proc;
                //cout << " ; p changed to = " << p << "\n";
                resultP[p] = 0;
            }
        }
        /*for (int i = 2; i < Size; i++)
        {
            if (resultP[i] != 0)
            {
                cout << i << "-";
            }
        }*/
        cout << "\n";
        MPI_Gather(resultP, Size_per_proc, MPI_INT, result, Size_per_proc, MPI_INT, Master, MPI_COMM_WORLD);
        for (int i = 2; i < Size; i++)
        {
            if (result[i] != 0)
            {
                cout << i << "-";
            }
        }
    }
    else {

        MPI_Bcast(&Size_per_proc, 1, MPI_LONG_LONG_INT, Master, MPI_COMM_WORLD);
        MPI_Bcast(&SizeR, 1, MPI_LONG_LONG_INT, Master, MPI_COMM_WORLD);
        result = (int *)malloc(sizeof(int) * Size);
        resultP = (int *)malloc(sizeof(int) * Size_per_proc);
        MPI_Scatter(result, Size_per_proc, MPI_INT, resultP, Size_per_proc, MPI_INT, Master, MPI_COMM_WORLD);
        int p=0,x;
        int offset = (rank + 1) * Size_per_proc;
        int offset1 = (rank) * Size_per_proc;
        for (int i = 2; i <= SizeR; i++)
        {
            x=round(offset1/i);
            for (int j = x; i*j < offset; j++)
            {
                p = i * j;
                //cout << " i and j is = " << i << " , " << j << " ; ";
                //cout << "P is = " << p;
                p = p % Size_per_proc;
                //cout << " ; p changed to = " << p << "\n";
                resultP[p] = 0;
            }
        }
        /*for (int i = 0; i < Size_per_proc; i++)
        {
            resultP[i] = 0;
        }*/
        
        MPI_Gather(resultP, Size_per_proc, MPI_INT, result, Size_per_proc, MPI_INT, Master, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
