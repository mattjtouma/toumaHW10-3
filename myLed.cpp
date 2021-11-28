#include <unistd.h>
#include <iostream>         // for the input/output
#include <stdlib.h>         // for the getenv call
#include <sys/sysinfo.h>    // for the system uptime call
#include <cgicc/Cgicc.h>    // the cgicc headers
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#define LED_GPIO "/sys/class/gpio/gpio60/"
using namespace std;
using namespace cgicc;

void writeGPIO(string filename, string value){
   fstream fs;
   string path(LED_GPIO);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

int main(){
   Cgicc form;                      // the CGI form object
   string cmd;                      // the Set LED command
   string times;
   writeGPIO("direction", "out");
   // get the state of the form that was submitted - script calls itself
   bool isStatus = form.queryCheckbox("status");
   form_iterator it = form.getElement("cmd");  // the radio command
   if (it == form.getElements().end() || it->getValue()==""){
      cmd = "off";                     // if it is invalid use "off"
   }
   else { cmd = it->getValue(); }      // otherwise use submitted value

   // Blink form
   form_iterator blink_it = form.getElement("times");
   if (blink_it == form.getElements().end() || blink_it->getValue()==""){
	    times = "0"; // if it is invalid blink zero times
   }
   else{
	   times = blink_it->getValue(); // otherwise use submitted value
   }

   char *value = getenv("REMOTE_ADDR");    // The remote IP address
 
   // generate the form but use states that are set in the submitted form
   // Functions come from the cgicc library
   cout << HTTPHTMLHeader() << endl;       // Generate the HTML form
   cout << html() << head() << title("Control LED") << head() << endl;
   cout << body() << h1("CPE 422/522 Project: Post LED Example") << endl;
   cout << h1("Controlling a GPIO pin via a web browser") << endl;
   cout << "<form action=\"/cgi-bin/myLed.cgi\" method=\"POST\">\n";
   cout << "<div>Set LED: <input type=\"radio\" name=\"cmd\" value=\"on\""
        << ( cmd=="on" ? "checked":"") << "/> On ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"off\""
        << ( cmd=="off" ? "checked":"") << "/> Off ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"flash\""
	<< ( cmd=="flash" ? "checked":"") << "/> Flash ";
   cout << "<input type=\"submit\" value=\"Set LED\" />";
   cout << "<p>Number of blinks: <input type=\"text\" name=\"times\"/></p>";
   cout << "</div></form>";
 
   // process the form data to change the LED state
   if (cmd=="on") writeGPIO("value", "1");              // turn on
   else if (cmd=="off") writeGPIO("value", "0");        // turn off
   else if (cmd=="flash"){
	   cout << "<div> Blinking the LED " << times << " times</div>";
	   writeGPIO("value", "0");
	   usleep(500000); // wait for 0.5 seconds
	   for (int k = 0; k<stoi(times); k++){
		   writeGPIO("value", "1"); // turn the LED on
		   usleep(500000); // wait for 0.5 seconds
		   writeGPIO("value", "0"); // turn the LED off
		   usleep(500000); // wait for 0.5 seconds
	   }
   }
   else cout << "<div> Invalid command! </div>";        // not possible
   cout << "<div> The CGI REMOTE_ADDR value is " << value << "</div>";
   cout << body() << html();
   return 0;
}
