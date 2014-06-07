Tether for iOS and Android
==========================
Internet connection sharing for iPhone, iPad, and Android.  Share your phones internet connection with other devices including your PC or Mac.  This is a simplified, cross-platform implementation of wi-fi tethering web proxy for sharing internet connections from your iOS or Android devices.


Overview
--------
The core of this project is written in pure C native code.  It runs on both Android via ndk-build and iOS as a result.  It is intended as a useful project to familiarise oneself with cross platform socket programming.  The project code currently supports GET requests over the proxy port 8080 with full HTTP headers.  It also allows HTTPS from the client browser.  It handles multiple requests simultaneously and does so quickly via multithreading (via unix forks).  

The intention is to release this to our Open Source community to improve the code base and additionally extend the core cross-platform socket code to be more comprehensive and to eventually add support for Windows phones and tablets using a this unified core networking codebase written in native C for all mobile platforms that support C (in Android's case via JNI and the NDK).

It follows the standard network socket usage steps,

1. socket() - The socket() function returns a socket file descriptor called sockfd
2. bind() - Attempt to bind to the port we passed into getaddrinfo()
3. listen() - Listen on the specified port for incoming connections
4. accept() - Accept the incoming connection
5. read/recv & write/send loop - Read and write the data coming through the connection
6. close() - Close the connection


Usage on OS X
-------------

Since C native code is used, the proxy code can initially be built, run and tested from an OS X command line to more easily view the workings of the server and client of the proxy via printf().  With a little work it can be run on Windows as well. 

This core code can be run from the command line for easier testing on OS X or Linux.  For the core cross-platform code used for both iOS and Android, please see the proxy.c file (located in the tether-android sub-project with the path of /tether/tether-android/jni/) to run the program using main() instead of proxy().  This includes the basic steps,

Compile using:
	
	$ gcc -o proxy proxy.c

Then run using:
	
	$ ./proxy 8080

For instance, in OS X setup the "Web Proxy (HTTP)" configuration such that all HTTP/HTTPS requests are piped through the running proxy IP and port 8080.

**Specifically, this is done using the following steps:**

1. Open "System Preferences"
2. Click "Network"
3. Click "Advanced..."
4. Click "Proxies"
5. Check "Web Proxy (HTTP)"
7. Under "Web Proxy Server" enter the IP Address or hostname of your local machine (for local testing) or your tethered Android or iOS device with the port of 8080 on which the app code is deployed and running
8. Click "OK"
9. Open your web browser or choice and surf the web


**Please note: The button animation may not complete once clicked in both iOS and Android, but the proxy is running in the background.**


Android Overview
----------------
The C native code is a library wrapped in an Android Activity and then called using System.loadLibrary(). Be sure to run the ndk-build before building the tether.apk from the Android project.

It has been tested and is working using a real device (the latest Google Nexus 7) using a wi-fi network configuration from OS X, in which all "Web Proxy (HTTP)" requests are sent to the Android device IP on port 8080, e.g. 192.168.1.2:8080.


iOS Overview
------------ 
The C native code is the same library wrapped in a shell of an iOS application which itself is written in Objective C.  It is included as a call to the native C "proxy(1)" from the UISwitch in the RFViewController.m

It has been tested and is working using an iPhone, an iPad and the iPhone Retina (3.5 inch) simulator using a wi-fi network configuration from OS X, in which all "Web Proxy (HTTP)" requests are sent to the iOS device IP on port 8080, e.g. 192.168.1.2:8080.

As with all things iOS, to build and install a signed ipa version of the file you will need an Apple developer account.


Using the Core Cross-Platform Native C Code
-------------------------------------------

The core cross-platform native C code which is reused for all OSes can be found in the `/<path>/tether/tether-android/proxy.c`
	
The proxy.c code can be setup to run as a either a native C standalone program (for use in testing on OS X and Linux) or as a native C library for use within an iOS or Android app.  The only difference is the the opening two lines.

To run it as a standalone C program use the "main" class code and proxy_port argv for the starting two lines of the class code (**be sure to comment out the library version of the class code**),

	int main(int argc, char **argv) {
		char *proxy_port = *(argv + 1);

To switch the same code back for use as C library code for inclusion in an Android or iOS app use the following starting two-lines (**again be sure to comment out the main standalone version of the class code if you've made the changes above**),

	int proxy(int start) {
		char *proxy_port = PORT; 

That's it!  You have a simple way to rapidly use, or develop and test the teher proxy code from your OS X or Linux command line and revert it back for use a native C library that you can include in your Android or iOS apps.


###To compile and run for OS X/Linux use:

__Be sure you are using the standalone C program code for the first two lines as noted above in proxy.c__
	
	$ cd /<path>/tether/tether-android/jni
	$ gcc -o proxy proxy.c

Then run using:

	$ ./proxy 8080
	
	
###To compile and run for Android use:

__Be sure you are using the C library program code for the first two lines as noted above in proxy.c__
	
	$ cd /<path>/tether/tether-android/
	$ ndk-build

Then run using:

	"run > build" in STS with the device connected
	or push the android app to the device.


###To compile and run for iOS use:

__Be sure you are using the C library program code for the first two lines as noted above in proxy.c__

Include the proxy.c library in the xcode sub-project (tether-ios) and wire it up to a UI element

Compile and run using an iOS emulator or deploy to your iOS device (an Apple developer account is required to run it on the real hardware of an iPhone or iPad)


Summation
---------

The key principles for this project are,

- The tethered proxy should work using the same core native C implementation for all MOSes (Mobile OSes) including iOS, Android, and Windows Phone 8.x (although wrapped in platform-specific applications)
- The tethered proxy should support HTTP & HTTPS fully
- The tethered proxy should be multithreaded to handle simultaneous HTTP requests typical of a single web page these days
- The project should be improved by our open source community to include best practices and combine feature excellence that anyone would love to have in a tethering proxy app
- The project should serve as a practical introduction to network socket programming for those interested in learning it