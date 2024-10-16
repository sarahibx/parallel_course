#include <mpi.h>
#include <stdio.h>
#include <math.h>

// Function to evaluate the curve (y = f(x))
float f(float x) {
    return x * x; // Example: y = x^2
}

// Function to compute the area of a trapezoid
float trapezoid_area(float a, float b, float d) { 
    float area = 0;
    for (float x = a; x < b; x+=d) {
        area += f(x) + f(x+d);
    }
    return area * d / 2.0f;
}

int main(int argc, char** argv) {
    int rank, size;
    float a = 0.0f, b = 1.0f;  // Limits of integration
    int n;
    float start, end, local_area, total_area;
    double start_time, end_time, local_time, total_time; // Add the time calculation variables

    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get number of processes

    if (rank == 0) {
        // Get the number of intervals from the user
        printf("Enter the number of intervals: ");
        fflush(stdout);
        scanf("%d", &n);
    }

    // Broadcast the number of intervals to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the interval size for each process
    float d = (b - a) / n; // delta
    float region = (b - a) / size;

    // Calculate local bounds for each process
    start = a + rank * region;
    end = start + region;

    // Start timing the local computation
    start_time = MPI_Wtime();
    
    // Each process calculates the area of its subinterval
    local_area = trapezoid_area(start, end, d);
    
    // Stop timing the local computation
    end_time = MPI_Wtime();
    local_time = end_time - start_time;
    
    // Reduce all local areas to the total area on the root process
    MPI_Reduce(&local_area, &total_area, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Reduce all local times to get the total computation time on the root process
    MPI_Reduce(&local_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The total area under the curve is: %f\n", total_area);
        printf("Total computation time: %f seconds\n", total_time);
    }

    MPI_Finalize(); // Finalize MPI
    return 0;
}


// Pseudocode for Parallel Bucket Sort using MPI

// Step 0: Set Up MPI and Input Data
// Initialize MPI
// Obtain the total number of processes (P) and the rank of each process (process_id)
// Input: A list of unsorted elements is present on process 0

// // Step 1: Divide and Distribute Data
// If process_id == 0:
//     Split the input list into P smaller subarrays
//     Use MPI_Scatter to send each subarray to the corresponding process
// Each process receives its chunk of data

// // Step 2: Sort Data Locally
// Each process sorts its own subarray using a sequential sorting algorithm like quicksort

// // Step 3: Assign Data to Buckets
// Find the global minimum and maximum values using MPI_Reduce
// Each process computes bucket ranges based on the global minimum and maximum
// For every element in the locally sorted subarray:
//     Place the element in its correct global bucket

// // Step 4: Share Buckets Across Processes
// Use MPI_Alltoall to redistribute the bucket elements to the appropriate processes

// // Step 5: Sort Buckets Locally
// Each process sorts the bucket data it has received
// Merge these sorted buckets to form the final sorted result

// // Step 6: Gather and Combine Results
// Process 0 collects all the sorted subarrays from the processes using MPI_Gather
// If process_id is 0:
//     Combine all subarrays into the final sorted array

// Finalize MPI
















