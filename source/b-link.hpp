// Copyright
#ifndef SOURCE_B_LINK_HPP_
#define SOURCE_B_LINK_HPP_

#include <utility>
#include <vector>
#include <algorithm>
template <class T>
bool comparison(T* a,T* b) { return *a<*b;}
namespace EDA {
namespace Concurrent {
template <std::size_t B,typename Type>
class BlinkNode{
public:
    BlinkNode(){
        is_leaf= true;
        m=B;
    }
    ~BlinkNode(){}
    typedef Type data_type;
    std::vector<data_type*> keys;
    std::vector<BlinkNode*> pointers;
    bool is_leaf;
    BlinkNode* linked_list;
    int m;
    void insert(const data_type& value){
        auto* p= new data_type;
        *p=value;
        keys.push_back(p);
    }
};
template <std::size_t B, typename Type>
class BLinkTree {
 public:
  typedef Type data_type;
  BLinkTree() {
      root=NULL;
  }
  ~BLinkTree() {}

  std::size_t size() const {}

  bool empty() const {
      if(!root)
          return true;
      else if(root->keys.empty())
          return true;
      return false;
  }
  bool search(const data_type& value) const {
      if (empty())
          return false;
      BlinkNode<B,data_type>* leafy;
      search_no_path(leafy,value);
      leafy = root;
      for(int i=0;i<leafy->keys.size();i++)
      {
          if(*leafy->keys[i]==value)
              return true;
      }
      return false;
  }
  void insert(const data_type& value) {
      if(root==NULL)
      {
          root= new BlinkNode<B,data_type>;
          root->insert(value);
          return;
      }
      data_type curr_value=value;
      std::vector<BlinkNode<B,data_type>*> traversal;
      BlinkNode<B,data_type>* leafy;
      search_w_path(leafy, traversal, value);
      BlinkNode<B,data_type>* to_be;
      while(true)
      {
          if(leafy->keys.size()==(leafy->m)-1)//there_is_an_overflow
          {
              int goes_up=((leafy->m)/2);
              if(leafy->m%2==0)//is_even
                  goes_up++;
              auto* p= new data_type;
              *p=curr_value;
              leafy->keys.push_back(p);
              std::sort(leafy->keys.begin(),leafy->keys.end(),comparison<data_type>);
              if(leafy->is_leaf)//if node is a leaf
              {
                  auto* temp = new BlinkNode<B,data_type>;
                  std::vector<data_type*> temp_1;
                  std::vector<data_type*> temp2;
                  int count_aux=goes_up;
                  curr_value=*leafy->keys[goes_up];
                  while((leafy->m)>count_aux)
                  {
                      temp_1.push_back(leafy->keys[count_aux]);
                      count_aux++;
                  }
                  temp->keys=temp_1;
                  count_aux=0;
                  while(count_aux<goes_up)
                  {
                      temp2.push_back(leafy->keys[count_aux]);
                      count_aux++;
                  }
                  leafy->keys=temp2;
                  leafy->linked_list=temp;
                  BlinkNode<B,data_type>* inmediate_next;
                  std::vector<BlinkNode<B,data_type>*> unnecessary;
                  search_w_path(inmediate_next, unnecessary, curr_value + 1);
                  if(leafy!=inmediate_next)
                      leafy->linked_list=inmediate_next;
                  if(leafy=root)
                  {
                      auto* temp_root = new BlinkNode<B,data_type>;
                      temp_root->insert(curr_value);
                      temp_root->pointers.push_back(leafy);
                      temp_root->pointers.push_back(temp);
                      return;
                  }
                  to_be=temp;
                  leafy=traversal.back();
                  traversal.pop_back();
              }
              else //if node isn't a leaf
              {
                  //Reordering pointers, it's done first since
                  //the only case when we're inserting on a leaf is
                  //if there's an overflow
                  //auto it = find(leafy->keys.begin(),leafy->keys.end(),&curr_value); //I don't know how to make this work
                  //int index=it-leafy->keys.begin();
                  int index;
                  for(int i=0;i<leafy->keys.size();i++)
                  {
                      if(*leafy->keys[i]==curr_value)
                          index=i;
                  }
                  std::vector<BlinkNode<B,data_type>*> pun_de;
                  for(int i=0;i<leafy->pointers.size();i++)
                  {
                      if(i==index+1)
                          pun_de.push_back(to_be);
                      pun_de.push_back(leafy->pointers[i]);
                  }
                  auto* temp = new BlinkNode<B,data_type>;
                  std::vector<data_type*> temp_1;//keys-right
                  std::vector<data_type*> temp2;//keys-left
                  int count_aux=goes_up;
                  curr_value=*leafy->keys[goes_up];
                  temp->is_leaf=false;
                  count_aux++;
                  while((leafy->m)>count_aux)
                  {
                      temp_1.push_back(leafy->keys[count_aux]);
                      count_aux++;
                  }
                  temp->keys=temp_1;
                  count_aux=0;
                  while(count_aux<goes_up)
                  {
                      temp2.push_back(leafy->keys[count_aux]);
                      count_aux++;
                  }
                  leafy->keys=temp2;
                  leafy->linked_list=temp;
                  to_be=temp;
                  if(leafy=root)
                  {
                      auto* temp_root = new BlinkNode<B,data_type>;
                      temp_root->insert(curr_value);
                      temp_root->pointers.push_back(leafy);
                      temp_root->pointers.push_back(temp);
                      return;
                  }
                  leafy=traversal.back();
                  traversal.pop_back();

              }
          }
          else//there_is_no_overflow
          {
              auto* p= new data_type;
              *p=curr_value;
              leafy->keys.push_back(p);
              std::sort(leafy->keys.begin(),leafy->keys.end(),comparison<data_type>);
              //auto it = find(leafy->keys.begin(),leafy->keys.end(),&curr_value); //I don't know how to make this work
              //int index=it-leafy->keys.begin();
              if(!leafy->is_leaf)
              {
                  int index;
                  for(int i=0;i<leafy->keys.size();i++)
                  {
                      if(*leafy->keys[i]==curr_value)
                          index=i;
                  }
                  std::vector<BlinkNode<B,data_type>*> pun_de;
                  for(int i=0;i<leafy->pointers.size();i++)
                  {
                      if(i==index+1)
                          pun_de.push_back(to_be);
                      pun_de.push_back(leafy->pointers[i]);
                  }
              }
              return;
          }
      }
  }
  void remove(const data_type& value) {}

 private:
    BlinkNode<B,data_type>* root;


//  This function searches the tree for a value and stores leaf in which
//  such value needs to go and also stores it's traversal across the tree. Both
//  of these being received as reference arguments so that they can be used in further
//  operations.
  void search_w_path(BlinkNode<B,data_type>* &leafy, std::vector<BlinkNode<B,data_type>*> &traversal, data_type value) {
      leafy = root;
      while (!leafy->is_leaf) {
          int quan = leafy->keys.size();
          for (int i = 0; i < quan; i++) {
              if (value < *leafy->keys[i]) {
                  traversal.push_back(leafy);
                  leafy = leafy->pointers[i];
                  break;
              }
              if (i == quan - 1)
                  traversal.push_back(leafy);
                  leafy = leafy->pointers[i + 1];
          }
      }
  }
//  This function searches the tree for a value and stores leaf in which
//  such value needs to go.
void search_no_path(BlinkNode<B,data_type>* &leafy, data_type value)
    {
        leafy = root;
        while (!leafy->is_leaf) {
            int quan = leafy->keys.size();
            for (int i = 0; i < quan; i++) {
                if (value < *leafy->keys[i]) {
                    leafy = leafy->pointers[i];
                    break;
                }
                if (i == quan - 1)
                leafy = leafy->pointers[i + 1];
            }
        }
    }
};

}  // namespace Concurrent
}  // namespace EDA

#endif  // SOURCE_B_LINK_HPP_
