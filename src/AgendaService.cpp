#include"AgendaService.hpp"


AgendaService::AgendaService(){
    m_storage=nullptr;
}


AgendaService::~AgendaService(){
    if(m_storage!=nullptr){
        m_storage->~Storage();
        m_storage=nullptr;
    }
}


bool AgendaService::userLogIn(const std::string & userName, const std::string & password){
    auto filter=[userName, password](const User & t_user)->bool{
        if(t_user.getName()!=userName)
            return false;
        if(t_user.getPassword()!=password)
            return false;
        return true;
    };
    std::list<User> correspond=m_storage->queryUser(filter);
    if(correspond.empty())
        return false;
    else
        return true;
}


bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                                 const std::string &email, const std::string &phone)
{
    // check whether the userName has been registered before. 
    auto filter=[userName](const User & t_user)->bool{
        if(t_user.getName()!=userName)
            return false;
        return true;
    };
    std::list<User> correspond=m_storage->queryUser(filter);
    if(!correspond.empty())
        return false;

    User t_user(userName, password, email, phone);
    m_storage->createUser(t_user);
    m_storage->sync();
    return true;
}


bool AgendaService::deleteUser(const std::string &userName, const std::string &password)
{
    auto filter=[=](const User & t_user)->bool{
        if(t_user.getName()!=userName)
            return false;
        if(t_user.getPassword()!=password)
            return false;
        return true;
    };
    if(m_storage->deleteUser(filter)!=0){
        m_storage->sync();
        return true;
    }

    // didn't fine the user or input an invalid password
    return false;
}


std::list<User> AgendaService::listAllUsers(void) const
{
    std::function<bool(const User&)> filter=[](const User& a)->bool{
        return true;
    };
    return m_storage->queryUser(filter);
}


bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                                  const std::string &startDate, const std::string &endDate,
                                  const std::vector<std::string> &participator)
{
    // check whether the sponor has been registered before. 
    auto sFilter=[userName](const User & t_user)->bool{
        if(t_user.getName()!=userName)
            return false;
        return true;
    };
    std::list<User> uCorrespond=m_storage->queryUser(sFilter);
    if(uCorrespond.empty())
        return false;
    
    // check whether the meeting has been created before. 
    auto tFilter=[title,userName](const Meeting & t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        if(t_meet.getTitle()!=title)
            return false;
        return true;
    };
    std::list<Meeting> mCorrespond=m_storage->queryMeeting(tFilter);
    if(!mCorrespond.empty())
        return false;
    
    // check whether the participators have been registered before.
    for(auto iter=participator.begin(); iter!=participator.end(); iter++){
        auto pFilter=[iter](const User &t_participator)->bool{
            if(*iter!=t_participator.getName())
                return false;
            return true;
        };
        uCorrespond=m_storage->queryUser(pFilter);
        if(uCorrespond.empty())
            return false;
    }

    // check whether the startDate equals to the endDate
    if(startDate==endDate)
        return false;

    // check whether the time of the meet is confilt with other meetings(Sponser)
    auto cuFilter=[userName](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        return true;
    };
    mCorrespond=m_storage->queryMeeting(cuFilter);
    for(auto iter=mCorrespond.begin(); iter!=mCorrespond.end(); iter++){
        Date start=Date::stringToDate(startDate);
        Date end=Date::stringToDate(endDate);
        if(start<iter->getEndDate() && end>=iter->getStartDate())
            return false;
    }

    // check whether the time of the meet is confilt with other meetings(participator)
    for(auto iter=participator.begin(); iter!=participator.end(); iter++){
        auto cpFilter=[iter](const Meeting &t_meet)->bool{
            // check whether *iter is the sponsor of t_meet.
            if(t_meet.getSponsor()==*iter)
                return true;
            // check whether *iter is the participator of t_meet.
            for(auto it=t_meet.getParticipator().begin(); it<t_meet.getParticipator().end(); it++){
                if(*it==*iter)
                    return true;
            }
            return false;
        };
        mCorrespond=m_storage->queryMeeting(cpFilter);
        for(auto iter=mCorrespond.begin(); iter!=mCorrespond.end(); iter++){
            Date start=Date::stringToDate(startDate);
            Date end=Date::stringToDate(endDate);
            if(start<iter->getEndDate() && end>=iter->getStartDate())
                return false;
        }
    }


    Meeting t_meeting(userName, participator, startDate, endDate, title);
    m_storage->createMeeting(t_meeting);
    m_storage->sync();
    return true;
}


bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator)
{
    // check whether the participator has been added into the meeting
    auto filter=[participator, title](const Meeting &t_meet)->bool{
        if(t_meet.getTitle()!=title)
            return false;
        auto iter=t_meet.getParticipator().begin();
        for(; iter!=t_meet.getParticipator().end(); iter++){
            if(*iter==participator)
                return true;
        }
        return false;
    };
    std::list<Meeting> correspond=m_storage->queryMeeting(filter);
    // That means the "title" meeting has a participator named "participator".
    if(correspond.size()!=0) 
        return false;

    /*****************divided line***********************/

    auto filter1=[title, userName](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        if(t_meet.getTitle()!=title)
            return false;
        return true;
    };
    auto switcher=[participator](Meeting &t_meet)->void{
        t_meet.addParticipator(participator);
    };
    if(m_storage->updateMeeting(filter1, switcher)>0){
        m_storage->sync();
        return true;
    }
    return false;
}


bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator)
{
    // // check whether the sponor has been registered before. 
    // auto sFilter=[userName](const User & t_user)->bool{
    //     if(t_user.getName()!=userName)
    //         return false;
    //     return true;
    // };
    // std::list<User> sCorrespond=m_storage->queryUser(sFilter);
    // if(sCorrespond.empty())
    //     return false;
    
    // // check whether the meeting exist. 
    // auto tFilter=[title](const Meeting & t_meeting)->bool{
    //     if(t_meeting.getTitle()!=title)
    //         return false;
    //     return true;
    // };
    // std::list<Meeting> tCorrespond=m_storage->queryMeeting(tFilter);
    // if(tCorrespond.empty())
    //     return false;
    
    // check whether the participator exists in the meeting
    auto filter=[participator, title](const Meeting &t_meet)->bool{
        if(t_meet.getTitle()!=title)
            return false;
        for(auto it=t_meet.getParticipator().begin(); it!=t_meet.getParticipator().end(); it++){
            if(*it==participator)
                return true;
        }
        return false;
    };
    std::list<Meeting> correspond=m_storage->queryMeeting(filter);
    if(correspond.size()==0)
        return false;
    
    auto fliter=[title, userName](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        if(t_meet.getTitle()!=title)
            return false;
        return true;
    };
    auto switcher=[participator](Meeting &t_meet)->void{
        t_meet.removeParticipator(participator);
    };
    if(m_storage->updateMeeting(fliter, switcher)>0){
        m_storage->sync();
        return true;
    }
    return false;
}


bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
    std::function<bool(const Meeting&)> Filter=[userName,title](const Meeting &t_meet)->bool{
        if(t_meet.getTitle()!=title)
            return false;
        for(auto iter=t_meet.getParticipator().begin(); iter!=t_meet.getParticipator().end(); iter++){
            if(*iter==userName)
                return true;
        }
        return false;
    };
    std::function<void(Meeting&)> switcher=[userName](Meeting &t_meet)->void{
        t_meet.removeParticipator(userName);
    };
    int ret=m_storage->updateMeeting(Filter, switcher);
    if(ret>0){
        m_storage->sync();
        return true;
    }
    return false;
}


std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const
{
    auto Filter=[userName, title](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        if(t_meet.getTitle()!=title)
            return false;
        return true;
    };
    return m_storage->queryMeeting(Filter);
}


std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &startDate,
                                               const std::string &endDate) const
{
    std::function<bool(const Meeting&)> Filter=[userName, startDate, endDate](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        if(!(t_meet.getStartDate()==Date::stringToDate(startDate)))
            return false;
        if(!(t_meet.getEndDate()==Date::stringToDate(endDate)))
            return false;
        return true;
    };
    return m_storage->queryMeeting(Filter);
}


std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
    std::function<bool(const Meeting&)> Filter=[userName](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()==userName)
            return true;
        std::vector<std::string>::const_iterator iter;
        for(iter=t_meet.getParticipator().begin(); iter!=t_meet.getParticipator().end(); iter++){
            if(*iter==userName)
                return true;
        }
        return false;
    };
    return m_storage->queryMeeting(Filter);
}


std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
    std::function<bool(const Meeting&)> Filter=[userName](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        return true;
    };
    return m_storage->queryMeeting(Filter);
}


std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const{
    auto Filter=[userName](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()==userName)
            return false;
        for(auto iter=t_meet.getParticipator().begin(); iter!=t_meet.getParticipator().end(); iter++){
            if(*iter==userName)
                return true;
        }
        return false;
    };
    return m_storage->queryMeeting(Filter);
}


bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
    std::function<bool(const Meeting&)> Filter=[userName, title](const Meeting &t_meet)->bool{
        if(t_meet.getSponsor()!=userName)
            return false;
        if(t_meet.getTitle()!=title)
            return false;
        return true;
    };
    int ret=m_storage->deleteMeeting(Filter);
    if(ret>0){
        m_storage->sync();
        return true;
    }
    return false;
}


bool AgendaService::deleteAllMeetings(const std::string &userName){
    std::function<bool(const Meeting&)> Filter=[userName](const Meeting &t_meet){
        if(t_meet.getSponsor()!=userName)
            return false;
        return true;
    };
    int ret=m_storage->deleteMeeting(Filter);
    if(ret>0){
        m_storage->sync();
        return true;
    }
    return false;
}


void AgendaService::startAgenda(void){
    if(m_storage==nullptr){
        m_storage=Storage::getInstance();
    }
}


void AgendaService::quitAgenda(void){
    if(m_storage!=nullptr){
        m_storage->~Storage();
        m_storage=nullptr;
    }
}