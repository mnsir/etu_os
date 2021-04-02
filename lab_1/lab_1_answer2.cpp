#include <iostream>
#include <list>
#include <thread>

void func(std::list<std::exception_ptr>& errors) {
	try {
		throw int{ 42 };
	}
	catch (...) {
		errors.push_back(std::current_exception());
	}
}

int main()
{
	std::list<std::exception_ptr> errors;
	std::thread th{ func, std::ref(errors) };
	th.join();
	for (auto& e : errors) {
		try {
			if (e) std::rethrow_exception(e);
		}
		catch (int n) {
			std::cout << n << " error\n";
		}
	}
}
