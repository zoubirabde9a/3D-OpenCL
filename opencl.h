#ifndef OPENCL_H
#define OPENCL_H
#include <CL/opencl.h>


int CreateCLProgramFromSrc(cl_program *Program, cl_kernel *KernelsOut,
                         cl_context context, cl_device_id device_id,
                         char *Src, char **KernelsIn, int KernelsCount);
int executeOpenCLFunction(char *KernelSource, char *FunctionToExecute,
                          uint32_t ElementSize, uint32_t ArgSize, int InCount, int OutCount,
                          void *In, void *Out, void *Arg);

#endif // OPENCL_H

