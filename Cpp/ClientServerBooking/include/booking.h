# ifndef BOOKING_H
# define BOOKING_H
# include "include/library.h"
using namespace std;

class Booking {
        string strSeat;
        string strName;
        
	public:
        Booking(string strSeat, string strName=""):strSeat(strSeat), strName(strName){}
        inline string getSeat(){ return strSeat; }
        inline string getBooking() { return strName; }
	void setSeat(string str_seat){ strSeat = str_seat; }
	void setBooking(string str_name) { strName = str_name; }
};
#endif
