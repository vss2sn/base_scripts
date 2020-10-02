/*
Useful functions
*/

#include "communication/utils.hpp"
std::mutex display_mutex;

// For Debugging
#define ANSI_COLOUR_BLACK "\x1b[1;30m"
#define ANSI_COLOUR_RED "\x1b[1;31m"
#define ANSI_COLOUR_GREEN "\x1b[1;32m"
#define ANSI_COLOUR_YELLOW "\x1b[1;33m"
#define ANSI_COLOUR_BLUE "\x1b[1;34m"
#define ANSI_COLOUR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOUR_CYAN "\x1b[1;36m"
#define ANSI_COLOUR_WHITE "\x1b[1;37m"
#define ANSI_COLOUR_RESET "\x1b[1;0m"
LogDetailed::~LogDetailed()
{
		std::lock_guard<std::mutex> lock(display_mutex);
		switch (_log_level){
		case LogLevel::Debug:
			set_display_colour(Colour::GREEN);
			break;
		case LogLevel::Info:
			set_display_colour(Colour::BLUE);
			break;
		case LogLevel::Warn:
			set_display_colour(Colour::YELLOW);
			break;
		case LogLevel::Err:
			set_display_colour(Colour::RED);
			break;
		case LogLevel::Status:
			set_display_colour(Colour::WHITE);
			break;
		}

		if (_log_level == LogLevel::Status){
			std::cout << _s.str();
			set_display_colour(Colour::RESET);
			std::cout << std:: endl;
		}
		else{
			time_t rawtime;
			time(&rawtime);
			struct tm *timeinfo = localtime(&rawtime);
			char time_buffer[10]{};
			strftime(time_buffer, sizeof(time_buffer), "%I:%M:%S", timeinfo);
			std::cout << "[" << time_buffer;

			switch(_log_level){
			case LogLevel::Debug:
				std::cout << "|Debug ] ";
				break;
			case LogLevel::Info:
				std::cout << "|Info ] ";
				break;
			case LogLevel::Warn:
				std::cout << "|Warn ] ";
				break;
			case LogLevel::Err:
				std::cout << "|Error ] ";
				break;
			case LogLevel::Status:
				std::cout << "|Status ] ";
				break;
			}
			set_display_colour(Colour::RESET);
			std::cout << _s.str();
			std::cout << " |" << _caller_filename << ":" << _caller_filenumber << "|";
			std::cout << std::endl;
		}
	}


void set_display_colour(Colour colour){
	switch (colour){
	case Colour::BLACK:
		std::cout << ANSI_COLOUR_BLACK;
		break;
	case Colour::RED:
		std::cout << ANSI_COLOUR_RED;
		break;
	case Colour::GREEN:
		std::cout << ANSI_COLOUR_GREEN;
		break;
	case Colour::YELLOW:
		std::cout << ANSI_COLOUR_YELLOW;
		break;
	case Colour::BLUE:
		std::cout << ANSI_COLOUR_BLUE;
		break;
	case Colour::MAGENTA:
		std::cout << ANSI_COLOUR_MAGENTA;
		break;
	case Colour::CYAN:
		std::cout << ANSI_COLOUR_CYAN;
		break;
	case Colour::WHITE:
		std::cout << ANSI_COLOUR_WHITE;
		break;
	case Colour::RESET:
		std::cout << ANSI_COLOUR_RESET;
		break;
	}
}

std::string exec_system_cmd(const char* cmd){
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"), pclose);
	if(!pipe){
		std::cout << "Unable to run system call" + std::string(cmd);
		return "";
	}
	else{
		while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
			result += buffer.data();
		return result;
	}
}


#ifdef BUILD_INDIVIDUAL
int main(){
	//Example use:
	LogDebug() << "Green";
	LogInfo()  << "Blue";
	LogWarn()  << "Yellow";
	LogErr()   << "Red";
	LogStatus()<<	"White";
}
#endif
