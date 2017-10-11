#include<bits/stdc++.h>
#include "First&Follow.h"
using namespace std;

int main(){
	set<char>Nterminal;
	set<char>Terminal;
	vector<pair<char,string> > prod;
	map<char,set<char> > first;
	map<char,set<char> > follow;
	map<pair<char,char>,pair<char,string> >parseTable;
	map<char,set<char> >::iterator it1;
	set<char>::iterator it2;
	vector<pair<char,string> >::iterator it3;
	////////////////////////////Productions////////////////////////////////
	Nterminal.insert('A');
	Nterminal.insert('E');
	Nterminal.insert('T');
	Nterminal.insert('B');
	Nterminal.insert('F');
	prod.push_back(make_pair('A',"TE"));
	prod.push_back(make_pair('E',"+TE"));
	prod.push_back(make_pair('E',"^"));
	prod.push_back(make_pair('T',"FB"));
	prod.push_back(make_pair('B',"*FB"));
	prod.push_back(make_pair('B',"^"));
	prod.push_back(make_pair('F',"(A)"));
	prod.push_back(make_pair('F',"i"));
	//////////////////////////////////////////////////////////////////////
	////////////////////////FindingTerminals/////////////////////////////////
	for(it3=prod.begin();it3!=prod.end();it3++){
		int siz=it3->second.size();
		for(int i=0;i<siz;i++){
			if(it3->second[i]!='^' && Nterminal.find(it3->second[i])==Nterminal.end())Terminal.insert(it3->second[i]);
		}
	}
	Terminal.insert('$');
	// cout<<Terminal.size();
	//////////////////////////////////////////////////////////////////////
	////////////////////////CheckingFirst/////////////////////////////////
	createFirst(first,Nterminal,prod);
	// for(it1=first.begin();it1!=first.end();it1++){
	// 	cout<<it1->first<<"	-	";
	// 	for(it2=it1->second.begin();it2!=it1->second.end();it2++)cout<<(*it2)<<" ";
	// 	cout<<endl;
	// }
	/////////////////////////////////////////////////////////////////////
	////////////////////////CheckingFollow/////////////////////////////////
	cout<<endl;	
	createFollow(follow,Nterminal,prod,first);
	// for(it1=follow.begin();it1!=follow.end();it1++){
	// 	cout<<it1->first<<"	-	";
	// 	for(it2=it1->second.begin();it2!=it1->second.end();it2++)cout<<(*it2)<<" ";
	// 	cout<<endl;
	// }
	//////////////////////////////////////////////////////////////////////////
	////////////////////////CreatingParseTable/////////////////////////////////
	createParseTable(parseTable,first,follow,prod,Nterminal);
	printParseTable(Nterminal,Terminal,parseTable);
	//////////////////////////////////////////////////////////////////////////
	cout<<"Enter string to be parsed - ";
	string var;
	cin>>var;
	if(parse(var,parseTable,Nterminal)==1)cout<<"String belongs to given language";
	else cout<<"String doesn't belongs to given language";
	cout<<endl;
	return 0;
}

