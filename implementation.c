#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

char s[48]= "";

// special character '$' for specifying internal node.
typedef struct node_
{
    char ch;
    int frequency;
    struct node_ *left, *right;
}node;

node* create_node(char ch, int frequency)
{
    node *temp = (node *)(malloc(sizeof(node)));
    temp->ch = ch;
    temp->frequency = frequency;
    temp->left = temp->right = NULL;
    return temp;
}
typedef struct min_heap_
{
    int n;
    int capacity;
    node **array;
}min_heap;

min_heap* create_min_heap(int size)
{
    min_heap *temp = (min_heap*)malloc(sizeof(min_heap));
    temp->n = 0;
    temp->capacity = size;
    temp->array = (node**)(malloc(sizeof(node*)*size));
    return temp;
}
void swap(node** x, node** y)
{
	node *temp = *x;
    *x = *y;
    *y = temp;
	return;
}
void insert(min_heap* h)
{
    (h->n)++;
	int temp = (h->n)-1;
	int parent = (temp-1)/2;
	while (temp != 0)
	{
		if (h->array[temp]->frequency < h->array[parent]->frequency)
        {
			swap(&(h->array[parent]), &(h->array[temp]));
            temp = parent;
            parent = (temp-1)/2;
        }
        else
            break;
	}
	return;
}
node* delete_node(min_heap* h)
{
    if (h->n == 0)
        return NULL;
    
    if (h->n == 1)
    {
        (h->n)--;
        return h->array[h->n];
    }
    swap(&(h->array[0]), &(h->array[h->n - 1]));
    if (h->n == 2)
    {
        (h->n)--;
        return h->array[h->n];
    }
	int i = 0, last_internal_node = (h->n-3)/2;
	while (i <= last_internal_node)
	{
		int left_child = 2*i + 1;
		int right_child = (2*i + 2 <= (h->n-2) ? 2*i + 2 : __INT32_MAX__);
        int temp;
        if (right_child == __INT32_MAX__)
            temp = left_child;
        else
            temp = h->array[left_child]->frequency < h->array[right_child]->frequency ? left_child : right_child;
		if ((h->array[i])->frequency > (h->array[temp])->frequency)
        {
			swap(&(h->array[i]), &(h->array[temp]));
            i = temp;
        }
        else break;
	}
    (h->n)--;
	return (h->array[h->n]);
}
min_heap* set_min_heap(min_heap *h)
{
    // reading file.
	FILE *file = fopen("input4.txt", "r");
    char ch; int fq;
    while (fscanf(file, " %c %d", &ch, &fq) != EOF)
    {
        h->array[h->n] = create_node(ch, fq);
        insert(h);
    }
    return h;
}
void print_min_heap(min_heap *h)
{
    for (size_t i = 0; i < h->n; i++)
        printf("%c %d\n", (h->array[i])->ch, (h->array[i])->frequency);
	return ;
}

node* build_huffman_tree(min_heap *h)
{
    node *left, *right, *top;
    while (h->n > 1)
    {
        left = delete_node(h);
        right = delete_node(h);

        top = create_node('$', left->frequency+right->frequency);
        top->left = left;
        top->right = right;
        h->array[h->n] = top;
        insert(h);
    }
    return delete_node(h);
}
char *str = s;


void print_codes(node *root, char *str, int depth)
{
    if (root->ch != '$')
    {
        printf("%c: %s\n", root->ch, str);
        str[strlen(str)-1] = '\0';
        return;
    }
    if (root->left)
    {
        str[depth] = '0';
        str[depth+1] = '\0';
        print_codes(root->left, str, depth+1);
    }
    if (root->right)
    {
        str[depth] = '1';
        str[depth+1] = '\0';    
        print_codes(root->right, str, depth+1);
    }
    return;
}

void huffman_code(min_heap* h)
{
    node *root = build_huffman_tree(h);
    // print codes.

    print_codes(root, str, 0);
    return;
}
int main()
{
    min_heap *h = create_min_heap(6);
    set_min_heap(h);
    print_min_heap(h);


    printf("\n");
    huffman_code(h);
    
	return 0;
}
