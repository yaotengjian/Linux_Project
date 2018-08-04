#include "data_pool.h"

data_pool::data_pool(int _cap):cap(_cap), pool(_cap)
{
	product_step = 0;
	consume_step = 0;
	sem_init(&blank_sem, 0, _cap);
	sem_init(&data_sem, 0, 0);
}

void data_pool::getData(std::string &outString)
{
	sem_wait(&data_sem); //P
	outString = pool[consume_step];
	consume_step++;
	consume_step %= cap;
	sem_post(&blank_sem); //V
}
void data_pool::putData(const std::string &inString)
{
	sem_wait(&blank_sem);
	pool[product_step] = inString;
	product_step++;
	product_step %= cap;
	sem_post(&data_sem);
}
data_pool::~data_pool()
{
	sem_destroy(&blank_sem);
	sem_destroy(&data_sem);
}










