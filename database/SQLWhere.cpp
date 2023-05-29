#include "SQLWhere.hpp"
#include "MyException.hpp"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <iomanip>
ColValue::ColValue(const string& col_name,const string& value,uint8 data_type)
:col_name_(col_name),data_type_(data_type)
{
    //根据数据类型,将value转换为对应的类型
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = stoi(value);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_double_ = stod(value);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = stoul(value);
        default:
            value_str_ = value;
    }
}
ColValue::ColValue(const string& col_name,double value):col_name_(col_name),value_double_(value),data_type_(T_DOUBLE){}
ColValue::ColValue(const string& col_name,int64 value):col_name_(col_name),value_int_(value),data_type_(T_BIG_INT){}
ColValue::ColValue(const string& col_name,const string& value):col_name_(col_name),value_str_(value),data_type_(T_CHAR){}
ColValue::ColValue(const string& col_name,uint8 data_type,vector<byte>::iterator begin,int len)
:col_name_(col_name),data_type_(data_type)
{
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = *(int64*)&(*begin);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_double_ = *(double*)&(*begin);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = *(uint64*)&(*begin);
        default:
            value_str_ = string(begin,begin+len);
    }
}
vector<byte> ColValue::getBytes() const{
    vector<byte> bytes;
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            bytes.resize(sizeof(int64));
            *(int64*)&bytes[0] = value_int_;
            break;
        case T_FLOAT:case T_DOUBLE:
            bytes.resize(sizeof(double));
            *(double*)&bytes[0] = value_double_;
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            bytes.resize(sizeof(uint64));
            *(uint64*)&bytes[0] = value_uint_;
        default:
            bytes.resize(value_str_.size());
            copy(value_str_.begin(),value_str_.end(),bytes.begin());
    }
    return bytes;
}
string ColValue::getColName() const{
    return col_name_;
}
string ColValue::getValue() const{
    //根据数据类型,将value转换为对应的类型
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return to_string(value_int_);
        case T_FLOAT:case T_DOUBLE:
            return to_string(value_double_);
        case T_DATE:
            //格式为YYYY-MM-DD
            //范围为1000-01-01到9999-12-31
            {
                int64 year = value_int_/10000;
                int64 month = (value_int_%10000)/100;
                int64 day = value_int_%100;
                return to_string(year)+"-"+to_string(month)+"-"+to_string(day);
            }
        case T_TIME:
            //格式为HH:MM:SS
            //范围为-838:59:59到838:59:59
            {
                int64 hour = value_int_/3600;
                int64 minute = (value_int_%3600)/60;
                int64 second = value_int_%60;
                return to_string(hour)+":"+to_string(minute)+":"+to_string(second);
            }
        case T_YEAR:
            //格式为YYYY
            //范围为1901到2155
            return to_string(value_uint_);
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                std::time_t val_time_t = value_uint_;
                std::tm timestamp = *std::localtime(&val_time_t);
                stringstream ss;
                ss << std::put_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                return ss.str();
            }
        default:
            return value_str_;
    }
}
uint8 ColValue::getDataType() const{
    return data_type_;
}
pair<string,string> ColValue::getColValue() const{
    //根据数据类型,将value转换为对应的类型
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return make_pair(col_name_,to_string(value_int_));
        case T_FLOAT:case T_DOUBLE:
            return make_pair(col_name_,to_string(value_double_));
        case T_DATE:
            //格式为YYYY-MM-DD
            //范围为1000-01-01到9999-12-31
            {
                int64 year = value_int_/10000;
                int64 month = (value_int_%10000)/100;
                int64 day = value_int_%100;
                return make_pair(col_name_,to_string(year)+"-"+to_string(month)+"-"+to_string(day));
            }
        case T_TIME:
            //格式为HH:MM:SS
            //范围为-838:59:59到838:59:59
            {
                int64 hour = value_int_/3600;
                int64 minute = (value_int_%3600)/60;
                int64 second = value_int_%60;
                return make_pair(col_name_,to_string(hour)+":"+to_string(minute)+":"+to_string(second));
            }
        case T_YEAR:
            //格式为YYYY
            //范围为1901到2155
            return make_pair(col_name_,to_string(value_uint_));
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                std::time_t val_time_t = value_uint_;
                std::tm timestamp = *std::localtime(&val_time_t);
                stringstream ss;
                ss << std::put_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                return make_pair(col_name_,ss.str());
            }
        default:
            return make_pair(col_name_,value_str_);
    }
}
bool ColValue::isSameType(const ColValue& other) const{
    return data_type_ == other.data_type_;
}
bool ColValue::operator==(const ColValue& other) const{
    if(data_type_ != other.data_type_){
        return false;
    }
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return value_int_ == other.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_double_ == other.value_double_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ == other.value_uint_;
        default:
            return value_str_ == other.value_str_;
    }
}
bool ColValue::operator!=(const ColValue& other) const{
    return !(*this == other);
}
bool ColValue::operator<(const ColValue& other) const{
    if(data_type_ != other.data_type_){
        throw ColValueError("ColValue::operator<:类型不匹配");
    }
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return value_int_ < other.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_double_ < other.value_double_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ < other.value_uint_;
        default:
            return value_str_ < other.value_str_;
    }
}
bool ColValue::operator<=(const ColValue& other) const{
    if(data_type_ != other.data_type_){
        throw ColValueError("ColValue::operator<=:类型不匹配");
    }
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return value_int_ <= other.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_double_ <= other.value_double_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ <= other.value_uint_;
        default:
            return value_str_ <= other.value_str_;
    }
}
bool ColValue::operator>(const ColValue& other) const{
    return !(*this <= other);
}
bool ColValue::operator>=(const ColValue& other) const{
    return !(*this < other);
}
SQLWhere::SQLWhere(){

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
uint64 SQLWhere::GetQueryKey(string index_name){
    //根据索引列名返回查询值,此处判断为等值查询,直接去找对应的值
    vector<WhereTerm> terms = rootClause_->getTerms();
    for(auto &term:terms){
        if(term.getColName()==index_name){
            uint64 value = 0;
            stringstream ss;
            ss<<term.getValue();
            ss>>value;
            return value;
        }
    }
}
uint64 SQLWhere::GetQueryLeftKey(string index_name){
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
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return value;
        }
        //如果不是,则进一步处理,如果为小于等于,则返回最小值,如果为大于等于,则返回当前值
        //即若 id < 5,左边界为最小值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL || related_terms[0].getOperator()==TermOperator::LESS){
            return 0;
        }
        //若 id > 5,左边界为当前值+1
        if(related_terms[0].getOperator()==TermOperator::GREATER){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return value+1;
        }
        //若 id >= 5,左边界为当前值
        uint64 value = 0;
        stringstream ss;
        ss<<related_terms[0].getValue();
        ss>>value;
        return value;
    }
    //如果有两个WhereTerm,则需要进一步处理
    //两个WhereTerm的下标分别是0和1
        //先从第一个WhereTerm中提取出值
        uint64 value = 0;
        //理论上不可能出现等值查询
        //如果为小于等于,则返回最小值,如果为大于等于,则返回当前值
        //即若 id < 5,左边界为最小值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL || related_terms[0].getOperator()==TermOperator::LESS){
            value = 0;
        }
        //若 id > 5,左边界为当前值+1
        if(related_terms[0].getOperator()==TermOperator::GREATER){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            value += 1;
        }
        if(related_terms[0].getOperator()==TermOperator::GREATER_EQUAL){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
        }
        //再从第二个WhereTerm中提取出值,最后的值为两个值中最大的
        uint64 value2 = 0;
        
        if(related_terms[1].getOperator()==TermOperator::LESS_EQUAL || related_terms[1].getOperator()==TermOperator::LESS){
            value2 = 0;
        }
        //若 id > 5,左边界为当前值+1
        if(related_terms[1].getOperator()==TermOperator::GREATER){
            uint64 value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
            value2 += 1;
        }
        if(related_terms[1].getOperator()==TermOperator::GREATER_EQUAL){
            uint64 value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
        }
    //如果连接符为AND
    if(rootClause_->getOperator()==ClauseOperator::AND){
        //AND条件,则返回满足两个条件中最大的值
        return (value>value2)?value:value2;

    }
    //如果连接符为OR,则返回满足两个条件中最小的值
    return (value>value2)?value2:value;
}
uint64 SQLWhere::GetQueryRightKey(string index_name){
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
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return value;
        }
        //如果不是,则进一步处理,如果为小于等于,则返回当前值,如果为大于等于,则返回最大值
        //即若 id < 5,右边界为当前值-1
        if(related_terms[0].getOperator()==TermOperator::LESS){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return value-1;
        }
        //若 id <= 5,右边界为当前值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            return value;
        }
        //若 id > 5或id >= 5,右边界为最大值
        if(related_terms[0].getOperator()==TermOperator::GREATER_EQUAL || related_terms[0].getOperator()==TermOperator::GREATER){
            return UINT64_MAX;
        }
    }
    //如果有两个WhereTerm,则需要进一步处理
    //返回满足两个条件中最小的值
    //两个WhereTerm的下标分别是0和1
        //先从第一个WhereTerm中提取出值
        uint64 value = 0;
        //理论上不可能出现等值查询
        //如果为小于等于,则返回最大值,如果为大于等于,则返回当前值
        //即若 id < 5,右边界为最大值
        if(related_terms[0].getOperator()==TermOperator::LESS_EQUAL || related_terms[0].getOperator()==TermOperator::LESS){
            value = UINT64_MAX;
        }
        //若 id > 5,右边界为当前值-1
        if(related_terms[0].getOperator()==TermOperator::GREATER){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
            value -= 1;
        }
        if(related_terms[0].getOperator()==TermOperator::GREATER_EQUAL){
            uint64 value = 0;
            stringstream ss;
            ss<<related_terms[0].getValue();
            ss>>value;
        }
        //再从第二个WhereTerm中提取出值,最后的值为两个值中最小的
        uint64 value2 = 0;
        //理论上不可能出现等值查询
        //如果为小于等于,则返回当前值,如果为大于等于,则返回最大值
        //即若 id < 5,右边界为最大值
        if(related_terms[1].getOperator()==TermOperator::LESS_EQUAL || related_terms[1].getOperator()==TermOperator::LESS){
            value2 = UINT64_MAX;
        }
        //若 id > 5,右边界为当前值-1
        if(related_terms[1].getOperator()==TermOperator::GREATER){
            uint64 value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
            value2 -= 1;
        }
        if(related_terms[1].getOperator()==TermOperator::GREATER_EQUAL){
            uint64 value2 = 0;
            stringstream ss;
            ss<<related_terms[1].getValue();
            ss>>value2;
        }   
    

    if(rootClause_->getOperator()==ClauseOperator::AND){
        //如果连接符为AND
        return (value>value2)?value2:value;
    }
    //如果连接符为OR
    return (value>value2)?value:value2;
}
shared_ptr<WhereClause> SQLWhere::PraseWhereClause(vector<string>::iterator it,vector<string>::iterator end,shared_ptr<WhereClause> parent=nullptr){
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
        //解析运算符
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
    shared_ptr<WhereClause> clause = make_shared<WhereClause>(terms_,op,parent);
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
        throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,OPERATOR NOT SUPPORT:"+eOperator);
    }
}
WhereClause::WhereClause(){
    outerClause_ = nullptr;
    op_ = ClauseOperator::NOP;
    negated_ = false;
}
void WhereClause::setOuterClause(shared_ptr<WhereClause> outerClause){
    outerClause_ = outerClause;
}
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

WhereClause::WhereClause(vector<WhereTerm> term,const string& op,shared_ptr<WhereClause> outerClause)
:terms_(term),outerClause_(outerClause)
{
    if(op=="AND"){
        op_ = ClauseOperator::AND;
    }else if(op=="OR"){
        op_ = ClauseOperator::OR;
    }else{
        throw SQLSyntaxError("SQL WHERE SYNTAX ERROR,OPERATOR NOT SUPPORT:"+op);
    }
}
shared_ptr<WhereClause> WhereClause::getOuterClause() const{
    return outerClause_;
}

ClauseOperator WhereClause::getOperator() const{
    return op_;
}
vector<WhereTerm> WhereClause::getTerms() const{
    return terms_;
}
ClauseOperator SQLWhere::getOperator() const{
    return rootClause_->getOperator();
}
