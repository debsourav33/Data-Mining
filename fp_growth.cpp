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

int n, k, support_cnt[maxn], fpats[maxn];
vector<int> v[maxtrans], items;
int trans, min_support, threshold, mark[maxn];

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
    map<int,node*> begin;
    map<int,node*> last;
    node* root;

    Trie(){
        root= new node();
    }

    node* insert(int val, int c, node* root){
        root->leaf= 0;

        if(root->index.count(val)==0){
            node* t= new node();
            t->val= val;
            root->childs.pb(t);
            root->index[val]= root->childs.size()-1;
        }

        int ind= root->index[val];

        root->childs[ind]->cnt+= c;
        root->childs[ind]->parent= root;
        root= root->childs[ind];

        if(begin.count(val)==0)
            last[val]= root, begin[val]= root;
        else{
            last[val]->next= root;
            last[val]= root;
        }

        root->next= null;
        return  root;
    }

}T, fp_tree;

void build_FPTree(int f){
    //fptree[f].root->val= 0;
    fp_tree.root->val= 0;

    if(T.begin.count(f)==0)  return;
    node* r= T.begin[f];

    while(r!=null){
        node* cur= r;
        cur= cur->parent;
        stack<int> st;

        while(cur->parent!=null){
            st.push(cur->val);
            cur= cur->parent;
        }

        node* root= fp_tree.root;
        while(!st.empty()){
            root= fp_tree.insert(st.top(),r->cnt,root);
            st.pop();
        }

        r= r->next;
    }
}

bool cmp(int a, int b){
    return support_cnt[a]>support_cnt[b];
}

void print_tree(node* root, string space=""){
    cout<<space<<"-->"<<root->val<<" : "<<root->cnt<<endl;

    fr(root->childs.size()){
        print_tree(root->childs[i],space+"   ");
    }
}

vector<vector<int> > pat;
vector<int> u;

void generate_patterns(node* root){
    if(root->leaf==1){
        if(!u.empty())  pat.pb(u);
        return;
    }

    fr(root->childs.size()){
        if(root->childs[i]->cnt >= min_support)  u.pb(root->childs[i]->val);
        generate_patterns(root->childs[i]);
        if(root->childs[i]->cnt >= min_support)  u.pop_back();  //backtrack
    }
}

vector<int> var;
map<vector<int> ,int> m;

void gen_combo(int pos, int rem, int p, int val){
    if(rem==0){
        if(m.count(var))  return;

        /*
        fr(var.size())
            outis(var[i]);
        outis(val);
        puts("");
        */

        m[var]= 1;
        fpats[val]+= 1;

        return;
    }

    //for(int i=pos;i<=pat[p].size()-rem;i++){
      for(int i=pos;i<pat[p].size();i++){
        var.push_back(pat[p][i]);
        gen_combo(i+1,rem-1,p,val);
        var.pop_back();  //backtrack
    }
}

void print_patterns(){
    puts("Conditional Patterns: ");
    fr(pat.size()){
        frj(pat[i].size()){
            outis(pat[i][j]);
        }

        puts("");
    }
}

void gen_conditional_fp_tree(int val){
    pat.clear();
    u.clear();
    m.clear();

    //print_tree(fp_tree.root);

    generate_patterns(fp_tree.root);
    //print_patterns();

    //cout<<"fps"<<endl;
    outii(val,pat.size());
    fr(pat.size()){
        frj1(pat[i].size()) {
            gen_combo(0, j, i, val);
        }
    }
}

void input(){
    int m, a;
    trans= 0;

    si(n);
    frj(n){
        si(m);

        fr(m){
            si(a);
            support_cnt[a]++;
            v[j].pb(a);

             if(!mark[a]){
                mark[a]= 1;
                items.pb(a);
            }

        }

        trans++;
    }

    frj(n){
        sort(all(v[j]),cmp);

        node* root= T.root;

        fr(v[j].size()){
            root = T.insert(v[j][i],1,root);
        }
    }

    float ms= (float) trans*(threshold/100.0);
    min_support= (int) ms+1;

}

void fileinput(){
    string S, t;
    ifstream myfile("mushroom.txt");
    //ifstream myfile("test.txt");

    trans= 0;
    int it;

    while(getline(myfile, S))
    {
        istringstream ss(S);
        string word;

        while(ss>> it){
            v[trans].pb(it);
        }

        trans++;
    }

    clr(mark);
    clr(support_cnt);

    fr(trans){
        frj(v[i].size()){
            int a= v[i][j];

            if(!mark[a]){
                mark[a]= 1;
                items.pb(a);
            }
            support_cnt[a]++;
        }
    }

    frj(trans){
        sort(all(v[j]),cmp);

        node* root= T.root;

        fr(v[j].size()){
            root = T.insert(v[j][i],1,root);
        }
    }

    sort(all(items));

    float ms= (float) trans*(threshold/100.0);
    min_support= (int) ms+1;

    //min_support= 2;
}

main(){
    si(threshold);
    T.root->val= 0;

    fileinput();

    outi(min_support);


    i64 total_fp= 0;

    fr1(items.size()) {
        fp_tree= Trie();

        outi(support_cnt[i]);

        build_FPTree(i);
        gen_conditional_fp_tree(i);


        //outii(i,fpats[i]);

        total_fp+= fpats[i];

    }

    outl(total_fp);
}

/*
 9
 3 1 2 5
 2 2 4
 2 2 3
 3 1 2 4
 2 1 3
 2 2 3
 2 1 3
 4 1 2 3 5
 3 1 2 3
 */
