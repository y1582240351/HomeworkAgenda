#include"Storage.hpp"
#include"Path.hpp"
#include<iostream>
#include<fstream>
#include<sstream>

std::shared_ptr<Storage> Storage::m_instance=nullptr;

Storage::Storage(){

}

// Storage::Storage(const Storage& t){

// }

// void Storage::operator=(const Storage & t){

// }

void fixStr(std::string & str){
    if(str.size()==0)
        return;
    std::string::iterator iter;

    iter=str.begin();
    if(*iter=='\"')
        str.erase(iter);

    iter=str.end()-1;
    if(*iter=='\"')
        str.erase(iter);
}

void setUser(User & usr, int i, std::string & str){
    switch(i){
        case 0: usr.setName(str);     break;
        case 1: usr.setPassword(str); break;
        case 2: usr.setEmail(str);    break;
        case 3: usr.setPhone(str);    break;
    }
}

void dealParticator(std::vector<std::string> & vtr, std::string str){
    std::stringstream sin(str);
    std::string temp;
    while(getline(sin, temp, '&')){
        vtr.push_back(temp);
        temp.clear();
    }
}

void setMeeting(Meeting & temp_meet, int i, std::string str){
    switch(i){
        case 0: 
            temp_meet.setSponsor(str);
            break;
        case 1:{
            std::vector<std::string> v;
            dealParticator(v, str);
            temp_meet.setParticipator(v);
            break;
        }
        case 2:{
            Date dS(str);
            temp_meet.setStartDate(dS);
            break;
        }
        case 3:{
            Date dE(str);
            temp_meet.setEndDate(dE);
            break;
        }
        case 4:
            temp_meet.setTitle(str);
            break;
    }
}

bool Storage::readFromFile(){
    std::ifstream fin_user(Path::userPath, std::ios::in);
    std::ifstream fin_meeting(Path::meetingPath, std::ios::in);

    if(!fin_user.is_open())
        return false;
    if(!fin_meeting.is_open())
        return false;

    std::string massage;
    std::string sub_massage;
    std::stringstream sin;
    User temp_user;
    int i;
    while(getline(fin_user, massage)){
        i=0;
        sin.clear();
        sin << massage;
        while(getline(sin, sub_massage, ',')){
            fixStr(sub_massage);
            setUser(temp_user, i++, sub_massage);
            sub_massage.clear();
        }
        m_userList.push_back(temp_user);
        massage.clear();
    }

    Meeting temp_meet;
    while(getline(fin_meeting, massage)){
        i=0;
        sin.clear();
        sin << massage;
        while(getline(sin, sub_massage, ',')){
            fixStr(sub_massage);
            setMeeting(temp_meet, i++, sub_massage);
            sub_massage.clear();
        }
        m_meetingList.push_back(temp_meet);
        massage.clear();
    }

    return true;
}

void transferParticator(const std::vector<std::string> & vtr, std::string & str){
    std::vector<std::string>::const_iterator iter;
    str.clear();
    for(iter=vtr.begin(); iter!=vtr.end(); iter++){
        str+=*iter+"&";
    }
    //删掉多余的&
    str.erase(str.end()-1);
}

bool Storage::writeToFile(void){
    std::ofstream fout;

    fout.open(Path::userPath);
    if(!fout.is_open())
        return false;
    std::list<User>::iterator iterUser;
    for(iterUser=m_userList.begin(); iterUser!=m_userList.end(); iterUser++){
        fout << '\"' << iterUser->getName() << '\"' << ','
             << '\"' << iterUser->getPassword() << '\"' << ','
             << '\"' << iterUser->getEmail() << '\"' << ','
             << '\"' << iterUser->getPhone() << '\"' << '\n';
    }
    fout.close();
    fout.clear();

    fout.open(Path::meetingPath);
    if(!fout.is_open())
        return false;
    std::list<Meeting>::iterator iterMeet;
    std::string particatorStr;
    for(iterMeet=m_meetingList.begin(); iterMeet!=m_meetingList.end(); iterMeet++){
        transferParticator(iterMeet->getParticipator(), particatorStr);
        fout << '\"' << iterMeet->getSponsor() << '\"' << ','
             << '\"' << particatorStr << '\"' << ','
             << '\"' << Date::dateToString(iterMeet->getStartDate()) <<'\"' << ','
             << '\"' << Date::dateToString(iterMeet->getEndDate()) <<'\"' << ','
             << '\"' << iterMeet->getTitle() << '\"' << '\n';
    }
    fout.close();
    fout.clear();

    return true;
}

std::shared_ptr<Storage> Storage::getInstance(void){
    if(Storage::m_instance==nullptr){
        Storage::m_instance.reset(new Storage);
    }
    Storage::m_instance->readFromFile();
    return m_instance;
}

Storage::~Storage(){
    m_instance=NULL;
}

void Storage::createUser(const User & t_user){
    m_userList.push_back(t_user);
    m_dirty=true;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const{
    std::list<User> ret;
    std::list<User>::const_iterator iter;
    for(iter=m_userList.begin(); iter!=m_userList.end(); iter++){
        if(filter(*iter)){
            ret.push_back(*iter);
        }
    }
    return ret;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                std::function<void(User &)> switcher){
    std::list<User>::iterator iter;
    int cnt=0;
    for(iter=m_userList.begin(); iter!=m_userList.end(); iter++){
        if(filter(*iter)){
            switcher(*iter);
            cnt++;
        }
    }
    if(cnt!=0)
        m_dirty=true;
    return cnt;
}

int Storage::deleteUser(std::function<bool(const User &)> filter){
    int initial=m_userList.size();
    m_userList.remove_if(filter);
    int after=m_userList.size();
    if(after!=initial)
        m_dirty=true;
    return initial-after;
}

void Storage::createMeeting(const Meeting & t_meeting){
    m_meetingList.push_back(t_meeting);
    m_dirty=true;
}

std::list<Meeting> Storage::queryMeeting(std::function<bool (const Meeting &)> filter) const{
    std::list<Meeting> ret;
    for(auto iter=m_meetingList.begin(); iter!=m_meetingList.end(); iter++){
        if(filter(*iter)){
            ret.push_back(*iter);
        }
    }
    return ret;
}

int Storage::updateMeeting(std::function<bool (const Meeting &)> filter, 
                            std::function<void (Meeting &)> switcher){
    int cnt=0;
    for(auto iter=m_meetingList.begin(); iter!=m_meetingList.end(); iter++){
        if(filter(*iter)){
            switcher(*iter);
            cnt++;
        }
    }
    if(cnt!=0)
        m_dirty=true;
    return cnt;
}

int Storage::deleteMeeting(std::function<bool (const Meeting &)> filter){
    int initial=m_meetingList.size();
    m_meetingList.remove_if(filter);
    int after=m_meetingList.size();
    if(after!=initial)
        m_dirty=true;
    return initial-after;
}

bool Storage::sync(void){
    if(m_dirty){
        if(writeToFile()){
            m_dirty=false;
            return true;
        }
        else
            return false;
    }
    return true;
}