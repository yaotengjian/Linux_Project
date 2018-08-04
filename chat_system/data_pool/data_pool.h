#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include <iostream>
#include <string>
#include <vector>
#include <semaphore.h>

class data_pool{
	public:
		data_pool(int _cap=256);
		void getData(std::string &outString);
		void putData(const std::string &inString);
		~data_pool();
	private:
		int cap;
		std::vector<std::string> pool;

		int product_step;
		int consume_step;

		sem_t blank_sem;
		sem_t data_sem;
};

#endif








