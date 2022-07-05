# Ujagaga file server
Minimalistic file server with upload, based on https://github.com/cesanta/mongoose
Additions in comparesment to the original:
- file upload feature to currently browsed folder
- no "Index of" in title, but actual path to folder being served
- parent folder link

# Building
I only created a "build.sh" to help build on linux, but mongoose does work on Windows and MacOs. Please refer to original repository to see how.

# Running
Build it using the "build.sh" script. Get hep by running with "-h" argument.

You can also use "share-files" script with no parameters. It displays the currently detected IP address and passes the calling directory to the application. For it to detect the correct IP address, please check the name of your network card and edit the "WIFI_CARD_NAME" variable. 


