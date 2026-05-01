#include "Header.h"

std::mutex mtx1, mtx2, mtx3, mtx4;
std::once_flag flag;

static void table_header()
{
	std::cout << "#\t" << "ID\t\t" << "Progress Bar\t\t" << "Time" << std::endl;
}

static void progress_bar(uint8_t cnt_of_threads)
{
	uint8_t progress_bar = 20;
	uint8_t total = 40;
	uint8_t position4time = 48;

	std::call_once(flag, table_header);

	mtx2.lock();
	Console_Timer timer1;
	Console_Layout face1;
	mtx2.unlock();

	mtx4.lock();
	face1.set_position(0, cnt_of_threads + 1);
	std::cout << cnt_of_threads << "\t" << std::this_thread::get_id() << "\t";
	mtx4.unlock();

	while (progress_bar < total)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		mtx1.lock();
		face1.set_position(progress_bar, cnt_of_threads + 1);
		std::cout << char(220);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		++progress_bar;
		mtx1.unlock();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(150));

	mtx3.lock();
	face1.set_position(position4time, cnt_of_threads + 1);
	timer1.show();
	mtx3.unlock();
}

int main(int argc, char** argv)
{
	uint8_t cnt_of_threads = 5;
	std::vector<std::thread> thread_vector(cnt_of_threads);

	for (size_t i = 0; i < cnt_of_threads; i++)
	{
		thread_vector[i] = std::thread(progress_bar, i);
	}

	for (auto& i : thread_vector)
		i.join();

	std::cout << "\n" << std::endl;
}