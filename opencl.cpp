#include "opencl.h"
#include "stdio.h"
#include <stdlib.h>
#include <QDebug>


int CreateCLProgramFromSrc(cl_program *Program, cl_kernel *KernelsOut,
                         cl_context context, cl_device_id device_id,
                         char *Src, char **KernelsIn, int KernelsCount)
{
    int err;

    *Program = clCreateProgramWithSource(context, 1, (const char **) &Src, NULL, &err);
    if (!*Program)
    {
        printf("Error: Failed to create compute program!\n");
        return err;
    }
    err = clBuildProgram(*Program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(*Program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
    }

    for(int KernelIndex = 0; KernelIndex < KernelsCount; KernelIndex++)
    {
        KernelsOut[KernelIndex] = clCreateKernel(*Program, KernelsIn[KernelIndex], &err);
        if (!KernelsOut[KernelIndex] || err != CL_SUCCESS)
        {
            printf("Error: Failed to create compute kernel!\n");
        }
    }

    return err;
}

int executeOpenCLFunction(char *KernelSource, char *FunctionToExecute,
                          uint32_t ElementSize, uint32_t ArgSize, int InCount, int OutCount,
                          void *In, void *Out, void *Arg)
{
    size_t global;
    size_t local;

    cl_platform_id platform_id;
    static cl_device_id device_id;
    static cl_context context = 0;
    cl_command_queue commands;
    cl_program program;
    cl_kernel kernel;

    cl_mem input;
    cl_mem output;
    cl_mem arg;

    int err;

    int platform_code = clGetPlatformIDs(1,
        &platform_id,
        NULL);

    int gpu = 1;
    err = clGetDeviceIDs(platform_id, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }

    if (context == 0)
    {
        context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
        if (!context)
        {
            printf("Error: Failed to create a compute context!\n");
            return EXIT_FAILURE;
        }
    }

    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    char * ProgramKernels[] =
    {
        FunctionToExecute
    };

    err = CreateCLProgramFromSrc(&program, &kernel, context, device_id,
                                 KernelSource, ProgramKernels, 1);


    input = clCreateBuffer(context, CL_MEM_READ_ONLY, InCount * ElementSize, NULL, NULL);
    arg = clCreateBuffer(context, CL_MEM_READ_ONLY, OutCount * ArgSize, NULL, NULL);

    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, OutCount * ElementSize, NULL, NULL);

    if (!input || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }

    err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, InCount * ElementSize, In, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(commands, arg, CL_TRUE, 0, OutCount * ArgSize, Arg, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 2, sizeof(int), &OutCount);
    err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &arg);

    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    global = OutCount;
    if (local >= global)
    {
        local = global;
    }

    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);

    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    clFinish(commands);

    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, OutCount * ElementSize, Out, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseMemObject(arg);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    //clReleaseContext(context);
}

