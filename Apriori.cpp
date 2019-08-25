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

int min_support;
//const int maxn= 15;
const int maxtrans= 10500;
const int maxnn= 100;

vector<int> tmp, v[maxtrans];
vector<vector<int> > mid_pruned_candies[maxnn], pruned_candies[maxnn], candidates[maxnn];
int it, trans, support_cnt[100005];
int threshold;

i64 total= 0;

struct node{
    int val, cnt, leaf;
    node* parent;

    vector<node*> childs;
    map<int,int> index;

    node() {
        parent= null;

        val= 0;
        leaf= 1;
        cnt= 0;
    }
};

struct Trie{
    node* root= new node();

    Trie(){
        root= new node();
    }

    node* insert(int val, node* root){
        root->leaf= 0;


        if(root->index.count(val)==0){
            node* t= new node();
            t->val= val;
            root->childs.pb(t);

            root->index[val]= root->childs.size()-1;
        }

        int ind= root->index[val];

        root->childs[ind]->parent= root;
        root= root->childs[ind];

        return  root;
    }

};

Trie T;

node* insert(int k){
    T= Trie();
    node* root;

    fr(mid_pruned_candies[k].size()){
        root= T.root;

        frj(mid_pruned_candies[k][i].size()){
            int u= mid_pruned_candies[k][i][j];
            root= T.insert(u,root);
        }
    }

    return T.root;
}

void print_tree(node* root, string space=""){
    cout<<space<<"-->"<<root->val<<" : "<<root->cnt<<endl;

    fr(root->childs.size()){
        print_tree(root->childs[i],space+"   ");
    }
}

void upd(node* root, int pos, int t){
    int val= root->val;

    int p= upper_bound(v[t].begin()+pos,v[t].end(),val-1)- v[t].begin();

    if(p==v[t].size() || v[t][p]!=val)  return;

    root->cnt++;

    fr(root->childs.size())
        upd(root->childs[i],p+1,t);

}

void update_count(node* root){
    frj(trans) {
        fr(root->childs.size()) {
            upd(root->childs[i], 0, j);
        }
    }
}

void prune_tree(node* root, int k){
    if(root->parent!=null && root->cnt<min_support)  return;

    if(root->leaf){
        pruned_candies[k].pb(tmp);
        return;
    }

    fr(root->childs.size()){

        tmp.pb(root->childs[i]->val);
        prune_tree(root->childs[i],k);
        tmp.pop_back();  //backtrack

    }
}

void join(int k){
    int lim= pruned_candies[k-1].size();

    fr(lim){
        tmp.clear();
        frj(pruned_candies[k-1][i].size()){
                int u= pruned_candies[k-1][i][j];
                tmp.pb(u);
        }

        for(int j=i+1; j<lim; ++j)
        {
            int unmatched= 0;

            for(int x=0;x<k-2;x++)
                if (pruned_candies[k-1][i][x] !=pruned_candies[k-1][j][x]) {
                    unmatched = 1;
                    break;
                }

            if(unmatched)  break;

            tmp.pb(pruned_candies[k-1][j][k-2]);
            candidates[k].pb(tmp);
            tmp.pop_back();
        }
    }
}

int check(node* root, int pos){
    if(root->cnt < min_support)  return 0;
    if(pos==tmp.size())  return 1;

    if(root->index.count(tmp[pos])==0)  return 0;

    int ind= root->index[tmp[pos]];
    return check(root->childs[ind], pos+1);
}

void mid_prune(int k){
    fr(candidates[k].size()){
        vector<int> x= candidates[k][i];
        tmp.clear();

        frj1(x.size()-1){
            tmp.pb(x[j]);
        }


        int take= 0, fl= 1;

        //cout<<"gen for:";
        //frj(x.size())  outis(x[j]);
        //puts("");

        while(take<x.size()){
            /*cout<<"searching for:";
            frj(tmp.size())  outis(tmp[j]);
            puts(""); */

            int fid= 0, ind;

            if(T.root->index.count(tmp[0])==0){
                fl= 0;
                break;
            }

            ind= T.root->index[tmp[fid]];

            node* r= T.root->childs[ind];

            if(!check(r,1)){
                fl= 0;
            }

            tmp.pb(x[take]);
            tmp[take]= -1;

            sort(all(tmp), greater<int>());
            tmp.pop_back();
            sort(all(tmp));


            take++;

        }

        if(fl)  mid_pruned_candies[k].pb(x);
    }

}

void print_candidates(int k){
    fr(candidates[k].size()){
        frj(candidates[k][i].size()){
                int u= candidates[k][i][j];
                outis(u);
        }
        puts("");
    }
}

void print_prune_candidates(int k){
    fr(pruned_candies[k].size()){
        frj(pruned_candies[k][i].size()){
                int u= pruned_candies[k][i][j];
                outis(u);
        }
        puts("");
    }
}

int mark[100006];
vector<int> items;


void fileinput(){
    clr(mark);
    string S, t;
    ifstream myfile("chess.txt");

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
        sort(all(v[j]));
    }

    sort(all(items));

    float ms= (float) trans*(threshold/100.0);
    min_support= (int) ms+1;

    printf("Minimum support count: %d\n",min_support);

    fr(items.size()){
        if(support_cnt[items[i] ] <min_support) continue;

        tmp.clear();
        tmp.pb(items[i]);

        candidates[1].pb(tmp);
        mid_pruned_candies[1].pb(tmp);
        pruned_candies[1].pb(tmp);
    }
}

main(){
    puts("Enter threshold: ");
    si(threshold);

    fileinput();

    clock_t tStart = clock();

    total= pruned_candies[1].size();

    node *root = insert(1);
    root->leaf= 0;

    update_count(root);

    puts("Level \t\t Before_Pruning \t After_Pruning \t\t Actual_Frequent_Patterns:");
    printf("1 \t\t %d \t\t\t %d \t\t\t %d\n",items.size(),items.size(),pruned_candies[1].size());

    for(int k=2; ;k++) {
        join(k);

        mid_prune(k);

        node *root = insert(k);
        root->leaf= 0;

        update_count(root);

        tmp.clear();
        prune_tree(root, k);

        printf("%d \t\t %d \t\t\t %d \t\t\t %d\n",k,candidates[k].size(),mid_pruned_candies[k].size(),pruned_candies[k].size());

        total+=  pruned_candies[k].size();

        //print_tree(root);
        //printf("%d-Candidates Generated\n",k);  print_candidates(k);
        //printf("%d-Candidates After Pruning\n",k);  print_prune_candidates(k);

        if(pruned_candies[k].empty())  break;
    }

    printf("Total Frequent Patterns: %d\n",total);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    puts("Done");
}

/*
 2

 3 1 3 4
 2 1 2

 1 2 5
 2 4
 2 3
 1 2 4
 1 3
 2 3
 1 3
 1 2 3 5
 1 2 3

 */

