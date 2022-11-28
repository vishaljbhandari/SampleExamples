# ifndef USER_H
# define USER_H
#include "include/library.h"
using namespace std;

class User {
	private:
	string strUserName;
	string strPassWord;
	public:
	User(string strUserName, string strPassWord):strUserName(strUserName), strPassWord(strPassWord) {}
	inline string getUserName(){ return strUserName; }
	int validate(string str_password) { return str_password == strPassWord; }
};
#endif
