#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<map>
#include<limits>
using namespace std;

enum status{PENDING,PREPARING,READY};

queue<string> grillQueue;
queue<string> fryerQueue;
queue<string> saladQueue;
queue<string> ovenQueue;
queue<string> dessertQueue;
queue<string> beverageQueue;

class Fooditem{
    public:
    string name;
    int preptime;
    string specification;
    Fooditem(string n,int p,string spec):
    name(n),preptime(p),specification(spec){}

};
int getStationload(const string& itemname) 
{
    if(itemname=="Burger"||itemname=="Steak"||itemname=="Chicken")return grillQueue.size();
    if(itemname=="Fries"||itemname=="Nuggets"||itemname=="Fried chicken")return fryerQueue.size();
    if(itemname=="Salad"||itemname=="Garden salad"||itemname=="Caesar salad")return saladQueue.size();
    if(itemname=="Pizza"||itemname=="Garlic bread")return ovenQueue.size();
    if(itemname=="Ice cream"||itemname=="Brownie"||itemname=="Cheese cake"||itemname=="Cake slice")return dessertQueue.size();
    if(itemname=="Coffee"||itemname=="Tea"||itemname=="Soda"||itemname=="Juice")return beverageQueue.size();
    return 0;
}
class order{
    public:
    int id;
    vector<Fooditem>items;
    string waiter;
    bool isVIP;
    status status;
    int estimatedtime;
    
    order(int id,vector<Fooditem>its,string w,bool vip):
    id(id),items(its),waiter(w),isVIP(vip),status(PENDING){
        estimatedtime=calculatetime();

    }
int calculatetime(){
    int total=0;
    for(auto& item : items){
    int baseTime = isVIP ? max(1, item.preptime - 2) : item.preptime;
    int loadpenalty = getStationload(item.name)*2;
    total += baseTime + loadpenalty;
    }
    return total;
}
string statustext() const{
    switch(status){
        case PENDING:return"Pending";
        case PREPARING:return"Preparing";
        case READY:return"Ready";

    }
    return"Unknown";
}
void print()const{
    cout<<"Order#"<<id<<"["<<(isVIP?"VIP":"Regular")<<"]-"
    <<"Status:"<<statustext()<<",Waiter:"<<waiter
    <<",Est.time:"<<estimatedtime<<"mins/n";
    cout<<"Items:";
    for(auto&item:items){
        cout<<item.name<<"["<<item.specification<<"]";

    }
    cout<<endl;
}
};

void assigntostations(const order&order){
    for(auto&item:order.items){
        string task="order#"+to_string(order.id)+":"+item.name;
        if(item.name=="Burger"||item.name=="Steak"||item.name=="Chicken")grillQueue.push(task);
        else if(item.name=="Fries"||item.name=="Nuggets"||item.name=="Fried chicken")fryerQueue.push(task);
        else if(item.name=="Salad"||item.name=="Garden salad"||item.name=="Caesar salad") saladQueue.push(task);
        else if(item.name=="Pizza"||item.name=="Garlic bread")ovenQueue.push(task);
        else if(item.name=="Ice cream"||item.name=="Brownie"||item.name=="Cheese cake"||item.name=="Cake slice")dessertQueue.push(task);
        else if(item.name=="Coffee"||item.name=="Tea"||item.name=="Soda"||item.name=="Juice")beverageQueue.push(task);
    }
}
void displaystationQueues(){
    queue<string>gq=grillQueue,fq=fryerQueue,sq=saladQueue,oq=ovenQueue,dq=dessertQueue,bq=beverageQueue;
    cout<<"===Kitchen station Queues===\n";
    cout<<"Grill:\n";while(!gq.empty()){
        cout<<"_"<<gq.front()<<endl;
        gq.pop();
    }
    cout<<"Fryer:\n";while(!fq.empty()){
        cout<<"_"<<fq.front()<<endl;
        fq.pop();
    }
    cout<<"Salad:\n";while(!sq.empty()){
        cout<<"_"<<sq.front()<<endl;
        sq.pop();
    }
    cout<<"Oven:\n";while(!oq.empty()){
        cout<<"_"<<oq.front()<<endl;
        oq.pop();
    }
    cout<<"Dessert:\n";while(!dq.empty()){
        cout<<"_"<<dq.front()<<endl;
        dq.pop();
    }
    cout<<"Beverage:\n";while(!bq.empty()){
        cout<<"_"<<bq.front()<<endl;
        bq.pop();
    }
    cout<<"====================\n";
}
void kitchendisplay(const vector<order>& orders){
    cout<<"===KITCHEN DISPLAY ==="<<endl;
    for (auto& order:orders){
        order.print();
    }
    cout<<"================="<<endl;
}
void notifywaiter(const order&order){
    cout<<"Notify"<<order.waiter<<":order#"<<order.id<<"is READY.\n";
}
void performancereport(const vector<order>& completed){
    int total=0,fastest=INT_MAX,slowest=0;
    for(auto& o : completed) {
        total += o.estimatedtime;
        fastest=min(fastest,o.estimatedtime);
        slowest=max(slowest,o.estimatedtime);
    }
    cout<<"\n===PERFORMANCE REPORT===\n";
    cout<<"Total orders:"<<completed.size()<<endl;
    cout<<"Average time:"<<(completed.size() ? total/completed.size() : 0)<<"mins/n";
    cout<<"Fastest order:"<<fastest<<"mins\n";
    cout<<"Slowest order:"<<slowest<<"mins\n";
    cout<<"=========================\n";
}
int main(){
    vector<order> activeorders;
    vector<order> completedorders;
    int nextorderid=1;
    int numorders;
    cout<<"Enter number of orders:";
    cin>>numorders;
    cin.ignore();
    
    for(int i=0;i<numorders;i++){
        cout<<"\n---Entering order # "<<nextorderid<<"---\n";
        string waiter;
        cout<<"Waiter name:";
        getline(cin,waiter);

        bool vip;
        cout<<"Is VIP(1=yes,0=no):";
        cin>>vip;

        int numitems;
        cout<<"Number of items:";
        cin>>numitems;
        cin.ignore();

        vector<Fooditem> items;
        for(int j=0;j<numitems;j++){
            string itemname;
            int prep;
            string spec;
            cout<<"Item"<<j+1<<"name:";
            getline(cin,itemname);
            cout<<"Preparation time(minutes):";
            cin>>prep;
            cin.ignore();
            cout<<"Any specification(e.g.no onions,extra cheese):";
            getline(cin,spec);
            items.push_back(Fooditem(itemname,prep,spec));
        }
        activeorders.push_back(order(nextorderid++,items,waiter,vip));
    }
    for (int i=0;i<activeorders.size()-1;i++){
        for(int j=0;j<activeorders.size()-i-1;j++){
            if(!activeorders[j].isVIP && activeorders[j+1].isVIP)
            swap(activeorders[j],activeorders[j+1]);
        }
    }
    for(auto& order:activeorders){
        assigntostations(order);
        order.status=PREPARING;
    }
    kitchendisplay(activeorders);
    displaystationQueues();
    for(auto& order:activeorders){
        order.status=READY;
        notifywaiter(order);
        completedorders.push_back(order);
    }
    kitchendisplay(completedorders);
    performancereport(completedorders);

    return 0;
}
