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
void writeToCSV(std::vector<canSignal> &vehData);
void insertRow(sqlite3* db, canSignal& soc, canSignal& speed, canSignal& rpm);
sqlite3* openDB(const char* filename);
void createTable(sqlite3* db);



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

void writeToCSV(std::vector<canSignal>&vehData)
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

sqlite3* openDB(const char* filename)
{
    sqlite3* db;
    if (sqlite3_open(filename, &db) != SQLITE_OK) 
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return db;
}

void createTable(sqlite3* db)
{
    const char* sqlCreate =
        "CREATE TABLE IF NOT EXISTS CANLOG ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "TIMESTAMP TEXT NOT NULL, "
        "SOC INT NOT NULL, "
        "SPEED INT NOT NULL, "
        "RPM INT NOT NULL);";

    if (sqlite3_exec(db, sqlCreate, nullptr, nullptr, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Table creation failed: " << sqlite3_errmsg(db) << std::endl;
    }
}



void insertRow(sqlite3* db, canSignal& soc, canSignal& speed, canSignal& rpm)
{
  std::ostringstream oss;
  oss << "INSERT INTO CANLOG (TIMESTAMP, SOC, SPEED, RPM) VALUES ("
      << "'" << soc.timestamp << "', "
      << soc.value << ", "
      << speed.value << ", "
      << rpm.value << ");";

  std::string sql = oss.str();
  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
  {
    std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
  }
}



int main() 
{
   const char* filename = "db.sqlite3";
  sqlite3* db = openDB(filename);
  if (!db) return 1;
  createTable(db);


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
    insertRow(db,socSignal,speedSignal,rpmSignal);

    std::cout << socSignal.sigName << ": " << socSignal.value << " | Timestamp: " << socSignal.timestamp << "\n";
    std::cout << speedSignal.sigName << ": " << speedSignal.value<< " | Timestamp: " << speedSignal.timestamp << "\n";
    std::cout << rpmSignal.sigName << ": " << rpmSignal.value<< " | Timestamp: " << rpmSignal.timestamp << "\n";
    std::cout << "------------------------\n";
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    i++;
    

  }
  writeToCSV(vehData);
  sqlite3_close(db);
  return 0;

}