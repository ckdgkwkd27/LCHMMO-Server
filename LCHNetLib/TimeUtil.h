#pragma once
class TimeUtil
{
public:
	static char* GetCurrentTime()
	{
		auto currentTime = std::chrono::system_clock::now();
		static char buffer[80];

		auto transformed = currentTime.time_since_epoch().count() / 1000000;

		auto millis = transformed % 1000;

		std::time_t tt;
		tm timeinfo = {};
		tt = system_clock::to_time_t(currentTime);
		localtime_s(&timeinfo, &tt);
		strftime(buffer, 80, "%F %H:%M:%S", &timeinfo);
		sprintf_s(buffer, "%s:%03d", buffer, (int)millis);
		return buffer;
	}
};

