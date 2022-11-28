# include "include/library.h"
# include "include/connect.h"
int server_socket;
extern uint32_t ip_port;
extern char *server_ip;
extern int client_socket;

void CleanUp() {
}

int main(int argc,char* argv[]) {
        if(argc!=3) {
                cout<<"Usage "<<argv[0]<<" <IP> <PORT>"<<endl;
                exit(1);
        }
	ip_port = (uint32_t)charToInt(argv[2]);
	server_ip = (char *)argv[1];
	try {
		cout<<"Connecting To The Booking Server... ";
		if(!ClientConnect()) {
			cout<<"Done"<<endl;
			string strUsername="", strUserPassCode="";
			cout<<"Welcome, Booking Center"<<endl<<"Enter Your Login Credentials"<<endl<<"Enter Username : ";
			cin>>strUsername;
			if(strUsername == string("admin")) { 
				cout<<"Admin Cant Not Login Here, Quitting"<<endl; exit(1); 
			}
			cout<<"Enter Passcode : ";
			cin>>strUserPassCode;
			char cResposeBuffer[1024] = {0};
			string strRequest = "";
			string strLoginString = "login|" + strUsername + "#" + strUserPassCode;
			cout<<"Login Request Prepared";
			send(client_socket, strLoginString.c_str(), strlen(strLoginString.c_str()), 0);
			cout<<", Sent";
			read(client_socket, cResposeBuffer, 1024);
			cout<<", Recieved";
			if(!validateLoginResponse(cResposeBuffer)) {
				cout<<", Validated...";
				while(1) {
					cout<<endl<<endl<<endl<<"Hello ["<<strUsername<<"] Welcome To Booking Center"<<endl;
					cout<<"Press 1 : For Booking Status"<<endl;
					cout<<"Press 2 : For My Bookings"<<endl;
					cout<<"Press 3 : For New Booking"<<endl;
					cout<<"Press 4 : For Cancel Booking"<<endl;
					cout<<"Press 5 : For Quiting"<<endl<<"Enter You Choice : ";
					for(int iC=0; iC<1024; iC++) cResposeBuffer[iC] = '\0';
					int choice1=0, choice2=0, quit=0, iNumSeats = 0;
					cin>>choice1;
					switch(choice1) {
						default : 	cout<<"Invalid Choice"<<endl; break;
						case 5  : 	quit=1; break;
						case 1  : 	strRequest = "avail_status|";
								cout<<"Request Prepared";
								send(client_socket, strRequest.c_str(), strlen(strRequest.c_str()), 0);
								cout<<", Sent";
								read(client_socket, cResposeBuffer, 1024);
								cout<<", Response Recieved"<<endl;
								printBookingStatus(cResposeBuffer);
								break;
						case 3  :	cout<<"Enter How Seats Do You Want To Book : ";
								cin>>iNumSeats;
								if(iNumSeats==0) {
									cout<<"Invalid Booking Request"<<endl; break;
								}
								strRequest = "book_now|" + strUsername + "#" + toString(iNumSeats);
								cout<<"Request Prepared";
								send(client_socket, strRequest.c_str(), strlen(strRequest.c_str()), 0);
								cout<<", Sent";
                                                                read(client_socket, cResposeBuffer, 1024);
								cout<<", Response Recieved"<<endl;
								printBookingConfirmation(cResposeBuffer);
								break;
						case 2  :	strRequest = "my_booking|" + strUsername;
								cout<<"Request Prepared";
								send(client_socket, strRequest.c_str(), strlen(strRequest.c_str()), 0);
								cout<<", Sent";
                                                                read(client_socket, cResposeBuffer, 1024);
								cout<<", Response Recieved"<<endl;
                                                                printMyBooking(cResposeBuffer);
								break;
						case 4  :	cout	<<"Press 1 To Cancel All Bookings"<<endl
									<<"Press 2 To Cancel Some Bookings"<<endl
									<<"To Add More Bookings, Press Option 3 Of Main Menu"<<endl
									<<"Press 3 to go to main Menu"<<endl
									<<"Press Your Option : ";
								cin>>choice2;
								switch(choice2) {
									default : cout<<"Invalid Choice"<<endl;
									case 3  : break;
									case 1  : strRequest = "alter_booking|" + strUsername + "#all";
										  cout<<"Request Prepared";
										  send(client_socket, strRequest.c_str(), strlen(strRequest.c_str()), 0);
                                                               			  cout<<", Sent";
                                                               			  read(client_socket, cResposeBuffer, 1024);
                                                               			  cout<<", Response Recieved"<<endl;
                                                               			  printMyAlterBooking(cResposeBuffer);
                                                               			  break;
									case 2  : cout<<"How Many Tickets You Want To Cancel? ";
										  cin>>iNumSeats;
										  if(iNumSeats==0) {
                                                                       		 	cout<<"Invalid Booking Request"<<endl; break;
                                                                		  }
										  strRequest = "alter_booking|" + strUsername + "#" + toString(iNumSeats);
                                                                                  cout<<"Request Prepared";
										  send(client_socket, strRequest.c_str(), strlen(strRequest.c_str()), 0);
                                                                                  cout<<", Sent";
                                                                                  read(client_socket, cResposeBuffer, 1024);
                                                                                  cout<<", Response Recieved"<<endl;
                                                                                  printMyAlterBooking(cResposeBuffer);
										  break;
								}
								break;
					}
					choice1=0; 
					choice2=0;
					cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
					if(quit!=0) {
						break;
					}
				}
			}
			else {
				cout<<", Could Not Be Validated... Quiting.. "<<endl<<endl<<endl;
			}
			close(client_socket);
		}
		else { 
			cout<<"Could Not Connect... Quiting..."<<endl; 
		}
	}
	catch(...) {
		cout<<"Exception Caught"<<endl;
	}
	CleanUp();
	return 0;
}
