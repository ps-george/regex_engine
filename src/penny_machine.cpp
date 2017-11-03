
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

#include "penny_machine.hpp"
#include <algorithm>
// include <iostream>
#include <list>
#include <stack>
#include <string>
#include <vector>
#include <stdexcept>   

using namespace std;

// Create list of one arrow
vector<Arrow *> list1(Arrow *a) {
  vector<Arrow *> v;
  v.push_back(a);
  return v;
}

void route(vector<Arrow *> &alist, nodePtr t1) {
  for (unsigned int i = 0; i < alist.size(); i++) {
    (*alist[i]).target = t1;
  }
}

vector<Arrow *> join(vector<Arrow *> &alist1, vector<Arrow *> &alist2) {
  for (unsigned int i = 0; i < alist2.size(); i++) {
    alist1.push_back(alist2[i]);
  }
  return alist1;
}

bool Arrow::epsilon() const {
  if (c == ' ')
    return true;
  else
    return false;
}

bool Node::is_match() const {
  if (!num)
    return true;
  else
    return false;
};

Penny::Penny(nodePtr &ptr) : current(ptr) {}

bool Arrow::traversable(const char &c1) const {
  if ((c == c1) | (c == ' '))
    return true;
  return false;
}

Machine::~Machine() {
  pennys.clear();

  std::vector<nodePtr> deleted;
  for (auto i = memTracker.begin(); i != memTracker.end(); ++i) {
    // If node has already been deleted, don't delete it again
    if (std::find(deleted.begin(), deleted.end(), *i) != deleted.end()) {
      continue;
    } else {
      delete *i;
      deleted.push_back(*i);
    }
  }

  memTracker.clear();
}

Machine::Machine(string psfx) {
  loopcount = 0;
  stack<Frag> fragStack;
  Frag f1, f2, tmpf;
  Node *tmp;
  for (unsigned int i = 0; i < psfx.length(); i++) {
    char c = psfx[i];
    switch (c) {
    // Default, token is a character. Create a Char node and push onto the stack
    default:
      // Create a start node, pointing to nothing
      tmp = new Node;
      tmp->a1 = Arrow({c, NULL});
      tmp->num = 1;
      memTracker.push_back(tmp);

      // Create a fragment with start = &tmp and added ptr to arrow to the list
      // tmpf
      fragStack.push(Frag({tmp, list1(&(tmp->a1))}));
      break;
    // Token is a concatentation token
    // Pop the top two items from the stack and join them together
    case '.':
      f2 = fragStack.top();
      fragStack.pop();
      f1 = fragStack.top();
      fragStack.pop();
      // Connect all the arrows in f1.alist to f2.start
      route(f1.alist, f2.start);
      fragStack.push(Frag({f1.start, f2.alist}));
      break;
    // Token is a Kleene Cross
    // Pop the first item off the stack, create a Node pointing to the start of
    // the fragment and point all the arrows of the fragment to that node
    case '+':
      if (fragStack.empty()) {
        throw std::invalid_argument("+ applied to nothing.");
      }
      f1 = fragStack.top();
      fragStack.pop();
      // For now, we will use space as epsilon, and maybe change it later.
      tmp = new Node({Arrow({' ', NULL}), Arrow({' ', f1.start}), 2});
      memTracker.push_back(tmp);

      route(f1.alist, tmp);
      fragStack.push(Frag({f1.start, list1(&(tmp->a1))}));
      break;
    // Token is an Alternation
    // pop the top two items from the stack
    // create a new node pointing to each of the popped off fragments
    // push a new fragment with all the outwards arrows of the two previoius
    // fragments combine
    case '|':
      if (fragStack.size()<2) {
        throw std::invalid_argument("| applied but not sufficient arguments to it apply to.");
      }
      f2 = fragStack.top();
      fragStack.pop();
      f1 = fragStack.top();
      fragStack.pop();
      tmp = new Node({Arrow({' ', f1.start}), Arrow({' ', f2.start}), 2});
      memTracker.push_back(tmp);
      fragStack.push(Frag({tmp, join(f1.alist, f2.alist)}));
    }
  }
  f1 = fragStack.top();
  fragStack.pop();
  // Connect all unconnected arrows to a matching state
  Node *match = new Node({});
  memTracker.push_back(tmp);

  route(f1.alist, match);
  slot = f1.start;
}

int Machine::check_matches() {
  int matches = 0;
  for (auto i = pennys.begin(); i != pennys.end(); ++i) {
    if ((*i).current->is_match()) {
      matches++;
    }
  }
  return matches;
}

void Machine::move_epsilon() {
  // For each penny in the list
  loopcount++;
  int erased = 0;
  for (auto it1 = pennys.begin(); it1 != pennys.end();) {
    // Get its current node
    erased = 0;
    nodePtr tmp = (*it1).current;
    if (tmp->num == 0) {
      ++it1;
      continue;
    }

    if (tmp->num > 0) {
      // If the arrows are epsilon arrows and don't point to themselves, move
      // penny
      if (tmp->a1.epsilon()) {
        if ((tmp->a1).target != tmp) {
          pennys.push_back(Penny((tmp->a1).target));
          // Delete the penny?
          it1 = pennys.erase(it1);
          erased++;
        }
      }
      if (tmp->num == 2) {
        if ((tmp->a2).target != tmp) {
          pennys.push_back(Penny((tmp->a2).target));
          // Delete the penny?
          if (!erased) {
            it1 = pennys.erase(it1);
          }
        }
      }
    }
    ++it1;
  }
  // Only go once, but add cloned pennys to the end of the list;
}

void Machine::start() {
  loopcount=0;
  // destroy all pennys
  pennys.clear();
  // Create a penny at the start node
  pennys.push_back(Penny(slot));
}

int Machine::input_char(char c) {
  // For each penny in the machine, move all the pennys
  int matches = 0;
  int traversed = 0;

  //  If we wanted to match substrings we'd put a penny in each time
  //  pennys.push_back(Penny(slot));
  move_epsilon();

  for (auto it = pennys.begin(); it != pennys.end();) {
    loopcount++;
    // For each penny, get the current node of the penny
    nodePtr tmp = (*it).current;

    // If the node has 0 arrows, delete the penny and go to next penny
    if (((*tmp).num) == 0) {
      matches++;
      it = pennys.erase(it);
      continue;
    }
    // if there are no arrows on the node, erase the penny
    traversed = 0;
    // for each arrow coming out of the node, check if it is traversable
    if (tmp->a1.traversable(c)) {
      // Erase penny
      it = pennys.erase(it);
      // Clone penny at target
      pennys.insert(it, Penny((tmp->a1).target));
      traversed++;
    }

    if (tmp->num == 2) {
      if (tmp->a2.traversable(c)) {
        // If the previous arrow wasn't traversed, erase this penny
        if (!traversed) {
          it = pennys.erase(it);
        }
        // Clone penny at target, it points to penny after
        pennys.insert(it, Penny((tmp->a2).target));
        traversed++;
      }
    }
    // if nothing matched, delete the penny
    if (!traversed) {
      it = pennys.erase(it);
    }
    // cout << "Pennys moved = " << traversed << endl;
  }
  move_epsilon();
  return check_matches();
}

void Machine::reset() {
  // Clear all pennys
  pennys.clear();
}
