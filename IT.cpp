
/***********************************************************************************************************
Implementation of -> 1. insert (node, interval) 2. delete (item) 3. search (node, interval) in Interval Trees
Using RED BLACK TREE

Created by Nitin Gaurav Singh

As asked in the Question :
Sample Data set:
{15, 20}, {10, 30}, {17, 19},{5, 20}, {12, 15}, {30, 40}
Check for {14,16}, {21,23}.

First the interval tree is formed using the given sample data and printed.
Then {14,16}, {21,23} are inserted and the final tree is printed to test the insertion
Then {14,16}, {21,23} are deleted and the final tree is printed to test the deletion
Then we search for {14,16} , {21,23} to test the search function.

STILL NEED TO RESOLVE SOME ERRORS (in deletion)
************************************************************************************************************/

#include <iostream>
#include <queue>

using namespace std;

enum node_color
{
    RED = 0,
    BLACK = 1
};


typedef struct nodeT
{
    struct nodeT *left;
    struct nodeT *right;
    struct nodeT *p;
    node_color color;
    int max;
    int low;
    int high;
} node;


typedef node *nodeptr;
node *nil = new node;

int FindMax(int a ,int b, int c)
{
    int max = 0;
    max = (a>b? a:b);
    max = (max>c? max:c);
    return max;
}


void LevelOrderTraversal(node *x)
{
    int current = 1;
    int next = 0;
	queue<node*> q;
	q.push(x);
	while(!q.empty())
	{
		node *currentNode = q.front();
		q.pop();
		cout << "[" << currentNode->low << ","
             << currentNode->high << "]" << currentNode->max
             << "(" << (currentNode->color == RED? "R":"B") << ") ";
		if (currentNode->left != nil)
		{
			q.push(currentNode->left);
            next++;
		}
		if(currentNode->right != nil)
		{
			q.push(currentNode->right);
            next++;
		}
        if (current == 1)
        {
            current = next;
            next = 0;
            cout << endl;
        }
        else
        {
            current--;
        }
	}
}


node* Tree_Search(node *x, int k)
{
    while (x != nil && x->low != k)
    {
        if (k < x->low)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    return x;

}


node* Tree_Minimum(node *x)
{
    while (x->left != nil)
    {
        x = x->left;
    }
    return x;
}

void IT_Transplant(node* &root, node* &u, node* &v)
{
    if (u->p == nil)
    {
        root = v;
    }
    else if (u == u->p->left)
    {
        u->p->left = v;
    }
    else
    {
        u->p->right = v;
    }
    if (v != nil)
    {
        v->p = u->p;
    }
}

void Left_Rotate(node* &root , node *x)
{
    node *y = x->right;
    x->right = y->left;
    if (y->left != nil)
    {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == nil)
    {
        root = y;
    }
    else if (x == x->p->left)
    {
        x->p->left = y;
    }
    else
    {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
    y->max = x->max;
    x->max = FindMax(x->max, x->left->max, x->right->max);
}


void Right_Rotate(node* &root , node *y)
{
    node *x = y->left;
    y->left = x->right;
    if (x->right != nil)
    {
        x->right->p = y;
    }
    x->p = y->p;
    if (y->p == nil)
    {
        root = x;
    }
    else if (y == y->p->left)
    {
        y->p->left = x;
    }
    else
    {
        y->p->right = x;
    }
    x->right = y;
    y->p = x;
    x->max = y->max;
    y->max = FindMax(y->max, y->left->max, y->right->max);
}


void IT_Insert_Fixup(node* &root, node *z)
{

    while (z->p->color == RED)
    {
        if (z->p == z->p->p->left)
        {
            node *y = z->p->p->right;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else if (z == z->p->right)
            {
                z = z->p;
                Left_Rotate(root, z);
            }
            else if (z->p != nil && z->p->p != nil)
            {
                z->p->color = BLACK;
                z->p->p->color = RED;
                Right_Rotate(root, z->p->p);
            }
        }
        else
        {
            node *y = z->p->p->left;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else if (z == z->p->left)
            {
                z = z->p;
                Right_Rotate(root, z);
            }
            else if (z->p != nil && z->p->p != nil)
            {
                z->p->color = BLACK;
                z->p->p->color = RED;
                Left_Rotate(root, z->p->p);
            }
        }

    }
    root->color = BLACK;
}


void Max_Fixup(node* &root, node *z)
{
    while (z->p != nil && z->max > z->p->max )
    {
        z->p->max = (z->max > z->p->max ? z->max:z->p->max);
        z = z->p;
    }
}


void IT_Insert(node* &root, node *z)
{
    node *y = nil;
    node *x = root;
    while (x != nil)
    {
        y = x;
        if (z->low < x->low)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->p = y;
    if (y == nil)
    {
        root = z;
    }
    else if (z->low < y->low)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = RED;
    z->max = z->high;
    IT_Insert_Fixup(root,z);
    Max_Fixup(root, z);

}


void IT_deletefixup(node** root, node *x) {
	while (x != *root && x->color == BLACK) {
		if (x == x->p->left) {
			node* w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				Left_Rotate(*root,x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
			 	if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					Right_Rotate(*root,w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				Left_Rotate(*root,x->p);
				x = *root;
			}
		}
		else {
			node * w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				Right_Rotate(*root,x->p);
				w = x->p->left;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					Left_Rotate(*root,w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				Right_Rotate(*root,x->p);
				x = *root;
			}
		}
	}
	x->color = BLACK;

}


node* Interveral_Search(node *root, node *i)
{
    node *x = root;
    while (x != nil && (i->low >= x->high || i->high <= x->low))
    {
        if (x->left != nil && x->left->max >= i->low)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    return x;
}

node* minimum(node* root) {
	while (root->left != nil)
		root = root->left;
	return root;
}


void IT_Delete(node** root, node* z) {
	node* l = Interveral_Search(*root, z);
	if (l == nil) {
		cout<<"Node to be deleted not found";
		return;
	}
	node* y = l;
	int yoc = y->color;
	node* x;
	if (l->left == nil) {
		x = l->right;
		IT_Transplant(*root,l,l->right);
	}
	else if (l->right == nil) {
		x = l->left;
		IT_Transplant(*root,l,l->left);
	}
	else {
		y = minimum(l->right);
		yoc = l->color;
		x = y->right;
		if (y->p == l)
			x->p = y;
		else {
			IT_Transplant(*root,y,y->right);
			y->right = l->right;
			y->right->p = y;
		}
		IT_Transplant(*root,l,y);
		y->left =l->left;
		y->left->p = y;
		y->color = l->color;
	}
	if (yoc == BLACK)
		IT_deletefixup(root,x);
   Max_Fixup(*root, x);
}


void Create_IT_tree(node* &root)
{
    node *node1 = new node();
    node1->low = 15;
    node1->high = 20;
    IT_Insert(root, node1);

    node *node2 = new node();
    node2->low = 10;
    node2->high = 30;
    IT_Insert(root, node2);
    node *node3 = new node();
    node3->low = 17;
    node3->high = 19;
    IT_Insert(root, node3);

    node *node4 = new node();
    node4->low = 5;
    node4->high = 20;
    IT_Insert(root, node4);
    node *node5 = new node();
    node5->low = 12;
    node5->high = 15;
    IT_Insert(root, node5);
    node *node6 = new node();
    node6->low = 30;
    node6->high =40;
    IT_Insert(root, node6);


}

//Main Program

int main(int argc, char* argv[])
{
    // Intinalize global varible
    nil->color = BLACK;
    // Create the binary search tree
    node *root = nil;
    Create_IT_tree(root);
    LevelOrderTraversal(root);
    cout<<"\n";
    cout << endl << "Test the Iterval Search Tree" << endl;
    cout<<"\n";
    node *n1 = new node();
    n1->low = 14;
    n1->high = 16;
    IT_Insert(root,n1);
    LevelOrderTraversal(root);
    cout<<"\n";
    cout<<"{14,16} inserted"<<endl;     //{14,16} inserted
    cout<<"\n";
    node *n2 = new node();
    n2->low = 21;
    n2->high = 23;
    IT_Insert(root,n2);
    LevelOrderTraversal(root);
    cout<<"\n";
    cout<<"{21,23} inserted"<<endl;    //{21,23} inserted
    cout<<"\n";


    node *f1 = Interveral_Search(root, n1); // search for n1
    if (f1 != nil)
    {
        cout << "[" << f1->low << "," << f1->high << "]"
             << f1->max << "(" << (f1->color == RED? "R":"B")
             << ") " << endl;
    }
    else
    {
        cout << "[" << n1->low << "," << n1->high << "]"
             << " is not found";
    }
    cout<<"search result for {14,16}"<<endl;
    cout<<"\n";

    node *f2 = Interveral_Search(root, n2);
    if (f2 != nil)
    {
        cout << "[" << f2->low << "," << f2->high << "]"
             << f2->max << "(" << (f2->color == RED? "R":"B")
             << ") " << endl;
    }
    else
    {
        cout << "[" << n2->low << "," << n2->high << "]"
             << " is not found"<<endl;
    }
    cout<<"search result for {21,23}"<<endl;
    cout<<"\n";

    node *n3 = new node();
    n3->low = 41;
    n3->high = 45;
    node *f3 = Interveral_Search(root, n3);
    if (f3 != nil)
    {
        cout << "[" << f3->low << "," << f3->high << "]"
             << f3->max << "(" << (f3->color == RED? "R":"B")
             << ") " << endl;
    }
    else
    {
        cout << "[" << n3->low << "," << n3->high << "]"
             << " is not found"<<endl;
    }
    cout<<"search result for {41,45}"<<endl;
    cout<<"\n";

   // node* nodeptr;
    node** r = &root;
    IT_Delete(r,n1); // {14,16} deleted
    LevelOrderTraversal(root);
    cout<<"{14,16} deleted"<<endl;
    cout<<"\n";
    IT_Delete(r,n2); // {21,23} deleted
    LevelOrderTraversal(root);
    cout<<"{21,23} deleted "<<endl;
    cout<<"\n";
    return 0;
}
