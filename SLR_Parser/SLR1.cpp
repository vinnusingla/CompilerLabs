#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace std;
string array1[100][2];
string terminals[100];
string variables[100];
map<string,string> m_first;
map<string,string> m_follow;
map<int ,map<string,int> > mp;
vector<vector<string> > items;
map<string,vector<string> > closures;
int cnt=0;
int p_cnt=0;
int t_cnt=0;
int len(string array1[100][2]){
	int c=0;
	for(int i=0;i<100;i++){
		if(array1[i][0]!=""){
			c++;
		}
	}
	return c;
}
int len(string variables[100]){
	int c=0;
	for(int i=0;i<100;i++){
		if(variables[i]!="s"){
			c++;
		}
	}
	return c;
}
bool checkt(string a){
	for(int i=0;i<len(terminals);i++){
		if(terminals[i]==a){
			return 0;
		}
	}
	return 1;
}
void read_grammar(){
	ifstream file;
	file.open("grammar1.txt");
	string line;
	int i=0;
	while(getline(file,line)){
		int j=0;
		string t="";
		while(line[j]!=':'){
			t=t+(line[j]);
			j++;
		}
		j++;
		array1[i][0]=t;

		variables[cnt]=t[0];
		cnt++;
		t="";
		while(j<line.length()){
			if(line[j]=='|'){
				array1[i][1]=t;
				t="";
				j++;
				array1[i+1][0] = array1[i][0];
				i++;
			}
			else{
				t=t+line[j];
				j++;
			}
			for(int z=0; z<t.length(); z++)	{
				int f=0;
				if(t[z]<65 || t[z]>90){
					for(int p=0;p<t_cnt;p++){
						if(string(1,t[z])==terminals[p] || (t[z]=='^' && terminals[p] == "$")){
							f=1;
							break;
						}
					}
					if(!f){
						if(t[z]=='^'){
							if(checkt("$")){
								terminals[t_cnt++] = "$";
							}
						}
						else{	
							if(checkt(string(1,t[z]))){
								terminals[t_cnt++] = string(1,t[z]);
							}
						}
					}
				}
			}
		}

		array1[i][1]=t;
		i++;
	}
	if(checkt("$")){
		terminals[t_cnt++] = "$";
	}
}
int checke(string a, string b)
{
	string s = m_first[a];
	int l = s.length();
	for(int i = 0;i<l;i++)
	{		
		if(s[i] == b[0])
		return 0;
	}
	return 1;
}
int checkf(string a, string b)
{
	string s = m_follow[a];
	int l = s.length();
	for(int i = 0;i<l;i++)
	{		
		if(s[i] == b[0])
		return 0;
	}
	return 1;
}

int check(vector<string> s, string b)
{
	int l = s.size();
	for(int i = 0;i<l;i++)
	{		
		if(s[i] == b)
		return 0;
	}
	return 1;
}
int checkv(vector<string> p,vector<string> q)
{	
	int m,n;
	n = p.size();
	m = q.size();
	if(m!=n)
	{
		return 0;
	}		
	for(int z=0;z<n;z++)
	{

		int w=0;
		for(;w<n;w++)
		{
			if(p[z]==q[w])
				break;
		}
		if(w == n)
		{
			return 0;
		}
	}
	return 1;
}
string first(string a){
	m_first[a]="";
	int l = len(array1);
	p_cnt=l;
	for(int i=0;i<l;i++){
		if(array1[i][0]!=a){
			continue;
		}
		if(array1[i][1]=="^" && checke(a,"^")){
			m_first[a]+='^';
			continue;
		}
		string s = array1[i][1];
		if((int(s[0])<65 || int(s[0])>90) && checke(a,string(1,s[0]))){
			m_first[a]+=s[0];
			continue;
		}
		else{
			string stri;
			for(int i=0;i<s.length();i++){
				
				if(m_first[string(1,s[i])]!=""){
					stri=m_first[string(1,s[i])];
				}
				else{
					if(string(1,s[i])!=a){
						first(string(1,s[i]));
						stri=m_first[string(1,s[i])];
					}
					
				}
				for(int j=0;j<stri.length();j++){
					if(checke(a,string(1,stri[j])))
					{	
						m_first[a]+=stri[j];
					}
				}
				if(checke(string(1,s[i]),"^")){
					break;
				}
			}
		}
	}
	return m_first[a];
}

string follow(string a,int flag){
	if(flag){
		m_follow[a]="";
	}
	int l = len(array1);
	for(int i=0;i<l;i++){
		string stri=array1[i][1];
		for(int j=0;j<stri.length();j++){
			if(string(1,stri[j])==a){
				if(j!=stri.length()-1){
					// add first(beta) to the follow(A)

					if(stri[j+1]<65 || stri[j+1]>90){
						m_follow[a]+=string(1,stri[j+1]);
					}
					else{
						string st = m_first[string(1,stri[j+1])];
						for(int k=0; k<st.length();k++)
						{
							if(st[k]!='^' && checkf(a,string(1,st[k]))){
								m_follow[a] += string(1,st[k]);
							}
							else if(st[k]=='^' && array1[i][0]!=a){
								string s_fl;
								if(m_follow[array1[i][0]]==""){
									s_fl = follow(array1[i][0],1);
								} 
								else{
									s_fl = m_follow[array1[i][0]];
								}
								for(int l=0;l<s_fl.length();l++){
									if(checkf(a,string(1,s_fl[l]))){
										m_follow[a] += string(1,s_fl[l]);
									}
								}
							}
						}
					}
				}
				else{
					if(array1[i][0]==a){
						continue;				
					}
					string s_fl;
					if(m_follow[array1[i][0]]==""){
						s_fl = follow(array1[i][0],1);
					} 
					else{
						s_fl = m_follow[array1[i][0]];
					}
					for(int l=0;l<s_fl.length();l++){
						if(checkf(a,string(1,s_fl[l]))){
							m_follow[a] += string(1,s_fl[l]);
						}
					}
				}
			}
		}
		
	}
	return m_follow[a];

}
vector<string> closure(string s){
	if(closures[s].size()!=0){
		return closures[s];
	}
	else{
		vector<string> clos ;
		clos.push_back(s);
		int i=0;
		for(;i<s.length();i++){
			if(s[i]=='.'){
				break;
			}
		}
		i++;
		if(i<s.length()){
			if(s[i]>=65 && s[i]<=90){
				for(int j=0;j<len(array1);j++){
					if(array1[j][0]==string(1,s[i]) && (check(clos,array1[j][0]+":."+array1[j][1]))){
						vector<string> c=closure(array1[j][0]+":."+array1[j][1]);
						for(int k=0;k<c.size();k++){
							if(check(clos,c[k])){
								clos.push_back(c[k]);
								
							}
						}
					}
				}
			}
			
		}
		closures[s]=clos;
		return clos;
	}
	
	
}
vector<string> gt(vector<string> s,char a){
	vector<string> got;
	for(int i=0;i<s.size();i++){
		string str=s[i];
		int j=0;
		string str1="";
		int flag=0;
		for(;j<str.length();j++){
			if(str[j]!='.'){
				str1+=str[j];
			}
			else{
				if(j+1<str.length() && str[j+1]==a){
					str1=str1+a+".";
					j++;
				}
				else{
					flag=1;
					break;
				}
				
			}
		}
		if(flag==0){
			vector<string> got1=closure(str1);
			for(int j=0;j<got1.size();j++){
				if(check(got,got1[j])){
					got.push_back(got1[j]);
				}
			}
		}
	}
	return got;
}
int create_graph()
{
	vector<string> a;  
	for(int i=0;i<items.size();i++)
	{
		for(int j=0;j<len(variables);j++)
		{
			a = gt(items[i],variables[j][0]);
			int k=0;
			for(;k<items.size();k++)
			{
				if(checkv(a,items[k]))
				{
					mp[i][variables[j]]=k+1;
					break;
				}
			}
			if(k==items.size() && a.size()!=0)
			{
				items.push_back(a);
				mp[i][variables[j]]=items.size();
			}
			
		}
		for(int j=0;j<len(terminals);j++)
		{
			a = gt(items[i],terminals[j][0]);
			int k=0;
			for(;k<items.size();k++)
			{
				if(checkv(a,items[k]))
				{
					mp[i][terminals[j]]=k+1;
					break;
				}
			}
			if(k==items.size() && a.size()!=0)
			{
				items.push_back(a);
				mp[i][terminals[j]]=items.size();
			}
			
		}
	}
}
void print_st(string stack[5000],string input,int st_len){
	for(int i=0;i<=st_len;i++){
		cout<<stack[i];
	}
	cout<<"\t\t\t"<<input<<endl;
	//cout<<setw(35)<<input<<endl;
}
void create_table(){
	string table[items.size()][len(terminals)+len(variables)];
	for(int i=0;i<items.size();i++){
		for(int j=0;j<len(terminals)+len(variables);j++){
			table[i][j]="";
		}
	}
	for(int i=0;i<items.size();i++){
		for(int j=0;j<len(terminals);j++){
			if(mp[i][terminals[j]]!=0){
				stringstream temp;
				temp<<mp[i][terminals[j]]-1;
				table[i][j]="s"+temp.str();
			}
		}
		for(int j=0;j<len(variables);j++){
			if(mp[i][variables[j]]!=0){
				stringstream temp;
				temp<<mp[i][variables[j]]-1;
				table[i][j+len(terminals)]=temp.str();
			}
		}
	}
	for(int i=0;i<items.size();i++){
		for(int j=0;j<items[i].size();j++){
			string s=items[i][j];
			if(s[s.length()-1]=='.'){
				int k=0;
				string str=s.substr(0,s.length()-1);
				cout<<str<<endl;
				for(;k<len(array1);k++){
					if(str==array1[k][0]+":"+array1[k][1]){
						break;
					}
				}
				str=m_follow[string(1,s[0])];
				for(int a=0;a<str.length();a++){	
					int b=0;
					for(;b<len(terminals);b++){
						if(terminals[b]==string(1,str[a])){
							break;
						}
					}
					stringstream temp;
					temp<<k;
					if(k==0){
						table[i][b]="A";
					}
					else{
						table[i][b]="r"+temp.str();	
					}
					
				}
				
			}
		}
	}
	for(int i=0;i<len(terminals);i++){
		cout<<'\t'<<terminals[i];
	}
	for(int i=0;i<len(variables);i++){
		cout<<'\t'<<variables[i];
	}
	cout<<endl;
	for(int i=0;i<items.size();i++){
		cout<<i;
		for(int j=0;j<len(terminals)+len(variables);j++){
			cout<<'\t'<<table[i][j];
		}
		cout<<endl;
	}
	cout<<"Enter the String you want to parse"<<endl;
	string input;
	cin>>input;
	input=input+"$";
	string stack[5000];
	stack[0]="0";
	int st_len=0;
	print_st(stack,input,st_len);
	while(true){
		int k=0;
		for(;k<len(terminals);k++){
			if(string(1,input[0])==terminals[k]){
				break;
			}
		}
		stringstream s1;
		s1<<stack[st_len];
		
		int cvt;
		s1>>cvt;
		string rule=table[cvt][k];
		if(rule==""){
			cout<<"String is not in the grammar"<<endl;
			break;
		}
		if(rule[0]=='s'){
			stack[st_len+1]=input[0];
			st_len++;
			input=input.substr(1,input.length()-1);
			stack[st_len+1]=rule.substr(1,rule.length()-1);
			st_len++;
			print_st(stack,input,st_len);
		}
		else if(rule[0]=='r'){
			stringstream ss;
			ss<<rule.substr(1,rule.length()-1);
			int z;
			ss>>z;
			int prod_len=array1[z][1].length();
			string go_ch=array1[z][0];
			st_len=st_len-(2*prod_len);
			stack[st_len+1]=go_ch;
			st_len++;
			int in=0;
			for(;in<len(variables);in++){
				if(go_ch==variables[in]){
					break;
				}
			}
			//print_st(stack,input,st_len);
			stringstream stm;
			stm<<stack[st_len-1];
			stm>>z;
			//print_st(stack,input,st_len);
			//cout<<in+len(terminals)<<endl;
			//cout<<table[z][in+len(terminals)]<<endl;
			if(table[z][in+len(terminals)]==""){
				cout<<"String is not in the grammar"<<endl;
				break;
			}
			stack[st_len+1]=table[z][in+len(terminals)];
			st_len++;
			print_st(stack,input,st_len);
			
		}
		else{
			if(input.length()>1){
				cout<<"String is not in the grammar"<<endl;
				break;
			}
			cout<<"String exists in the grammar"<<endl;
			break;
		}
	}
	
}
int main(){
	closures.clear();
	mp.clear();
	for(int i=0;i<100;i++){
		array1[i][0]="";
		array1[i][1]="";
	}
	for(int i=0;i<100;i++){
		variables[i]="s";
	}
	for(int i=0;i<100;i++){
		terminals[i]="s";
	}
	m_first.clear();
	read_grammar();
	for(int i=0;i<cnt;i++){
		if(m_first[variables[i]]==""){
			first(variables[i]);
		}
	}
	for(int i=0;i<cnt;i++){
		if(m_follow[variables[i]]==""){
			if(i==0){
				m_follow[variables[i]]="$";
				follow(variables[i],0);
			}
			else{
				follow(variables[i],1);
			}
		}
	}
	vector<string> s =closure(array1[0][0]+":."+array1[0][1]);
	items.push_back(s);
	create_graph(); 
	create_table();
	
}
