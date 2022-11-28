# ifndef FILE_H
# define FILE_H
#include "include/library.h"
#include "include/booking.h"
#include "include/user.h"
using namespace std;

void printDetailedSeatAllocation();
int validateLoginResponse(char *) ;
void printBookingStatus(const char *);
void printMyAlterBooking(const char *);
void printMyBooking(const char *);
string getAllUserBooking(string);
string toString(int);
int strToInt(string);
int charToInt(char *);
int getUserBookingCount(string);
int getAvailability();
string makeBooking(string, int);
string alterBookings(string,int);
void getAllBookings();
int getTotalUsers(); 
int getTotalSeats(); 
void getAllUsers();
void showSystemSummary();
void printBookingConfirmation(char *);
void commitBookings();
#endif
