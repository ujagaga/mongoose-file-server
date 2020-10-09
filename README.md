# mongoose_file-server
Minimalistic file server with upload, based on https://github.com/cesanta/mongoose/tree/master/examples/simplest_web_server.
Additions in comparesment to the original:
- file upload feature to currently browsed folder
- no "Index of" in title, but actual path to folder being served
- parent folder link

# Building
I only created a "build.sh" to help build on linux, but mongoose does work on Windows and MacOs. Please refer to original repository to see how.

There is a latest executable provided, but you can also build it using the "build.sh" script. It will produce "mongoose-fs" which you can run by calling it and passing the path to folder to be served as an argument. 

You can also use "share-files.sh" script with no parameters. It displays the currently detected IP address and passes the calling directory to the "mongoose-fs". For it to detect the correct IP address, please check the name of your network card and edit the "WIFI_CARD_NAME" variable. 


