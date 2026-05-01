#pragma once
#include <chrono>оформление
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <Windows.h>

class Console_Layout
{
private:
	static HANDLE hStdOut;

public:
	static void set_color(uint8_t font, uint8_t back)
	{
		SetConsoleTextAttribute(hStdOut, (WORD)((back << 4) | font));
	}

	static void set_position(uint8_t x, uint8_t y)
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(hStdOut, point);
	}
};
HANDLE Console_Layout::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

class Console_Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::string m_name;
	std::chrono::time_point<clock_t> m_begin;
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_begin).count();
	}

public:
	Console_Timer() : m_begin(clock_t::now())
	{

	}

	Console_Timer(std::string name) : m_name(name), m_begin(clock_t::now())
	{

	}

	void run(std::string name)
	{
		m_name = name;
		m_begin = clock_t::now();
	}

	void show() const
	{
		std::cout << "ID" << std::this_thread::get_id() << "\t" << elapsed() * 1000 << " ms" << std::endl;
	}
};
