#ifndef TEMPLATE_OCTREE_H
#define TEMPLATE_OCTREE_H

#include <iostream>
#include "AtomOcTreeNode.h"
#include <octomap/OccupancyOcTreeBase.h>

namespace octomap {
  // tree definition
  class AtomOcTree : public OccupancyOcTreeBase < AtomOcTreeNode > {

  public:
    /// Default constructor, sets resolution of leafs
    AtomOcTree(double resolution);
      
    /// virtual constructor: creates a new object of same type
    /// (Covariant return type requires an up-to-date compiler)
    AtomOcTree* create() const {return new AtomOcTree(resolution); }//changed to this->resolution else templates cause problems

    std::string getTreeType() const {return "AtomOcTree";}
   
    // set node dat at given key or coordinate. Replaces previous dat.
    AtomOcTreeNode* setNodeData(const OcTreeKey& key, const Handle& r);

    AtomOcTreeNode* setNodeData(const float& x, const float& y, 
                                 const float& z, const Handle& r) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return setNodeData(key,r);
    }
    // update inner nodes, sets dat to average child dat
    void updateInnerOccupancy(){}

    
  protected:
    ////void updateInnerOccupancyRecurs(AtomOcTreeNode<T>* node, unsigned int depth);

    /**
     * Static member object which ensures that this OcTree's prototype
     * ends up in the classIDMapping only once. You need this as a 
     * static member in any derived octree class in order to read .ot
     * files through the AbstractOcTree factory. You should also call
     * ensureLinking() once from the constructor.
     */
     
    class StaticMemberInitializer{
       public:
         StaticMemberInitializer() {
           AtomOcTree* tree = new AtomOcTree(0.1);
           AbstractOcTree::registerTreeType(tree);
         }
  
         /**
         * Dummy function to ensure that MSVC does not drop the
         * StaticMemberInitializer, causing this tree failing to register.
         * Needs to be called from the constructor of this octree.
         */
         void ensureLinking() {};
    }; 
    /// static member to ensure static initialization (only once)
    static StaticMemberInitializer atomOcTreeMemberInit;

  };

  //! user friendly output in format (r g b)
  //std::ostream& operator<<(std::ostream& out, AtomOcTreeNode::T const& c);
  ////std::ostream& operator<<(std::ostream& out, Handle const& c);

} // end namespace

#endif
