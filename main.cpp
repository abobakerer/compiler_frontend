#include <bits/stdc++.h>
#include <fstream>

using namespace std;

#define IO ios_base::sync_with_stdio(0),cin.tie(0),cout.tie(0);
#define endl '\n'
#define ll long long
#define YES(x) (x)?cout<<"YES"<<endl : cout<<"NO"<<endl
#define FOR(i,n,s) for(int i=s;i<n;i++)
#define sep " | "
#define vecll vector<ll>
#define pb(x) push_back(x)
#define all(v) v.begin(),v.end()
#define sz(x) x.size()
void output(string s) {
    #ifndef ONLINE_JUDGE
        cout << s;
    #endif
}

vector<string> split(string line,char dil){
    vector<string> res;
    string s;
    int startCut = 0;
    for(int i=0 ; i<line.size() ; i++){
        if(line[i] == dil){
            s = line.substr(startCut,i-startCut);
            res.push_back(s);
            s = "";
            startCut = i+1;
        }
    }
    res.push_back(line.substr(startCut));
    return res;
}

void clearStack(stack<string> &s){
    while(!s.empty()){
        s.pop();
    }
}

//parser data
string startSympol;
map<string,set<string>> first;
map<string,set<string>> follow;
map<pair<string,string> , vector<string>> table;

//scanner data
vector<string> states,finalStates,deadStates;
string startState;
map<string,string> tokens;
map<pair<string,string>,string> transitionTable;





// get grammer
map<string,vector<string>> getGrammer(){
    fstream in;

    //take grammer
    map<string,vector<string>> grammer;
    in.open("grammer.txt",ios::in);
    if(in.is_open()){
        string line;
        int i = 1;
        while(getline(in,line)){
            if(i)
                startSympol = split(line,' ')[0];
            i = 0;
            vector<string> vec = split(line,' ');
            string t = line.substr(vec[0].size()+1);
            grammer[vec[0]].push_back(t);
        }
        in.close();
    }
    return grammer;
}

//make first
void makeFirst(string nonT, map<string,vector<string>> grammer){
    bool finish = true;
    vector<string> rules = grammer[nonT];
    for(string rule : rules){
        finish = true;
        vector<string> symbols = split(rule,' ');
        for(string s : symbols){
            if(grammer.find(s) == grammer.end()){
                first[nonT].insert(s);
                finish = true;
            }
            else{
                makeFirst(s,grammer);
                finish = true;
                for(string ter : first[s]){
                    if(ter == "ep")
                        finish = false;
                    else
                        first[nonT].insert(ter);
                }
            }
            if(finish)
                break;
        }
        if(!finish)
            first[nonT].insert("ep");
    }
}

//make follow
void makeFollow(string nonT,map<string,vector<string>> grammer){
    for(auto [a,b] : grammer){
        for(string rule : b){
            vector<string> symbols = split(rule,' ');
            if(nonT == startSympol){
                    follow[nonT].insert("$");
            }
            if(find(all(symbols),nonT) != symbols.end()){
                int index = find(all(symbols),nonT) - symbols.begin();

                if(index != symbols.size()-1){
                    if(grammer.find(symbols[index+1]) == grammer.end())
                            follow[nonT].insert(symbols[index+1]);
                    else{
                        bool finish = true;
                        for(int i = index+1;i<symbols.size();i++){
                            if(grammer.find~(symbols[index+1]) == grammer.end()){
                                follow[nonT].insert(symbols[index+1]); // variable finish will exit
                            }
                            else{
                                finish = true;
                                for(string ff : first[symbols[i]]){
                                    if(ff == "ep"){
                                        finish = false;
                                    }
                                    else
                                        follow[nonT].insert(ff);
                                }
                            }
                            if(finish)
                                break;
                            if(i == symbols.size()-1){
                                if(a == nonT){
                                    continue;
                                }
                                else{
                                    if(follow.find(a) != follow.end()){
                                        for(string s : follow[a]){
                                            follow[nonT].insert(s);
                                        }
                                    }
                                    else{
                                        makeFollow(a,grammer);
                                        for(string s : follow[a]){
                                            follow[nonT].insert(s);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if(index == symbols.size()-1){
                    if(a == symbols[index]){
                        continue;
                    }
                    else{
                        if(follow.find(a) != follow.end()){
                            for(string s : follow[a]){
                                follow[nonT].insert(s);
                            }
                        }
                        else{
                            makeFollow(a,grammer);
                            for(string s : follow[a]){
                                follow[nonT].insert(s);
                            }
                        }
                    }
                }
            }
        }
    }
}

//make predictive table
void makePredictiveTable(map<string,set<string>> first, map<string,set<string>> follow,  map<string,vector<string>> grammer){
    for(auto [a,b] : grammer){
        for(string s : b){
            vector<string> temp = split(s,' ');
            set<string> ff;
            if(first.find(temp[0]) != first.end())
                ff = first[temp[0]];
            else
                ff.insert(temp[0]);
            for(auto x : ff){
                if(x == "ep"){
                    set<string> follows = follow[a];
                    for(auto j : follows)
                        table[{a,j}] = split(s,' ');
                }
                else{
                    table[{a,x}] = split(s,' ');
                }
            }
        }
    }
}

//take all states
void takeStates(){
    fstream in;
    in.open("automaton.txt",ios::in);
    if(in.is_open()){
        int countt=1;
        string line;
        while(getline(in,line)){
            if(countt == 1){
                states = split(line,' ');
            }
            else if(countt == 2){
                startState = line;
            }
            else if(countt == 3){
                finalStates = split(line,' ');
            }
            else if(countt == 4){
                deadStates = split(line,' ');
            }
        countt++;
        }
        in.close();
    }
}

//take tokens
void takeTokens(){
    fstream in;
    in.open("tokens.txt",ios::in);
    if(in.is_open()){
        string s;
        while(getline(in,s)){
            vector<string> t = split(s,' ');
            tokens[t[0]] = t[1];
        }
        in.close();
    }
}

//take transition table
void takeTransitionTable(){
    fstream in;
    in.open("transition.txt",ios::in);
    if(in.is_open()){
        string s;
        while(getline(in,s)){
            vector<string> t = split(s,' ');
            transitionTable[{t[0],t[1]}] = t[2];
        }
        in.close();
    }
}

void scan(){
    fstream in,out;
    bool flag = true;
    in.open("input.txt",ios::in);
    out.open("scanner_output.txt",ios::out);
    if(in.is_open()){
        string s;
        string currState = startState;
        while(getline(in,s)){
            currState = startState;
            flag = true;
            string pr="";
            for(int i=0;i<s.size();i++){

                string str(1,s[i]),nextStr(1,s[i+1]);
                pair<string,string> key = {currState,str};
                pr += str;

                if(find(all(deadStates),currState) != deadStates.end()){
                    cout<<"This String Is Invalid ! "<<endl;
                    flag = false;
                    break;
                }

                if(transitionTable.find(key) != transitionTable.end()){
                    currState = transitionTable[key];
                }
                else{
                    cout<<"ERROR This Not accepted in this scanner"<<endl;
                    flag = false;
                    break;
                }

                if(find(all(finalStates),currState) != finalStates.end()){
                    if(transitionTable[{currState,nextStr}] == currState)
                        continue;
                    //string cToken = pr + "  |  " + tokens[currState]; // this for me
                    string cToken = tokens[currState]; // this for parser
                    //cout<<cToken<<endl;
                    if(i == s.size()-1)
                        out << cToken ;
                    else
                        out << cToken << ' ';
                    currState = startState;
                    pr = "";
                }
            }

            out<<endl;
        }
        in.close();
        out.close();
    }
}

void parsing(){

    fstream in,out;
    in.open("scanner_output.txt",ios::in);
    out.open("parserResult.txt",ios::out);
    stack<string> instructions;
    if(in.is_open()){
        string line;
        while(getline(in,line)){
            clearStack(instructions);
            instructions.push("$");
            instructions.push(startSympol);
            bool flag = true;
            vector<string> input = split(line,' ');
            input.push_back("$");
            for(int i=0;i<input.size();){
                if(instructions.top() == "ep"){
                    instructions.pop();
                }
                //cerr<<instructions.top()<<"    "<<sep<<"    '"<<input[i]<<"'"<<endl;
                if(instructions.top() == input[i]){
                    instructions.pop();
                    i++;
                    continue;
                }

                if(table.find({instructions.top(),input[i]}) != table.end()){
                    vector<string> temp = table[{instructions.top(),input[i]}];
                    instructions.pop();
                    for(int j=temp.size()-1;j>=0;j--){
                        //cerr<<"hola hola : "<<temp[j]<<endl;
                        instructions.push(temp[j]);
                    }
                }
                else{
                    flag = false;
                    break;
                }
            }
            if(flag && instructions.empty()){
                cout<<line<<" - This Is A valid Input. "<<endl;
                out << line << " - This Is A valid Input. "<<endl;
            }
            else{
                cout<<line<<" - This Is A Invalid Input!!! "<<endl;
                out<<line<<" - This Is A Invalid Input!!! "<<endl;
            }

        }
        in.close();
        out.close();
    }
}


int main()
{

    //take grammer
    map<string,vector<string>> grammer = getGrammer();

    //loop in grammer

    for(auto [a,b] : grammer){
        cout<<a<<" : ";
        for(string x : b)
            cout<<x<<" | ";
        cout<<endl;
    }



    cout<<"=======================================\n\n";
    //make first
    for(auto [a,b] : grammer){
        makeFirst(a,grammer);
    }

    //loop on first

    for(auto [a,b] : first){
        cout<<a<<" : ";
        for(string x : b)
            cout<<x<<" | ";
        cout<<endl;
    }


    cout<<"=======================================\n\n";
    //make follow
    for(auto [a,b] : grammer){
        makeFollow(a,grammer);
    }

    //loop on follow

    for(auto [a,b] : follow){
        cout<<a<<" : ";
        for(string x : b)
            cout<<x<<" | ";
        cout<<endl;
    }


    cout<<"=======================================\n\n";
    //make predictive table
    makePredictiveTable(first,follow,grammer);

    //loop on predictive table

    for(auto [a,b] : table){
            cout<<a.first<<" "<<a.second<<" ";
            for(string x : b)
                cout<<x<<" ";
            cout<<endl;
    }


    cout<<"=======================================\n\n";

    takeStates();
    takeTokens();
    takeTransitionTable();
    scan();
    parsing();


    return 0;
}
