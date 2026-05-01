#include "Header.h"

using namespace std::chrono_literals;
std::atomic_int8_t count(1);

void client_thread(uint8_t clients)
{
	while (count < clients)
	{
		count.fetch_add(1, std::memory_order_relaxed);
		std::this_thread::sleep_for(1s);
		std::cout << count + 1 << " clients in line." << std::endl;
	}
}

void operator_thread()
{
	while (count > 0)
	{
		count.fetch_sub(1, std::memory_order_relaxed);
		std::this_thread::sleep_for(2s);
		if (count > 0)
		{
			std::cout << "\toperator has served the client" << std::endl;
			std::cout << count - 1 << " clients in line." << std::endl;
		}
		else
		{
			break;
		}
	}
}

void task_1(uint8_t clients)
{
	std::thread client_t(client_thread, clients);
	std::thread operator_t(operator_thread);

	client_t.join();
	operator_t.join();

	system("cls");
}

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

void task_2(uint8_t cnt_of_threads)
{
	std::vector<std::thread> thread_vector(cnt_of_threads);

	for (size_t i = 0; i < cnt_of_threads; i++)
	{
		thread_vector[i] = std::thread(progress_bar, i);
	}

	for (auto& i : thread_vector)
		i.join();

	std::cout << "\n\n" << std::endl;
}

void swap_lock(Data &data1, Data &data2)
{
	if (&data1 == &data2)
		return;
	std::lock(data1.mtx, data2.mtx);
	std::lock_guard<std::mutex> lock_data1(data1.mtx, std::adopt_lock);
	std::lock_guard<std::mutex> lock_data2(data2.mtx, std::adopt_lock);
	std::swap(data1._data, data2._data);
	data1.show_data();
	data2.show_data();
	std::cout << std::endl;
}

void swap_scoped_lock(Data &data1, Data &data2)
{
	if (&data1 == &data2)
		return;
	std::scoped_lock lock(data1.mtx, data2.mtx);
	std::swap(data1._data, data2._data);
	data1.show_data();
	data2.show_data();
	std::cout << std::endl;
}

void swap_unique_lock(Data& data1, Data& data2)
{
	if (&data1 == &data2)
		return;
	std::unique_lock<std::mutex> lock_data1(data1.mtx, std::defer_lock);
	std::unique_lock<std::mutex> lock_data2(data2.mtx, std::defer_lock);
	std::lock(lock_data1, lock_data2);
	std::swap(data1._data, data2._data);
	data1.show_data();
	data2.show_data();
	std::cout << std::endl;
}

void task_3(uint8_t _data1, uint8_t _data2)
{
	Data data1(_data1), data2(_data2);
	auto print = [&data1, &data2]()
		{
			data1.show_data();
			data2.show_data();
			std::cout << std::endl;
		};

	print();

	std::thread thr1(swap_lock, std::ref(data1), std::ref(data2));
	std::thread thr2(swap_scoped_lock, std::ref(data1), std::ref(data2));
	std::thread thr3(swap_unique_lock, std::ref(data1), std::ref(data2));

	thr1.join();
	thr2.join();
	thr3.join();
}

int main(int argc, char** argv)
{
	task_1(3);
	task_2(5);
	task_3(7, 14);

	return EXIT_SUCCESS;
}
