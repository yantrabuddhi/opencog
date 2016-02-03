#ifndef TEMPLATE_OCTREE_NODE_H
#define TEMPLATE_OCTREE_NODE_H

#include <iostream>
#include <octomap/OcTreeNode.h>

namespace octomap {
  typedef int Handle;
  // node definition
  class AtomOcTreeNode : public OcTreeNode {    
  public:
    AtomOcTreeNode() : OcTreeNode() {}

    AtomOcTreeNode(const AtomOcTreeNode& rhs) : OcTreeNode(rhs), dat(rhs.dat) {}

    bool operator==(const AtomOcTreeNode& rhs) const{
      return (rhs.value == value && rhs.dat == dat);
    }
    
    // children
    inline AtomOcTreeNode* getChild(unsigned int i) {
      return static_cast<AtomOcTreeNode*> (OcTreeNode::getChild(i));
    }
    inline const AtomOcTreeNode* getChild(unsigned int i) const {
      return static_cast<const AtomOcTreeNode*> (OcTreeNode::getChild(i));
    }

    bool createChild(unsigned int i) {
      if (children == NULL) allocChildren();
      children[i] = new AtomOcTreeNode();
      return true;
    }

    bool pruneNode();
    void expandNode();
    
    inline Handle getData() const { return dat; }
    inline void  setData(Handle c) {this->dat = c; }

    Handle& getData() { return dat; }

    // file I/O
    std::istream& readValue (std::istream &s);
    std::ostream& writeValue(std::ostream &s) const;
    
  protected:
    Handle dat;
  };

} // end namespace

#endif
