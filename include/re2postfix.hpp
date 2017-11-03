#ifndef RE2POSTFIX_HPP_
#define RE2POSTFIX_HPP_

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

void remove_pattern(std::string& s, std::string& p) { 
  std::string::size_type n = p.length();
  for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p))
      s.erase(i, n);
}

std::string pre_process(std::string in){
  std::vector<char> out;
  // If there is a [
  // Replace it with a (
  // Keep going until you find a ]
  bool open = false;
  char c;
  for (unsigned int i = 0; i<in.length(); i++){
    // If you come across a square backet, replace it with a curvy bracket
    c = in[i];
    // If you find closing square bracket and there has been no open square bracket, error.
    if (c == ']') {
      if (!open){
        return "Error: Mismatched square brackets.";
      }
      out.pop_back();
      out.push_back(')');
      open = false;
    }
    // If you find a open square bracket, set open to true, replace with curvy bracket
    else if (c == '[') {
      open = true;
      out.push_back('(');
    }
    else {
      out.push_back(c);
      if(open)
        out.push_back('|');
    }
  }
  std::string s(out.begin(), out.end());
  std::string p = "()";
  remove_pattern(s, p);
  return s;
}
// Convert infix to postfix function
std::string re2postfix(std::string re) {
  re = pre_process(re);
  // Create the std::stacks
  std::stack<char> opStack;
  std::vector<char> outQueue;
  std::stack<std::pair<int, int>> counter;

  int num_char = 0;
  int num_alt = 0;
  // For each character in the std::string
  for (unsigned int i = 0; i < re.length(); i++) {
    char c = re[i];
    switch (c) {
    // If the token is a character, then push it to the output queue.
    // If there is more than one of these, put a concat
    default:
      if (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      outQueue.push_back(c);
      num_char++;
      break;

    // If there have been two characters before, concatenate them
    // If the token is a left parenthesis (i.e. "("), raise the level of
    // num_alt and num_char and reset them
    case '(':
      if (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      counter.push(std::pair<int, int>(num_alt, num_char));
      num_alt = 0;
      num_char = 0;
      break;

    // Dump all binary operators that are waiting to be added to output
    case ')':
      if (counter.empty()){
        throw std::invalid_argument("Mismatched brackets ().");
      }
      while (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      while (num_alt > 0) {
        num_alt--;
        outQueue.push_back('|');
      }

      // Retreive previous values of num_alt and num_char before open brackets
      num_alt = counter.top().first;
      num_char = counter.top().second;
      counter.pop();
      num_char++;
      break;
    // For unary operators, just place them straight to the output
    case '+':
      outQueue.push_back(c);
      break;
    // For binary operators, push concats while there are concatentations to be
    // made and increment the number of alts to add at the end
    case '|':
      while (--num_char > 0) {
        outQueue.push_back('.');
      }
      num_alt++;
      break;
    }
  }

  // exhaust all remaining binary operators from the virtual std::stack
  if (!counter.empty()) {
    throw std::invalid_argument("Mismatched brackets ().");
  }
  
  while (--num_char > 0) {
    outQueue.push_back('.');
  }
  
  while (num_alt > 0) {
    outQueue.push_back('|');
    num_alt--;
  }
  // tranform outQueue std::vector into std::string
  std::string postfix(outQueue.begin(), outQueue.end());
  return postfix;
}

#endif
