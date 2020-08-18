#include"User.hpp"
#include<string>

User::User(const std::string &t_userName, const std::string &t_userPassword,
       const std::string &t_userEmail, const std::string &t_userPhone)
    :m_name(t_userName), m_password(t_userPassword), m_email(t_userEmail), m_phone(t_userPhone){

}

User::User(const User& t_user){
    m_name=t_user.m_name;
    m_password=t_user.m_password;
    m_email=t_user.m_email;
    m_phone=t_user.m_phone;
}

std::string User::getName() const{
    return m_name;
}

void User::setName(const std::string& t_userName){
    m_name=t_userName;
}

std::string User::getPassword() const{
    return m_password;
}

void User::setPassword(const std::string& t_userPassword){
    m_password=t_userPassword;
}

std::string User::getEmail() const{
    return m_email;
}

void User::setEmail(const std::string& t_userEmail){
    m_email=t_userEmail;
}

std::string User::getPhone() const{
    return m_phone;
}

void User::setPhone(const std::string& t_userPhone){
    m_phone=t_userPhone;
}