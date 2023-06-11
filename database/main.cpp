#include "Interpreter.hpp"
#include "MyException.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
using namespace std;
void sayFailure(const char* msg){
    //返回的JSON示例如下
    /**
     * {
     *  "status_code":1,
     *  "status_msg":"wrong msg",
     *  "data":[]
     * }
     * 
     */
    cout<<"{";
    cout<<"\"status_code\":1,";
    cout<<"\"status_msg\":\""<<msg<<"\",";
    cout<<"\"data\":\"[]\"";
    cout<<"}"<<"\n";
}
int main(int argc,char* argv[]){
    //不断从标准输入读取SQL语句
    string buf;
    stringstream statement;
    /*if(argc != 2){
        //cerr<<"Usage: "<<argv[0]<<" <database path>\n";
        return 0;
    }*/
    Interpreter interpreter("./");
    if(argc == 2){
        //跟着的是数据库名称
        try{
            stringstream sqlUSE;
            sqlUSE << "USE " << argv[1] <<";"<< endl;
            interpreter.ExecuteSQL(sqlUSE.str());
        }catch(const SQLSyntaxError& e){
            //cerr<<e.what()<<"\n";
            //cerr<<"Statement Syntax Error,You can check the manual for its Syntax\n";
            sayFailure(e.what());
        }
    }
    //读入SQL语句,读到分号执行一句,否则继续读,同时利用try catch捕获异常
    while(getline(cin,buf)){
        statement<<buf;
        if(buf.find(';') != string::npos){
            try{
                //测量执行时间
                if(statement.str().find("quit") != string::npos || statement.str().find("QUIT") != string::npos || statement.str().find("Quit") != string::npos){
                    interpreter.ExecuteSQL(statement.str());
                    break;
                }
                auto start = chrono::high_resolution_clock::now();
                interpreter.ExecuteSQL(statement.str());
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                cerr<<"SQL Execute Time: "<<duration.count()/1000.0<<"ms"<<"\n";
                
            }catch(const SQLSyntaxError& e){
                sayFailure(e.what());
            }catch(const SQLTypeError& e){
                sayFailure(e.what());
            }catch(const DBManagerError& e){
                sayFailure(e.what());
            }catch(const TableOpenError& e){
                sayFailure(e.what());
            }catch(const TableDataError& e){
                sayFailure(e.what());
            }catch(const TableIndexError& e){
                sayFailure(e.what());
            }catch(const TableIndexExist& e){
                sayFailure(e.what());
            }catch(const TableCreateError& e){
                sayFailure(e.what());
            }catch(const TableDeleteError& e){
                sayFailure(e.what());
            }catch(const ColValueError& e){
                sayFailure(e.what());
            }catch(const BPTreeException & e){
                //cerr<<e.what()<<"\n";
                //cerr<<"BPTree Error"<<"\n";
                sayFailure(e.what());
            }catch(const BPNodeException & e){
                //cerr<<e.what()<<endl;
                //cerr<<"BPNode Error"<<"\n";
                sayFailure(e.what());
            }
            //将分号以及之前的语句清空
            statement.str("");
            statement.clear();
            //将buf第一个分号后的语句重新写入statement,不是分号之前的
            statement<<buf.substr(buf.find(';')+1);
            //cout<<statement.str()<<"\n";
        }
    }
    return 0;
}