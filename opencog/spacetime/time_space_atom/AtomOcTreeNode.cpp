#include "AtomOcTreeNode.h"

namespace octomap {


  // node implementation  --------------------------------------
  std::ostream& AtomOcTreeNode::writeValue (std::ostream &s) const {
    // 1 bit for each children; 0: empty, 1: allocated
    std::bitset<8> children;
    for (unsigned int i=0; i<8; i++) {
      if (this->childExists(i)) children[i] = 1;
      else                children[i] = 0;
    }
    char children_char = (char) children.to_ulong();
    
    // write node data
    s.write((const char*) &value, sizeof(value)); // occupancy
    s.write((const char*) &dat, sizeof(Handle)); // dat
    s.write((char*)&children_char, sizeof(char)); // child existence

    // write existing children
    for (unsigned int i=0; i<8; ++i) 
      if (children[i] == 1) this->getChild(i)->writeValue(s);    
    return s;
  }

  std::istream& AtomOcTreeNode::readValue (std::istream &s) {
    // read node data
    char children_char;
    s.read((char*) &value, sizeof(value)); // occupancy
    s.read((char*) &dat, sizeof(Handle)); // dat
    s.read((char*)&children_char, sizeof(char)); // child existence

    // read existing children
    std::bitset<8> children ((unsigned long long) children_char);
    for (unsigned int i=0; i<8; i++) {
      if (children[i] == 1){
        createChild(i);
        getChild(i)->readValue(s);
      }
    }
    return s;
  }
  // pruning ============= mandeep: change dat here when pruned, depending on type of dat
  bool AtomOcTreeNode::pruneNode() {
    // checks for equal occupancy only, dat ignored
    if (!this->collapsible()) return false;
    // set occupancy value 
    setLogOdds(getChild(0)->getLogOdds());
    // set dat to average dat
    ////if (isColorSet()) dat = getAverageChildColor();//commented by mandeep
    dat=0;
    // delete children
    for (unsigned int i=0;i<8;i++) {
      delete children[i];
    }
    delete[] children;
    children = NULL;
    return true;
  }

  void AtomOcTreeNode::expandNode() {
    assert(!this->hasChildren());
    for (unsigned int k=0; k<8; k++) {
      this->createChild(k);
      this->children[k]->setValue(value);
      this->getChild(k)->setData(dat);
    }
  }

} // end namespace

