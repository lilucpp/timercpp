#include <iostream>
#include <thread>
#include "timercpp.h"

void test1()
{
	std::shared_ptr<Timer> t(new Timer());
	t->setInterval([&]() {
		std::cout << "id=" << std::this_thread::get_id() << std::endl;
		},
		1000);

	std::thread([t]() {
		for (int i = 1; i < 5; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			t->stop();
		}}).detach();

    std::thread([t]() {
        for (int i = 1; i < 5; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2100));
            t->setInterval([i]() {
                std::cout << i << "id=" << std::this_thread::get_id() << std::endl;
                },
                1000);
        }}).detach();
}

int main()
{
	std::thread(test1).detach();

	getchar();

	return 0;
}
