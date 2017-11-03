#ifndef PENNY_MACHINE_HPP_
#define PENNY_MACHINE_HPP_
/*
1. Put a penny into the machine
2. If there are any blank arrows, create a penny at the destination of the arrow
3. Whenever the machine reads an input character c, move all the pennies. If a
penny
is on a node with an outgoing arrow labeled c, move the penny along the arrow.
If
there is more than one such arrow, the penny clones itself and follows both
arrows. If a
penny is on a node with no arrow labeled c, remove the penny.
4.  When the input is all read, machine return 'Yes' if there are any pennys on
final nodes
 */

 
#include <list>
#include <string>
#include <vector>

using namespace std;

typedef class Node *nodePtr;

struct Arrow {
  bool traversable(const char &c1) const;
  char c;
  nodePtr target;
  bool epsilon() const;
};

class Node {
public:
  //~Node();
  Arrow a1;
  Arrow a2;
  int num;
  bool is_match() const;
};

struct Frag {
  nodePtr start;         // Points to the start state
  vector<Arrow *> alist; // list of all unconnected arrows
};

class Penny {
public:
  Penny(nodePtr &ptr);
  nodePtr current;
};
// Machine contains all the nodes and pennys
// a(a|b) - Create a state, create a state, go back one, create b state
class Machine {
public:
  Machine(std::string psfx);
  ~Machine();
  //! Start up the machine (place penny on the starting node)
  void start();
  //! Input a character to the machine.
  int input_char(char c);
  void reset();
  void move_epsilon();
  int get_loopcount() const { return loopcount; };
  int check_matches();

private:
  // Points to the start of the machine
  nodePtr slot;
  std::list<Penny> pennys;
  int loopcount;
  std::vector<Node *> memTracker;
};

#endif
