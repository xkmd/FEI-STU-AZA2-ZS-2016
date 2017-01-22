//
//  main.cpp
//  azaz2
//
//  Created by MacAir on 27.12.16.
//  Copyright © 2016 MacAir. All rights reserved.
//

#include <iostream>
#include <set>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;


class Vertex{
private:
    int id;
    string keyStr;
    string fileName;
    set <Vertex*> neighbors;
public:
    Vertex(int ide, string keyString, string fileN){
        id = ide;
        keyStr = keyString;
        fileName = fileN;
    }
    int getId(){
        return id;
    }
    void setId(int id){
        this->id = id;
    }
    string getKeyStr(){
        return keyStr;
    }
    void setKeyStr(string keyStr){
        this->keyStr = keyStr;
    }
    string getFileName(){
        return fileName;
    }
    void setFileName(string fileName){
        this->fileName = fileName;
    }
    set <Vertex*> getNeighbors(){
        return neighbors;
    }
    void setNeighbor(Vertex *n){
        neighbors.insert(n);
    }
};

class Graph{
    
public:
    Graph(){
        
    }
    void loadKeyWords(const char* fileName){
        //cout << "keywords:" << endl;
        ifstream file;
        file.open(fileName);
        
        string new_line;
        
        
        
        while (getline(file, new_line)) {
            string correct = "";
            for(int i = 0; i < new_line.size(); i ++){
                if(isalnum(new_line[i])){
                    correct += new_line[i];
                }
            }
            keyWords.push_back(correct);
            //cout << correct << endl;
        }
        file.close();
    }
    
    void loadFiles(const char* fileName){
        //cout << "files:" << endl;
        ifstream file;
        file.open(fileName);
        
        string new_line;
        while (getline(file, new_line)) {
            string correct = "";
            for(int i = 0; i < new_line.size(); i++){
                if(isgraph(new_line[i]))
                    correct+=new_line[i];
            }
            files.push_back(correct);
            //cout <<  correct << endl;
        }
        file.close();
    }
    void loadArticles(){
        //cout << "articles & filenamesizes:" << endl;
        for(int i = 0; i < files.size(); i++){
            ifstream file(files[i]);
            char c;
            string article = "";
            while (file.get(c)){
                if(isalnum(c))
                    article += c;
            }
            //cout << article << " " << files[i].size() << endl;
            articles.push_back(article);
            
            file.close();
        }
    }
    /**
     * Compute temporary array to maintain size of suffix which is same as prefix
     * Time/space complexity is O(size of pattern)
     */
    int* computeTemporaryArray(string keyWord){
        int n = (int)keyWord.size();
        int *lps = (int*)malloc(n*sizeof(int));
        int index = 0;
        for(int i=1; i < n;){
            if(keyWord[i] == keyWord[index]){
                lps[i] = index + 1;
                index++;
                i++;
            }else{
                if(index != 0){
                    index = lps[index-1];
                }else{
                    lps[i] = 0;
                    i++;
                }
            }
        }
        return lps;
    }
    
    /**
     * KMP algorithm of pattern matching.
     */
    vector <long> KMP(string article, string keyWord){
        vector <long> kwPositions;
        int *lps = computeTemporaryArray(keyWord);
        int i = 0;
        int j = 0;
        while(i < article.size() && j < keyWord.size()){
            if(article[i] == keyWord[j]){
                i++;
                j++;
            }else{
                if(j!=0){
                    j = lps[j-1];
                }else{
                    i++;
                }
            }
            if(j == keyWord.size()){
                j = 0;
                kwPositions.push_back(i - keyWord.size());
            }
        }
        return kwPositions;
    }
    
    void mapKWP(){
        for(int i = 0; i < files.size(); i++){
            map <long, char> fefsp;
            for(int j = 0; j < keyWords.size(); j++){
                vector <long> kwIndexes = KMP(articles[i], keyWords[j]);
                for(int x = 0; x < kwIndexes.size(); x++){
                    fefsp[kwIndexes[x]] = (char)(j + 48);
                }
            }
            forString[i] = fefsp;
        }
        
    }
    
    void createVertices(){
        //cout << "vertices:" << endl;
        for(int i = 0; i < files.size(); i++){
            string keyString = "";
            for(auto it : forString[i]){
                keyString += it.second;
            }
            Vertex *v = new Vertex(i, keyString, files[i]);
            P.insert(v);
            vertices.push_back(v);
            //cout << v->getId() << endl;
        }
    }
    
    long max(long a, long b)
    { return(a>b)?a:b;}
    long min(long a, long b)
    { return(a<b)?a:b;}
    
    int longestSubstr(string s, string t) {
        if (s.empty() || t.empty()) {
            return 0;
        }
        
        long m = s.size();
        long n = t.size();
        int cost = 0;
        int maxLen = 0;
        vector<int> p;
        p.resize(n);
        vector<int> d;
        d.resize(n);
        
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (s[i] != t[j]) {
                    cost = 0;
                } else {
                    if ((i == 0) || (j == 0)) {
                        cost = 1;
                    } else {
                        cost = p[j - 1] + 1;
                    }
                }
                d[j] = cost;
                
                if (cost > maxLen) {
                    maxLen = cost;
                }
            }
            vector<int> swap = p;
            p = d;
            d = swap;
        }
        
        return maxLen;
    }
    
    void addNeighbors(){
        //cout << "lcs & min: " << endl;
        for(int i = 0; i < files.size() - 1; i++){
            for(int j = i + 1; j < files.size(); j++){
                int lcs = longestSubstr(vertices[i]->getKeyStr(), vertices[j]->getKeyStr());
                int mini = min(vertices[i]->getKeyStr().size(), vertices[j]->getKeyStr().size());
                if(lcs >= 0.2 * mini){
                    vertices[i]->setNeighbor(vertices[j]);
                    vertices[j]->setNeighbor(vertices[i]);
                }
                //cout << " l: " << lcs << " m: " << mini;
            }
            //cout << endl;
        }
    }
    
    /*
    void testing(){
        set <Vertex*> P;
        set <Vertex*> R;
        set <Vertex*> X;
        
        vector <Vertex*> V;
        
        for(int i = 0; i < 20; i++){
            string s = "v";
            s += (char)48;
            Vertex *v = new Vertex(i, s, "file");
            V.push_back(v);
        }
        for(int i = 0; i < 19; i++){
            for(int j = i + 1; j < 20; j++){
                V[i]->setNeighbor(V[j]);
                V[j]->setNeighbor(V[i]);
            }
        }
        
        for(auto it : V){
            P.insert(it);
        }
        
        for(auto it: P){
            cout << it->getId() << ": ";
            for(auto it1: it->getNeighbors()){
                cout << it1->getId() << " ";
            }
            cout << endl;
        }
        
        BronKerbosch(R, P, X);
    }*/
    
    void writeResults(const char* fileName){
        set <Vertex*> P1 = P;
        set <Vertex*> R;
        set <Vertex*> X;
        
        BronKerbosch(R, P1, X);
        
        ofstream myfile;
        myfile.open (fileName);
        for(auto it : results){
            myfile << it << endl;
        }
        myfile.close();
        //cout << "neigbors & keystrings:" << endl;
        for(auto it : P){
            //cout << it->getId() << ": ";
            for(auto it1: it->getNeighbors()){
                //cout << it1->getId() << " ";
            }
            //cout << "ks: " << it->getKeyStr() << endl;
        }
        
    }
    void BronKerbosch(set <Vertex*> R, set <Vertex*> P, set <Vertex*> X){
        if(P.empty() && X.empty()){
            for(auto it : R)
                //cout << it->getId() << " ";
                results.push_back(it->getFileName());
            //cout << endl;
        }
            set <Vertex*> nP = P;
        for(auto it : P){
            set <Vertex*> nR = R;
            nR.insert(it);
            set <Vertex*> Nv = it->getNeighbors();
            set <Vertex*> interP, interX;
            
            for(auto it1: nP){
                    if(Nv.find(it1) != Nv.end()){
                        interP.insert(it1);
                    }
                }
                for(auto it1: X){
                    if(Nv.find(it1) != Nv.end()){
                        interX.insert(it1);
                    }
                }

            BronKerbosch(nR, interP, interX);
            nP.erase(it);
            X.insert(it);
        }
     }
    
private:
    set <Vertex*> P;
    
    vector <Vertex*> vertices;
    
    vector <string> keyWords;
    vector <string> files;
    vector <string> articles;
    map <int, map <long, char> > forString;
    vector <string> results;
};

int main(int argc, const char * argv[]) {
    Graph *G = new Graph();
    G->loadKeyWords(argv[2]);
    G->loadFiles(argv[1]);
    G->loadArticles();
    G->mapKWP();
    G->createVertices();
    //G->testing();
    //G->tester();
    G->addNeighbors();
    G->writeResults(argv[3]);
    //G->pkw();
    
    //cout << "done" << endl;
    return 0;
}
