#pragma  once

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>

class Timer : public std::enable_shared_from_this<Timer> {
	std::atomic<bool> active{ true };

public:

	template<typename Function>
	void setTimeout(Function function, int delay) {
		active = true;
		auto self = shared_from_this();
		std::thread t([self, function, delay]() {
			if (!self->active.load()) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			if (!self->active.load()) return;
			function();
			});
		t.detach();
	}

	template<typename Function>
	void setInterval(Function function, int interval) {
		active = true;
		auto self = shared_from_this();
		std::thread t([self, function, interval]() {
			while (self->active.load()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(interval));
				if (!self->active.load()) return;
				function();
			}
			});
		t.detach();
	}

	void stop();

};

inline void Timer::stop() {
	active = false;
}
