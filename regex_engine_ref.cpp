#include <iostream>
#include <string>
#include <regex>

int main(int argc, char *argv[])

{
    // Read regex and subtituion string
    std::regex rgx;

    if (argc==1){
      std::cerr << "No regex string provided." << std::endl;
      return 1;
    }
    if (argc==2){
      rgx = argv[1];
    }
    
    else {
      std::cerr << "Too many arguments." << std::endl;
      return 1;
    }
    
    std::string tmp;
    while(std::getline(std::cin, tmp) ){
      //   Apply the regex and print out the new string
      if (std::regex_match(tmp,rgx)) {
        std::cout << "Match" << std::endl;
      }
      else {
        std::cout << "NoMatch" << std::endl;
      }
    }
    return 0;
}
