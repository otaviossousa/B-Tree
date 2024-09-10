/* Pseudo code for B-tree search

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms (3rd ed.). MIT Press.
p. 406

B-Tree-Search(x, k)
1. i = 1
2. while i <= x.n and k > x.keyi
3.     i = i + 1
4. if i <= x.n and k == x.keyi
5.     return (x, i)
6. elseif x.leaf
7.     return NIL
8. else DISK-READ(x.ci)
9. return BTree-Search(x.ci, k)

*/

/* Pseudo code for B-tree create

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms (3rd ed.). MIT Press.
p. 406-407

B-Tree-Create(T)
1. x = ALLOCATE-NODE()
2. x.leaf = TRUE
3. x.n = 0
4. DISK-WRITE(x)
5. T.root = x

 */

/* Pseudo code for B-tree split child

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms (3rd ed.). MIT Press.
p. 407-408

B-Tree-Split-Child(x, i)
1. z = ALLOCATE-NODE()
2. y = x.ci
3. z.leaf = y.leaf
4. z.n = t - 1
5. for j = 1 to t - 1
6.     z.keyj = y.keyj+t
7. if not y.leaf
8.     for j = 1 to t
9.         z.cj = y.cj+t
10. y.n = t - 1
11. for j = x.n + 1 downto i + 1
12.     x.cj+1 = x.cj
13. x.ci+1 = z
14. for j = x.n downto i
15.     x.keyj+1 = x.keyj
16. x.keyi = y.keyt
17. x.n = x.n + 1
18. DISK-WRITE(y)
19. DISK-WRITE(z)
20. DISK-WRITE(x)

*/

/* Pseudo code for B-tree insert

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms (3rd ed.). MIT Press.
p. 409

B-Tree-Insert(T, k)
1. r = T.root
2. if r.n == 2t - 1
3.     s = ALLOCATE-NODE()
4.     T.root = s
5.     s.leaf = FALSE
6.     s.n = 0
7.     s.c1 = r
8.     B-Tree-Split-Child(s, 1)
9.     B-Tree-Insert-Nonfull(s, k)
10. else B-Tree-Insert-Nonfull(r, k)

 */

/* Pseudo code for B-tree insert nonfull

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms (3rd ed.). MIT Press.
p. 410

B-Tree-Insert-Nonfull(x, k)
1. i = x.n
2. if x.leaf
3.     while i >= 1 and k < x.keyi
4.         x.keyi+1 = x.keyi
5.         i = i - 1
6.     x.keyi+1 = k
7.     x.n = x.n + 1
8.     DISK-WRITE(x)
9. else while i >= 1 and k < x.keyi
10.       i = i - 1
11.     i = i + 1
12.     DISK-READ(x.ci)
13.     if x.ci.n == 2t - 1
14.         B-Tree-Split-Child(x, i, x.ci)
15.         if k > x.keyi
16.             i = i + 1
17.     B-Tree-Insert-Nonfull(x.ci, k)

 */


/* Pseudo code for B-tree delete

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms (3rd ed.). MIT Press.
p. 413

 */


/*
Fonte do código: https://www.programiz.com/dsa/deletion-from-a-b-tree
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

struct BTreeNode {
  int item[MAX + 1], count;
  struct BTreeNode *linker[MAX + 1];
};

struct BTreeNode *root;

// Node creation
struct BTreeNode *createNode(int item, struct BTreeNode *child) {
  struct BTreeNode *newNode;
  newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  newNode->item[1] = item;
  newNode->count = 1;
  newNode->linker[0] = root;
  newNode->linker[1] = child;
  return newNode;
}

// Add value to the node
void addValToNode(int item, int pos, struct BTreeNode *node,
          struct BTreeNode *child) {
  int j = node->count;
  while (j > pos) {
    node->item[j + 1] = node->item[j];
    node->linker[j + 1] = node->linker[j];
    j--;
  }
  node->item[j + 1] = item;
  node->linker[j + 1] = child;
  node->count++;
}

// Split the node
void splitNode(int item, int *pval, int pos, struct BTreeNode *node,
         struct BTreeNode *child, struct BTreeNode **newNode) {
  int median, j;

  if (pos > MIN)
    median = MIN + 1;
  else
    median = MIN;

  *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
  j = median + 1;
  while (j <= MAX) {
    (*newNode)->item[j - median] = node->item[j];
    (*newNode)->linker[j - median] = node->linker[j];
    j++;
  }
  node->count = median;
  (*newNode)->count = MAX - median;

  if (pos <= MIN) {
    addValToNode(item, pos, node, child);
  } else {
    addValToNode(item, pos - median, *newNode, child);
  }
  *pval = node->item[node->count];
  (*newNode)->linker[0] = node->linker[node->count];
  node->count--;
}

// Set the value in the node
int setValueInNode(int item, int *pval,
           struct BTreeNode *node, struct BTreeNode **child) {
  int pos;
  if (!node) {
    *pval = item;
    *child = NULL;
    return 1;
  }

  if (item < node->item[1]) {
    pos = 0;
  } else {
    for (pos = node->count;
       (item < node->item[pos] && pos > 1); pos--)
      ;
    if (item == node->item[pos]) {
      printf("Duplicates not allowed\n");
      return 0;
    }
  }
  if (setValueInNode(item, pval, node->linker[pos], child)) {
    if (node->count < MAX) {
      addValToNode(*pval, pos, node, *child);
    } else {
      splitNode(*pval, pval, pos, node, *child, child);
      return 1;
    }
  }
  return 0;
}

// Insertion operation
void insertion(int item) {
  int flag, i;
  struct BTreeNode *child;

  flag = setValueInNode(item, &i, root, &child);
  if (flag)
    root = createNode(i, child);
}

// Copy the successor
void copySuccessor(struct BTreeNode *myNode, int pos) {
  struct BTreeNode *dummy;
  dummy = myNode->linker[pos];

  for (; dummy->linker[0] != NULL;)
    dummy = dummy->linker[0];
  myNode->item[pos] = dummy->item[1];
}

// Remove the value
void removeVal(struct BTreeNode *myNode, int pos) {
  int i = pos + 1;
  while (i <= myNode->count) {
    myNode->item[i - 1] = myNode->item[i];
    myNode->linker[i - 1] = myNode->linker[i];
    i++;
  }
  myNode->count--;
}

// Do right shift
void rightShift(struct BTreeNode *myNode, int pos) {
  struct BTreeNode *x = myNode->linker[pos];
  int j = x->count;

  while (j > 0) {
    x->item[j + 1] = x->item[j];
    x->linker[j + 1] = x->linker[j];
  }
  x->item[1] = myNode->item[pos];
  x->linker[1] = x->linker[0];
  x->count++;

  x = myNode->linker[pos - 1];
  myNode->item[pos] = x->item[x->count];
  myNode->linker[pos] = x->linker[x->count];
  x->count--;
  return;
}

// Do left shift
void leftShift(struct BTreeNode *myNode, int pos) {
  int j = 1;
  struct BTreeNode *x = myNode->linker[pos - 1];

  x->count++;
  x->item[x->count] = myNode->item[pos];
  x->linker[x->count] = myNode->linker[pos]->linker[0];

  x = myNode->linker[pos];
  myNode->item[pos] = x->item[1];
  x->linker[0] = x->linker[1];
  x->count--;

  while (j <= x->count) {
    x->item[j] = x->item[j + 1];
    x->linker[j] = x->linker[j + 1];
    j++;
  }
  return;
}

// Merge the nodes
void mergeNodes(struct BTreeNode *myNode, int pos) {
  int j = 1;
  struct BTreeNode *x1 = myNode->linker[pos], *x2 = myNode->linker[pos - 1];

  x2->count++;
  x2->item[x2->count] = myNode->item[pos];
  x2->linker[x2->count] = myNode->linker[0];

  while (j <= x1->count) {
    x2->count++;
    x2->item[x2->count] = x1->item[j];
    x2->linker[x2->count] = x1->linker[j];
    j++;
  }

  j = pos;
  while (j < myNode->count) {
    myNode->item[j] = myNode->item[j + 1];
    myNode->linker[j] = myNode->linker[j + 1];
    j++;
  }
  myNode->count--;
  free(x1);
}

// Adjust the node
void adjustNode(struct BTreeNode *myNode, int pos) {
  if (!pos) {
    if (myNode->linker[1]->count > MIN) {
      leftShift(myNode, 1);
    } else {
      mergeNodes(myNode, 1);
    }
  } else {
    if (myNode->count != pos) {
      if (myNode->linker[pos - 1]->count > MIN) {
        rightShift(myNode, pos);
      } else {
        if (myNode->linker[pos + 1]->count > MIN) {
          leftShift(myNode, pos + 1);
        } else {
          mergeNodes(myNode, pos);
        }
      }
    } else {
      if (myNode->linker[pos - 1]->count > MIN)
        rightShift(myNode, pos);
      else
        mergeNodes(myNode, pos);
    }
  }
}

// Delete a value from the node
int delValFromNode(int item, struct BTreeNode *myNode) {
  int pos, flag = 0;
  if (myNode) {
    if (item < myNode->item[1]) {
      pos = 0;
      flag = 0;
    } else {
      for (pos = myNode->count; (item < myNode->item[pos] && pos > 1); pos--)
        ;
      if (item == myNode->item[pos]) {
        flag = 1;
      } else {
        flag = 0;
      }
    }
    if (flag) {
      if (myNode->linker[pos - 1]) {
        copySuccessor(myNode, pos);
        flag = delValFromNode(myNode->item[pos], myNode->linker[pos]);
        if (flag == 0) {
          printf("Given data is not present in B-Tree\n");
        }
      } else {
        removeVal(myNode, pos);
      }
    } else {
      flag = delValFromNode(item, myNode->linker[pos]);
    }
    if (myNode->linker[pos]) {
      if (myNode->linker[pos]->count < MIN)
        adjustNode(myNode, pos);
    }
  }
  return flag;
}

// Delete operaiton
void delete (int item, struct BTreeNode *myNode) {
  struct BTreeNode *tmp;
  if (!delValFromNode(item, myNode)) {
    printf("Not present\n");
    return;
  } else {
    if (myNode->count == 0) {
      tmp = myNode;
      myNode = myNode->linker[0];
      free(tmp);
    }
  }
  root = myNode;
  return;
}

// Search operation
void searching(int item, int *pos, struct BTreeNode *myNode) {
  if (!myNode) {
    return;
  }

  if (item < myNode->item[1]) {
    *pos = 0;
  } else {
    for (*pos = myNode->count;
       (item < myNode->item[*pos] && *pos > 1); (*pos)--)
      ;
    if (item == myNode->item[*pos]) {
      printf("%d present in B-tree", item);
      return;
    }
  }
  searching(item, pos, myNode->linker[*pos]);
  return;
}

// Não utilizado
void traversal(struct BTreeNode *myNode) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      traversal(myNode->linker[i]);
      printf("%d ", myNode->item[i + 1]);
    }
    traversal(myNode->linker[i]);
  }
}

void printTree(struct BTreeNode *node, int level) {
  int i;
  if (node) {
    for (i = node->count - 1; i >= 0; i--) {
      printTree(node->linker[i + 1], level + 1);

      for (int j = 0; j < level; j++) {
        printf("    ");
      }
      printf("%d\n", node->item[i + 1]);
    }
    printTree(node->linker[0], level + 1);
  }
}

void printTreeWithArrows(struct BTreeNode *node, int level) {
  if (node != NULL) {
    int i;

    for (i = node->count - 1; i >= 0; i--) {
      printTreeWithArrows(node->linker[i + 1], level + 1);

      for (int j = 0; j < level; j++) {
        printf("    ");
      }

      printf("-> %d\n", node->item[i + 1]);
    }

    printTreeWithArrows(node->linker[0], level + 1);
  }
}


int main() {
  int choice, item, pos;

  printf("B-Tree Implementation Test\n");
  while (1) {
    printf("\nMenu:\n");
    printf("1. Inserir um valor\n");
    printf("2. Excluir um valor\n");
    printf("3. Buscar um valor\n");
    printf("4. Exibir a B-Tree\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao:");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        printf("Digite o valor para inserir:");
        scanf("%d", &item);
        insertion(item);
        printf("\n");
      break;

      case 2:
        printf("Digite o valor para excluir:");
        scanf("%d", &item);
        delete(item, root);
        printf("\n");
        break;

      case 3:
        printf("Digite o valor para buscar:");
        scanf("%d", &item);
        pos = 0;
        searching(item, &pos, root);
        printf("\n");
        break;

      case 4:
        printf("Visualizacao da B-Tree:\n");
        // Esclher modelo de visualização

        // printTreeWithArrows(root, 0);
        // printTreeWithArrows(root, 0);
        printf("\n");
        break;

      case 5:
        printf("Encerrando o programa.\n");
        exit(0);

      default:
        printf("Opcao invalida. Tente novamente.\n");
    }
  }
  return 0;
}