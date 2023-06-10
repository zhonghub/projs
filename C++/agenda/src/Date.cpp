#include <initializer_list>
#include <string>
#include<iostream>
#include"Date.hpp"
#include<math.h>
#include<vector>
using namespace std;


Date::Date(){
	this->m_year=0;
	this->m_month=0;
	this->m_day=0;
	this->m_hour=0;
	this->m_minute=0;
}


Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
      this->m_year = t_year;
      this->m_month =  t_month;
      this->m_day = t_day;
      this->m_hour = t_hour;
      this->m_minute = t_minute;
}

Date::Date(const std::string &dateString){
	bool ok= true ;
	if(dateString.size()!=16)  ok = false ;
    if(dateString[4]!='-'||dateString[7]!='-'||dateString[10]!='/'||dateString[13]!=':')  ok = false ;
	for(int i=0;i<16;i++){
		if(i==4||i==7||i==10||i==13){
        }
		else{
			if(dateString[i]<'0'||dateString[i]>'9') ok = false ;
		}
	}
    if(ok){
        this->m_year = (dateString[0]-'0')*1000+(dateString[1]-'0')*100+(dateString[2]-'0')*10+(dateString[3]-'0') ;
        this->m_month =(dateString[5]-'0')*10+(dateString[6]-'0');
        this->m_day = (dateString[8]-'0')*10+(dateString[9]-'0');
        this->m_hour = (dateString[11]-'0')*10+(dateString[12]-'0');
        this->m_minute = (dateString[14]-'0')*10+(dateString[15]-'0');;
         }
	else if(ok==false){
	      this->m_year = 0;
	      this->m_month = 0;
	      this->m_day = 0;
	      this->m_hour = 0;
	      this->m_minute = 0;
	}
  }
 
   int Date::getYear(void) const{
      return this->m_year ;
  }

  void Date::setYear(const int t_year){
      this->m_year = t_year ;
  }

  int Date::getMonth(void) const{
      return this->m_month ;
  }

  void Date::setMonth(const int t_month){
      this->m_month = t_month ;
  }

  int Date::getDay(void) const{
      return this->m_day ;
  }

  void Date::setDay(const int t_day){
      this->m_day = t_day ;
  }

  int Date::getHour(void) const{
      return this->m_hour ;
  }

  void Date::setHour(const int t_hour){
      this->m_hour = t_hour ;
  }

  int Date::getMinute(void) const{
      return this->m_minute ;
  }

  void Date::setMinute(const int t_minute){
      this->m_minute = t_minute ;
  }

 bool Date::isValid(const Date &t_date){
		int y= t_date.getYear();
		int i = t_date.getMonth();
		int  day = t_date.getDay();
		int days;
	    if(i==1||i==3||i==5||i==7||i==8||i==10||i==12)  
	        days=31;  
	    else if (i==4||i==6||i==9||i==11)  
	        days=30;  
	    else if ((y%4==0 && y % 100 != 0)|| y % 400 == 0)  
	        days=29;  
	    else  
	        days=28;  
	    if (y<1000|| y>9999||i<1||i>12||day<1||day>days)  
	        return  false ;  
	    if(t_date.getHour()<0|| t_date.getHour()>23||t_date.getMinute()<0||t_date.getMinute()>59)  
	        return false;
	    return true; 
      }

  Date Date::stringToDate(const std::string &t_dateString){
	Date d(t_dateString);
	return d;
  }

  std::string Date::dateToString(const Date &t_date){
        if(t_date.isValid(t_date) ==false){
    	string str1 = "0000-00-00/00:00";
    	return str1;
  	}
        char a[17];
        a[4] = '-' ;
        a[7] = '-' ;
        a[10] = '/' ;
        a[13] = ':' ;
        a[16] = '\0' ;
        a[0] = t_date.getYear()/1000+'0';
        a[1] = (t_date.getYear()/100)%10+'0';
        a[2] = (t_date.getYear()/10)%10+'0';
        a[3] = t_date.getYear()%10+'0';
        a[5] = t_date.getMonth()/10+'0';
        a[6] = t_date.getMonth()%10 +'0';
        a[8] = t_date.getDay()/10+'0';
        a[9] = t_date.getDay()%10 +'0';
        a[11] = t_date.getHour()/10+'0';
        a[12] = t_date.getHour()%10+'0' ;
        a[14] = t_date.getMinute()/10+'0';
        a[15] = t_date.getMinute()%10+'0' ;
        std::string s(a) ;
        return s ;
  }

  Date& Date::operator=(const Date &t_date){
      this->m_year = t_date.getYear() ;
      this->m_month = t_date.getMonth() ;
      this->m_day = t_date.getDay() ;
      this->m_hour = t_date.getHour() ;
      this->m_minute = t_date.getMinute() ;
      return *this ; 
  }

 bool Date::operator==(const Date &t_date) const{
      if(t_date.getYear() != m_year ) return false ;
      if(t_date.getMonth() != m_month) return false ;
      if(t_date.getDay() != m_day) return false ;
      if(t_date.getHour() != m_hour) return false ;
      if(t_date.getMinute() != m_minute) return false ;
      return true ;
  }

  bool Date::operator>(const Date &t_date) const{
      int t[5];
      t[0] = m_year-t_date.getYear();
      t[1] = m_month - t_date.getMonth();
      t[2] = m_day - t_date.getDay();
      t[3] = m_hour-t_date.getHour() ;
      t[4] = m_minute - t_date.getMinute();
      int dt = (((t[0]*12+t[1])*30+t[2])*24+t[3])*60+t[4];
      if(dt>0) return true ;
      else return false ;
  }

  bool Date::operator<(const Date &t_date) const{
     return !(*this>t_date || *this==t_date) ;
  }

  bool Date::operator>=(const Date &t_date) const{
      return (*this>t_date || *this==t_date) ;
  }

  bool Date::operator<=(const Date &t_date) const{
      return !(*this>t_date ) ;
  }