# Ujagaga file server
Minimalistic file server with upload, based on https://github.com/cesanta/mongoose
Additions in comparesment to the original:
- file upload feature to currently browsed folder
- no "Index of" in title, but actual path to folder being served
- parent folder link
- context menu
- login support

# Building
I only created a "build.sh" to help build on linux, but mongoose does work on Windows and MacOs, so please refer to original repository to see how.
If you need to run it on a platform other than linux, you will need to adjust the system commands for context menu at the start of the web_file_server.c.  
Before you build the app, please review the settings.h. if you want the login support, rename credentials.h.example to credentials.h and configure the login credentials. 
Keep in mind that user account info is saved in RAM, so if you plan on using a large number of user accounts, you should add support for a database or at least an external text file based configuration.
After that run:

    ./build.sh [app name]


# Running
Get help by running with "-h" argument.

    ./ujagagafs -h

You can also use "share-files" script with no parameters. It displays the currently detected IP address and passes the calling directory to the application. For it to detect the correct IP address, please check the name of your network card and edit the "WIFI_CARD_NAME" variable. 


