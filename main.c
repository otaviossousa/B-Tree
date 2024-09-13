// Fonte do código: https://www.programiz.com/dsa/deletion-from-a-b-tree

#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

struct BTreeNode {
  int item[MAX + 1], count;
  struct BTreeNode *linker[MAX + 1];
};

struct BTreeNode *root;

/* Pseudo code node creation

B-Tree-CreateNode(item, child)
1. newNode = ALLOCATE-NODE()
2. newNode.item[1] = item
3. newNode.count = 1
4. newNode.linker[0] = root
5. newNode.linker[1] = child
6. RETURN newNode

*/

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

/* Pseudo code add valu to the node

B-Tree-AddValToNode(item, pos, node, child)
1. j = node.count
2. WHILE j > pos
3.   node.item[j + 1] = node.item[j]
4.   node.linker[j + 1] = node.linker[j]
5.   j = j - 1
6. node.item[j + 1] = item[j]
7. node.linker[j + 1] = child[j]
8. node.count++

*/

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

/* Pseudo code Split the node

B-Tree-SplitNode(item, pval, pos, node, child, newNode)
1. IF pos > MIN THEN
2.     median = MIN + 1
3.   ELSE
4.     median = MIN
5. newNode = ALLOCATE-NODE()
6. j = median + 1
7. WHILE j <= MAX
8.      newNode.item[j - median] = node.item[j]
9.      newNode.linker[j - median] = node.linker[j]
10.     j = j + 1
11. node.count = median
12. newNode.count = MAX - median
13. IF pos <= MIN THEN
14.     B-Tree-AddValToNode(item, pos, node, child)
15.   ELSE
16.     B-Tree-AddValToNode(item, pos - median, newNode, child)
17. pval = node.item[node.count]
18. newNode.linker[0] = node.linker[node.count]
19. node.count = node.count - 1

*/

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

/* Pseudo code Set the value in the node

B-Tree-SetValueInNode(item, pval, node, child)
1. IF node IS NULL THEN
2.     *pval = item
3.     *child = NULL
4.     RETURN 1
5. IF item < node.item[1] THEN
6.     pos = 0
7.   ELSE
8.     FOR pos = node.count DOWNTO 1
9.     IF item < node.item[pos] THEN
10.       pos = pos - 1
11.    ELSE
12.      BREAK
13.    IF item == node.item[pos] THEN
14.       PRINT "Duplicates not allowed"
15.       RETURN 0
16. IF B-Tree-SetValueInNode(item, pval, node.linker[pos], child) THEN
17.     IF node.count < MAX THEN
18.        B-Tree-AddValToNode(*pval, pos, node, *child)
19.     ELSE
20.        B-Tree-SplitNode(*pval, pval, pos, node, *child, child)
21.        RETURN 1
22. RETURN 0

*/

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

/* Pseudo code for insertion 

B-Tree-Insertion(item)
1. flag, i = 0
2. child = NULL
3. flag = B-Tree-SetValueInNode(item, &i, root, &child)
4. IF flag THEN
5.   root = B-Tree-CreateNode(i, child)

*/

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

/* Pseudo code for delete

B-Tree-DeleteValueFromNode(item, myNode)
1. pos = 0
2. flag = 0
3. IF myNode THEN
4.     IF item < myNode->item[1] THEN
5.        pos = 0
6.        flag = 0
7.     ELSE
8.        FOR pos = myNode->count DOWNTO 1 DO
9.           IF item < myNode->item[pos] THEN
10.              BREAK
11.        IF item == myNode->item[pos] THEN
12.           flag = 1
13.        ELSE
14.           flag = 0
15.     IF flag THEN
16.        IF myNode->linker[pos - 1] THEN
17.           B-Tree-CopySuccessor(myNode, pos)
18.           flag = B-Tree-DeleteValueFromNode(myNode->item[pos], myNode->linker[pos])
19.           IF flag == 0 THEN
20.              PRINT "Given data is not present in B-Tree"
21.        ELSE
22.           B-Tree-RemoveValue(myNode, pos)
23.     ELSE
24.        flag = B-Tree-DeleteValueFromNode(item, myNode->linker[pos])
25.     IF myNode->linker[pos] THEN
26.        IF myNode->linker[pos]->count < MIN THEN
27.           B-Tree-AdjustNode(myNode, pos)
28. RETURN flag

*/

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

/* Pseudo code for search

B-Tree-Search(item, pos, myNode)
1. IF NOT myNode THEN
2.   RETURN
3.
4. IF item < myNode->item[1] THEN
5.   *pos = 0
6. ELSE
7.   FOR *pos = myNode->count DOWNTO 1 DO
8.      IF item < myNode->item[*pos] THEN
9.         BREAK
10.   IF item == myNode->item[*pos] THEN
11.      PRINT item "present in B-tree"
12.      RETURN
13.
14. B-Tree-Search(item, pos, myNode->linker[*pos])
15. RETURN

*/

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

/* Pseudo code for printTree

B-Tree-PrintTree(node, level)
1. IF node THEN
2.   FOR i = node->count - 1 DOWNTO 0 DO
3.      B-Tree-PrintTree(node->linker[i + 1], level + 1)
4.      FOR j = 0 TO level - 1 DO
5.         PRINT "    "
6.      PRINT node->item[i + 1]
7.   B-Tree-PrintTree(node->linker[0], level + 1)

*/

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
        printTree(root, 0);
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
