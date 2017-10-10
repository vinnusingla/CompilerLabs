#include<bits/stdc++.h>
using namespace std;

template<typename S>
S union_sets(const S& s1, const S& s2){
     S result = s1;
     result.insert(s2.begin(), s2.end());
     return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
set<char> firstOfString(map<char,set<char> > &first,set<char> &Nterminal,string &s){
	int siz=s.size();
	set<char>x;
	for(int i=0;i<siz;i++){
		x.erase('^');
		if(Nterminal.find(s[i])==Nterminal.end()){
			x.insert(s[i]);
			break;
		}
		else{
			x=union_sets(x,first[s[i]]);
			if(x.find('^')==x.end())break;
		}
	}
	//cout<<"Enteredfos "<<s<<"	"<<x.size()<<endl;
	return x;
}

///////////////////////////////////////////////////////First////////////////////////////////////////////////////////////////////////////
void firsty(map<char,set<char> > &ans,char c,vector<pair<char,string> > &prod,set<char> &Nterminal){
	vector<pair<char,string> >::iterator it;
	ans[c];
	for(it=prod.begin();it!=prod.end();it++){
		if(it->first==c){
			int siz =it->second.size();
			for(int i=0;i<siz;i++){
				if(Nterminal.find(it->second[i])==Nterminal.end()){
					ans[c].insert(it->second[0]);
					break;
				}
				else{
					if(it->second[i]==c)break;
					firsty(ans,it->second[i],prod,Nterminal);
					ans[c]=union_sets(ans[c],ans[it->second[i]]);
					if(i!=siz-1)ans[c].erase('^');
					if(ans[it->second[i]].find('^')==ans[it->second[i]].end())break;
				}
			}
		}
	}
}

void createFirst(map<char,set<char> > &ans,set<char> &Nterminal,vector<pair<char,string> > &prod){	
	set<char>::iterator it;
	for(it=Nterminal.begin();it!=Nterminal.end();it++){
		if(ans.find(*it)==ans.end()){
			firsty(ans,*it,prod,Nterminal);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////Follow///////////////////////////////////////////////////////////////////////
void followy(map<char,set<char> > &ans,char c,vector<pair<char,string> > &prod,set<char> &Nterminal,map<char,set<char> > first){
	vector<pair<char,string> >::iterator it;
	ans[c];
	// cout<<c<<endl;
	for(it=prod.begin();it!=prod.end();it++){
		int siz=it->second.size();
		for(int i=0;i<siz;i++){
			if(it->second[i]==c){
				set<char>x;				
				if(c=='A')x.insert('$');
				x.insert('^');
				for(int j=i+1;j<siz;j++){
					x.erase('^');
					if(Nterminal.find(it->second[j])!=Nterminal.end()){
						x=union_sets(x,first[it->second[j]]);
						if(x.find('^')==x.end())break;
					}
					else{
						x.insert(it->second[j]);
						x.erase('^');
						break;
					}
				}			
				if(x.find('^')!=x.end() && it->first!=c){
					// cout<<"Entered c = "<<c<<" prod = "<<it->first<<" --> "<<it->second<<endl;
					if(ans.find(it->first) == ans.end())followy(ans,it->first,prod,Nterminal,first);
					x=union_sets(x,ans[it->first]);
				}
				x.erase('^');
				ans[c]=union_sets(x,ans[c]);
				break;
			}
		}
	}
}

void createFollow(map<char,set<char> > &ans,set<char> &Nterminal,vector<pair<char,string> > &prod,map<char,set<char> > first){	
	set<char>::iterator it;
	for(it=Nterminal.begin();it!=Nterminal.end();it++){
		if(ans.find(*it)==ans.end()){
			followy(ans,*it,prod,Nterminal,first);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////CreateParseTable/////////////////////////////////////////////////////////

void createParseTable(map<pair<char,char>,pair<char,string> > &ans,map<char,set<char> > &first ,
					  map<char,set<char> > &follow ,vector<pair<char,string> > &prod,set<char> &Nterminal){
	vector<pair<char,string> >::iterator it;
	set<char>::iterator it2;
	set<char>::iterator z;
	set<char>x;
	for(it=prod.begin();it!=prod.end();it++){
		x=firstOfString(first,Nterminal,it->second);
		if(x.find('^')!=x.end()){
			x.erase('^');
			z=follow[it->first].end();
			for(it2=follow[it->first].begin();it2!=z;it2++){
				ans[make_pair(it->first,*it2)]= *it ;
			}
		}
		for(it2=x.begin();it2!=x.end();it2++){
			ans[make_pair(it->first,*it2)]= *it ;
		}
	}
}

void printParseTable(set<char> &Nterminal,set<char> &Terminal ,map<pair<char,char>,pair<char,string> > &parseTable){
	set<char>::iterator it1;
	set<char>::iterator it2;
	pair<char,string>x;
	int siz=0;
	for(it2=Terminal.begin();it2!=Terminal.end();it2++)siz++;
	siz++;
	for(int i=0;i<siz;i++)cout<<"___________";
	cout<<endl;
	cout<<setw(11)<<"|";
	for(it2=Terminal.begin();it2!=Terminal.end();it2++)cout<<setw(10)<<(*it2)<<"|";
	cout<<endl;
	for(int i=0;i<siz;i++)cout<<"__________|";cout<<endl;
	for(it1=Nterminal.begin();it1!=Nterminal.end();it1++){
		cout<<setw(10)<<(*it1)<<"|";
		for(it2=Terminal.begin();it2!=Terminal.end();it2++){
			if(parseTable.find(make_pair(*it1,*it2))!=parseTable.end()){
				x=parseTable[make_pair(*it1,*it2)];
				x.second=" -> "+ x.second;
				x.second=x.first+x.second;
				cout<<setw(10)<<x.second<<"|";
			}
			else cout<<setw(10)<<"Blank"<<"|";
		}
		cout<<endl;
	}
	for(int i=0;i<siz;i++)cout<<"__________|";
	cout<<endl;
}