#include"Date.hpp"
#include<iostream>
#include<string>
using namespace std;
Date::Date(){
    m_year=0;
    m_month=0;
    m_day=0;
    m_hour=0;
    m_minute=0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    m_year=t_year;
    m_month=t_month;
    m_day=t_day;
    m_hour=t_hour;
    m_minute=t_minute;
}


int getInt(int a, string::const_iterator iter){
    int ret=0;
    for (int i=0; i<a; i++){
        ret*=10;
        ret+=*iter-'0';
        iter++;
    }
    return ret;
}

Date::Date(const string &dateString){
    string::const_iterator iter=dateString.begin();
    m_year=getInt(4, iter);
    iter+=5;
    
    m_month=getInt(2, iter);
    iter+=3;

    m_day=getInt(2, iter);
    iter+=3;

    m_hour=getInt(2, iter);
    iter+=3;

    m_minute=getInt(2, iter);
}

int Date::getYear() const{
    return m_year;
}

void Date::setYear(const int t_year){
    m_year=t_year;
}

int Date::getMonth() const{
    return m_month;
}

void Date::setMonth(const int t_month){
    m_month=t_month;
}

int Date::getDay() const{
    return m_day;
}

void Date::setDay(const int t_day){
    m_day=t_day;
}

int Date::getHour() const{
    return m_hour;
}

void Date::setHour(const int t_hour){
    m_hour=t_hour;
}

int Date::getMinute() const{
    return m_minute;
}

void Date::setMinute(const int t_minute){
    m_minute=t_minute;
}

int monthDay[13]={0, 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isLeap(int year){
    if(year%4!=0)
        return false;
    if(year%100==0&&year%400!=0)
        return false;
    return true;
}

bool Date::isValid(const Date &other){
    if(other.m_year<1000||other.m_year>9999)
        return false;
    if(other.m_minute>=60){
        return false;
    }else if(other.m_hour>=24){
        return false;
    }else if(other.m_month>12){
        return false;
    }

    if(other.m_month==2){
        if(isLeap(other.m_year)){
            if(other.m_day>29)
                return false;
        }else{
            if(other.m_day>28)
                return false;
        }
    }else{
        if(other.m_day>monthDay[other.m_month])
            return false;
    }

    return true;
}

bool checkString(const string &s){
    if(s.length()!=16)
        return false;
    string::const_iterator iter=s.begin();
    // year
    for(int i=0; i<4; i++){
        if(*iter>'9'||*iter<'0')
            return false;
        iter++;
    }
    if(*iter!='-')
        return false;
    iter++;
    // month
    for (int i=0; i<2; i++){
        if(*iter>'9'||*iter<'0')
            return false;
        iter++;
    }
    if(*iter!='-')
        return false;
    iter++;
    // day
    for (int i=0; i<2; i++){
        if(*iter>'9'||*iter<'0')
            return false;
        iter++;
    }
    if(*iter!='/')
        return false;
    iter++;
    // hour
    for (int i=0; i<2; i++){
        if(*iter>'9'||*iter<'0')
            return false;
        iter++;
    }
    if(*iter!=':')
        return false;
    iter++;
    // minute
    for (int i=0; i<2; i++){
        if(*iter>'9'||*iter<'0')
            return false;
        iter++;
    }

    return true;
}

Date Date::stringToDate(const string &t_string){
    if(checkString(t_string)){
        Date ret(t_string);
        if(!isValid(ret)){
            ret.setYear(0);
            ret.setMonth(0);
            ret.setDay(0);
            ret.setHour(0);
            ret.setMinute(0);
        }
        return ret;
    }else{
        Date ret(0, 0, 0, 0, 0);
        return ret;
    }
}



string my_intToS(int n, int size){
    string str="\0";
    char temp;
    for (int i=0; i<size; i++){
        temp=n%10+'0';
        n/=10;
        str=temp+str;
    }
    return str;
}

string Date::dateToString(const Date &t_date){
    if(!Date::isValid(t_date))
        return "0000-00-00/00:00";
    string str=my_intToS(t_date.m_year, 4)+'-'+my_intToS(t_date.m_month, 2)+'-'
        +my_intToS(t_date.m_day, 2)+'/'+my_intToS(t_date.m_hour, 2)+':'+my_intToS(t_date.m_minute, 2);
    return str;
}

Date& Date::operator=(const Date &t_date){
    this->m_year=t_date.m_year;
    this->m_month=t_date.m_month;
    this->m_day=t_date.m_day;
    this->m_hour=t_date.m_hour;
    this->m_minute=t_date.m_minute;
    return *this;
}

bool Date::operator==(const Date& t_date) const{
    if(t_date.m_year!=m_year)
        return false;
    if(t_date.m_month!=m_month)
        return false;
    if(t_date.m_day!=m_day)
        return false;
    if(t_date.m_hour!=m_hour)
        return false;
    if(t_date.m_minute!=m_minute)
        return false;

    return true;
}

bool Date::operator<(const Date& t_date) const{
    if(m_year<t_date.m_year)
        return true;
    if(m_year>t_date.m_year)
        return false;

    if(m_month<t_date.m_month)
        return true;
    if(m_month>t_date.m_month)
        return false;
    
    if(m_day<t_date.m_day)
        return true;
    if(m_day>t_date.m_day)
        return false;

    if(m_hour<t_date.m_hour)
        return true;
    if(m_hour>t_date.m_hour)
        return false;

    if(m_minute<t_date.m_minute)
        return true;
    if(m_minute>t_date.m_minute)
        return false;
    
    return false;
}

bool Date::operator>(const Date& t_date) const{
    if(*this==t_date)
        return false;
    if(*this<t_date)
        return false;
    return true;
}

bool Date::operator<=(const Date& t_date) const{
    if(*this>t_date)
        return false;
    return true;
}

bool Date::operator>=(const Date& t_date) const{
    if(*this<t_date)
        return false;
    return true;
}