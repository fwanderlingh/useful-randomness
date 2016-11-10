#include <iostream>
#include <cstdio>
#include <unistd.h> // needed for readlink
#include <cstdlib>
#include <ctime>
#include <vector>

namespace FUTILS {
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
			freq(frequency), spinIndex(0), spin_chars("/-\\|") {
		clock_gettime(CLOCK_MONOTONIC, &last);
		period = 1 / static_cast<double>(freq + 1E-6);
		//std::cout << "period: " << period << "s" << std::endl;
	}

	void operator()(void) {

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
			freq(freq), spinIndex(0), spin_chars("... .. .. .. .... .... .") {
		clock_gettime(CLOCK_MONOTONIC, &last);
		period = 1 / static_cast<double>(freq);
		//std::cout << "period: " << period << "s" << std::endl;
	}

	void operator()(void) {

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

template<typename T>
void PrintSTLVector(T vecObj) {
	for (typename T::iterator itr = vecObj.begin(); itr != vecObj.end(); ++itr) {
		std::cout << *itr << " ";
	}
}

template<typename T>
void PrintSTLVectOfVects(T vecObj) {
	for (typename T::iterator itr = vecObj.begin(); itr != vecObj.end(); ++itr) {
		std::cout << "#" << itr - vecObj.begin() << ": ";
		PrintSTLVector(*itr);
		std::cout << "\n";
	}
}

std::string get_selfpath() {
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

}
