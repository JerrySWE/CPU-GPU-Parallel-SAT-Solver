#include <stdio.h>
#include <chrono>
#include <unistd.h>
#include <iostream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

__device__ bool getValue(uint64_t assignment, int variable) {
    if (variable < 0) {
        int position = -1 * variable - 1;
        return ((assignment >> position) & 0x1) == 0;
    } else {
        int position = variable - 1;
        return ((assignment >> position) & 0x1) == 1;
    }
}

__device__ bool solve(int *CNF, uint64_t assignment, int c_count) {
    for (int i = 0; i < c_count; i++) {
        int c1 = CNF[3*i];
        int c2 = CNF[3*i+1];
        int c3 = CNF[3*i+2];

        if (getValue(assignment, c1) || getValue(assignment, c2) || getValue(assignment,c3)) {
            continue;
        }

        return false;
    }

    return true;
}

__global__ void
solve_kernel(int* CNF, size_t thread_size, int c_count, bool *flag) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    uint64_t start_assignment = index * thread_size;
    uint64_t end_assignment = (index + 1) * thread_size;

    for (uint64_t assignment = start_assignment; assignment < end_assignment; assignment++) {
        if (solve(CNF, assignment, c_count)) {
            *flag = true;
            break;
        }
    }
}

void
SATCuda(int* CNF, int v_count, int c_count) {

    uint64_t base = 1;
    uint64_t problem_size = base << v_count;

    // compute number of blocks and threads per block
    const int threadsCount = 1<<5;
    const int threadsPerBlock = 32;
    const int blocks = (threadsCount) / threadsPerBlock;

    size_t thread_size = problem_size / threadsCount;
    
    bool flag = false;

    int* device_CNF;
    bool* device_flag;

    int vectorBytes = sizeof(int) * 3 * c_count;
    cudaMalloc(&device_CNF, vectorBytes);
    cudaMalloc(&device_flag, sizeof(bool));

    // start timing after allocation of device memory
    auto start = std::chrono::high_resolution_clock::now();
    cudaMemcpy(device_CNF, CNF, vectorBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(device_flag, &flag, sizeof(bool), cudaMemcpyHostToDevice);
    // start timing after data copy

    // run kernel
    solve_kernel<<<blocks, threadsPerBlock>>>(device_CNF, thread_size, c_count, device_flag);
    cudaDeviceSynchronize();

    // end timining after all threads are computed
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    std::cout << "CUDA execution time " << duration.count() << " ms\n"; 

    cudaMemcpy(&flag, device_flag, sizeof(bool), cudaMemcpyDeviceToHost);

    if (flag) {
        std::cout << "SOLVABLE!\n";
    } else {
        std::cout << "UNSOLVABLE!\n";
    }

    // end timing after result has been copied back into host memory

    cudaError_t errCode = cudaPeekAtLastError();
    if (errCode != cudaSuccess) {
        fprintf(stderr, "WARNING: A CUDA error occured: code=%d, %s\n", errCode, cudaGetErrorString(errCode));
    }

    // TODO free memory buffers on the GPU
    cudaFree(device_CNF);
}

void
printCudaInfo() {

    // for fun, just print out some stats on the machine

    int deviceCount = 0;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    printf("---------------------------------------------------------\n");
    printf("Found %d CUDA devices\n", deviceCount);

    for (int i=0; i<deviceCount; i++) {
        cudaDeviceProp deviceProps;
        cudaGetDeviceProperties(&deviceProps, i);
        printf("Device %d: %s\n", i, deviceProps.name);
        printf("   SMs:        %d\n", deviceProps.multiProcessorCount);
        printf("   Global mem: %.0f MB\n",
               static_cast<float>(deviceProps.totalGlobalMem) / (1024 * 1024));
        printf("   CUDA Cap:   %d.%d\n", deviceProps.major, deviceProps.minor);
    }
    printf("---------------------------------------------------------\n");
}
