#include "SQLWhere.hpp"
#include "MyException.hpp"
#include <algorithm>
#include <sstream>


SQLWhere::SQLWhere(){
    rootClause_ = nullptr;
    
}
void SQLWhere::PraseSQLVector(vector<string> tokens){
    auto it = tokens.begin();
    if(*it=="where"){
        ++it;
        rootClause_ = PraseWhereClause(it,tokens.end()); 
    }
    //随后通过rootClause_来读取涉及到的列名
    vector<WhereTerm> terms = rootClause_->getTerms();
    for(auto &term:terms){
        col_name_set_.insert(term.getColName());
    }
}
string SQLWhere::GetBestIndex(vector<string> &col_name_list,const string primary){
    if(rootClause_==nullptr){
        //根本就没有where从句
        return "*";
    }
    //读取WhereClause,返回最佳索引
    //如果主键在条件中,则直接返回主键
    if(col_name_set_.find(primary)!=col_name_set_.end()){
        return primary;
    }
    //如果主键不在条件中,则返回第一个在条件中的列
    for(auto &col_name:col_name_list){
        if(col_name_set_.find(col_name)!=col_name_set_.end()){
            return col_name;
        }
    }
    //如果没有在条件中,则返回set中的第一个
    if(col_name_set_.size()>0){
        return *col_name_set_.begin();
    }
    //如果set为空,该错误处理了
    throw SQLWhereError("SQLWhere::GetBestIndex:col_name_set_ is empty");
    return "";
}
QueryType SQLWhere::GetQueryType(string index_name){
    //根据索引列名返回查询类型
    //查找下在clause中对应的WhereTerm
    vector<WhereTerm> terms = rootClause_->getTerms();
    for(auto &term:terms){
        if(term.getColName()==index_name){
            return (term.getOperator() == TermOperator::EQUAL)?QueryType::QUERY_EQ:QueryType::NOP;
        }
    }
    return QueryType::NOP;
}
Key SQLWhere::GetQueryKey(string index_name,uint8 data_type){
    //根据索引列名返回查询值,此处判断为等值查询,直接去找对应的值
    vector<WhereTerm> terms = rootClause_->getTerms();
    for(auto &term:terms){
        if(term.getColName()==index_name){
            Key value(term.getValue(),data_type);
            return value;
        }
    }
    return Key(false);
}
Key SQLWhere::GetQueryLeftKey(string index_name,uint8 data_type){
    //如果为空,则返回最小值
    if(rootClause_==nullptr){
        return Key(false);
    }
    //根据索引列名返回查询值,此处判断为范围查询,去找对应的值
    vector<WhereTerm> terms = rootClause_->getTerms();
    //此处查询为左值,即查询范围的左边界
    //查询范围左边界有两种情况,一种是单一运算符,一种是由AND或者OR连接的两个运算符
    //先提取出相关的WhereTerm
    vector<WhereTerm> related_terms;
    for(auto &term:terms){
        if(term.getColName()==index_name){
            related_terms.push_back(term);
        }
    }
    //如果只有一个WhereTerm,则直接返回
    if(related_terms.size()==1){
        //看看这个WhereTerm是不是等值查询,如果是,则返回
        if(related_terms[0].getOperator()==TermOperator::EQUAL){
            return Key(related_terms[0].getValue(),data_type);
        }
        //如果不是,则进一步处理,如果为小于等于,则返回最小值,如果为大于等于,则返回当前值
        //即若 id < 5,左边界为最小值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL || related_terms[0].getOperator()==TermOperator::LESS){
            return Key(false);
        }
        //若 id > 5,左边界为当前值+1
        if(related_terms[0].getOperator()==TermOperator::GREATER){
            int64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return Key(value+1);
        }
        //若 id >= 5,左边界为当前值
        int64 value = 0;
        stringstream ss;
        ss<<related_terms[0].getValue();
        ss>>value;
        return Key(value);
    }
    //如果有两个WhereTerm,则需要进一步处理
    //两个WhereTerm的下标分别是0和1
        //先从第一个WhereTerm中提取出值
        int64 value = 0;
        //理论上不可能出现等值查询
        //如果为小于等于,则返回最小值,如果为大于等于,则返回当前值
        //即若 id < 5,左边界为最小值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL || related_terms[0].getOperator()==TermOperator::LESS){
            value = INT64_MIN;
        }
        //若 id > 5,左边界为当前值+1
        if(related_terms[0].getOperator()==TermOperator::GREATER){
            value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            value += 1;
        }
        if(related_terms[0].getOperator()==TermOperator::GREATER_EQUAL){
            value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
        }
        //再从第二个WhereTerm中提取出值,最后的值为两个值中最大的
        int64 value2 = 0;
        
        if(related_terms[1].getOperator()==TermOperator::LESS_EQUAL || related_terms[1].getOperator()==TermOperator::LESS){
            value2 = INT64_MIN;
        }
        //若 id > 5,左边界为当前值+1
        if(related_terms[1].getOperator()==TermOperator::GREATER){
            value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
            value2 += 1;
        }
        if(related_terms[1].getOperator()==TermOperator::GREATER_EQUAL){
            value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
        }
    Key key1(value);
    Key key2(value2);
    //如果连接符为AND
    if(rootClause_->getOperator()==ClauseOperator::AND){
        //AND条件,则返回满足两个条件中最大的值
        return (key1>key2)?key1:key2;

    }
    //如果连接符为OR,则返回满足两个条件中最小的值
    return (key1<key2)?key1:key2;
}
Key SQLWhere::GetQueryRightKey(string index_name,uint8 data_type){
    //如果为空,则返回最大值
    if(rootClause_==nullptr){
        return Key(true);
    }
    //根据索引列名返回查询值,此处判断为范围查询,去找对应的值
    vector<WhereTerm> terms = rootClause_->getTerms();
    //此处查询为右值,即查询范围的右边界
    //查询范围右边界有两种情况,一种是单一运算符,一种是由AND或者OR连接的两个运算符
    //先提取出相关的WhereTerm
    vector<WhereTerm> related_terms;
    for(auto &term:terms){
        if(term.getColName()==index_name){
            related_terms.push_back(term);
        }
    }
    //如果只有一个WhereTerm,则直接返回
    if(related_terms.size()==1){
        //看看这个WhereTerm是不是等值查询,如果是,则返回
        if(related_terms[0].getOperator()==TermOperator::EQUAL){
            return Key(related_terms[0].getValue(),data_type);
        }
        //如果不是,则进一步处理,如果为小于等于,则返回当前值,如果为大于等于,则返回最大值
        //即若 id < 5,右边界为当前值-1
        if(related_terms[0].getOperator()==TermOperator::LESS){
            int64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return Key(value-1);
        }
        //若 id <= 5,右边界为当前值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL){
            int64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return Key(value);
        }
        //若 id > 5或id >= 5,右边界为最大值
        if(related_terms[0].getOperator()==TermOperator::GREATER_EQUAL || related_terms[0].getOperator()==TermOperator::GREATER){
            return Key(true);
        }
    }
    //如果有两个WhereTerm,则需要进一步处理
    //返回满足两个条件中最小的值
    //两个WhereTerm的下标分别是0和1
        //先从第一个WhereTerm中提取出值
        int64 value = 0;
        //理论上不可能出现等值查询
        //如果为小于等于,则返回最大值,如果为大于等于,则返回当前值
        //即若 id < 5,右边界为最大值
        if(related_terms[0].getOperator()==TermOperator::GREATER_EQUAL || related_terms[0].getOperator()==TermOperator::GREATER){
            value = INT64_MAX;
        }
        //若 id > 5,右边界为当前值-1
        if(related_terms[0].getOperator()==TermOperator::LESS){
            value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            value -= 1;
        }
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL){
            value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
        }
        //再从第二个WhereTerm中提取出值,最后的值为两个值中最小的
        int64 value2 = 0;
        //理论上不可能出现等值查询
        //如果为小于等于,则返回当前值,如果为大于等于,则返回最大值
        //即若 id < 5,右边界为最大值
        if(related_terms[1].getOperator()==TermOperator::GREATER_EQUAL || related_terms[1].getOperator()==TermOperator::GREATER){
            value2 = INT64_MAX;
        }
        //若 id > 5,右边界为当前值-1
        if(related_terms[1].getOperator()==TermOperator::LESS){
            value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
            value2 -= 1;
        }
        if(related_terms[1].getOperator()==TermOperator::LESS_EQUAL){
            value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
        }   
    
    Key key1(value);
    Key key2(value2);
    if(rootClause_->getOperator()==ClauseOperator::AND){
        //如果连接符为AND
        return (key1<key2)?key1:key2;
    }
    //如果连接符为OR
    return (key1<key2)?key2:key1;
}
shared_ptr<WhereClause> SQLWhere::PraseWhereClause(vector<string>::iterator it,vector<string>::iterator end){
    //解析一个Where子句
    //子句被简化,仅会有两个子句和一个算式,不会有括号,算式为AND或OR
    //TODO 此处随后可以加解析括号和NOT
    //解析算式
    bool is_attr = true;
    vector<WhereTerm> terms_;
    ClauseOperator op = ClauseOperator::NOP;
    while (is_attr)
    {
        is_attr = false;
        //解析WhereTerm
        WhereTerm term = PraseWhereTerm(it);
        it += 3;
        //解析运算符
        if(it == end){
            terms_.push_back(term);
            break;
        }
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        if(*it == "and"&&(op==ClauseOperator::NOP||op==ClauseOperator::AND)){
            op = ClauseOperator::AND;
            ++it;
            is_attr = true;
            terms_.push_back(term);
        }else if(*it == "or"&&(op==ClauseOperator::NOP||op==ClauseOperator::OR)){
            op = ClauseOperator::OR;
            ++it;
            is_attr = true;
            terms_.push_back(term);
        }else if(*it == "and" || *it == "or"){
            //抛出异常,目前不支持切换
            throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,SWITCH CONDITION OPERATOR NOT SUPPORT:"+*it);
        }else if(it != end){
            //应该到结尾了,没结尾就抛出异常
            throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,OPERATOR NOT SUPPORT:"+*it);
        }
    }
    shared_ptr<WhereClause> clause = make_shared<WhereClause>(terms_,op);
    return clause;
}
WhereTerm SQLWhere::PraseWhereTerm(vector<string>::iterator it){
    //解析一个Where算式
    //Where算式由一个列名、一个运算符和一个值组成
    //列名
    string col_name = *it;
    ++it;
    //运算符
    string op = *it;
    ++it;
    //有可能遇到括号,此时为一个新的clause,但此处不考虑这么复杂的条件了,后期加上
    //值
    string value = *it;
    ++it;
    return WhereTerm(col_name,op,value);
}
int SQLWhere::getTermNum() const{
    return rootClause_->getTermNum();
}
bool SQLWhere::Filter(ColValue val) const{
    //看看val能不能满足逻辑
    if(rootClause_ == nullptr){
        return true;
    }
    return rootClause_->Filter(val);
}



WhereClause::WhereClause(){
    //outerClause_ = nullptr;
    op_ = ClauseOperator::NOP;
    negated_ = false;
}
/*
void WhereClause::setOuterClause(shared_ptr<WhereClause> outerClause){
    outerClause_ = outerClause;
}
*/
void WhereClause::setOperator(const string& op){
    string tmp = op;
    transform(tmp.begin(), tmp.end(), tmp.begin(), (int (*)(int))toupper);
    if(op=="AND"){
        op_ = ClauseOperator::AND;
    }else if(op=="OR"){
        op_ = ClauseOperator::OR;
    }else{
        throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,OPERATOR NOT SUPPORT:"+op);
    }
}
void WhereClause::addTerm(const WhereTerm& term){
    terms_.push_back(term);
}
void WhereClause::setNegated(bool is_negated){
    negated_ = is_negated;
}
bool WhereClause::Filter(ColValue val) const{
    bool result = true;
    for(auto term:terms_){
        //找到对应的列
        if(term.getColName() == val.getColName()){
            //是同一列,看看能不能满足条件
            bool now_result = term.Filter(val);
            if(op_!=ClauseOperator::OR){
                result = result && now_result;
            }else{
                result = result || now_result;
            }
        }
    }
    return result;
}

WhereTerm::WhereTerm(const string& col_name,const string& eOperator,const string& value)
:col_name_(col_name),eValue_(value)
{
    if(eOperator=="="){
        eOperator_ = TermOperator::EQUAL;
    }else if(eOperator=="<>"){
        eOperator_ = TermOperator::NOT_EQUAL;
    }else if(eOperator==">"){
        eOperator_ = TermOperator::GREATER;
    }else if(eOperator=="<"){
        eOperator_ = TermOperator::LESS;
    }else if(eOperator==">="){
        eOperator_ = TermOperator::GREATER_EQUAL;
    }else if(eOperator=="<="){
        eOperator_ = TermOperator::LESS_EQUAL;
    }else{
        throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,OPERATOR NOT SUPPORT:"+eOperator+",col_name:"+col_name+",value:"+value);
    }
}
string WhereTerm::getColName() const{
    return col_name_;
}
int WhereClause::getTermNum() const{
    return terms_.size();
}
TermOperator WhereTerm::getOperator() const{
    return eOperator_;
}
string WhereTerm::getValue() const{
    return eValue_;
}
bool WhereTerm::isNegated() const{
    return negated_;
}
void WhereTerm::setNegated(bool is_negated){
    negated_ = is_negated;
}
bool WhereTerm::Filter(ColValue val) const{
    ColValue tmp(val.getColName(),eValue_,val.getDataType());
    switch (eOperator_)
    {
    case TermOperator::EQUAL:
        return tmp == val;
    case TermOperator::NOT_EQUAL:
        return tmp != val;
    case TermOperator::GREATER:
        return tmp > val;
    case TermOperator::LESS:
        return tmp < val;
    case TermOperator::GREATER_EQUAL:
        return tmp >= val;
    case TermOperator::LESS_EQUAL: 
        return tmp <= val;
    default:
        break;
    }
    return true;
}
WhereClause::WhereClause(vector<WhereTerm> term,ClauseOperator op)
:terms_(term),op_(op)
{
    negated_ = false;
}
/*shared_ptr<WhereClause> WhereClause::getOuterClause() const{
    return outerClause_;
}*/

ClauseOperator WhereClause::getOperator() const{
    return op_;
}
vector<WhereTerm> WhereClause::getTerms() const{
    return terms_;
}
ClauseOperator SQLWhere::getOperator() const{
    if(rootClause_==nullptr){
        return ClauseOperator::NOP;
    }
    return rootClause_->getOperator();
}
