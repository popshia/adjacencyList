// 10627130 林冠良 & 10627131 李峻瑋 // Code Blocks 17.12
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <time.h>
#include <math.h>

using namespace std;

typedef struct Successor {
    char getID[10];
    float weight;
} Successor;

typedef struct DataStruct {
    char putID[10];
    vector<Successor> successor;
    int influence = 0;
    bool visited = false;
} DataStruct;

typedef struct Temp {
    char putID[10];
    char getID[10];
    float weight;
} Temp;

static string FileNumber = "0";
static int Count = 0;
static int Command = 0;
static fstream input;
static fstream output;

class function1 {
    DataStruct temp;
public:
    vector<DataStruct> dataBase;
    
    static bool ComparePresuccessor( const DataStruct presuccessor1, const DataStruct presuccessor2 ) {
        return presuccessor1.putID < presuccessor2.putID;
    } // ComparePresuccessor()
    
    static bool CompareWeight( const Successor weight1, const Successor weight2 ) {
        return weight1.weight > weight2.weight;
    } // CompareWeight()
    
    void CreateAdjacencyList() {
        dataBase.clear();
        Temp tempData;
        DataStruct data;
        int studentNo = 0;
        
        input.seekg( 0, input.end );
        studentNo = input.tellg() / sizeof( tempData );
        input.seekg( 0, input.beg );
        
        for ( int i = 0 ; i < studentNo ; i++ ) {
            input.read( (char*)& tempData, sizeof( tempData ) );
            InsertPresuccessor( tempData.putID );
            InsertPresuccessor( tempData.getID );
            InsertSuccessor( tempData.putID, tempData.getID, tempData.weight );
        } // insert data
        
        for( int i = 0 ; i < dataBase.size() ; i++ ) {
            for( int j = i ; j < dataBase.size() ; j++ ) {
                if( strcmp( dataBase[i].putID, dataBase[j].putID ) > 0 ) swap( dataBase[i], dataBase[j] );
            } // for
        } // sort AdjacencyList
        
        for ( int i = 0 ; i < dataBase.size() ; i++ ) sort( dataBase[i].successor.begin(), dataBase[i].successor.end(), CompareWeight );
    } // CreateAdjacencyList()
    
    void InsertPresuccessor( char ID[10] ) {
        DataStruct data;
        
        if ( dataBase.empty() ) {
            strcpy( data.putID, ID );
            dataBase.push_back( data );
        } // first putID
        
        else {
            for ( int i = 0 ; i < dataBase.size() ; i++ ) {
                //cout << dataBase.size() << endl;
                //cout << dataBase[i].putID << "  " << ID << endl ;
                if ( strcmp( dataBase[i].putID ,ID ) == 0 ) return;
            } // search the dataBase
            
            strcpy( data.putID, ID );
            dataBase.push_back( data );
        } // second and after putIDs
    } // InsertPresuccessor()
    
    void InsertSuccessor( char putID[10], char getID[10], float weight ) {
        Successor temp;
        
        for ( int i = 0 ; i < dataBase.size() ; i++ ) {
            if ( strcmp( dataBase[i].putID ,putID ) == 0 ) {
                strcpy( temp.getID, getID);
                temp.weight = weight;
                dataBase[i].successor.push_back( temp );
                return;
            } // insert into the same presuccessor
        } // search the dataBase
    } // InsertSuccessor()
    
    void PrintAdjacencyList() {
        string outputFileName = "pairs" + FileNumber + ".adj";
        output.open( outputFileName.c_str(), fstream::out );
        for ( int i = 0 ; i < dataBase.size() ; i++ ) {
            output << "presuccessor" << i+1 << ":  " << dataBase[i].putID << "\tsuccessor:  " ;
            cout << dataBase[i].putID << "\t" ;
            for ( int j = 0 ; j < dataBase[i].successor.size() ; j++ ) {
                output << dataBase[i].successor[j].getID << "\t" ;
                cout<< dataBase[i].successor[j].weight << "\t";
            } // print out succesor
            cout<< endl;
            output << endl ;
        } // print out dataBase
    } // PrintAdjacencyList()
};

class function2 : public function1 {
public:
    static bool CompareInfluence( const DataStruct presuccessor1, const DataStruct presuccessor2 ) {
        return presuccessor1.influence > presuccessor2.influence;
    } // ComparePresuccessor()
    
    void CalculateInfluence( vector<DataStruct> dataBase ) {
        vector<string> queue ;
        
        for( int i = 0 ; i < dataBase.size() ; i++ ) {
            dataBase[i].visited = true;
            
            for( int j = 0 ; j < dataBase[i].successor.size() ; j++ ) queue.push_back( dataBase[i].successor[j].getID ); // push back the successor of presuccessor in queue
            
            while( !queue.empty() ) {
                for( int k = 0 ; k < dataBase.size() ; k++ ) {
                    if( dataBase[k].putID == queue[0] && dataBase[k].visited == false  ) {
                        dataBase[k].visited = true;
                        dataBase[i].influence++;
                        for( int n = 0 ; n < dataBase[k].successor.size() ; n++ ) queue.push_back( dataBase[k].successor[n].getID );
                    } // count influence and push back queue
                } // find successor of the successor
                
                queue.erase( queue.begin() );
            } // BFS methods
            
            for( int m = 0 ; m < dataBase.size() ; m++ ) dataBase[m].visited = false;
        } // all presuccessor management
        
        sort( dataBase.begin(), dataBase.end(), CompareInfluence );
        PrintInfluenceList( dataBase );
    } // Calculate
    
    void PrintInfluenceList( vector<DataStruct> dataBase ) {
        string outputFileName = "pairs" + FileNumber + ".cnt";
        output.open( outputFileName.c_str(), fstream::out );
        for ( int i = 0 ; i < dataBase.size() ; i++ ) {
            output << "presuccessor" << i+1 << ":  " << dataBase[i].putID << "\tsuccessor:  " ;
            for ( int j = 0 ; j < dataBase[i].successor.size() ; j++ ) {
                output << dataBase[i].successor[j].getID << "\t" ;
            } // print out succesor
            output << endl;
            cout<< dataBase[i].influence << endl;
        } // print out dataBase
    } // PrintInfluenceList
};


int main() {
    bool continueOrNot = false;
    function1 One;
    function2 Two;
    
    do {
        cout << "**********************************************" << endl; // welcome message
        cout << "*****               DS2ex03              *****" << endl;
        cout << "***** 0 : Quit                           *****" << endl;
        cout << "***** 1 : Create Adjacency Lists         *****" << endl;
        cout << "***** 2 : Calculate Influence            *****" << endl;
        cout << "**********************************************" << endl;
        cout << endl << "Please enter your choice:" << endl;
        
        cin >> Command; // read in user command
        cout << endl;
        
        if ( Command == 0 ) { // bye :(((
            cout << "Bye :(((" << endl;
            return 0;
        } // quit
        
        else if ( Command > 2 || Command < 0 ) {
            cout << "Error command! please enter an acceptable command:" << endl << endl;
            continueOrNot = true;
        } // wrong command
        
        else if ( Command == 1 ) {
            bool function1Confirm = false;
            do {
                cout << "Please enter the file number you want to create Adjacency Lists or [0] to quit:" << endl;
                cin >> FileNumber;
                
                if ( FileNumber == "0" ) {
                    function1Confirm = true;
                    continueOrNot = true;
                    cout << endl;
                } // quit
                
                else {
                    string binFileName = "pairs" + FileNumber + ".bin";
                    input.open( binFileName.c_str(), fstream::in | fstream::binary );
                    
                    if ( input.is_open() ) {
                        One.CreateAdjacencyList();
                        One.PrintAdjacencyList();
                        function1Confirm = true;
                        continueOrNot = true;
                    } // binary file open successfully
                    
                    else {
                        cout << endl << "*****        file does not exist !       *****" << endl << endl;
                        function1Confirm = false;
                    } // no such file
                } // continue
            } while ( !function1Confirm );
            input.close();
            output.close();
            Count = 0;
        } // mission 1
        
        else if ( Command == 2 ) {
            bool function2Confirm = false;
            do {
                if ( !One.dataBase.empty() ) {
                    Two.CalculateInfluence( One.dataBase );
                    function2Confirm = true;
                    continueOrNot = true;
                } // open successfully
                
                else {
                    cout << "***** Please proceed function 1 first !  *****" << endl;
                    function2Confirm = true;
                    continueOrNot = true;
                } // function1 first!
            } while ( !function2Confirm );
            
            Count = 0;
            FileNumber = "0";
        } // mission 2
        
        cout << endl;
    } while ( continueOrNot );
} // main
