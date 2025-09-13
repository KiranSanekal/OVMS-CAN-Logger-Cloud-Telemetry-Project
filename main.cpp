#include <iostream>
#include <random>
#include<ctime>
#include <iomanip>
#include <sstream>
#include<thread>
//using namespace std;

struct canSignal
{
    //Example signals: Speed (0–200 km/h), RPM (0–7000), Battery SOC (0–100%)
    //Represent each message as a struct with signal name, value, and timestamp.
    std::string sigName;
    int value;
    std::string timestamp;

};

std::string getTime();
int getRandVal(int min, int max);



std::string getTime()
{
  std::time_t rawtime;
  std::time(&rawtime); // Pass the address of rawtime which has seconds
  std::tm localtime = *std::localtime(&rawtime);
  std::ostringstream oss;
  oss << std::put_time(&localtime, "%Y-%m-%d %H:%M:%S");  // write formatted time into stream
  return oss.str();
}

int getRandVal(int min, int max)
{
  // Initialize a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max); // Generate random number in the range [min, max]
    int randomValue = distrib(gen);
    return randomValue;

}







int main() 

{
  int i=0;
  while(i<10)
  {
    std::string timestamp=getTime();
    canSignal socSignal{"SOC",getRandVal(0,100), timestamp};
    canSignal speedSignal{"Speed",getRandVal(0,200),timestamp};
    canSignal rpmSignal{"RPM",getRandVal(0,2000), timestamp};

    std::cout << socSignal.sigName << ": " << socSignal.value << " | Timestamp: " << socSignal.timestamp << "\n";
    std::cout << speedSignal.sigName << ": " << speedSignal.value<< " | Timestamp: " << speedSignal.timestamp << "\n";
    std::cout << rpmSignal.sigName << ": " << rpmSignal.value<< " | Timestamp: " << rpmSignal.timestamp << "\n";
    std::cout << "------------------------\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    i++;


  }
  


   return 0;
}