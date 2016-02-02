#ifndef TEMPLATE_OCTREE_NODE_H
#define TEMPLATE_OCTREE_NODE_H

#include <iostream>
#include <octomap/OcTreeNode.h>

namespace octomap {
  
  // node definition
  template <class T> 
  //T should have == and != operators functional, and blank default constructor and iostream should be overloaded too
  class TemplateOcTreeNode : public OcTreeNode {    
  public:
    TemplateOcTreeNode() : OcTreeNode() {}

    TemplateOcTreeNode(const TemplateOcTreeNode& rhs) : OcTreeNode(rhs), dat(rhs.dat) {}

    bool operator==(const TemplateOcTreeNode& rhs) const{
      return (rhs.value == value && rhs.dat == dat);
    }
    
    // children
    inline TemplateOcTreeNode* getChild(unsigned int i) {
      return static_cast<TemplateOcTreeNode*> (OcTreeNode::getChild(i));
    }
    inline const TemplateOcTreeNode* getChild(unsigned int i) const {
      return static_cast<const TemplateOcTreeNode*> (OcTreeNode::getChild(i));
    }

    bool createChild(unsigned int i) {
      if (children == NULL) allocChildren();
      children[i] = new TemplateOcTreeNode();
      return true;
    }

    bool pruneNode();
    void expandNode();
    
    inline T getData() const { return dat; }
    inline void  setData(T c) {this->dat = c; }

    T& getData() { return dat; }

    // file I/O
    std::istream& readValue (std::istream &s);
    std::ostream& writeValue(std::ostream &s) const;
    
  protected:
    T dat;
  };

} // end namespace

#endif
