#include <iostream>
#include <sstream>
#include <algorithm>

#include "Interpreter.hpp"
#include "MyException.hpp"

using namespace std;

int main(int argc,char* argv[]){
    //不断从标准输入读取SQL语句
    string buf;
    stringstream statement;
    if(argc != 2){
        cerr<<"Usage: "<<argv[0]<<" <database path>\n";
        return 0;
    }
    Interpreter interpreter(argv[1]);
    //读入SQL语句,读到分号执行一句,否则继续读,同时利用try catch捕获异常
    while(getline(cin,buf)){
        statement<<buf;
        if(buf.find(';') != string::npos){
            try{
                if(statement.str().find("quit") != string::npos){
                    interpreter.ExecuteSQL(statement.str());
                    break;
                }
                interpreter.ExecuteSQL(statement.str());
            }catch(const SQLSyntaxError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Statement Syntax Error,You can check the manual for its Syntax\n";
            }catch(const SQLTypeError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Statement Type Error,You can check the manual for Supported SQL Type\n";
            }
            statement.str("");
        }
    }
    return 0;
}