#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

typedef cudaError_t (*cudaMallocManaged_t)(void**, size_t, unsigned int);
cudaMallocManaged_t original_cudaMallocManaged = NULL;
typedef cudaError_t (*cudaMalloc_t)(void**, size_t);
cudaMalloc_t original_cudaMalloc = NULL;
cudaError_t cudaMalloc(void** devPtr, size_t size)
{
    if (!original_cudaMalloc) {
        original_cudaMalloc = (cudaMalloc_t)dlsym(RTLD_NEXT, "cudaMalloc");
        if (!original_cudaMalloc) {
            fprintf(stderr, "Error locating original cudaMalloc: %s\n", dlerror());
            return cudaErrorUnknown;
        }
    }

    if (!original_cudaMallocManaged) {
        original_cudaMallocManaged = (cudaMallocManaged_t)dlsym(RTLD_NEXT, "cudaMallocManaged");
        if (!original_cudaMallocManaged) {
            fprintf(stderr, "Error locating original cudaMallocManaged: %s\n", dlerror());
            return cudaErrorUnknown;
        }
    }

    // 将 cudaMalloc() 调用替换成 cudaMallocManaged() 调用
    // 通过NVIDIA UVM机制来分配内存，使得程序可以使用系统上包括host和device在内的多个处理器内存
    // 详见https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY.html#group__CUDART__MEMORY_1gd228014f19cc0975ebe3e0dd2af6dd1b
    return original_cudaMallocManaged(devPtr, size, cudaMemAttachGlobal);
}

typedef cudaError_t (*cudaMemcpyAsync_t)(void* dst, const void* src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
cudaMemcpyAsync_t original_cudaMemcpyAsync = NULL;
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream)
{
    if (!original_cudaMemcpyAsync) {
        original_cudaMemcpyAsync = (cudaMemcpyAsync_t)dlsym(RTLD_NEXT, "cudaMemcpyAsync");
        if (!original_cudaMemcpyAsync) {
            fprintf(stderr, "Error locating original cudaMemcpyAsync: %s\n", dlerror());
            return cudaErrorUnknown;
        }
    }

    // 在内存拷贝时将 kind 参数替换成 cudaMemcpyDefault
    // 在支持UVA的系统下使用该参数进行拷贝可以自动推导数据拷贝的方向
    // 详见https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY.html#group__CUDART__MEMORY_1g85073372f776b4c4d5f89f7124b7bf79
    return original_cudaMemcpyAsync(dst, src, count, cudaMemcpyDefault, stream);
}

// typedef cudaError_t (*cudaHostAlloc_t)(void** pHost, size_t size, unsigned int  flags);
// cudaHostAlloc_t original_cudaHostAlloc = NULL;
// cudaError_t cudaHostAlloc(void** pHost, size_t size, unsigned int flags)
// {
//     if (!original_cudaHostAlloc) {
//         original_cudaHostAlloc = (cudaHostAlloc_t)dlsym(RTLD_NEXT, "cudaHostAlloc");
//         if (!original_cudaHostAlloc) {
//             fprintf(stderr, "Error locating original cudaHostAlloc: %s\n", dlerror());
//             return cudaErrorUnknown;
//         }
//     }

//     return original_cudaHostAlloc(pHost, size, flags);
// }

// typedef cudaError_t (*cudaMemsetAsync_t)(void* devPtr, int  value, size_t count, cudaStream_t stream);
// cudaMemsetAsync_t original_cudaMemsetAsync = NULL;
// cudaError_t cudaMemsetAsync(void* devPtr, int  value, size_t count, cudaStream_t stream)
// {
//     if (!original_cudaMemsetAsync) {
//         original_cudaMemsetAsync = (cudaMemsetAsync_t)dlsym(RTLD_NEXT, "cudaMemsetAsync");
//         if (!original_cudaMemsetAsync) {
//             fprintf(stderr, "Error locating original cudaMemsetAsync: %s\n", dlerror());
//             return cudaErrorUnknown;
//         }
//     }

//     return original_cudaMemsetAsync(devPtr, value, count, stream);
// }