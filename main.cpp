#include <iostream>
#include <random>
#include<ctime>
#include <iomanip>
#include <sstream>
#include<thread>
#include <fstream>
#include<iostream>
#include<vector>
extern "C" {
#include <sqlite3.h>
}



//using namespace std;

struct canSignal
{
    //Example signals: Speed (0–200 km/h), RPM (0–7000), Battery SOC (0–100%)
    //Represent each message as a struct with signal name, value, and timestamp.
    std::string sigName;
    int value;
    std::string timestamp;

};
std::vector<canSignal>vehData;
std::string getTime();
int getRandVal(int min, int max);
void writeToFile(std::vector<canSignal> &vehData);
void writeToDB(std::vector<canSignal> &vehData,char* filename);
void openDB(const char* filename);




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

void writeToFile(std::vector<canSignal>&vehData)
{
  std::ofstream myfile("canlog.csv");
  if(!myfile.is_open())
  {
    std::cerr<<"Fail failed to open\n";
  }
  myfile << "Timestamp,SOC,Speed,RPM\n";
  canSignal temp;
  for(size_t i=0;i<vehData.size();i=i+3)
  {
    temp=vehData[i];
    
    myfile<<vehData[i].timestamp<<",";
    myfile<<vehData[i+1].value<<",";
    myfile<<vehData[i+2].value<<",";
    myfile<<vehData[i+3].value<<"\n";
    
  }
  myfile.close();
 std::cout << "Data written to canlog.csv\n";

}

void writeToDB(std::vector<canSignal>&vehData, char* filename)
{
  
}

void openDB(const char* filename)
{
    sqlite3* db;
    int opened = sqlite3_open(filename, &db);
    if(opened)
    { 
        printf("Database could not be opened %s \n", sqlite3_errmsg(db)); // check if opening the database is successful
    } 
    else 
    {
        printf("opened database successfuly \n");
    }

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
    vehData.push_back(socSignal);
    vehData.push_back(speedSignal);
    vehData.push_back(rpmSignal);
    std::cout<<"veh data size: "<<vehData.size()<<std::endl;


    std::cout << socSignal.sigName << ": " << socSignal.value << " | Timestamp: " << socSignal.timestamp << "\n";
    std::cout << speedSignal.sigName << ": " << speedSignal.value<< " | Timestamp: " << speedSignal.timestamp << "\n";
    std::cout << rpmSignal.sigName << ": " << rpmSignal.value<< " | Timestamp: " << rpmSignal.timestamp << "\n";
    std::cout << "------------------------\n";
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    i++;


  }
  writeToFile(vehData);
  const char* filename = "db.sqlite3";
  openDB(filename);  
}