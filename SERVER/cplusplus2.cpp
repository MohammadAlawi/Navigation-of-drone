#include <iostream>
#include <unistd.h>
using namespace std;

unsigned int microsecond = 1000000;
unsigned int counter = 0;

int main () {

	while(1){
if (counter >= 1){
	usleep(3 * microsecond);//sleeps for 3 second
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>THIS SHOULD BE 1</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";
   cout << "<h2>THIS SHOULD BE 1</h2>\n";
   cout << "</body>\n";
   cout << "</html>\n";
	counter = 0;
}
if (counter >= 0){
	usleep(3 * microsecond);//sleeps for 3 second
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>2222222222222222</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";
   cout << "<h2>22222222222222222</h2>\n";
   cout << "</body>\n";
   cout << "</html>\n";
	counter = 1;
}
   }

   return 0;
}
