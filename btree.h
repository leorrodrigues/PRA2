  #define MAX 4
  #define MIN 2

  struct btreeNode {
        int val[MAX + 1],posicao[MAX+1], count;
        struct btreeNode *link[MAX + 1];
  };

  struct btreeNode *root;

  /* creating new node */
  struct btreeNode * createNode(int val,int posicao, struct btreeNode *child) {
        struct btreeNode *newNode;
        newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
        newNode->val[1] = val;
        newNode->posicao[1]=posicao;
        newNode->count = 1;
        newNode->link[0] = root;
        newNode->link[1] = child;
        return newNode;
  }

  /* Places the value in appropriate position */
  void addValToNode(int val, int pos,int posicao, struct btreeNode *node,
                        struct btreeNode *child) {
        int j = node->count;
        while (j > pos) {
                node->val[j + 1] = node->val[j];
                node->posicao[j+1]=node->posicao[j];
                node->link[j + 1] = node->link[j];
                j--;
        }
        node->val[j + 1] = val;
        node->posicao[j+1]=posicao;
        node->link[j + 1] = child;
        node->count++;
  }

  /* split the node */
  void splitNode (int val, int *pval, int pos,int posicao, struct btreeNode *node,
     struct btreeNode *child, struct btreeNode **newNode) {
        int median, j;

        if (pos > MIN)
                median = MIN + 1;
        else
                median = MIN;

        *newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
        j = median + 1;
        while (j <= MAX) {
                (*newNode)->val[j - median] = node->val[j];
                (*newNode)->link[j - median] = node->link[j];
                j++;
        }
        node->count = median;
        (*newNode)->count = MAX - median;

        if (pos <= MIN) {
                addValToNode(val, pos,posicao, node, child);
        } else {
                addValToNode(val, pos - median, posicao, *newNode, child);
        }
        *pval = node->val[node->count];
        (*newNode)->link[0] = node->link[node->count];
        node->count--;
  }

  /* sets the value val in the node */
  int setValueInNode(int val, int *pval, int posicao,
     struct btreeNode *node, struct btreeNode **child) {

        int pos;
        if (!node) {
                *pval = val;
                *child = NULL;
                return 1;
        }

        if (val < node->val[1]) {
                pos = 0;
        } else {
                for (pos = node->count;
                        (val < node->val[pos] && pos > 1); pos--);
                if (val == node->val[pos]) {
                        printf("Duplicates not allowed\n");
                        return 0;
                }
        }
        if (setValueInNode(val, pval,posicao, node->link[pos], child)) {
                if (node->count < MAX) {
                        addValToNode(*pval, pos,posicao, node, *child);
                } else {
                        splitNode(*pval, pval, pos, posicao, node, *child, child);
                        return 1;
                }
        }
        return 0;
  }

  /* insert val in B-Tree */
  void insertion(int val,int posicao) {
        int flag, i;
        struct btreeNode *child;

        flag = setValueInNode(val, &i,posicao, root, &child);
        if (flag)
                root = createNode(i,posicao, child);
  }

  /* shifts value from parent to right child */
  void doRightShift(struct btreeNode *myNode, int pos) {
        struct btreeNode *x = myNode->link[pos];
        int j = x->count;

        while (j > 0) {
                x->val[j + 1] = x->val[j];
                x->link[j + 1] = x->link[j];
        }
        x->val[1] = myNode->val[pos];
        x->link[1] = x->link[0];
        x->count++;

        x = myNode->link[pos - 1];
        myNode->val[pos] = x->val[x->count];
        myNode->link[pos] = x->link[x->count];
        x->count--;
        return;
  }

  /* shifts value from parent to left child */
  void doLeftShift(struct btreeNode *myNode, int pos) {
        int j = 1;
        struct btreeNode *x = myNode->link[pos - 1];

        x->count++;
        x->val[x->count] = myNode->val[pos];
        x->link[x->count] = myNode->link[pos]->link[0];

        x = myNode->link[pos];
        myNode->val[pos] = x->val[1];
        x->link[0] = x->link[1];
        x->count--;

        while (j <= x->count) {
                x->val[j] = x->val[j + 1];
                x->link[j] = x->link[j + 1];
                j++;
        }
        return;
  }

  /* merge nodes */
  void mergeNodes(struct btreeNode *myNode, int pos) {
        int j = 1;
        struct btreeNode *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

        x2->count++;
        x2->val[x2->count] = myNode->val[pos];
        x2->link[x2->count] = myNode->link[0];

        while (j <= x1->count) {
                x2->count++;
                x2->val[x2->count] = x1->val[j];
                x2->link[x2->count] = x1->link[j];
                j++;
        }

        j = pos;
        while (j < myNode->count) {
                myNode->val[j] = myNode->val[j + 1];
                myNode->link[j] = myNode->link[j + 1];
                j++;
        }
        myNode->count--;
        free(x1);
  }

  /* adjusts the given node */
  void adjustNode(struct btreeNode *myNode, int pos) {
        if (!pos) {
                if (myNode->link[1]->count > MIN) {
                        doLeftShift(myNode, 1);
                } else {
                        mergeNodes(myNode, 1);
                }
        } else {
                if (myNode->count != pos) {
                        if(myNode->link[pos - 1]->count > MIN) {
                                doRightShift(myNode, pos);
                        } else {
                                if (myNode->link[pos + 1]->count > MIN) {
                                        doLeftShift(myNode, pos + 1);
                                } else {
                                        mergeNodes(myNode, pos);
                                }
                        }
                } else {
                        if (myNode->link[pos - 1]->count > MIN)
                                doRightShift(myNode, pos);
                        else
                                mergeNodes(myNode, pos);
                }
        }
  }

  /* B-Tree Traversal */
  void traversal(struct btreeNode *myNode, char *index) {
        FILE *arqIndice;
        int i;
        arqIndice = fopen(index,"ab");
        if(arqIndice!=NULL){
            if (myNode) {
                for (i = 0; i < myNode->count; i++) {
                        traversal(myNode->link[i],index);
                        if(arqIndice!=NULL){
                            fwrite(&(myNode->val[i+1]),sizeof(int),1,arqIndice);
                            fwrite(&(myNode->posicao[i+1]),sizeof(int),1,arqIndice);
                        }
                        else{
                            puts("Erro na abertura do arquivo de indices!");
                        }
                }
                traversal(myNode->link[i],index);
            }
            fclose(arqIndice);
        }
        else{
            puts("Erro na abertura do arquivo INDEX!");
        }
  }
