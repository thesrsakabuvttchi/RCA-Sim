#include <iostream>

using namespace std;

void multiply(const double* A, const double* B, double* result, int block_size) {
    for (int i = 0; i < block_size; ++i) {
        for (int j = 0; j < block_size; ++j) {
            for (int k = 0; k < block_size; ++k){
                result[i * block_size + j] += A[i * block_size + k] * B[k * block_size + j];
            }
        }
    }
}

// Function to perform matrix multiplication with padding
void blockedMatrixMultiply(const double* A, const double* B, double* C,
                            int m, int n, int p, int block_size) {
    // Pad dimensions to ensure they are divisible by block_size
    int m_padded = (m + block_size - 1) / block_size * block_size;
    int n_padded = (n + block_size - 1) / block_size * block_size;
    int p_padded = (p + block_size - 1) / block_size * block_size;

    // Create padded matrices
    double* A_padded = new double[m_padded * n_padded]();  // Zero-initialized
    double* B_padded = new double[n_padded * p_padded]();  // Zero-initialized
    double* C_padded = new double[m_padded * p_padded]();  // Zero-initialized

    // Copy data to padded matrices
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            A_padded[i * n_padded + j] = A[i * n + j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            B_padded[i * p_padded + j] = B[i * p + j];
        }
    }

    // Perform blocked matrix multiplication
    for (int i = 0; i < m_padded; i += block_size) {
        for (int j = 0; j < p_padded; j += block_size) {
            for (int k = 0; k < n_padded; k += block_size) {

                double *A_tmp = new double [block_size * block_size]();
                double *B_tmp = new double [block_size * block_size]();
                double *C_tmp = new double [block_size * block_size]();

                for (int ii = i; ii < i + block_size; ++ii) {
                    for (int jj = j; jj < j + block_size; ++jj) {
                        for (int kk = k; kk <k + block_size; ++kk) {
                            A_tmp[(ii-i)*block_size+(kk-k)] = A_padded[ii * n_padded + kk];
                            B_tmp[(kk-k)*block_size+(jj-j)] = B_padded[kk * p_padded + jj];
                        }
                    }
                }
                multiply(A_tmp,B_tmp,C_tmp,block_size);
                for(int ii=0;ii<block_size;ii++)
                {
                    for(int jj=0;jj<block_size;jj++)
                    {
                        C_padded[(i+ii)*p_padded+j+jj] += C_tmp[ii*block_size+jj]; 
                    }
                }
            }
        }
    }

    // Copy result from padded matrix to output matrix C
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            C[i * p + j] = C_padded[i * p_padded + j];
        }
    }

    // Deallocate memory
    delete[] A_padded;
    delete[] B_padded;
    delete[] C_padded;
}

int main() {
    // Example usage
    int m = 8;
    int n = 3;
    int p = 5;

    // Initialize matrices A, B, and C
    double* A = new double[m * n];
    double* B = new double[n * p];
    double* C = new double[m * p];

    // Fill matrices A and B with random values (for demonstration purposes)
    for (int i = 0; i < m * n; ++i) {
        A[i] = static_cast<double>(rand()) / RAND_MAX;
    }

    for (int i = 0; i < n * p; ++i) {
        B[i] = static_cast<double>(rand()) / RAND_MAX;
    }

    // Specify block size
    int block_size = 5;

    // Perform blocked matrix multiplication with padding
    blockedMatrixMultiply(A, B, C, m, n, p, block_size);

    // Print the result
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            cout << C[i * p + j] << " ";
        }
        cout << endl;
    }

    // Deallocate memory
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
