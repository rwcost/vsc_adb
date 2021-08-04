/* put header information here */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Generator
{
private:

    /* data */
public:
    int plantId;
    int id;
    string status;
    Generator();
    ~Generator();
};

Generator::Generator()
{

}

Generator::~Generator()
{
}

class Reactor : Generator
{
private:
    int temp;

    /* data */
public:
    Reactor(int tempIn, int plantIdIn, int idIn, string statusIn);
    ~Reactor();
};

Reactor::Reactor(int tempIn, int plantIdIn, int idIn, string statusIn) : Generator()
{
    this->temp = tempIn;
    this->plantId = plantIdIn;
    this->id = idIn;
}

Reactor::~Reactor()
{
}

class Turbine : Generator
{
private:
    int rpm;

    /* data */
public:
    Turbine(int rpmIn, int plantIdIn, int idIn, string statusIn);
    ~Turbine();
};

Turbine::Turbine(int rpmIn, int plantIdIn, int idIn, string statusIn)
{
    this->rpm = rpmIn;
}

Turbine::~Turbine()
{
}

class Plant
{

public: Plant() {
}


private:

    /* data */
public:
    int id;
    string name;
    vector<Reactor> plantReactors;
    vector<Turbine> plantTurbines;

    Plant(int id, string nameIn);
    ~Plant();
};



Plant::Plant(int idIn, string nameIn)
{
    this->id = idIn;
    this->name = nameIn;
}


//Plant::Plant()
//{
//}

Plant::~Plant()
{
}

class Utils
{
public:
    vector<Plant>::iterator itSort;
    bool done = false;
    void readFile(string filename, vector<string>& fileToMem);
    int getInteger(string& temp, string searchString, int pos, int length);
    string getString(string temp);
    void sort(vector<Plant>* plantList);

};
void Utils::readFile(string filename, vector<string>& fileToMem)
{  // Method/function defined inside the class
    // open a file in read mode.
    ifstream infile(filename);
    if (infile.is_open())
    {
        string readline;
        while (getline(infile, readline))
        {
            //cout << readline << endl;
            fileToMem.push_back(readline);
        }
        infile.close();
    }
}

// getinteger
int Utils::getInteger(string& temp, string searchString, int pos, int length)
{
    if (temp.find(searchString) != -1)
    {
        return(stoi(temp.substr(temp.find(searchString) + pos, length)));
    }
    else 
    {
        return(-9999);
    }
}

// getString
   string Utils::getString(string temp)
{
    int tempIdxStart = temp.find("\"");
    int tempIdxStop = temp.rfind("\"");
    return(temp.substr(tempIdxStart, tempIdxStop - tempIdxStart));
}
  

// sort
   void Utils::sort(vector<Plant>* plantList)
   {
       for (vector<Plant>::iterator itSort = plantList->begin(); itSort < plantList->end()-1; itSort++)
       { 
           int leftNum = itSort->id;
           itSort++;
           int rightNum = itSort->id;   
           if (leftNum < rightNum) {
               done = true;
           }
           if (leftNum > rightNum) 
           {
               itSort--;
               Plant leftPlantStorage = *itSort; //put left plant in temp plant storage
               itSort++;
               Plant rightPlant = *itSort; //put right plant in temp plant storage
               itSort--;
               *itSort = rightPlant;
               itSort++;
               *itSort = leftPlantStorage;
               leftPlantStorage = rightPlant;
               itSort--;
           }
           else
           {
               itSort--;
           }
       } 
       if (!done) {
           sort(plantList);
       }

   }
//***********************************************************************************************************
      int main()
      {
          /* read file into memory and close file */
          vector<string> fileToMem;

          /* this is the list of plants */
          vector<string> plantStringList;

          /* this is the list of plants */
          vector<Plant>* plantList = new vector<Plant>();

          /* this is the list of generators (reactors and turbines) */
          vector<string> generatorList;

          /* this is the list of generators (reactors and turbines) */
          vector<string> thresholdsList;

          vector<std::string>::iterator it;
          /* step 1. read in file by line*/

          vector<Plant>::iterator it_plant;
          /* step 1. read in file by line*/

          Plant myPlant;
          string temp;
          int status;

          Utils myUtil;
          myUtil.readFile("powerGrid.txt", fileToMem);


          /* This loop will put each of the strings into an array of
            plants, generators or threshholds
          */
          for (it = fileToMem.begin(); it < fileToMem.end(); it++)
          {
              temp = *it;
              if (temp.find("PowerGrid") != -1)
              {
                  status = 1;
              }
              if (temp.find("Generators") != -1)
              {
                  status = 2;
              }
              if (temp.find("Thresholds") != -1)
              {
                  status = 3;
              }
              //
              switch (status)
              {
              case 1:plantStringList.push_back(temp); break;
              case 2: generatorList.push_back(temp); break;
              case 3: thresholdsList.push_back(temp); break;
              }
              //    
          }
          // remove the first string which is the name of the items
          plantStringList.erase(plantStringList.begin());
          generatorList.erase(generatorList.begin());
          thresholdsList.erase(thresholdsList.begin());

          // display the array of vector strings 
          for (it = plantStringList.begin(); it < plantStringList.end(); it++)
          {
              temp = *it;
              if (!(temp.compare("") == 0))
              {
                  // seach the string line and return an integer id number
                  int id = myUtil.getInteger(temp, "id=", 3, 2);
                  //search the string for the "xxx" name in quotes
                  plantList->push_back(Plant(id, (myUtil.getString(temp))));
              }
          }//end for

           // display the array of vector strings 
          for (it_plant = plantList->begin(); it_plant < plantList->end(); it_plant++)
          {
              // search, create, destroy reactors in array
              for (it = generatorList.begin(); it < generatorList.end(); it++)
              {
                  temp = *it;
                  if (temp.find("Reactor") != -1)
                  {
                      int tempPlantId = myUtil.getInteger(temp, "plantId=", 8, 2);
                      if (tempPlantId == it_plant->id)
                      {
                          // get reactor parameters from line

                          int rt = myUtil.getInteger(temp, "Temp=", 5, 2);
                          int rpi = myUtil.getInteger(temp, "plantId=", 8, 2);
                          int ri = myUtil.getInteger(temp, "id=", 3, 2);
                          string status = myUtil.getString(temp);
                          it_plant->plantReactors.push_back(Reactor(rt, rpi, ri, status));
                      }
                  }
                  else
                  {
                      int tempPlantId = myUtil.getInteger(temp, "plantId=", 8, 2);
                      if (tempPlantId == it_plant->id)
                      {
                          // get reactor parameters from line
                          int rt = myUtil.getInteger(temp, "Rpm=", 4, 2);
                          int rpi = myUtil.getInteger(temp, "plantId=", 8, 2);
                          int ri = myUtil.getInteger(temp, "id=", 3, 2);
                          string status = myUtil.getString(temp);
                          it_plant->plantTurbines.push_back(Turbine(rt, rpi, ri, status));
                      }
                  }              

              }

          }

          for (it_plant = plantList->begin(); it_plant < plantList->end(); it_plant++)
          {
              myUtil.sort(plantList);
          }

          
          for (it_plant = plantList->begin(); it_plant < plantList->end(); it_plant++)
          {
            int tempPlantId = it_plant->id;
            string tempName = it_plant->name;

            cout << tempName << " " << tempPlantId << endl;
          }

      }
