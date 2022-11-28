#include "include/library.h"
#include "include/file.h"
using namespace std;
class User;
class Booking;

int iBookingControlFlag = 0;
map<string, User*> mapUsers;
map<string, Booking*> mapBooking;

void printDetailedSeatAllocation() {
        map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
        while(itmapBooking!=mapBooking.end()) {
		if(itmapBooking==mapBooking.begin()) {
			cout<<itmapBooking->second->getSeat()<<"["<<itmapBooking->second->getBooking()<<"]";
		}
		else {
			cout<<", "<<itmapBooking->second->getSeat()<<"["<<itmapBooking->second->getBooking()<<"]";
                }
		itmapBooking++;
        }
}

int validateLoginResponse(char *cResponse) { 
	string strResponse(cResponse);
	string::size_type pos = strResponse.find('|');
	string strResponseult = strResponse.substr(pos + 1);
	string str_connect("connected");
	if(strResponseult == str_connect) {
		return 0;
	}
	return 1;
}

void printBookingStatus(const char *cResponse) {
	string strResponse(cResponse);
	string::size_type pos1 = strResponse.find('|');
	string strResult = strResponse.substr(pos1 + 1);
	string::size_type pos2 = strResult.find('#');
	string str_seats = strResult.substr(pos2 + 1);
	cout<<"Total Seat Available : "<<str_seats<<endl;
}

void printMyAlterBooking(const char *cResponse) {
	string strResponse(cResponse);
	string::size_type pos1 = strResponse.find('|');
        string strResult = strResponse.substr(pos1 + 1);
	cout<<strResult<<endl;	
}

void printMyBooking(const char *cResponse) {
	string strResponse(cResponse);
	string::size_type pos1 = strResponse.find('|');
        string strResult = strResponse.substr(pos1 + 1);
	if(strResult=="") {
		cout<<"You Dont Have Any Booking With Us"<<endl;
	}
	else {
		int iTotalSeats = (int) std::count(strResult.begin(), strResult.end(), ','); 
		cout<<"Total Booked ["<<++iTotalSeats<<"], Seats : "<<strResult<<endl;
	}
}

string getAllUserBooking(string strUsername) {
	string strDetails = "";
	map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
	while(itmapBooking!=mapBooking.end()) {
		if(itmapBooking->second->getBooking()==strUsername) {
			if(strDetails!="") { strDetails+=","; }
			strDetails += itmapBooking->second->getSeat();
		}
		itmapBooking++;
	}
	return strDetails;
}

string toString(int iNum) {
    char* cStr = new char[2];
    sprintf(cStr, "%d", iNum);
    return string(cStr);
}


int strToInt(string strString) {
    const char *cStr = strString.c_str();
    int iNum;
    sscanf(cStr, "%d", &iNum);
    return iNum;
}

int charToInt(char * cStr) {
    int iNum;
    sscanf(cStr, "%d", &iNum);
    return iNum;
}

int getUserBookingCount(string strUsername) {
	int iSeatCount=0;
	map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
        while(itmapBooking!=mapBooking.end()) {
                if(itmapBooking->second->getBooking()==strUsername)
                        iSeatCount++;
                itmapBooking++;
        }
        return iSeatCount;
}

int getAvailability() {
        int iSeatCount=0;
        map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
        while(itmapBooking!=mapBooking.end()) {
                if(itmapBooking->second->getBooking()=="") {
                        iSeatCount++;
		}
                itmapBooking++;
        }
        return iSeatCount;
}

string makeBooking(string booking_name, int iHowManyReq) {
	string str_seat_list="Booking Server Is Busy...";
	while(!iBookingControlFlag) {
		str_seat_list="";
		iBookingControlFlag = 1;
        	int iSeatCount=0;
        	map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
        	while(itmapBooking!=mapBooking.end()) {
                	if(itmapBooking->second->getBooking()=="") {
				itmapBooking->second->setBooking(booking_name);
				if(str_seat_list!="") { 
					str_seat_list+=","; 
				}
				str_seat_list += itmapBooking->second->getSeat();
                        	iSeatCount++;
				if(iSeatCount==iHowManyReq) {
					break;
				}
			}	
                	itmapBooking++;
        	}
	}
	iBookingControlFlag = 0;
        return str_seat_list;
}

string alterBookings(string strUsername,int iCancelSeats=0) {
	string strAlterResponse = "Searver Is Busy, We can not alter your Bookings";
	int iFlagIsAll = iCancelSeats==0?1:0;
	if(iCancelSeats==0) {
		iCancelSeats = getUserBookingCount(strUsername);
	}
        while(!iBookingControlFlag) {
                iBookingControlFlag = 1;
                int iSeatCount=0;
                map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
                while(itmapBooking!=mapBooking.end()) {
                        if(itmapBooking->second->getBooking()==strUsername) {
                                itmapBooking->second->setBooking("");
                                iSeatCount++;
                                if(iSeatCount==iCancelSeats) {
					break;
				}
                        }
                        itmapBooking++;
                }
		if(iSeatCount==0) {
			strAlterResponse="You had no bookings with us, Nothing to cancel";
		}
		else if(iCancelSeats>iSeatCount) {
			strAlterResponse="You had only " + toString(iSeatCount) + " seats to Cancel, Which are cancelled now";
		}
		else {
			strAlterResponse="Your ";
        		if(iFlagIsAll==1) {
				strAlterResponse+="all ";
			} 
			strAlterResponse+=toString(iCancelSeats) + " seats have been cancelled";
		}
        }
        iBookingControlFlag = 0;
        return strAlterResponse;
}

void getAllBookings() {
	ifstream infile;
        string strLine;
        infile.open(BOOKING_FILE);
        if(infile.is_open()) {
                while (getline(infile,strLine)) {
			string::size_type pos = strLine.find('|');
			if(strLine.npos != pos) {
        			string name = strLine.substr(pos + 1);
        			string seat = strLine.substr(0, pos);
				Booking *bk = new Booking(seat, name);
				mapBooking.insert(std::pair<string, Booking*>(seat, bk));
			}	
                }
		cout<<"Done";
        }
	else {
		cout<<"Failed";		
	}
        infile.close();
}

int getTotalUsers() { 
	return mapUsers.size(); 
}

int getTotalSeats() { 
	return mapBooking.size(); 
}

void getAllUsers() {
	ifstream infile;
	string strLine;
        infile.open(USER_FILE);
	if(infile.is_open()) {
		while ( getline(infile,strLine)) {
			string::size_type pos = strLine.find('|');
			if(strLine.npos != pos) {
				string strPassword = strLine.substr(pos + 1);
				string strUsername = strLine.substr(0, pos);
				User *usr = new User(strUsername, strPassword);
				mapUsers.insert(std::pair<string, User*>(strUsername,usr));
    			}
		}
		cout<<"Done";
	}
	else {
		cout<<"Failed";
	}
	infile.close();
}

void showSystemSummary() {
        cout<<"CURRENT SYSTEM SUMMERY : Users ["<<getTotalUsers()<<"] Seats [Total:"<<getTotalSeats()<<"] [Available:"<<getAvailability()<<"]"<<endl;
        cout<<"Seat Allocations : ";
        printDetailedSeatAllocation();
	cout<<endl;
}

void printBookingConfirmation(char *cResponse) {
        string strResponse(cResponse);
        string::size_type pos = strResponse.find('|');
        string result = strResponse.substr(pos + 1);
	cout<<result<<endl;
}

void commitBookings() {
	ofstream bookFile;
	bookFile.open(BOOKING_FILE, std::ofstream::out | std::ofstream::trunc);
	if(bookFile.is_open()) {
	        map<string, Booking*>::iterator itmapBooking = mapBooking.begin();
	        while(itmapBooking!=mapBooking.end()) {
			bookFile<<itmapBooking->second->getSeat()<<"|"<<itmapBooking->second->getBooking()<<endl;
	                itmapBooking++;
	        }
	}
	else { 
		cout<<"Unable To Commit Bookings To Disk File"<<endl; 
	}
	bookFile.close();
}
