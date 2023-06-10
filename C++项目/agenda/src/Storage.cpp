#include "Storage.hpp"
#include "Path.hpp"
#include <iostream>
#include <fstream>
using namespace std;
std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
    m_dirty = false;
    readFromFile();
}

bool Storage::readFromFile(void) {
    std::cout << "readFromFile\n";
    ifstream R_users(Path::userPath);
    ifstream R_meetings(Path::meetingPath);
    if (!R_users.is_open() || !R_meetings.is_open()) {
        std::cout << "open false\n";
        return false;
    }
    else {
        std::cout << "open ok\n";
    }
    string s1;
    while (getline(R_users, s1)) {
        string username, password, email, phone;
        if (s1.size() == 0) continue;
        int n = 0; //recode',';
        // user格式：”<username>”,”<password>”,”<email>”,”<phone>”
        for (int i = 0; i < s1.size();++i) {
            if (s1[i] == ',') ++n;
            else if (s1[i] == '"') {}
            else {
                if (n == 0) username.push_back(s1[i]);
                if (n == 1) password.push_back(s1[i]);
                if (n == 2) email.push_back(s1[i]);
                if (n == 3) phone.push_back(s1[i]);
            }
        }
        User u(username, password, email, phone);
        this->m_userList.push_back(u);
    }
    R_users.close();

    string s2;
    while (getline(R_meetings, s2)) {
        string sponsor, participators, d1, d2, title;
        if (s2.size() == 0) continue;
        int n = 0; //recode',';
        //”<sponsor>”,”<B&C&D>”,”<Date d1>”,”<Date d2>”,”<title>”
        for (int i = 0; i < s2.size();++i) {
            if (s2[i] == ',') ++n;
            else if (s2[i] == '"') {}
            else {
                if (n == 0) sponsor.push_back(s2[i]);
                if (n == 1) participators.push_back(s2[i]);
                if (n == 2) d1.push_back(s2[i]);
                if (n == 3) d2.push_back(s2[i]);
                if (n == 4) title.push_back(s2[i]);
            }
        }
        participators.push_back('&');//格式变为 B&C&D&
        vector<string> v_participators;
        string temp;
        for (int i = 0; i < participators.size();++i) {
            if (participators[i] != '&')
                temp.push_back(participators[i]);
            else {
                v_participators.push_back(temp);
                temp.clear();
            }
        }
        Date start(d1);
        Date end(d2);
        Meeting m(sponsor, v_participators, start, end, title);
        this->m_meetingList.push_back(m);
    }
    R_meetings.close();
    return true;
}

bool Storage::writeToFile(void) {
    std::cout << "writeToFile\n";
    ofstream W_users(Path::userPath);
    ofstream W_meetings(Path::meetingPath);
    if (!W_users.is_open() || !W_meetings.is_open()) return false;
    for (auto it = m_userList.begin();it != m_userList.end();it++) {
        W_users << '"' << (*it).getName() << '"' << ',' << '"' << (*it).getPassword() << '"' << ',' << '"' << (*it).getEmail() << '"' << ',' << '"' << (*it).getPhone() << '"';
        W_users << "\n";
    }
    W_users.close();
    for (auto it = m_meetingList.begin();it != m_meetingList.end();it++) {
        W_meetings << '"' << (*it).getSponsor() << '"' << ',' << '"';
        vector<string> ptors = (*it).getParticipator();
        for (int i = 0; i < ptors.size() - 1; ++i) {
            W_meetings << ptors[i] << '&';
        }
        W_meetings << ptors[ptors.size() - 1] << '"' << ',' << '"';
        W_meetings << (*it).getStartDate().dateToString((*it).getStartDate()) << '"' << ',' << '"' << (*it).getEndDate().dateToString((*it).getEndDate()) << '"' << ',' << '"' << (*it).getTitle() << '"';
        W_meetings << "\n";
    }
    W_meetings.close();
    return true;
}

std::shared_ptr<Storage> Storage::getInstance(void) {
    if (m_instance == nullptr)
        m_instance = shared_ptr<Storage>(new Storage());
    return m_instance;
}

Storage::~Storage() {
    sync();
}

void Storage::createUser(const User& t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) const {
    std::list<User> result;
    for (auto it = m_userList.begin();it != m_userList.end();it++) {
        if (filter(*it))
            result.push_back(*it);
    }
    return result;
}

int Storage::updateUser(std::function<bool(const User&)> filter, std::function<void(User&)> switcher) {
    int num = 0;
    for (auto it = m_userList.begin();it != m_userList.end();it++) {
        if (filter(*it)) {
            switcher(*it);
            num++;
        }
    }
    if (num > 0) m_dirty = true;
    return num;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int num = 0;
    for (auto it = m_userList.begin();it != m_userList.end();) {
        if (filter(*it)) {
            it = m_userList.erase(it);
            num++;
        }
        else {
            it++;
        }
    }
    if (num > 0) m_dirty = true;
    return num;
}

void Storage::createMeeting(const Meeting& t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting(
    std::function<bool(const Meeting&)> filter) const {
    list<Meeting> meeting;
    for (auto it = m_meetingList.begin();it != m_meetingList.end();it++) {
        if (filter(*it))
            meeting.push_back(*it);
    }
    return meeting;
}

int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
    std::function<void(Meeting&)> switcher) {
    int num = 0;
    for (auto it = m_meetingList.begin();it != m_meetingList.end();it++) {
        if (filter(*it)) {
            switcher(*it);
            num++;
        }
    }
    if (num > 0) m_dirty = true;
    return num;
}


int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int num = 0;
    for (auto it = m_meetingList.begin();it != m_meetingList.end();) {
        if (filter(*it)) {
            it = m_meetingList.erase(it);
            num++;
        }
        else
            it++;
    }
    if (num > 0) {
        m_dirty = true;
    }
    return num;
}

bool Storage::sync(void) {
    m_dirty = false;
    return writeToFile();
}
