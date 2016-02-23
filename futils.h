/**
 * @author Francesco Wanderlingh
 * @date Year 2015
 *
 * @brief A set of useful C++ utilities ironically called "futils".
 *
 * @details The utilities implemented, meant primarily for Linux-based OS, include:
 * 			- Elapsing Spinner Command-Line Animation
 * 			- Waiting Dotter Command-Line Animation
 * 			- Executable file self path retrieval for path-safe file saving, loading
 * 			- STL Vector Printing
 * 			- Debug print macro
 */

#ifndef FUTILS_H_
#define FUTILS_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>

#include "term_colors.h"

#ifdef DEBUG_PRINT
#	define dout std::cerr
#else
#	define dout 0 && std::cerr
#endif

/** Escape sequence
 *  \033[<code>m or \e[<code>m
 *
 *  color : \[\033[ <code>m\] \]
 *
 *  e.g.
 *   bg=Blue, Bold, fg=Red
 *    1. \033[44;1;31m
 *    2. \033[44m\033[1;31m
 *
 *
 * Text attributes
 *  0  All attributes off
 *  1  Bold on
 *  4  Underscore (on monochrome display adapter only)
 *  5  Blink on
 *  7  Reverse video on
 *  8  Concealed on
 *
 *
 * Foreground colors
 *  0;30   Black          1;30   Dark Gray
 *  0;31   Red            1;31   Light Red
 *  0;32   Green          1;32   Light Green
 *  0;33   Brown          1;33   Yellow
 *  0;34   Blue           1;34   Light Blue
 *  0;35   Purple         1;35   Light Purple
 *  0;36   Cyan           1;36   Light Cyan
 *  0;37   Light Gray     1;37   White
 */

#if defined(__linux__) || defined(linux)
#define TC_RED          "\033[5;31m"
#define TC_CYAN         "\033[1;36m"
#define TC_GREEN        "\033[1;32m"
#define TC_BLUE         "\033[1;34m"
#define TC_YELLOW       "\033[1;33m"
#define TC_BLACK        "\033[0;30m"
#define TC_BROWN        "\033[0;33m"
#define TC_MAGENTA      "\033[1;35m"
#define TC_GRAY         "\033[1;37m"
#define TC_NONE         "\033[0m"

namespace tc {
const char* const none = 	"\033[0m";
const char* const blck =  	"\033[0;30m";
const char* const grayD = 	"\033[1;30m";
const char* const red =  	"\033[0;31m";
const char* const redL = 	"\033[1;31m";
const char* const grn =  	"\033[0;32m";
const char* const grnL = 	"\033[1;32m";
const char* const brwn =  	"\033[0;33m";
const char* const yel =  	"\033[1;33m";
const char* const blu =  	"\033[0;34m";
const char* const bluL = 	"\033[1;34m";
const char* const mag =  	"\033[0;35m";
const char* const magL = 	"\033[1;35m";
const char* const cyan =  	"\033[0;36m";
const char* const cyanL = 	"\033[1;36m";
const char* const grayL =  	"\033[0;37m";
const char* const white =  	"\033[1;37m";
}
#endif

namespace FUTILS
{
/// My first functor!!! :D

/** 
 * Prints a spinner that, put in a while loop with negligible execution time,
 * spins at a given frequency.
 *
 * @param freq Animation update frequency
 */
struct Spinner
{
	Spinner(int frequency) :
			freq(frequency), spinIndex(0), spin_chars("/-\\|")
	{
		clock_gettime(CLOCK_MONOTONIC, &last);
		period = 1 / static_cast<double>(freq + 1E-6);
		//std::cout << "period: " << period << "s" << std::endl;
	}

	void operator()(void)
	{

		clock_gettime(CLOCK_MONOTONIC, &now);
		double timeElapsed = (now.tv_sec - last.tv_sec) + (now.tv_nsec - last.tv_nsec) / 1E9;

		//std::cout << "timeElapsed: " << timeElapsed << std::endl;
		if (period - timeElapsed < 1E-3) {
			//std::cout << "fabs(freq - timeElapsed): " << fabs(freq - timeElapsed) << std::endl;
			//printf("\e[?25l"); /* hide the cursor */
			putchar(' ');
			putchar(spin_chars[spinIndex % spin_chars.length()]);
			putchar(' ');
			fflush(stdout);
			putchar('\r');
			spinIndex++;
			last = now;
		}

	}

private:
	struct timespec last, now;
	int freq;
	double period;
	unsigned long spinIndex;
	std::string spin_chars;
};

/**
 * Prints a classic dot animation that, put in a while loop with negligible execution time,
 * plays at a given frequency.
 *
 * @param freq Animation update frequency
 */
struct Dotter
{
	Dotter(int freq) :
			freq(freq), spinIndex(0), spin_chars("... .. .. .. .... .... .")
	{
		clock_gettime(CLOCK_MONOTONIC, &last);
		period = 1 / static_cast<double>(freq);
		//std::cout << "period: " << period << "s" << std::endl;
	}

	void operator()(void)
	{

		clock_gettime(CLOCK_MONOTONIC, &now);
		double timeElapsed = (now.tv_sec - last.tv_sec) + (now.tv_nsec - last.tv_nsec) / 1E9;

		//std::cout << "timeElapsed: " << timeElapsed << std::endl;
		if (period - timeElapsed < 1E-3) {
			//std::cout << "fabs(freq - timeElapsed): " << fabs(freq - timeElapsed) << std::endl;
			putchar(' ');
			putchar(spin_chars[spinIndex % spin_chars.length()]);
			putchar(spin_chars[(spinIndex % spin_chars.length()) + 1]);
			putchar(spin_chars[(spinIndex % spin_chars.length()) + 2]);
			putchar(spin_chars[(spinIndex % spin_chars.length()) + 3]);
			putchar(' ');
			fflush(stdout);
			putchar('\r');
			spinIndex += 4;
			last = now;
		}

	}

private:
	struct timespec last, now;
	int freq;
	double period;
	unsigned long spinIndex;
	std::string spin_chars;
};

/**
 * Simple timer for getting time elapsed and intermediate laps.
 * All values are returned in seconds, with nanosecond precision.
 */
struct Timer
{
	struct timespec start, lap, now;

	void Start()
	{
		clock_gettime(CLOCK_MONOTONIC, &start);
		lap = start;
	}

	double Elapsed()
	{
		clock_gettime(CLOCK_MONOTONIC, &now);
		double elapsedTime = (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1E9;
		return elapsedTime;
	}

	double Lap()
	{
		clock_gettime(CLOCK_MONOTONIC, &now);
		double lapTime = (now.tv_sec - lap.tv_sec) + (now.tv_nsec - lap.tv_nsec) / 1E9;
		lap = now;
		return lapTime;
	}
};

template<typename T>
void PrintSTLVector(T vecObj)
{
	for (typename T::iterator itr = vecObj.begin(); itr != vecObj.end(); ++itr) {
		std::cout << *itr << " ";
	}
}

template<typename T>
void PrintSTLVectOfVects(T vecObj)
{
	for (typename T::iterator itr = vecObj.begin(); itr != vecObj.end(); ++itr) {
		std::cout << "#" << itr - vecObj.begin() << ": ";
		PrintSTLVector(*itr);
		std::cout << "\n";
	}
}

/**
 * Returns the path of the folder containing executable that calls this functions
 *
 * @return String with the folder path
 */
std::string get_selfpath()
{
	char buff[2048];
	ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
	if (len != -1) {
		buff[len] = '\0';
		std::string path(buff);   ///Here the executable name is still in
		std::string::size_type t = path.find_last_of("/");   // Here we find the last "/"
		path = path.substr(0, t);                             // and remove the rest (exe name)
		return path;
	} else {
		printf("Cannot determine executable path! [Exiting]\n");
		exit(-1);
	}
}

/**
 * Returns the current date and time formatted as %Y-%m-%d_%H.%M.%S
 *
 * @return Current date
 */
std::string GetCurrentDateFormatted()
{

	std::time_t t = std::time(NULL);
	char mbstr[20];
	std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d_%H.%M.%S", std::localtime(&t));
	std::string currentDate(mbstr);

	return currentDate;
}

} /* namespace FUTILS */



#endif /* FUTILS_H_ */

