# include "include/library.h"
# include "include/user.h"
# include "include/connect.h"

using namespace std;

extern uint32_t ip_port;
extern char *server_ip;

extern map<string, User*> mapUsers;
int server_socket;
extern int server_fd;
extern struct sockaddr_in address;
int addrlen = sizeof(address);
void signalCallbackHandler(int signum){
	cout<<"Caught signal SIGPIPE"<<signum<<endl;;
}
void CleanUp() {
	cout<<"Cleaning Up, Before Terminiating"<<endl;	
}

string processBuffer(char *req_buff) {
	string str_opt, str_arg, req_string = std::string(req_buff);
	string strResponse = "Invalid";
        string::size_type pos1 = req_string.find('|');
        if(req_string.npos != pos1) {
		str_opt = req_string.substr(0, pos1);
	        str_arg = req_string.substr(pos1 + 1);
		cout<<"[DEBUG] "<<str_opt<<"|"<<str_arg<<endl;
		if(str_opt == "my_booking") {
			string::size_type pos2 = str_arg.find('#');
			string booking_name = str_arg.substr(0, pos2);
			strResponse = "my_booking|" + getAllUserBooking(booking_name);	
		}
		else if(str_opt == "check_avail") {
			string::size_type pos2 = str_arg.find('#');
                        string booking_name = str_arg.substr(0, pos2);
			string seat_count = str_arg.substr(pos2 + 1);
                        int iHowMany = getAvailability();
			int iReq = strToInt(seat_count);
			strResponse = "check_avail|Total Available " + toString(iHowMany) + ", Requested " + toString(iReq) + ", " + ((iHowMany-iReq)>0 ? "YOU CAN BOOK" : "YOU CAN NOT BOOK");

		}
		else if(str_opt == "avail_status") {
			int iHowMany = getAvailability();
			strResponse = "avail_status|total#" + toString(iHowMany);
		}
		else if(str_opt == "book_now") {
			cout<<"[DEBUG] Inside book_now"<<endl;
			string::size_type pos2 = str_arg.find('#');
			string booking_name = str_arg.substr(0, pos2);
			string strHowManyReq = str_arg.substr(pos2 + 1);
			int iHowManyAvl = getAvailability();
			int iHowManyReq = strToInt(strHowManyReq);
			if(iHowManyAvl<iHowManyReq) {
				strResponse = "book_now|Booking Request Failed. Available Seats " + toString(iHowManyAvl) + " Only"; 
			}
			else {
				if(iHowManyReq!=0){
					string book_seats = makeBooking(booking_name, iHowManyReq);
					int icount = count(book_seats.begin(), book_seats.end(), ',');	
				    	if (icount==0)	{
						strResponse = "book_now|Total " + toString(++icount) + " seat booked, Seat is " + book_seats;
					}
					else {
						strResponse = "book_now|Total " + toString(++icount)+ " seats booked, Seats are " + book_seats;
					}
					commitBookings();
				}
				else { 
					strResponse = "book_now|Invalid Booking Request"; 
				}
			}	
		}
		else if(str_opt == "login") {
			string::size_type pos2 = str_arg.find('#');
                        string strUserName = str_arg.substr(0, pos2);
                        string strUserPassCode = str_arg.substr(pos2 + 1);
			string strAdmin("admin");
			strResponse = "login|failed";
        		if(strUserName != strAdmin) {
				cout<<"[DEBUG] Total Users "<<mapUsers.size()<<endl;
				std::map<string, User*>::iterator itmapUser = mapUsers.begin();
				while(itmapUser!=mapUsers.end()) {
		                	if(itmapUser->first == strUserName) {
						cout<<"[DEBUG] User Found"<<endl;
	        	                	if(itmapUser->second->validate(strUserPassCode)) {
							strResponse = "login|connected";
							cout<<"[DEBUG] strResponse "<<strResponse<<endl;
						}
        		        	}
                			itmapUser++;
		        	}
			}
		}
		else if(str_opt == "alter_booking") {
			string::size_type pos2 = str_arg.find('#');
                        string strUserName = str_arg.substr(0, pos2);
                        string strRequest = str_arg.substr(pos2 + 1);
			if(strRequest == "all") {
				strResponse = alterBookings(strUserName,0);
			}
			else {
				strResponse = alterBookings(strUserName,strToInt(strRequest));
			}
			commitBookings();
		}
		else {
			strResponse = "Invalid Request|Invalid Request#Invalid Request";	
		}
	}
	cout<<"[DEBUG] Response ["<<strResponse<<"]"<<endl;		
	return strResponse;
}

int main(int argc,char* argv[]) {
	if(argc!=2) {
		cout<<"Usage "<<argv[0]<<" <PORT>"<<endl;
		exit(1);
	}
	ip_port = (uint32_t)charToInt(argv[1]);
	signal(SIGPIPE, signalCallbackHandler);
	cout<<"WELCOME TO BOOKING SYSTEM"<<endl;
	cout<<"Loading User Details... ";
	getAllUsers();
	cout<<endl<<"Enter Admin Login Password : ";
	string strUserPassCode;
	cin>>strUserPassCode;
	int iValidate = 0;
	string strAdmin("admin");
	std::map<string, User*>::iterator itmapUser = mapUsers.begin();
	while(itmapUser!=mapUsers.end()) {	
		if(itmapUser->first == strAdmin) {
			iValidate = itmapUser->second->validate(strUserPassCode);
		}
		itmapUser++;
	}
	if(iValidate) {
		try {
		    cout<<"Loading Bookings... ";
		    getAllBookings();
		    cout<<endl;
		    showSystemSummary();
		    cout<<"Starting Booking Server ...";
		    int iOtry=1;
		    while(iOtry) {
			if(!ServerConnect()) {
				cout<<"Server Started Successfully!! [Code:"<<server_socket<<"]"<<endl<<endl<<endl;
				int itry=1;
				while(1) {
					try {
						if ((server_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
							perror("Socket Connection Count Not Be Accepted");
						}
						cout<<"A Client Is Listening On This Server [Code:"<<server_socket<<":"<<server_fd<<"]"<<endl<<endl<<endl;
					}
					catch(...) {
						cout<<"Exception was caught ";
						itry = 0;
					}
					while(itry) {
						try {
							if(server_fd) {
								char cServerBuffer[1024];
								for(int iC=0; iC<1024; iC++) cServerBuffer[iC] = '\0';
								read(server_socket, cServerBuffer, 1024);
								cout<<"Client Request Recieved ["<<cServerBuffer<<"]"<<endl;
								if(cServerBuffer[0]!='\0'){
									string strResponse = processBuffer(cServerBuffer);
									cout<<"Server Response Prepared ["<<strResponse<<"] ....";
									if(strResponse!="") {
										send(server_socket, strResponse.c_str(), strlen(strResponse.c_str()), 0);
										cout<<" Response Is Sent To Client"<<endl;
										showSystemSummary();
									}
 									else {
										cout<<"Invalid Response Generated By Server, Can Not Send To Client"<<endl;
									}
									for(int iC=0; iC<1024; iC++) cServerBuffer[iC] = '\0';
								}
								else {
									cout<<"Invalid Request Recieved From Client, Can Not Proceed"<<endl;
									itry=0;
								}
								cout<<endl<<endl<<endl;
							}	
						}
						catch(...) {
							cout<<"Exception was caught ";
							itry=0;
						}	
					}
					close(server_socket);
					cout<<"SERVER_SOCKET Closed ["<<itry<<"]"<<endl;
					if(!itry) break;
				}
				close(server_fd);
				cout<<"SERVER FD Closed ["<<itry<<"]"<<endl;
			}
			else {
				cout<<"Failed!! [Code:"<<server_socket<<"]"<<endl;
				exit(EXIT_FAILURE);
				iOtry=0;
			}
		    }
		}
		catch(...) {
			cout<<"Exception Caught"<<endl;
		}
	}
	else {
		cout<<"Invalid Admin Password"<<endl;
	}
	commitBookings();
	CleanUp();
	return 0;
}
