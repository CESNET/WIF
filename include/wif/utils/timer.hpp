/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Timer interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <chrono>
#include <memory>
#include <thread>

namespace WIF {

/**
 * @brief TimerCallback class
 *
 * Abstract class, which can be overloaded and whose methods are called periodically by the
 * parenting Timer instance
 */
class TimerCallback {
public:
	/**
	 * @brief Destroy the Timer callback object
	 */
	virtual ~TimerCallback() = default;

	/**
	 * @brief Method, which is called before the first onTick() call and can be used to setup
	 * TimerCallback class
	 */
	virtual void onStart() {};

	/**
	 * @brief Method, which is called periodically
	 */
	virtual void onTick() = 0;

	/**
	 * @brief Method, which is called after the Timer object was instructed to end and can be used
	 * for final cleanup
	 */
	virtual void onEnd() {};
};

/**
 * @brief Timer class
 *
 * Class representing a timer which periodically calls onTick method on the TimerCallback instance
 */
class Timer {
public:
	/**
	 * @brief Construct a new Timer object
	 *
	 * @param intervalInSeconds time interval in which function will be called
	 * @param timerCallback timer callback instance, on which methods are called
	 */
	Timer(unsigned intervalInSeconds, std::unique_ptr<TimerCallback> timerCallback)
		: m_intervalInSeconds(intervalInSeconds)
		, m_timerCallback(std::move(timerCallback))
	{
	}

	/**
	 * @brief Destroy the Timer object
	 */
	~Timer() { cancel(); }

	/**
	 * @brief Start the timer thread
	 */
	void start() { m_tickingThread = std::thread(&WIF::Timer::Timer::timer, this); }

	/**
	 * @brief Wait for the timeout, call the function one last time and then stop the thread
	 * Blocking call - internally calls join() on the ticking thread
	 */
	void cancel()
	{
		if (m_tickingThread.joinable()) {
			m_tick = false;
			m_tickingThread.join();
		}
	}

private:
	void timer()
	{
		m_timerCallback->onStart();
		while (m_tick) {
			std::this_thread::sleep_for(std::chrono::seconds(m_intervalInSeconds));
			m_timerCallback->onTick();
		}
		m_timerCallback->onEnd();
	}

	bool m_tick = true;
	unsigned m_intervalInSeconds;
	std::thread m_tickingThread;
	std::unique_ptr<TimerCallback> m_timerCallback;
};

} // namespace WIF
