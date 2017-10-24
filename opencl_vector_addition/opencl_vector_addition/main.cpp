# define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <exception>


int main()
{
	try
	{
		const long N_elements = 102400;

		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		std::vector<cl::Device> devices;
		platforms[1].getDevices(CL_DEVICE_TYPE_ALL, &devices);

		std::ifstream helloWorldFile("VectorAddition.cl");
		std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));

		cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

		cl::Context context(devices);

		cl::Program program(context, sources);

		cl_int err = program.build(devices, "-cl-std=CL1.2");

		int* vector1 = new int[N_elements];
		std::fill(vector1, vector1 + N_elements, 1);

		//for (long i = 0; i < N_elements; i++) std::cout << vector1[i] << " ";
		//std::cout << std::endl;
		
		cl::Buffer vec1Buff(context, CL_MEM_READ_ONLY, sizeof(int) * N_elements, vector1);

		int* vector2 = new int[N_elements];
		std::fill(vector2, vector2 + N_elements, 2);

		//for (long i = 0; i < N_elements; i++) std::cout << vector2[i] << " ";
		//std::cout << std::endl;

		cl::Buffer vec2Buff(context, CL_MEM_READ_ONLY, sizeof(int) * N_elements, vector2);

		int* vector3 = new int[N_elements];
		cl::Buffer vec3Buff(context, CL_MEM_WRITE_ONLY, sizeof(int) * N_elements, vector3);

		cl::Kernel kernel(program, "vectorAddition", &err);
		kernel.setArg(0, vec1Buff);
		kernel.setArg(1, vec2Buff);
		kernel.setArg(2, vec3Buff);

		cl::CommandQueue queue(context, devices[0]);
		queue.enqueueWriteBuffer(vec1Buff, CL_TRUE, 0, sizeof(int) * N_elements, vector1);
		queue.enqueueWriteBuffer(vec2Buff, CL_TRUE, 0, sizeof(int) * N_elements, vector2);

		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(N_elements), cl::NDRange(1024));

		queue.enqueueReadBuffer(vec3Buff, CL_TRUE, 0, sizeof(int) * N_elements, vector3);

		for (long i = 0; i < N_elements; i++) std::cout << vector3[i] << " ";

		delete[] vector1;
		delete[] vector2;
		delete[] vector3;
	}
	catch (cl::Error error)
	{
		std::cout << error.what() << "(" << error.err() << ")" << std::endl;
	}

	std::cin.get();

	return 0;
}
