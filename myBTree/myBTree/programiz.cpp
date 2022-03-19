////
////  main.cpp
////  myBTree
////
////  Created by xiaoyuyu on 2022/3/18.
////  Copyright © 2022 xiaoyuyu. All rights reserved.
////
//
//// 网上找来的代码：https://www.programiz.com/dsa/b-tree
//// 基本上大部分代码，都没有涉及到B Tree的节点删除，还是以插入和查询为主，本代码也是如此
//// 添加一下注释，熟悉一下
//
//// Searching a key on a B-tree in C++
//
//#include <iostream>
//using namespace std;
//
//// 节点结构
//class TreeNode {
//  int* keys;    // 存放所有关键字
//  int t;    // 用t来控制树的阶，n == 2 * t - 1 => t = (n + 1) / 2，当t = 2，n = 3，阶为2 * t - 1
//  TreeNode** C; // C is TreeNode**，可以理解为一个Treenode[]，里面的每个元素都是*TreeNode
//  int n;    // 关键字个数，最多可以容纳n - 1个关键字
//  bool leaf;    // 判断是否位叶子节点，true代表是叶子节点
//
//public:
//  TreeNode(int temp, bool bool_leaf);
//
//  void insertNonFull(int k);
//  void splitChild(int i, TreeNode *y);
//  void traverse();
//
//  TreeNode *search(int k);
//
//  friend class BTree;
//};
//
//// 树结构
//class BTree {
//  TreeNode *root;
//  int t;
//
//   public:
//  BTree(int temp) {
//    root = NULL;
//    t = temp;
//  }
//
//  void traverse() {
//    if (root != NULL)
//      root->traverse();
//  }
//
//  // 查找值为k的节点，并返回节点的地址
//  TreeNode* search(int k) {
//    return (root == NULL) ? NULL : root->search(k);
//  }
//
//  void insert(int k);
//};
//
//TreeNode::TreeNode(int t1, bool leaf1) {
//  t = t1;
//  leaf = leaf1;
//
//  keys = new int[2 * t - 1];
//  C = new TreeNode *[2 * t];
//
//  n = 0;
//}
//
//// 遍历并输出所有节点的值
//void TreeNode::traverse() {
//  int i;
//  for (i = 0; i < n; i++) {
//    if (leaf == false)
//      C[i]->traverse();
//    cout << " " << keys[i];
//  }
//
//  if (leaf == false)
//    C[i]->traverse();
//}
//
//TreeNode* TreeNode::search(int k) {
//  int i = 0;
//  // 找到第一个不小于等于key的位置
//  // keys[i] 相当于*(keys + i)
//  while (i < n && k > keys[i])
//    i++;
//  // 如果是等于，已经找到对应节点
//  if (keys[i] == k)
//    return this;
//  // 是叶子节点，还比k大，本树中不存在该节点
//  if (leaf == true)
//    return NULL;
//  // 从自己的孩子节点里递归搜索
//  return C[i]->search(k);
//}
//
//// 插入一个节点
//void BTree::insert(int k) {
//  // 树为空，创建根节点
//  if (root == NULL) {
//    root = new TreeNode(t, true);
//    root->keys[0] = k;
//    root->n = 1;
//  } else {
//    if (root->n == 2 * t - 1) {
//      // 达到存储的节点的个数上限，需要分裂
//      TreeNode *s = new TreeNode(t, false); // 创建新的root，且不是叶子节点
//
//      s->C[0] = root;   // C is TreeNode**，可以理解为一个Treenode[]，里面的每个元素都是*TreeNode
//
//      s->splitChild(0, root);
//
//      int i = 0;
//      if (s->keys[0] < k)
//        i++;
//      // 找到合适的位置并插入节点
//      s->C[i]->insertNonFull(k);
//
//      root = s; // root更新为s
//    } else
//      root->insertNonFull(k);
//  }
//}
//
//void TreeNode::insertNonFull(int k) {
//  int i = n - 1;
//
//  if (leaf == true) {
//    // 如果是叶子节点，直接插入即可
//    while (i >= 0 && keys[i] > k) {
//      keys[i + 1] = keys[i];
//      i--;
//    }
//
//    keys[i + 1] = k;
//    n = n + 1;
//  } else {
//    // 非叶子节点，要在子树上找到合适的插入位置
//    while (i >= 0 && keys[i] > k)
//      i--;
//
//    if (C[i + 1]->n == 2 * t - 1) {
//      // 已达关键字个数上限
//      splitChild(i + 1, C[i + 1]);
//
//      if (keys[i + 1] < k)
//        i++;
//    }
//    C[i + 1]->insertNonFull(k);
//  }
//}
//
//void TreeNode::splitChild(int i, TreeNode *y) {
//  TreeNode *z = new TreeNode(y->t, y->leaf);
//  z->n = t - 1;
//
//  for (int j = 0; j < t - 1; j++)
//    z->keys[j] = y->keys[j + t];
//
//  if (y->leaf == false) {
//    for (int j = 0; j < t; j++)
//      z->C[j] = y->C[j + t];
//  }
//
//  y->n = t - 1;
//  for (int j = n; j >= i + 1; j--)
//    C[j + 1] = C[j];
//
//  C[i + 1] = z;
//
//  for (int j = n - 1; j >= i; j--)
//    keys[j + 1] = keys[j];
//
//  keys[i] = y->keys[t - 1];
//  n = n + 1;
//}
//
////int main() {
////  BTree t(3);
////  t.insert(8);
////  t.insert(9);
////  t.insert(10);
////  t.insert(11);
////  t.insert(15);
////  t.insert(16);
////  t.insert(17);
////  t.insert(18);
////  t.insert(20);
////  t.insert(23);
////
////  cout << "The B-tree is: ";
////  t.traverse();
////
////  int k = 10;
////  (t.search(k) != NULL) ? cout << endl
////                 << k << " is found"
////              : cout << endl
////                 << k << " is not Found";
////
////  k = 2;
////  (t.search(k) != NULL) ? cout << endl
////                 << k << " is found"
////              : cout << endl
////                 << k << " is not Found\n";
////}
