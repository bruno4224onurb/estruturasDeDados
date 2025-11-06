// ==================================================
// Definitions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM_M 100

#define MINIMUM 15
#define MAXIMUM 25

// ==================================================
// Utility Functions

const char* get_file_name(int argc, char *argv[], const char* default_file) {
    if (argc >= 2) {
        return argv[1];  // Return the first argument
    } else {
        return default_file;  // Return default value if no argument
    }
}

// Function to open a file for writing or reading, returns FILE pointer
FILE* open_file(const char* file_name, const char* modes)
{
    FILE* file = fopen(file_name, modes);
    if (!file) {
        perror("Error opening file");
    }
    return file;
}

// Function to close a file
void close_file(FILE* file)
{
    if (file) fclose(file);
}


void write_matrix_to_std(const char *title, int M, int matrix[][100])
{
    printf("%s", title != NULL ? title : "");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void write_dish_to_std(int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M], int M)
{
    write_matrix_to_std(NULL, M, temp);
	printf("\n");
    write_matrix_to_std(NULL, M, cells);
}

// Generate aleatory temperatures between MINIMUN and MAXIMUN
void write_random_temperatures_to_file(FILE *f, int M)
{
	int amplitude = MAXIMUM - MINIMUM;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			int temp = MINIMUM + (rand() % amplitude);
			fprintf(f, "%d ", temp);
		}
		fprintf(f, "\n");
	}
}

// Generae aleatory cells distribution
void write_random_cells_to_file(FILE *f, int M)
{
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			int alive = rand() % 2;
			fprintf(f, "%d ", alive);
		}
		fprintf(f, "\n");
	}
}

int write_random_dish_to_file(const char* file_name, int M)
{
    FILE* f = open_file(file_name, "w");
    if (!f) return 1;

	write_random_temperatures_to_file(f, M);
	fprintf(f, "\n");
	write_random_cells_to_file(f, M);

	close_file(f);
    return 1;
}

// ==================================================
// Main Functions

// Check if coordinates (x, y) are inside an n x n grid
int in_bounds(int x, int y, int n)
{
    if ((x <= n-1 && x >=0) && (y <= n-1 && y >=0))
        return 1;
    else 
        return 0;// return true if (x, y) is inside the matrix of size NxN
}

// Count alive neighbors
int count_alive_neighbors(int cells[MAXIMUM_M][MAXIMUM_M], int M, int i, int j)
{
    int count = 0;
    int di[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dj[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    if (in_bounds(i,j,M)==1){
        for (int d = 0; d < 8; d++) {
        if (in_bounds(i+di[d],j+dj[d],M) && cells[i+di[d]][j+dj[d]] == 1)
        {
            count++;
        }
    }
    }
    return count;
}

// Compute updated temperatures
void compute_updated_temperatures(int M, int temp[][100], int temp_next[][100])
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if ( i==0 || j==0 || i == M-1 || j == M-1){
                temp_next[i][j]=temp[i][j];
            }
            else{
                temp_next[i][j] = (temp[i-1][j]+ temp[i+1][j] + temp[i][j-1] + temp[i][j+1])/4;
            }
            // store updated values in temp_next
        }
    }
}

// Compute updated cells
void compute_updated_cells(int M, int cells[][100], int temp_next[][100], int cells_next[][100], int Tmin, int Tmax)
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if(temp_next[i][j]>Tmax || temp_next[i][j]<Tmin){
                cells_next[i][j] = 0;
            }
            else{
                int aux = count_alive_neighbors(cells,M,i,j);
                if(cells[i][j]==1){
                    if(aux > 3 || aux < 2){
                        cells_next[i][j] = 0;
                    }
                    else{
                        cells_next[i][j] = 1;
                    }
                }
                else{
                    if (aux == 3){
                        cells_next[i][j]=1;
                    }
                    else{
                        cells_next[i][j]=0;
                    }
                }
            }
        }
    }
}

// Copy updated matrices for next step
void updated_matrix(int M, int origin[][100], int destiny[][100])
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            destiny[i][j] = origin[i][j];
        }
    }
}

int main(int argc, char *argv[])
{
    // ATTENTION: Change input read mode back to standard input (keyboard) before submitting!
    // Current mode may be set to read from file or custom source for testing.

    int M, K, Tmin, Tmax, i = 0, j = 0;
    scanf("%d", &M);
    scanf("%d", &K);
    scanf("%d", &Tmin);
    scanf("%d", &Tmax);
    // MxM matrices (*_next store update)
    int temp[MAXIMUM_M][MAXIMUM_M] = {{0}};
    int temp_next[MAXIMUM_M][MAXIMUM_M] = {{0}};
    int cells[MAXIMUM_M][MAXIMUM_M] = {{0}};
    int cells_next[MAXIMUM_M][MAXIMUM_M] = {{0}};

    for(int a = 0; a < M; a++){
        for(int b = 0; b < M; b++){
            scanf("%d", &temp[a][b]);
        }
    }
    for(int a = 0; a < M; a++){
        for(int b = 0; b < M; b++){
            scanf("%d", &cells[a][b]);
        }
    }

    // K iterations
    for (int step = 0; step < K; step++) {
        // --- 1. Update temperatures ---
        compute_updated_temperatures(M, temp, temp_next);

        // --- 2. Update Cells (Game of Life + Temperature) ---
        compute_updated_cells(M, cells, temp_next, cells_next, Tmin, Tmax);

        // Update matrices for the next iteration
        updated_matrix(M, temp_next, temp);
        updated_matrix(M, cells_next, cells);
    }

    // --- Show final results ---
    write_dish_to_std(temp, cells, M);

    return 0;
}
