#include "penny_machine.hpp"
#include "re2postfix.hpp"
#include <iostream>
#include <string>


int main(int argc, char *argv[]) {
  std::string re;
  std::string postfix;
  std::string in;
  int total = 0;
  // Get the regular expression
  // getline(cin,re);
  //
  // Get the regular expression from command lines
  if (argc == 1) {
    std::cerr << "No regex or subsitution string provided." << std::endl;
    return 1;
  }

  else if (argc == 2) {
    re = argv[1];
  } else {
    std::cerr << "Too many arguments provided." << std::endl;
    return 1;
  }
  // Convert it to postfix
  postfix = re2postfix(re);
  // cerr << postfix << endl;
  if (postfix.empty()){
    std::cerr << "Empty regex string." << std::endl;
    return 1;
  }
  // Construct the machine
  Machine m(postfix);
  // Each time there's a new line
  int matches = 0;
  while (getline(cin, in)) {
    // If there is no input, stop and print out statistics
    if (!in[0])
      break;
    total = 0;
    m.start();
    for (unsigned int i = 0; i < in.length(); i++) {
      matches = 0;
      // cout << i+1 << "th character: " << in[i] << endl;
      matches = m.input_char(in[i]);
      total += matches;
      // cout << "Matches: " << matches << " Total Matches thus far: " << total
      // << endl;
    }
    // last character must be a match
    if (matches > 0)
      cout << "Match" << endl;
    else
      cout << "NoMatch" << endl;
    cerr << "Total loops: " << m.get_loopcount() << endl;
  }
  return 0;
}
