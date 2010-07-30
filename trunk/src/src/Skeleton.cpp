//c++ -o TreeRipper TreeRipper.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs`
#include <Magick++.h>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>

//#include "ManipImg.h"
using namespace std;
using namespace Magick;

 void print_answer(int answer){
   if (answer<0){
     std::cout << "Answer corrupt\n";
     return;
   }
   std::cout << "The answer is " << answer << '\n';
 }


int main(int argc,char **argv) {
	if (argc<3){
		std::cout << "Usage: mprog.exe -f <filename>" << std::endl;
		return 1;
	}
	std::string parm = argv[1];
	if (parm != "-f"){
		std::cout << "Usage: mprog.exe -f <filename>" << std::endl;
		return 1;
	}
	std::string filename=argv[2];		
	try {
		time_t start,end,begin;
		time (&begin);
        Image master(filename);// IpsPage09.jpg ImageColor.jpg LandPlant4.jpg
        Image second = master;
          int cols= master.columns();
  int rows= master.rows();
  std::cout << "Image width: " << cols << endl;
  std::cout << "Image length: " << rows << endl;
        Image splitImg(second);
          cols= second.columns();
  rows= second.rows();
  std::cout << "Image width: " << cols << endl;
  std::cout << "Image length: " << rows << endl;
   print_answer(3);
		std::ofstream log("./tmp.log", std::ios::binary|std::ios::out);        
		// get size of image

	}
	catch( exception &error_ )
    {
    	cout << "Caught exception: " << error_.what() << endl;
      	return 1;
    }
	return 0;
}

char splitImg(Image original) {
  int cols= original.columns();
  int rows= original.rows();
  std::cout << "Image width: " << cols << endl;
  std::cout << "Image length: " << rows << endl;
  original.scale( "10%" ); 
  return(original);
 };
