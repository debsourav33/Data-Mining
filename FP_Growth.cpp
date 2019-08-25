#include<bits/stdc++.h>
using namespace std;

//{
#define si(a) scanf("%d",&a)
#define sii(a,b) scanf("%d %d",&a,&b);
#define siii(a,b,c) scanf("%d %d %d",&a,&b,&c);

#define sl(a) scanf("%lld",&a)
#define sll(a,b) scanf("%lld %lld",&a,&b);
#define slll(a,b,c) scanf("%lld %lld %lld",&a,&b,&c);

#define outi(a) printf("%d\n",a)
#define outii(a,b) printf("%d %d\n",a,b)
#define outis(a) printf(" %d",a)

#define outl(a) printf("%lld\n",a)
#define outll(a,b) printf("%lld %lld\n",a,b)
#define outls(a) printf(" %lld",a)

#define cel(n,k) ((n-1)/k+1)
#define sets(a) memset(a, -1, sizeof(a))
#define clr(a) memset(a, 0, sizeof(a))
#define fr(n) for(int i=0;i<n;i++)
#define fr1(n) for(int i=1;i<=n;i++)
#define frj(n) for(int j=0;j<n;j++)
#define frj1(n) for(int j=1;j<=n;j++)
#define pb push_back
#define all(v) v.begin(),v.end()
#define mp make_pair
#define ff first
#define ss second
#define INF 10000007
#define fastIO() ios_base::sync_with_stdio(false); cin.tie(NULL);
#define null NULL

typedef long long i64;
typedef unsigned long long ull;
typedef pair<int,int> pii;
typedef pair<long long,long long> pll;
//}

const int maxn= 1e3;
const int maxtrans= 1e5;

int n, k, fpats[maxn];
vector<int> v[maxtrans], items;
int trans, min_support, threshold;

struct node{
    int val, cnt, leaf;
    node* parent;
    node* next;

    vector<node*> childs;
    map<int,int> index;

    node() {
        parent= null;
        next= null;

        leaf= 1;
        cnt= 0;
    }
};

struct Trie{
    int leaf_cnt;
    map<int,node*> begin;
    map<int,node*> last;
    map<int,int> support_cnts;
    node* root;

    vector<int> items;

    Trie(){
        root= new node();
        leaf_cnt= 1;
    }


    node* insert(int val, int c, node* root){
        if(root->leaf) {
            leaf_cnt--;
            root->leaf = 0;
        }

        if(root->index.count(val)==0){
            leaf_cnt++;
            node* t= new node();
            t->val= val;
            root->childs.pb(t);
            root->index[val]= root->childs.size()-1;

            if(begin.count(val)==0)  //first_time_insert
                last[val]= t, begin[val]= t;
            else{
                last[val]->next= t;
                last[val]= t;
            }
        }

        if(support_cnts.count(val)==0)  items.pb(val),  support_cnts[val]= 0;
        support_cnts[val]+= c;

        int ind= root->index[val];

        root->childs[ind]->cnt+= c;
        root->childs[ind]->parent= root;
        root= root->childs[ind];

        return  root;
    }

}T, fp_tree;

map<int,int> support_cnt;

bool cmp(int a, int b){
    if(support_cnt[a] == support_cnt[b])  return a<b;
    return support_cnt[a] > support_cnt[b];
}

Trie build_FPTree(Trie T, int f){
    Trie fp_tree= Trie();

    support_cnt.clear();
    vector<vector<int> > db;
    vector<int> db_cnt, tmp;

    fp_tree.root->val= 0;

    if(T.begin.count(f)==0)  return fp_tree;
    node* r= T.begin[f];

    while(r!=null){
        node* cur= r;
        cur= cur->parent;

        tmp.clear();

        while(cur->parent!=null){
            int a= cur->val;
            tmp.pb(a);

            cur= cur->parent;

            if(support_cnt.count(a)==0)  support_cnt[a]= 0;
            support_cnt[a]+= r->cnt;
        }

        if(!tmp.empty()){
            db.pb(tmp);
            db_cnt.pb(r->cnt);
        }

        r= r->next;
    }

    frj(db.size()){
        //sort(all(db[j]));
        sort(all(db[j]),cmp);

        node* r= fp_tree.root;

        fr(db[j].size()){
            int val= db[j][i];

            if(support_cnt[val]>=min_support)
                r= fp_tree.insert(val,db_cnt[j],r);
        }
    }

    //sort(all(fp_tree.items));
    sort(all(fp_tree.items),cmp);

    return fp_tree;
}

void print_tree(node* root, string space=""){
    cout<<space<<"-->"<<root->val<<" : "<<root->cnt<<endl;

    fr(root->childs.size()){
        print_tree(root->childs[i],space+"   ");
    }
}

void input(){
    float ms= (float) trans*(threshold/100.0);
    min_support= (int) ms+1;

    outi(min_support);

    min_support= 2;

    int m, a;
    trans= 0;

    si(n);
    frj(n){
        si(m);

        fr(m){
            si(a);
            v[j].pb(a);

            if(support_cnt.count(a)==0)  support_cnt[a]= 0;
            support_cnt[a]++;
        }

        trans++;
    }

    frj(trans){
        sort(all(v[j]));
        sort(all(v[j]),cmp);
        node* r= T.root;

        fr(v[j].size()){
            int val= v[j][i];

            //if(support_cnt[val]>=min_support)
                r= T.insert(val,1,r);
        }

    }

    sort(all(T.items));
    sort(all(T.items),cmp);

}

void fileinput(){
    string S, t;
    ifstream myfile("mushroom.txt");

    trans= 0;
    int a;

    while(getline(myfile, S))
    {
        istringstream ss(S);
        string word;

        while(ss>> a){
            v[trans].pb(a);

            if(support_cnt.count(a)==0)  support_cnt[a]= 0;
            support_cnt[a]++;
        }

        trans++;
    }

    float ms= trans*(threshold/100.0);
    min_support= (int) ms+1;

    printf("Minimum support count: %d\n",min_support);

    frj(trans){
        //sort(all(v[j]));
        sort(all(v[j]),cmp);
        node* r= T.root;

        fr(v[j].size()){
            int val= v[j][i];

            if(support_cnt[val]>=min_support)
                r= T.insert(val,1,r);
        }
    }

    //sort(all(T.items));
    sort(all(T.items),cmp);

}

int total_pats= 0, tree_cnt= 0;

void FPTreeMine(Trie tree, int curr){
    if(!tree.items.empty())  tree_cnt++;

    for(int i=tree.items.size()-1; i>=0; i--){
        int it= tree.items[i];

        if(tree.support_cnts[it]>= min_support){
            total_pats++;

            //tmp.pb(it);
            //if(!tmp.empty()){  fr(tmp.size())  outis(tmp[i]);  puts("");}

            Trie t= build_FPTree(tree,it);
            //cout<<"Calling for: "<< it<<endl;  print_tree(t.root);

            FPTreeMine(t,it);
            //tmp.pop_back();
        }
    }
}

main(){
    printf("Enter Threshold: ");
    si(threshold);
    T.root->val= 0;

    fileinput();
    //print_tree(T.root);
    //fr(T.items.size())  outis(T.items[i]);  puts("");
    //outi(fp_tree.leaf_cnt);

    clock_t tStart = clock();
    FPTreeMine(T,-1);

    printf("Total No. of Frequent Patterns: %d\n",total_pats);
    printf("Total No. of Intermediate Conditional Trees: %d\n",tree_cnt);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

/*
 4
 3 1 3 2
 3 2 1 4
 1 4
 1 4
 */
