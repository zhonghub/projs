#include<iostream>
#include<string>
#include<list>
#include<vector>
#include"AgendaService.hpp" 
using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}
AgendaService::~AgendaService() {
	quitAgenda();
}
bool AgendaService::userLogIn(const std::string& userName, const std::string& password) {
	auto filter = [userName, password](const User& user) {//设置filter
		if (userName == user.getName() && password == user.getPassword())
			return true;
		else
			return false;
	};
	list<User>temp = m_storage->queryUser(filter);//返回所有满足filter的User
	int size = temp.size();
	if (size > 0)
		return true;
	else
		return false;
}

bool AgendaService::userRegister(const std::string& userName, const std::string& password, const std::string& email, const std::string& phone) {
	User U(userName, password, email, phone);
	auto filter = [userName](const User& user) {
		if (user.getName() == userName)
			return true;
		else
			return false;
	};
	list<User> temp = m_storage->queryUser(filter);
	if (temp.empty()) {
		m_storage->createUser(U);
		return true;
	}
	else
		return false;
}

bool AgendaService::deleteUser(const std::string& userName, const std::string& password) {
	auto filter = [userName, password](const User& user) {
		if (userName == user.getName() && password == user.getPassword())
			return true;
		else	
			return false;
	};
	deleteAllMeetings(userName);
	auto parlist = listAllParticipateMeetings(userName);
	for (auto it = parlist.begin();it != parlist.end();it++) {
		quitMeeting(userName, it->getTitle());
	}
	int num = m_storage->deleteUser(filter);
	if (num > 0)	return true;
	else	return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
	return m_storage->queryUser([&](const User& user) {return true;});
}

bool AgendaService::createMeeting(const std::string& userName, const std::string& title, const std::string& startDate, const std::string& endDate, const std::vector<std::string>& participator) {
	Date start(startDate);
	Date end(endDate);
	if (!start.isValid(start) || !end.isValid(end) || start >= end) {//时间要求
		return false;
	}
	//title要求不能重名
	if (!(m_storage->queryMeeting([&](const Meeting& meeting) { return meeting.getTitle() == title; })).empty()) {
		return false;
	}
	//会议发起者要在User里
	if ((m_storage->queryUser([&](const User& user) { return user.getName() == userName; })).empty()) {
		return false;
	}

	if (participator.empty()) {//会议参与者要求非空
		return false;
	}

	for (int i = 0; i < participator.size(); i++) {//参与者重复
		for (int j = 0; j < participator.size(); j++) {
			if (i != j && participator[i] == participator[j])
				return false;
		}
	}
	for (int i = 0;i < participator.size();++i) {//参与者都要在User里
		if ((m_storage->queryUser([&](const User& userq) { return userq.getName() == participator[i]; })).empty()) {
			return false;
		}
	}
	for (int i = 0;i < participator.size();++i) { //会议发起者不能为在会议参与者里面
		if (participator[i] == userName)
			return false;
	}

	list<Meeting>m = listAllMeetings(userName);
	for (auto it = m.begin();it != m.end();it++) {//会议发起者的时间要求
		if (!(it->getEndDate() <= start || it->getStartDate() >= end)) {
			return false;
		}
	}
	for (int i = 0;i < participator.size();++i) {//会议参与者的时间要求
		list<Meeting> tmp = listAllMeetings(participator[i]);
		for (auto it = tmp.begin();it != tmp.end();it++) {
			if (!(it->getEndDate() <= start || it->getStartDate() >= end))
				return false;
		}
	}
	Meeting meet(userName, participator, start, end, title);
	m_storage->createMeeting(meet);
	return m_storage->sync();
}


bool AgendaService::addMeetingParticipator(const std::string& userName, const std::string& title, const std::string& participator) {
	auto filter = [participator](const User& user) {
		if (user.getName() == participator)
			return true;
		else	return false;
	};
	std::list<User>U = m_storage->queryUser(filter); //首先查询participator是否在User里
	if (U.size() != 1) {
		return false;
	}
	auto filter1 = [&](const Meeting& meeting) {//找到对应的会议meeting
		if (meeting.getSponsor() == userName && meeting.getTitle() == title) {
			std::list<Meeting>l = m_storage->queryMeeting([&](const Meeting& meeting1) {//所有参加的会议中冲突的meeting1
				if (meeting1.getSponsor() == participator || meeting1.isParticipator(participator)) {
					if (meeting.getStartDate() >= meeting1.getEndDate() || meeting.getEndDate() <= meeting1.getStartDate())
						return false;
					else	return true;
				}
				return false;
				});
			if (l.empty()) {//无冲突会议
				return true;
			}
			return false;//有冲突
		}
		return false;
	};
	auto switcher = [participator](Meeting& meeting) {
		meeting.addParticipator(participator);
	};
	int num = m_storage->updateMeeting(filter1, switcher);
	if (num > 0) return true;
	else	return false;
}

bool AgendaService::removeMeetingParticipator(const std::string& userName, const std::string& title, const std::string& participator) {
	if (userName == participator) {
		return false;
	}
	auto filter = [userName](const User& u) {
		if (u.getName() == userName)
			return true;
		else	return false;
	};
	auto filter1 = [participator](const User& u) {
		if (u.getName() == participator)
			return true;
		else	return false;
	};
	list<User>u1 = m_storage->queryUser(filter);
	list<User>u2 = m_storage->queryUser(filter1);
	if (u1.empty() || u2.empty()) {
		return false;
	}
	auto filter2 = [userName, title](const Meeting& meeting) {
		if (meeting.getSponsor() == userName && meeting.getTitle() == title)
			return true;
		else
			return false;
	};
	list<Meeting>l = m_storage->queryMeeting(filter2);//找到对应的会议
	if (l.size() != 1) {
		return false;
	}
	Meeting m = *(l.begin());
	if (!m.isParticipator(participator)) {
		return false;
	}
	auto switcher = [participator](Meeting& meeting) {//删除
		meeting.removeParticipator(participator);
	};

	int num = m_storage->updateMeeting(filter2, switcher);//进行相应操作
	m_storage->deleteMeeting([](const Meeting& meeting) { //参与者为空时删除会议
		return meeting.getParticipator().empty();
		});//
	if (num > 0)	return true;
	else	return false;
}

bool AgendaService::quitMeeting(const std::string& userName, const std::string& title) {
	auto filter = [userName, title](const Meeting& meeting) {
		if (meeting.isParticipator(userName) && meeting.getTitle() == title)
			return true;
		else
			return false;
	};
	auto switcher = [userName](Meeting& meeting) {
		meeting.removeParticipator(userName);
	};
	int num = m_storage->updateMeeting(filter, switcher);
	auto filter1 = [](const Meeting& meeting) {
		if (meeting.getParticipator().empty())
			return true;
		else
			return false;
	};
	m_storage->deleteMeeting(filter1);
	if (num > 0)
		return true;
	else	return false;
}
std::list<Meeting> AgendaService::meetingQuery(const std::string& userName, const std::string& title) const {
	list<Meeting>l;
	list<User>U = m_storage->queryUser([userName](const User& u) {
		if (u.getName() == userName)
			return true;
		else
			return false;
		});
	if (U.empty())	return l;
	auto filter = [userName, title](const Meeting& meeting) {
		if ((userName == meeting.getSponsor() || meeting.isParticipator(userName)) && title == meeting.getTitle())
			return true;
		else
			return false;
	};
	list<Meeting>temp = m_storage->queryMeeting(filter);
	return temp;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string& userName, const std::string& startDate, const std::string& endDate) const {
	Date start(startDate);
	Date end(endDate);
	list<Meeting> temp;
	if (start.isValid(start) == false || end.isValid(end) == false || start > end) {//
		return temp;
	}
	list<User>U = m_storage->queryUser([userName](const User& u) {
		if (u.getName() == userName)
			return true;
		else
			return false;
		});
	if (U.empty()) {
		return temp;
	}
	auto filter = [userName, start, end](const Meeting& meeting) {
		if ((meeting.getSponsor() == userName || meeting.isParticipator(userName)) && meeting.getEndDate() >= start && meeting.getStartDate() <= end)
			return true;
		else
			return false;
	};
	temp = m_storage->queryMeeting(filter);
	return temp;
}
std::list<Meeting> AgendaService::listAllMeetings(const std::string& userName) const {
	auto filter = [userName](const Meeting& meeting) {
		if (userName == meeting.getSponsor() || meeting.isParticipator(userName))
			return true;
		else	return false;
	};
	list<Meeting>temp = m_storage->queryMeeting(filter);
	return temp;
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string& userName) const {
	auto filter = [userName](const Meeting& meeting) {
		if (userName == meeting.getSponsor())
			return true;
		else
			return false;
	};
	list<Meeting>temp = m_storage->queryMeeting(filter);
	return temp;
}
std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string& userName) const {
	auto filter = [userName](const Meeting& meeting) {
		if (meeting.isParticipator(userName))
			return true;
		else
			return false;
	};
	list<Meeting>temp = m_storage->queryMeeting(filter);
	return temp;
}
bool AgendaService::deleteMeeting(const std::string& userName, const std::string& title) {
	auto filter = [userName, title](const Meeting& meeting) {
		if (userName == meeting.getSponsor() && title == meeting.getTitle())
			return true;
		else
			return false;
	};
	int num = m_storage->deleteMeeting(filter);
	if (num > 0)	return true;
	else	return false;
}
bool AgendaService::deleteAllMeetings(const std::string& userName) {
	auto filter = [userName](const Meeting& meeting) {
		if (userName == meeting.getSponsor())
			return true;
		else
			return false;
	};
	int num = m_storage->deleteMeeting(filter);
	if (num > 0)	return true;
	else	return false;
}
void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}
void AgendaService::quitAgenda(void) {
	m_storage->sync();
}
