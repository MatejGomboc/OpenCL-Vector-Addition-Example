# define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <exception>


int main()
{
	std::ofstream errorLogFile("error_log.txt");
	std::ofstream queryFile("opencl_query.txt");

	try
	{
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		for (int i = 0; i < platforms.size(); i++)
		{
			std::string str;
			
			platforms[i].getInfo(CL_PLATFORM_NAME, &str);
			queryFile << "platform name: " << str.c_str() << std::endl;

			platforms[i].getInfo(CL_PLATFORM_PROFILE, &str);
			queryFile << "platform profile: " << str.c_str() << std::endl;

			platforms[i].getInfo(CL_PLATFORM_VERSION, &str);
			queryFile << "platform version: " << str.c_str() << std::endl;

			platforms[i].getInfo(CL_PLATFORM_VENDOR, &str);
			queryFile << "platform vendor: " << str.c_str() << std::endl;

			platforms[i].getInfo(CL_PLATFORM_EXTENSIONS, &str);
			queryFile << "platform extensions: " << str.c_str() << std::endl;

			queryFile << std::endl;

			std::vector<cl::Device> devices;
			platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

			for (int j = 0; j < devices.size(); j++)
			{
				cl_device_type dev_type;
				cl_uint numb;
				std::vector<size_t> sizes;
				size_t size;
				cl_ulong numblon;

				devices[j].getInfo(CL_DEVICE_NAME, &str);
				queryFile << "device name: " << str.c_str() << std::endl;

				devices[j].getInfo(CL_DEVICE_VENDOR, &str);
				queryFile << "device vendor: " << str.c_str() << std::endl;

				devices[j].getInfo(CL_DEVICE_TYPE, &dev_type);
				queryFile << "device type: " << dev_type << std::endl;

				devices[j].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &numb);
				queryFile << "device max compute units: " << numb << std::endl;

				devices[j].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &numb);
				queryFile << "device max work item dimensions: " << numb << std::endl;

				devices[j].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &sizes);
				queryFile << "device max work item sizes: ";
				for (int k = 0; k < sizes.size(); k++)
				{
					queryFile << sizes[k] << " ";
				}
				queryFile << std::endl;

				devices[j].getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &size);
				queryFile << "device max workgroup size: " << size << std::endl;

				devices[j].getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &numb);
				queryFile << "device max clk freq: " << numb << std::endl;

				devices[j].getInfo(CL_DEVICE_ADDRESS_BITS, &numb);
				queryFile << "device addr bits: " << numb << std::endl;

				devices[j].getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &numblon);
				queryFile << "device max mem allocation size: " << numblon << std::endl;

				devices[j].getInfo(CL_DEVICE_IMAGE_SUPPORT, &numb);
				queryFile << "device image support: " << (bool)numb << std::endl;

				queryFile << std::endl;
				queryFile << std::endl;
			}

			queryFile << std::endl;
			queryFile << std::endl;
		}
	}
	catch (cl::Error error)
	{
		errorLogFile << error.what() << "(" << error.err() << ")" << std::endl;
	}
	catch (std::exception& ex)
	{
		errorLogFile << ex.what() << std::endl;
	}

	errorLogFile.close();
	queryFile.close();

	return 0;
}
