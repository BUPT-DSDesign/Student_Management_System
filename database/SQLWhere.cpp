#include "SQLWhere.hpp"
#include "MyException.hpp"
#include <algorithm>
SQLWhere::SQLWhere(){

}
void SQLWhere::PraseSQLVector(vector<string> &sql_vector){
    //将token解析为逆波兰表达式,并合并子查询,最终运算符只留下逻辑运算符
    //最终结果放在rpn_中
    //将token解析为逆波兰表达式
    //利用priority_来判断运算符的优先级
    stack<string> op_stack_;
    bool is_quote = false;//判断是否在引号中
    vector<SQLWhereOperand> rpn_tmp_;
    for(auto &it:sql_vector){
        string tmp = it;
        SQLWhereOperand operand;
        transform(tmp.begin(), tmp.end(), tmp.begin(), (int (*)(int))toupper);
        //先判断是不是运算符(在priority_中)
        if(priority_.find(tmp) != priority_.end()){
            //是运算符
            if(op_stack_.empty()){
                //栈空,直接入栈
                op_stack_.push(tmp);
            }else{
                //栈不空,判断优先级
                if(priority_[tmp] > priority_[op_stack_.top()]){
                    //优先级高,直接入栈
                    op_stack_.push(tmp);
                }else{
                    //优先级低,将栈中的运算符出栈,直到栈空或者遇到优先级更低的运算符
                    while(!op_stack_.empty() && priority_[tmp] <= priority_[op_stack_.top()]){
                        operand.type_ = OPERATOR;
                        operand.value_ = op_stack_.top();
                        op_stack_.pop();
                        rpn_tmp_.push_back(operand);
                    }
                    op_stack_.push(tmp);
                }
            }
        }else{
            //不是运算符,是立即数或列名
            if(tmp == "("){
                //左括号直接入栈
                op_stack_.push(tmp);
            }else if(tmp == ")"){
                //右括号,将栈中的运算符出栈,直到遇到左括号
                while(op_stack_.top() != "("){
                    operand.type_ = OPERATOR;
                    operand.value_ = op_stack_.top();
                    op_stack_.pop();
                    rpn_tmp_.push_back(operand);
                }
                op_stack_.pop();
            }else{
                //立即数或列名,直接入栈
                //判断是立即数还是列名,立即数以数字或引号开头,列名以字母开头
                if(tmp[0] >= '0' && tmp[0] <= '9' || is_quote){
                    //立即数
                    operand.type_ = IMMEDIATE;
                    operand.value_ = it;
                    rpn_tmp_.push_back(operand);
                }else if(tmp=="\'" || tmp == "\""){
                    if(!is_quote){
                        is_quote = true;
                        continue;//下一个token为值
                    }else{
                        is_quote = false;
                    }
                }else{
                    //列名
                    operand.type_ = COLUMN;
                    operand.value_ = it;
                    rpn_tmp_.push_back(operand);
                }
            }
        }
    }
    //将栈中剩余的运算符出栈
    while(!op_stack_.empty()){
        SQLWhereOperand operand;
        operand.type_ = OPERATOR;
        operand.value_ = op_stack_.top();
        op_stack_.pop();
        rpn_tmp_.push_back(operand);
    }
    if(is_quote){
        //引号匹配错误
        throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,QUOTATION MARK NOT MATCH");
    }
    //将rpn_tmp_中的子查询合并
    //例如将a 1 > 当成一个子查询存在一起,方便B+树进行范围查询
    //当然 a 1 > a 3 < AND 也可以作为一个子查询,只要是个闭区间就可以
    //将子查询的结果放在sub_query_中
    //并将最后简化的结果放在rpn_中
    //rpn_tmp是一个初步的逆波兰表达式
    stack<SQLWhereOperand> sub_query_stack_;
    for(auto &it:rpn_tmp_){
        if(it.type_==OPERATOR){
            if(it.value_=="AND" || it.value_=="OR"){
                //逻辑运算符,直接入栈
                rpn_.push_back(it);
            }else if(it.value_=="NOT"){
                //NOT运算符,将栈顶的子查询取反
                if(sub_query_stack_.empty()){
                    throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,EXCEPT SUB QUERY,BUT GET OTHER:"+it.value_);
                }
                SQLWhereOperand operand = sub_query_stack_.top();
                sub_query_stack_.pop();
                if(operand.type_!=SUB_QUERY){
                    throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,EXCEPT SUB QUERY,BUT GET OTHER:"+it.value_);
                }
                //取反方法:将运算符取反,例如>,<,=,>=,<=,<>分别取反为<=,>=,<>,=,>,<
                //新建一个unordered_map,将原来的运算符作为key,取反后的运算符作为value
                if(reverse_operator_.find(operand.value_) == reverse_operator_.end()){
                    //TODO 是AND NOT,OR NOT,NOT NOT这种情况
                }else{
                    operand.value_ = reverse_operator_[operand.value_];
                }
                sub_query_stack_.push(operand);
            }else{
                //其他运算符,将栈顶的两个元素合并
                if(sub_query_stack_.size()<2){
                    throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,EXCEPT SUB QUERY,BUT GET OTHER:"+it.value_);
                }
                SQLWhereOperand operand1 = sub_query_stack_.top();
                sub_query_stack_.pop();
                SQLWhereOperand operand2 = sub_query_stack_.top();
                sub_query_stack_.pop();
                
                //将两个元素合并
                SQLWhereOperand operand;
                operand.type_ = SUB_QUERY;
                operand.value_ = (operand1.type_==COLUMN)?operand1.value_:operand2.value_;//列名
                //根据运算符类型,确定对应范围值
                sub_query_stack_.push(operand);
            }
        }else if(it.type_==IMMEDIATE || it.type_==COLUMN){
            sub_query_stack_.push(it);
        }
    }


}