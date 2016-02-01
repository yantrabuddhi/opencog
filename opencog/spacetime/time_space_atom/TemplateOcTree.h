/*
 * OctoMap - An Efficient Probabilistic 3D Mapping Framework Based on Octrees
 * http://octomap.github.com/
 *
 * Copyright (c) 2009-2013, K.M. Wurm and A. Hornung, University of Freiburg
 * All rights reserved.
 * License: New BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef OCTOMAP_TEMPLATE_OCTREE_H
#define OCTOMAP_TEMPLATE_OCTREE_H


#include <iostream>
#include <octomap/OcTreeNode.h>
#include <octomap/OccupancyOcTreeBase.h>

namespace octomap {
  
  // node definition
  template <class T> 
  //T should have == and != operators functional, and blank default constructor and iostream should be overloaded too
  class TemplateOcTreeNode : public OcTreeNode {    
  public:
    /*
    class T {
    public:
    T() : r(255), g(255), b(255) {}
    T(unsigned char _r, unsigned char _g, unsigned char _b) 
      : r(_r), g(_g), b(_b) {}
      inline bool operator== (const T &other) const {
        return (r==other.r && g==other.g && b==other.b);
      }
      inline bool operator!= (const T &other) const {
        return (r!=other.r || g!=other.g || b!=other.b);
      }
      unsigned char r, g, b;
    };
	*/
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


    //void updateColorChildren();


    //TemplateOcTreeNode::T getAverageChildColor() const;
  
    // file I/O
    std::istream& readValue (std::istream &s);
    std::ostream& writeValue(std::ostream &s) const;
    
  protected:
    T dat;
  };


  // tree definition
  template <class T>
  class TemplateOcTree : public OccupancyOcTreeBase <TemplateOcTreeNode<T>> {

  public:
    /// Default constructor, sets resolution of leafs
    TemplateOcTree(double resolution);
      
    /// virtual constructor: creates a new object of same type
    /// (Covariant return type requires an up-to-date compiler)
    TemplateOcTree* create() const {return new TemplateOcTree(resolution); }

    std::string getTreeType() const {return "TemplateOcTree";}
   
    // set node dat at given key or coordinate. Replaces previous dat.
    TemplateOcTreeNode* setNodeData(const OcTreeKey& key, const T& r);

    TemplateOcTreeNode* setNodeData(const float& x, const float& y, 
                                 const float& z, const T& r) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return setNodeData(key,r);
    }
    // update inner nodes, sets dat to average child dat
    void updateInnerOccupancy();

    
  protected:
    void updateInnerOccupancyRecurs(TemplateOcTreeNode* node, unsigned int depth);

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
           TemplateOcTree* tree = new TemplateOcTree(0.1);
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
    static StaticMemberInitializer templateOcTreeMemberInit;

  };

  //! user friendly output in format (r g b)
  //std::ostream& operator<<(std::ostream& out, TemplateOcTreeNode::T const& c);
  std::ostream& operator<<(std::ostream& out, T const& c);

} // end namespace

#endif
