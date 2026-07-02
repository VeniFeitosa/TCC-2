#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include<sys/stat.h>
#include<unistd.h>
#include <windows.h>
#include <direct.h>
#include<vector>

using namespace std;

#define WHITE -1
#define GRAY  0
#define BLACK  1
#define RED  2
#define GREEN  3

std::vector < int > mark; // For Deep First Search

void dfs(int *y, long i, int ** matrix, long n) {
  for (long j=0;j<n;j++) {
        if(i!=j && y[j]>0 && matrix[i][j] && mark[j] == WHITE) {
            mark[j] = GRAY;
            dfs(y,j,matrix,n);
        }
   }
   mark[i] = BLACK;
}

bool isConnected(int *y, int ** matrix, long n) {
    mark.clear();
    for (long i=0;i<n;i++)
        mark.push_back(WHITE);
    for (long i=0;i<n;i++) {  /* there is no vertex with GRAY color */
        if(y[i] > 0) {
            if (mark[i] == BLACK) continue;
            mark[i] = GRAY;
            dfs(y,i,matrix,n);
            break;   // You only need to take an initial vertex to know if the subgraph is connected
        }
    }
    for (long i=0;i<n;i++)
        if(y[i]>0 && mark[i] != BLACK) return false;

    return true;
}

int createGraphFile(char *instanceTypeName, long n, int density, int connected) {
	char linestr[4000], outputFileName[1000];
	int go=0, cont=0, **matrix = NULL;
	long i=0,j=0,k=0;
	long long cont_m=0, m=0;
	fstream outputFile;

    matrix = (int **) malloc(n*sizeof(int*));
    for(i=0;i<n;i++) matrix[i] = (int *) malloc(n*sizeof(int));

    srand( (unsigned)time(NULL) );
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            matrix[i][j] = 0;

    m = ceil(( (double) (density*n*(n-1)) )/ 200);

    int *y = (int *) malloc(n*sizeof(int));
    for(long i=0;i<n;i++) y[i] = 1;

    go=0;
    while(!go) {
        while(cont_m < m) {
            i = (rand()%n);
            j = (rand()%n);
            if( i!=j && matrix[i][j]!=1 ) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
                cont_m++;
            }
        }
        if( connected && !isConnected(y,matrix,n) ) {
            go=0;
            cont_m=0;
            for(i=0;i<n;i++)
                for(j=0;j<n;j++)
                    matrix[i][j] = 0;
        } else go=1;
    }
    free(y);

    // Generating by edge probability (p=density)
    /* for(i=0;i<n;i++) {
        matrix[i][i] = 0;
        for(j=i+1;j<n;j++) {
            cont = (rand()%100) + 1;
            if(cont <= density) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
                m++;
            } else {
                    matrix[i][j] = 0;
                    matrix[j][i] = 0;
            }
        }
    } */

    double dens = 200*( m/ (double) (n*(n-1)));
    printf("m= %lld, Density: %d, Real Density: %.2lf\n", m,density,dens);

    cont = 1; go=0;
    while(!go) {
		sprintf(outputFileName,"%s-n%ld-d%.0lf-%d.col",instanceTypeName,n,dens,cont);
		int errcode = access(outputFileName, F_OK);
		if (errcode != 0) go=1;     // file doesnt exist
		else cont++;                // file already exist
	}
    outputFile.open(outputFileName, ios::in | ios::out | ios::trunc);
    outputFile << "c FILE: " << outputFileName << endl << endl;
    outputFile << "p edge " << n << " " << m;
    for(i=0;i<n;i++)
        for(j=i;j<n;j++)
            if( matrix[i][j] ) outputFile << endl << "e " << i+1 << " " << j+1;

    outputFile.close();
    for(i=0;i<n;i++) free(matrix[i]);
    free(matrix);

	return 0;
}

void generateBatch(const char *folder, long n, int density, int connected, int count = 10)
{
    struct stat st;
    if (stat(folder, &st) != 0)
    {
        _mkdir(folder);
        printf("Pasta criada: %s\n", folder);
    }

    for (int i = 0; i < count; i++)
    {
        char instanceName[1000];
        sprintf(instanceName, "%s/random", folder);

        printf("[%d/%d] Gerando grafo n=%ld d=%d...\n", i + 1, count, n, density);
        createGraphFile(instanceName, n, density, connected);

        if (i < count - 1)
            Sleep(1430);
    }

    printf("Concluido: %d grafos gerados em '%s'\n", count, folder);
}

// int main (int argc, char *argv[]) {
//     int d=0,connected=0;
//     long n=0;

// 	if (argc < 5) {
// 		printf("Please, specify the instance type name, number of vertices, density and if the graph need to be connected (1 or 0).\n");
// 		return 0;
// 	}
//     for(int i=0; argv[2][i] != '\0'; i++) {
//         n= n*10;
//         n+= argv[2][i] - '0';
//     }
//     for(int i=0; argv[3][i] != '\0'; i++) {
//         d= d*10;
//         d+= argv[3][i] - '0';
//     }
//     for(int i=0; argv[4][i] != '\0'; i++) {
//         connected= connected*10;
//         connected+= argv[4][i] - '0';
//     }
//     //printf("n= %ld, d= %d\n",n,d);
// 	createGraphFile(argv[1],n,d,connected);
// 	return 0;
// }

int main(int argc, char *argv[]) {
    if (argc >= 2 && strcmp(argv[1], "batch") == 0)
    {
        if (argc < 6)
        {
            printf("Uso batch: ./GenerateRandomGraph batch <folder> <n> <density> <connected> [count]\n");
            printf("Exemplo:   ./GenerateRandomGraph batch n100-d20 100 20 1\n");
            printf("Exemplo:   ./GenerateRandomGraph batch n100-d20 100 20 1 5\n");
            return 0;
        }

        char *folder = argv[2];
        long n = atol(argv[3]);
        int d = atoi(argv[4]);
        int connected = atoi(argv[5]);
        int count = (argc >= 7) ? atoi(argv[6]) : 10;

        generateBatch(folder, n, d, connected, count);
        return 0;
    }

    // Modo original
    int d = 0, connected = 0;
    long n = 0;

    if (argc < 5) {
        printf("Please, specify the instance type name, number of vertices, density and if the graph need to be connected (1 or 0).\n");
        return 0;
    }
    for (int i = 0; argv[2][i] != '\0'; i++) { n = n * 10; n += argv[2][i] - '0'; }
    for (int i = 0; argv[3][i] != '\0'; i++) { d = d * 10; d += argv[3][i] - '0'; }
    for (int i = 0; argv[4][i] != '\0'; i++) { connected = connected * 10; connected += argv[4][i] - '0'; }

    createGraphFile(argv[1], n, d, connected);
    return 0;
}