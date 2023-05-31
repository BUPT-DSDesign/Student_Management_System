#include <iostream>
#include <sstream>
#include <algorithm>

#include "Interpreter.hpp"
#include "MyException.hpp"

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
    cout<<"\"data\":[]";
    cout<<"}"<<endl;
}
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
                if(statement.str().find("quit") != string::npos || statement.str().find("QUIT") != string::npos || statement.str().find("Quit") != string::npos){
                    interpreter.ExecuteSQL(statement.str());
                    break;
                }
                interpreter.ExecuteSQL(statement.str());
            }catch(const SQLSyntaxError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Statement Syntax Error,You can check the manual for its Syntax\n";
                sayFailure(e.what());
            }catch(const SQLTypeError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Statement Type Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const DBManagerError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Database Manager Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const TableOpenError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Table Open Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const TableDataError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Table Data Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const TableIndexError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Table Index Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const TableIndexExist& e){
                cerr<<e.what()<<"\n";
                cerr<<"Table Index Exist Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const TableCreateError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Table Create Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const TableDeleteError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Table Delete Error,You can check the manual for Supported SQL Type\n";
                sayFailure(e.what());
            }catch(const ColValueError& e){
                cerr<<e.what()<<"\n";
                cerr<<"Col Value Error,You can check the manual for Supported SQL Type\n";
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