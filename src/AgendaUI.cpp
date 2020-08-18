#include"AgendaUI.hpp"
#include<iterator>

AgendaUI::AgendaUI(){
    m_userName.clear();
    m_userPassword.clear();
}

void printBeforeLoad(){
    std::cout << std::endl
              << "-------------------------------------Agenda-------------------------------------" 
              << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "l    - log in Agenda by user name and password" << std::endl;
    std::cout << "r    - register an Agenda accout" << std::endl;
    std::cout << "q    - quit Agenda" << std::endl;
    std::cout << "--------------------------------------------------------------------------------"
              << std::endl;
}

void printAfterLoad(){
    std::cout << std::endl
              << "-------------------------------------Agenda-------------------------------------"
              << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "o    - log out Agenda" << std::endl;
    std::cout << "dc   - delete Agenda account" << std::endl;
    std::cout << "lu   - list all Agenda user" << std::endl;
    std::cout << "cm   - create a meeting" << std::endl;
    std::cout << "amp  - add meeting participator" << std::endl;
    std::cout << "rmp  - remove meeting participator" << std::endl;
    std::cout << "rqm  - request to qiut meeting" << std::endl;
    std::cout << "la   - list all meetings" << std::endl;
    std::cout << "las  - list all sponor meetings" << std::endl;
    std::cout << "lap  - list all participator meetings" << std::endl;
    std::cout << "qm   - query meeting by title" << std::endl;
    std::cout << "qt   - query meeting by time interval" << std::endl;
    std::cout << "dm   - delete meeting by title" << std::endl;
    std::cout << "da   - delete all meetings" << std::endl;
    std::cout << "--------------------------------------------------------------------------------"
              << std::endl;
}

void AgendaUI::startAgenda(void){
    m_agendaService.startAgenda();
}


void AgendaUI::OperationLoop(void){
    system("clear");
    printBeforeLoad();
    std::cout << std::endl 
              << "Agenda :~$ ";
    std::string command=getOperation();
    while(executeOperation(command)){
        if(m_userName.empty()){
            printBeforeLoad();
            std::cout << std::endl << "Agenda :~$ ";
            command=getOperation();
            // system("clear");
            // std::cout << std::endl << "Agenda :~$ " << command << std::endl;
        }else{
            std::cout << std::endl << "Agenda@" << m_userName << " :~# ";
            command=getOperation();
            // system("clear");
            // printAfterLoad();
            // std::cout << std::endl << "Agenda@" << m_userName << " :~# " << command << std::endl;
        }
    }
}

std::string AgendaUI::getOperation(void){
    std::string operation;
    std::cin >> operation;
    return operation;
}

bool AgendaUI::executeOperation(const std::string &t_operator){
    if(m_userName.empty()){    
        if(t_operator=="q"){
            quitAgenda();
            return false;
        }
        if(t_operator=="l"){
            userLogIn();
            return true;
        }
        if(t_operator=="r"){
            userRegister();
            return true;
        }
    }else{
        if(t_operator=="o"){
            userLogOut();
            return true;
        }
        if(t_operator=="dc"){
            deleteUser();
            return true;
        }
        if(t_operator=="lu"){
            listAllUsers();
            return true;
        }
        if(t_operator=="cm"){
            createMeeting();
            return true;
        }
        if(t_operator=="amp"){
            addMeetingParticipator();
            return true;
        }
        if(t_operator=="rmp"){
            removeMeetingParticipator();
            return true;
        }
        if(t_operator=="rqm"){
            quitMeeting();
            return true;
        }
        if(t_operator=="la"){
            listAllMeetings();
            return true;
        }
        if(t_operator=="las"){
            listAllSponsorMeetings();
            return true;
        }
        if(t_operator=="lap"){
            listAllParticipateMeetings();
            return true;
        }
        if(t_operator=="qm"){
            queryMeetingByTitle();
            return true;
        }
        if(t_operator=="qt"){
            queryMeetingByTimeInterval();
            return true;
        }
        if(t_operator=="dm"){
            deleteMeetingByTitle();
            return true;
        }
        if(t_operator=="da"){
            deleteAllMeetings();
            return true;
        }
    }

    if(t_operator=="666"){
        std::cout << std::endl 
                  << "守护最好的割割(>O<)!!" 
                  <<std::endl;
        return true;
    }

    std::cout << std::endl 
              << "Don't find the operation! Please enter the right command!"<< std::endl;
    return true;
}


void AgendaUI::userLogIn(void){
    std::cout << std::endl
              << "[log in] [username] [password]" << std::endl;
    std::cout << "[log in] ";
    std::cin >> m_userName >> m_userPassword;
    if(m_agendaService.userLogIn(m_userName, m_userPassword)){
        std::cout << "[log in] succeed!" << std::endl;
        printAfterLoad();
    }else{
        std::cout << "[log in] Password error or user doesn't exist" << std::endl;
        m_userName.clear();
        m_userPassword.clear();
    }
}


void AgendaUI::userRegister(void){
    std::cout << std::endl
              << "[register] [username] [password] [email] [phone]" << std::endl;
    std::cout << "[register] ";
    std::string userName, password, phone, email;
    std::cin >> userName >> password >> email >> phone;
    if(m_agendaService.userRegister(userName, password, email, phone)){
        std::cout << "[register] succeed!" << std::endl;
    }else{
        std::cout << "[register] This username has been registered!" <<std::endl;
    }
}


void AgendaUI::userLogOut(void){
    m_userName.clear();
    m_userPassword.clear();
}


void AgendaUI::quitAgenda(void){
    m_agendaService.quitAgenda();
    m_userName.clear();
    m_userPassword.clear();
}


void AgendaUI::deleteUser(void){
    if(m_agendaService.deleteUser(m_userName, m_userPassword)){
        std::cout << std::endl
                  << "[delete agenda account] succeed!" << std::endl;
        m_userName.clear();
        m_userPassword.clear();
    }
}


void AgendaUI::listAllUsers(void){
    std::cout << std::endl
              << "[list all users]" << std::endl;
    std::cout << std::endl
              << "name\t" << "email\t" << "phone\t" << std::endl;

    std::list<User> listUser=m_agendaService.listAllUsers();
    for(auto iter=listUser.begin(); iter!=listUser.end(); iter++){
        std::cout<<iter->getName()<<'\t'<<iter->getEmail()<<'\t'<<iter->getPhone()<<std::endl; 
    }
}

void AgendaUI::createMeeting(void){
    int num;
    std::cout << std::endl
              << "[create meeting] [the number of participators]" << std::endl;
    std::cout << "[create meeting] ";
    std::cin >> num;

    std::vector<std::string> pList;
    std::string participator;
    for(int i=1; i<=num; i++){
        std::cout << "[create meeting] [please enter the participator" << i << ']' << std::endl;
        std::cout << "[create meeting] ";
        std::cin >> participator;
        pList.push_back(participator);
    }

    std::string title, startTime, endTime;
    std::cout << "[create meeting] [title][start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::cout << "[create meeting]";
    std::cin >> title;
    std::cin >> startTime;
    std::cin >> endTime;
    
    if(m_agendaService.createMeeting(m_userName, title, startTime, endTime, pList)){
        std::cout << "[create meeting] succeed!" << std::endl;
    }else{
        std::cout << "[create meeting] error!" << std::endl;
    }
}


void AgendaUI::addMeetingParticipator(void){
    std::cout << std::endl
              << "[add participator] [meeting title] [participator username]" <<std::endl;
    std::cout << "[add participator] ";
    std::string title, pName;
    std::cin >> title >> pName;
    if(m_agendaService.addMeetingParticipator(m_userName, title, pName)){
        std::cout << "[add participator] succeed!" << std::endl;
    }else{
        std::cout << "[add participator] error!" << std::endl;
    }
}


void AgendaUI::removeMeetingParticipator(void){
    std::cout << std::endl
              << "[remove participator] [meeting title] [participator username]" << std::endl;
    std::cout << "[remove participator]";
    std::string title, pName;
    std::cin >> title >> pName;

    if(m_agendaService.removeMeetingParticipator(m_userName, title, pName)){
        std::cout << "[remove participator] succeed!" <<std::endl;
    }else{
        std::cout << "[remove participator] error!" << std::endl;
    }
}


void AgendaUI::quitMeeting(void){
    std::cout << std::endl
              << "[quit meeting] [meeting title]" << std::endl;
    std::cout << "[quit meeting]";
    std::string title;
    std::cin >> title;
    if(m_agendaService.quitMeeting(m_userName, title)){
        std::cout << "[quit meeting] succeed!" <<std::endl;
    }else{
        std::cout << "[quit meeting] error!" << std:: endl;
    }
}


void AgendaUI::printMeetings(const std::list<Meeting> &t_meetings){
    std::cout << std::endl
              << "title\t" << "sponsor\t" << "start time\t" << "end time\t" << "participator\t" << std::endl;
    for(auto iter=t_meetings.begin(); iter!=t_meetings.end(); iter++){
        std::cout<<iter->getTitle()<<'\t'<<iter->getSponsor()<<'\t'
                 <<Date::dateToString(iter->getStartDate())<<'\t'<<Date::dateToString(iter->getEndDate())<<'\t'; 
        for(auto it=iter->getParticipator().begin(); it!=iter->getParticipator().end(); it++){
            if(it!=iter->getParticipator().begin())
                std::cout << ',';
            std::cout << *it; 
        }
        std::cout << std::endl;  
    }
}

void AgendaUI::listAllMeetings(void){
    std::cout << std::endl
              << "[list all meetings]" << std::endl;
    std::list<Meeting> mList=m_agendaService.listAllMeetings(m_userName);
    printMeetings(mList);
}


void AgendaUI::listAllSponsorMeetings(void){
    std::cout << std::endl
              << "[list all sponsor meetings] " << std::endl;
    std::list<Meeting> mList=m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(mList);
}


void AgendaUI::listAllParticipateMeetings(void){
    std::cout << std::endl
              << "[list all participator meetings]" << std::endl;
    std::list<Meeting> mList=m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(mList);
}


void AgendaUI::queryMeetingByTitle(void){
    std::cout << std::endl
              << "[query meetings] [title]:" << std::endl;
    std::cout << "[query meeting] ";
    std::string title;
    std::cin >> title;
    std::list<Meeting> mList=m_agendaService.meetingQuery(m_userName, title);
    printMeetings(mList);
}


void AgendaUI::queryMeetingByTimeInterval(void){
    std::cout << std::endl 
              <<"[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::cout <<"[query meetings] ";
    std::string startDate, endDate;
    std::cin >> startDate >> endDate;
    std::cout << "[query meetings]" << std::endl;

    std::list<Meeting> mList=m_agendaService.meetingQuery(m_userName, startDate, endDate);
    printMeetings(mList);
}


void AgendaUI::deleteMeetingByTitle(void){
    std::cout << std::endl 
              << "[delete meetings] [title]" << std::endl;
    std::cout << "[delete meetings] ";
    std::string title;
    std::cin >> title;
    if(m_agendaService.deleteMeeting(m_userName,title)){
        std::cout << "[delete meeting] succeed!" << std::endl;
    }else{
        std::cout << "[error] delete meeting fail!" <<std::endl;
    }
}


void AgendaUI::deleteAllMeetings(void){
    if(m_agendaService.deleteAllMeetings(m_userName)){
        std::cout << std::endl
                << "[delete all meeting] succeed!" << std::endl;
    }
}


