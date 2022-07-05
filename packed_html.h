#ifndef UJAGAGA_FS_HTML
#define UJAGAGA_FS_HTML

#define APP_NAME "Ujagaga File Server"

#define FAVICON_ICO "data:image/png;base64,\
iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAADAFBMVEUAAAAAADMAAGYAAJkAAMwAAP8AMwAAMzMAM2YAM5kAM8wAM/8AZg\
AAZjMAZmYAZpkAZswAZv8AmQAAmTMAmWYAmZkAmcwAmf8AzAAAzDMAzGYAzJkAzMwAzP8A/wAA/zMA/2YA/5kA/8wA//8zAAAzADMzAGYz\
AJkzAMwzAP8zMwAzMzMzM2YzM5kzM8wzM/8zZgAzZjMzZmYzZpkzZswzZv8zmQAzmTMzmWYzmZkzmcwzmf8zzAAzzDMzzGYzzJkzzMwzzP\
8z/wAz/zMz/2Yz/5kz/8wz//9mAABmADNmAGZmAJlmAMxmAP9mMwBmMzNmM2ZmM5lmM8xmM/9mZgBmZjNmZmZmZplmZsxmZv9mmQBmmTNm\
mWZmmZlmmcxmmf9mzABmzDNmzGZmzJlmzMxmzP9m/wBm/zNm/2Zm/5lm/8xm//+ZAACZADOZAGaZAJmZAMyZAP+ZMwCZMzOZM2aZM5mZM8\
yZM/+ZZgCZZjOZZmaZZpmZZsyZZv+ZmQCZmTOZmWaZmZmZmcyZmf+ZzACZzDOZzGaZzJmZzMyZzP+Z/wCZ/zOZ/2aZ/5mZ/8yZ///MAADM\
ADPMAGbMAJnMAMzMAP/MMwDMMzPMM2bMM5nMM8zMM//MZgDMZjPMZmbMZpnMZszMZv/MmQDMmTPMmWbMmZnMmczMmf/MzADMzDPMzGbMzJ\
nMzMzMzP/M/wDM/zPM/2bM/5nM/8zM////AAD/ADP/AGb/AJn/AMz/AP//MwD/MzP/M2b/M5n/M8z/M///ZgD/ZjP/Zmb/Zpn/Zsz/Zv//\
mQD/mTP/mWb/mZn/mcz/mf//zAD/zDP/zGb/zJn/zMz/zP///wD//zP//2b//5n//8z///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAABlenwdAAABAHRSTlP///////////////////////////////////////////////////////////////////////\
//////////////////////////////////////////////////////////////////////////////////////////////////////////\
//////////////////////////////////////////////////////////////////////////////////////////////////////////\
////8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG8mZagAAAAlwSFlzAAAOxAAADsQBlSsOGwAAAFlJREFUGJVt\
j9sRwCAIBLcf+qHK60fLCY+YmDH3weACHjBbsJIKrpBzA0yjJKOAZbmRJSAK4N0Tz+liyYYH0AuQZoYdcIJjZP8U+WPbLmXbi7Xsf/XjuO\
/5F3y0dt8wYzlqAAAAAElFTkSuQmCC"

static const char login_html[] = "<!DOCTYPE html><html lang='en'>"
    "<link rel='icon' type='image/png' sizes='16x16' href='"FAVICON_ICO"' />"
    "<head><meta charset='utf-8'/>"
    "<title>"APP_NAME" Log In</title>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'/>"
    "<style type='text/css'>"
    "*{outline:none;}"
    "body{background-color:#3C3F41;color:#3c3f41;margin:0;padding:0;font:16px/1.4 Helvetica,Arial,sans-serif;}"
    "div.content{width:600px;margin:2em auto;padding:20px 50px;background-color:#D0D0D0;border-radius:1em;}"
    "h1{margin:5px;}"
    "#err-msg{color:red;margin:5px;}"
    "label{display:inline-block;min-width:7em;}"
    "input{border:1px solid #ccc;padding:0.2em;}"
    "input[type='submit']{min-width:7em;background-color:#3C3F41;color:#D0D0D0;border-radius:0.5em;}"
    "@media(max-width:700px){body{background-color:#D0D0D0;}div.content{width:auto;margin:0 auto;padding:1em;}}"
    "</style>"
    "</head><body>"
    "<div class='content'>"
    "<h1>"APP_NAME" Login</h1>"
    "<p id='err-msg'>%s</p>"
    "<form method='POST'>"
    "<div><label>Username:</label><input type='text' name='user' autofocus/></div>"
    "<div><label>Password:</label><input type='password' name='pass'/>"
    "</div><div>"
    "<input type='submit' value='Log in'>"
    "</div></form></div>"
    "</body></html>\n\n";


static const char about_html[] = "<!DOCTYPE html><html lang='en'>"
    "<link rel='icon' type='image/png' sizes='16x16' href='"FAVICON_ICO"' />"
    "<head><meta charset='utf-8'/>"
    "<title>About "APP_NAME"</title>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'/>"
    "<style type='text/css'>"
    "*{outline:none;}"
    "body{background-color:#d0d0d0;color:#3c3f41; margin:0;padding:20px;font:16px/1.4 Helvetica,Arial,sans-serif;}"
    "h1{margin:5px;}"
    ".code{width:100%;padding: 20px;background-color:#3c3f41;color:#d0d0d0;}"
    "</style>"
    "</head><body>"
    "<div class='content'>"
    "<h1>About <a href='/'>"APP_NAME"</a></h1>"
    "<p>Web based file server using <a href='https://github.com/cesanta/mongoose' target='_blank'>Mongoose - Embedded Web Server</a> as base.</p>"
    "<p>Github page: <a href='https://github.com/ujagaga/mongoose-file-server' target='_blank'>https://github.com/ujagaga/mongoose-file-server</a></p>"
    "<h2>Upgrades</h2>"
    "<ol>"
    "<li>Login</li>"
    "<li>Context menu</li>"
    "<li>File upload</li>"
    "<li>Styling</li>"
    "</ol>"
    "<h2>File Upload</h2>"
    "<p>You may use the web browser UI or upload via console or script. Added support for chunked file upload, so any file size is excepted.</p>"
    "<p>To upload using the <b>curl</b> utillity, please provide file name, and user credentials:</p>"
    "<p class='code'>curl -H \"Transfer-Encoding: chunked\" -F file=@my_large.file [server_url]/upload?name=my_large.file&u=username&p=passwd</p>"
    "<h2>File Download</h2>"
    "<p>To download a whole folder, use the context menu to archive the selected folder and then download the archive.</p>"
    "<p>TODO: console based download with provided user credentials.</p>"
    "</div></body></html>\n\n";


static const char logout_png[] = "data:image/png;base64,"
    "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAADAFBMVEUAAAAAADMAAGYAAJkAAMwAAP8AMwAAMzMAM2YAM5kAM8wAM/8AZg"
    "AAZjMAZmYAZpkAZswAZv8AmQAAmTMAmWYAmZkAmcwAmf8AzAAAzDMAzGYAzJkAzMwAzP8A/wAA/zMA/2YA/5kA/8wA//8zAAAzADMzAGYz"
    "AJkzAMwzAP8zMwAzMzMzM2YzM5kzM8wzM/8zZgAzZjMzZmYzZpkzZswzZv8zmQAzmTMzmWYzmZkzmcwzmf8zzAAzzDMzzGYzzJkzzMwzzP"
    "8z/wAz/zMz/2Yz/5kz/8wz//9mAABmADNmAGZmAJlmAMxmAP9mMwBmMzNmM2ZmM5lmM8xmM/9mZgBmZjNmZmZmZplmZsxmZv9mmQBmmTNm"
    "mWZmmZlmmcxmmf9mzABmzDNmzGZmzJlmzMxmzP9m/wBm/zNm/2Zm/5lm/8xm//+ZAACZADOZAGaZAJmZAMyZAP+ZMwCZMzOZM2aZM5mZM8"
    "yZM/+ZZgCZZjOZZmaZZpmZZsyZZv+ZmQCZmTOZmWaZmZmZmcyZmf+ZzACZzDOZzGaZzJmZzMyZzP+Z/wCZ/zOZ/2aZ/5mZ/8yZ///MAADM"
    "ADPMAGbMAJnMAMzMAP/MMwDMMzPMM2bMM5nMM8zMM//MZgDMZjPMZmbMZpnMZszMZv/MmQDMmTPMmWbMmZnMmczMmf/MzADMzDPMzGbMzJ"
    "nMzMzMzP/M/wDM/zPM/2bM/5nM/8zM////AAD/ADP/AGb/AJn/AMz/AP//MwD/MzP/M2b/M5n/M8z/M///ZgD/ZjP/Zmb/Zpn/Zsz/Zv//"
    "mQD/mTP/mWb/mZn/mcz/mf//zAD/zDP/zGb/zJn/zMz/zP///wD//zP//2b//5n//8z///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAABlenwdAAABAHRSTlP///////////////////////////////////////////////////////////////////////"
    "//////////////////////////////////////////////////////////////////////////////////////////////////////////"
    "//////////////////////////////////////////////////////////////////////////////////////////////////////////"
    "////8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG8mZagAAAAlwSFlzAAAOxAAADsQBlSsOGwAAAGZJREFUOI2l"
    "01sKgEAMQ9G7/xV2OYIompoSof0b5tDJvKhQ/ANcNQBQgAJoQFudoDUVYVbWOCMgAiLokfmWADMPERABe0AC25Bpm+kkt5eVrrsCeEb+Tb"
    "5GEv8GJSD9CxPMgLn24ACIWvVAuoMbQAAAAABJRU5ErkJggg==";


static const char about_png[] = "data:image/png;base64,"
    "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAACXBIWXMAAA7EAAAOxAGVKw4bAAADUElEQVRYhcWXS0gbURSGv9xpFLKwIK"
    "S4ETFFxOgiiKKQUlHBIC6kkW6KiLtSXQhCVmI3Le3OlYUB912U+thXKhSDogQERSTQisjEFy4M+CgxmS5mks7EySQTK/3hQs45c/7/PjJz"
    "z3HhDE+AbiAAPAUe6/4L4CewBawApw55beECwsA34BZQi4xb/dmwnnsvBNFWVUy00NjSORxDAO+B9D3EsyOtcwkrIastqgQ+o23hHTQ3Nx"
    "MKhQgEAtTW1uLxeLi5uUFRFDY2Npifn+fw8NAqdQF4Bfy2m4AEfAFe5MdCoRBjY2M0NTVZkeeQSqWQZZnZ2VlUVTWGVGAReAlkjIJGvANe"
    "W0yMpaUlampqbMUBJEmio6MDSZJYX183hlxAE/AI+G50ZhEEflDgrLa3t6msrDT50uk0LpcLIe6mpFIpent7OT4+zg9lgOdAFIOYC/hUSN"
    "yIvb09pqen6enpwe/34/f7GRoaYmdnx/Sc2+2mq6vLikLoWqZdDlPk37y8vKz29fUVjLe0tKjxeNw0Jicn7TjDoJ0HwJtiKx8cHOTy8rJg"
    "PJlM3vFdXV3ZUb4BFgR/P6+2sBMHGBgYuOPb3Ny0S+kGngj9R/7b4AidnZ2Mj4+bfGtra8RiMbs0CegWaBdL2Whvb0eWZSoqKnI+RVGIRC"
    "KlpAcE2q1WFtra2pibm8Pj8eR8Z2dnjI6Ocnpa0oXoE/y9Uh2hoaEBWZZN4icnJwwPD3NwcFAqzeOi770VhBDMzMxQVVWV852fnzMyMsL+"
    "/r4zLrRiwhGCwSCNjY0mXyQScSwOXAi0SsYRWltbTXYikWB1ddUpDcAvgVY0OEJ1dbXJPjo6KkccYEug1XBpJ1mSZP5sXF9flyOeBlYEWg"
    "G5Ug7DPbECnGZvpDAwX2pmXV0dXq83ZyeTSeLxuNMJDKFVSYB2Nd6n+CynWHVlhbOwLUiMmJiYoL+/P2fHYjGmpqZKXbmpIHlkCESBj0BR"
    "Jq/Xi8/ny9mJRKJUcXSNaNbIX+1btHNR+fdQde63Rmf+BDJopfPiA0xgUefOGJ1WdUAa+Ip2PM+wqJDr6+txu90oioKiKOzu7hKNRvMfyy"
    "IDfADG0No2R/hvrZkRD9qcOu1eje25D3N7/osy2vM/zbFmY3uzZhEAAAAASUVORK5CYII=";


static const char up_arrow_png[] = "data:image/png;base64,"
    "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAADAFBMVEUAAAAAADMAAGYAAJkAAMwAAP8AMwAAMzMAM2YAM5kAM8wAM/8AZgA"
    "AZjMAZmYAZpkAZswAZv8AmQAAmTMAmWYAmZkAmcwAmf8AzAAAzDMAzGYAzJkAzMwAzP8A/wAA/zMA/2YA/5kA/8wA//8zAAAzADMzAGYzAJ"
    "kzAMwzAP8zMwAzMzMzM2YzM5kzM8wzM/8zZgAzZjMzZmYzZpkzZswzZv8zmQAzmTMzmWYzmZkzmcwzmf8zzAAzzDMzzGYzzJkzzMwzzP8z/"
    "wAz/zMz/2Yz/5kz/8wz//9mAABmADNmAGZmAJlmAMxmAP9mMwBmMzNmM2ZmM5lmM8xmM/9mZgBmZjNmZmZmZplmZsxmZv9mmQBmmTNmmWZm"
    "mZlmmcxmmf9mzABmzDNmzGZmzJlmzMxmzP9m/wBm/zNm/2Zm/5lm/8xm//+ZAACZADOZAGaZAJmZAMyZAP+ZMwCZMzOZM2aZM5mZM8yZM/+"
    "ZZgCZZjOZZmaZZpmZZsyZZv+ZmQCZmTOZmWaZmZmZmcyZmf+ZzACZzDOZzGaZzJmZzMyZzP+Z/wCZ/zOZ/2aZ/5mZ/8yZ///MAADMADPMAG"
    "bMAJnMAMzMAP/MMwDMMzPMM2bMM5nMM8zMM//MZgDMZjPMZmbMZpnMZszMZv/MmQDMmTPMmWbMmZnMmczMmf/MzADMzDPMzGbMzJnMzMzMz"
    "P/M/wDM/zPM/2bM/5nM/8zM////AAD/ADP/AGb/AJn/AMz/AP//MwD/MzP/M2b/M5n/M8z/M///ZgD/ZjP/Zmb/Zpn/Zsz/Zv//mQD/mTP/"
    "mWb/mZn/mcz/mf//zAD/zDP/zGb/zJn/zMz/zP///wD//zP//2b//5n//8z///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAABlenwdAAABAHRSTlP//////////////////////////////////////////////////////////////////////////////////"
    "///////////////////////////////////////////////////////////////////////////////////////////////////////////"
    "//////////////////////////////////////////////////////////////////////////////////////////////////8AAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG8mZagAAAAlwSFlzAAAOxAAADsQBlSsOGwAAAJFJREFUOI2908ERgCAMRNH0k3"
    "7ohyqlHEUxbDZoPJmj/43MaJAGo2PwmUAVGzA3gDqMB6GbuEDMfSZY90sIvb8UOkWobxsJcQcc3YsOlLoT6sDoKDqIHUUTXXQQasD1KQxQN"
    "2GgZuCcO+Kn+gO0d+D/RQT6BcAZ42NUOIEXhkbjynH/trT52ucXJ796+eV9vP47aLZDjzrbq0YAAAAASUVORK5CYII=";

static const char new_dir_png[] = "data:image/png;base64,"
    "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAADAFBMVEUAAAAAADMAAGYAAJkAAMwAAP8AMwAAMzMAM2YAM5kAM8wAM/8AZgA"
    "AZjMAZmYAZpkAZswAZv8AmQAAmTMAmWYAmZkAmcwAmf8AzAAAzDMAzGYAzJkAzMwAzP8A/wAA/zMA/2YA/5kA/8wA//8zAAAzADMzAGYzAJ"
    "kzAMwzAP8zMwAzMzMzM2YzM5kzM8wzM/8zZgAzZjMzZmYzZpkzZswzZv8zmQAzmTMzmWYzmZkzmcwzmf8zzAAzzDMzzGYzzJkzzMwzzP8z/"
    "wAz/zMz/2Yz/5kz/8wz//9mAABmADNmAGZmAJlmAMxmAP9mMwBmMzNmM2ZmM5lmM8xmM/9mZgBmZjNmZmZmZplmZsxmZv9mmQBmmTNmmWZm"
    "mZlmmcxmmf9mzABmzDNmzGZmzJlmzMxmzP9m/wBm/zNm/2Zm/5lm/8xm//+ZAACZADOZAGaZAJmZAMyZAP+ZMwCZMzOZM2aZM5mZM8yZM/+"
    "ZZgCZZjOZZmaZZpmZZsyZZv+ZmQCZmTOZmWaZmZmZmcyZmf+ZzACZzDOZzGaZzJmZzMyZzP+Z/wCZ/zOZ/2aZ/5mZ/8yZ///MAADMADPMAG"
    "bMAJnMAMzMAP/MMwDMMzPMM2bMM5nMM8zMM//MZgDMZjPMZmbMZpnMZszMZv/MmQDMmTPMmWbMmZnMmczMmf/MzADMzDPMzGbMzJnMzMzMz"
    "P/M/wDM/zPM/2bM/5nM/8zM////AAD/ADP/AGb/AJn/AMz/AP//MwD/MzP/M2b/M5n/M8z/M///ZgD/ZjP/Zmb/Zpn/Zsz/Zv//mQD/mTP/"
    "mWb/mZn/mcz/mf//zAD/zDP/zGb/zJn/zMz/zP///wD//zP//2b//5n//8z///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAABlenwdAAABAHRSTlP//////////////////////////////////////////////////////////////////////////////////"
    "///////////////////////////////////////////////////////////////////////////////////////////////////////////"
    "//////////////////////////////////////////////////////////////////////////////////////////////////8AAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG8mZagAAAAlwSFlzAAAOxAAADsQBlSsOGwAAAGFJREFUOI3F07ENACEIhWH2YZ"
    "/bh33c5zmOno2nEEgMF//Ggq/QRKgGUQbgmQnomwGYfLEDJRRY4wj08S9ARNgFAG4CwaxEAEdg/Io+e97z0jNHJQLLrRNA9cG2OKqk3fRrV"
    "RsuILx9l6wAAAAASUVORK5CYII=";



static const char sort_table_js[] = "<script>"
    "var tb = document.getElementById('tb');"
    "var order = [1, 1, 1];"
    "function srt(sc, so) {"
        "if(so == 2){order[sc] = -1 * order[sc];}else{order[sc] = so;}"
        "so = order[sc];"
        "var tr = Array.prototype.slice.call(tb.rows, 0),"
        "tr = tr.sort(function (a, b) { "
            "var c1 = a.cells[sc], c2 = b.cells[sc],"
            "n1 = c1.getAttribute('name'), n2 = c2.getAttribute('name'), "
            "t1 = a.cells[2].getAttribute('name'), "
            "t2 = b.cells[2].getAttribute('name'); "
            "return so * (t1 < 0 && t2 >= 0 ? -1 : t2 < 0 && t1 >= 0 ? 1 : "
            "n1 ? parseInt(n2) - parseInt(n1) : "
            "c1.textContent.trim().localeCompare(c2.textContent.trim())); "
        "});"
        "for (var i = 0; i < tr.length; i++) tb.appendChild(tr[i]);"        
    "};"
   
    "window.onload = function() {"               
        "srt(0, 1);"
    "}"
    "</script>";

static const char upload_file_js[] = "<script>"
    "function getCookie(name) {"
    "var cookieArr=document.cookie.split(';');"
    "for(var i=0; i<cookieArr.length; i++){"
        "var cookiePair=cookieArr[i].split('=');"
        "if(name == cookiePair[0].trim()) {"
            "return decodeURIComponent(cookiePair[1]);"
    "}}return null;}"
    "var target;var name;var fileObj;let dropArea=document.getElementById('drop-area');"
    "let fupld=document.getElementById('fupld');"
    "var ctxMenu=document.getElementById('menu');"
    "var dldOpt=document.getElementById('downld');"
    "function browse_file(){fupld.click()}"
    "function hide_menu(){ctxMenu.style.display='none';}"
    ";['dragenter','dragover','dragleave','drop'].forEach(eventName=>{"
    "dropArea.addEventListener(eventName,preventDefaults,false)});"
    ";['dragenter','dragover'].forEach(eventName=>{dropArea.addEventListener(eventName,highlight,false)});"
    ";['dragleave','drop'].forEach(eventName=>{dropArea.addEventListener(eventName,unhighlight,false)});"
    "dropArea.addEventListener('drop',handleDrop,false);dropArea.addEventListener('click',browse_file,false);"
    "function preventDefaults(e){e.preventDefault();e.stopPropagation();}"
    "function highlight(e){dropArea.classList.add('highlight')}"
    "function unhighlight(e){dropArea.classList.remove('highlight')}"
    "function handleDrop(e){let dt=e.dataTransfer;fileObj=dt.files[0];uploadFile(fileObj);}\n"
    "function uploadFile(file){dropArea.innerHTML='Uploading <span class=\"animate\">'+file.name+'</span>';"
    "let url='/upload';let formData=new FormData();formData.append('session',getCookie('fssession'));formData.append('file',fileObj);\n"
    "fetch(url,{method:'POST',body:formData}).then(()=>{window.location.reload(false);})\n"
    ".catch(()=>{dropArea.innerHTML='Error uploading file.';console.log('Error uploading file.');})}\n"      
    "function submitFile(){fileObj=fupld.files[0];uploadFile(fileObj);}"
    "</script>";    

static const char main_style_css[] = "<style>"
    "body{font-family:Arial;background:#242424;color:#d19344;padding-top:2em;}"
    "table{min-width: 100ch;margin-top:10px;}"
    "th,td{text-align:left;padding-right:1em;font-family:monospace;}"
    "th a, td a{color:#d19344;text-decoration:none;}"   
    "td a{font-weight:bold;}"     
    "tr:nth-child(even){background-color:#343434;}"
    "td:nth-child(1){min-width:50ch;}"
    "td:nth-child(2){width:20ch;}"
    "td:nth-child(3){width:13ch;}"    
    "tr:hover{background-color:#444444;}"
    "thead tr:hover{background-color:transparent;}" 
    ".click{cursor: pointer;}"
    ".dirpath{padding:5px 50px;width:100%;margin:-10px;position:relative;color:#ccc;}"
    ".dirpath span{margin-left:40px;}"
    "#btn-up:hover{cursor:pointer;}"
    "#drop-area{border:2px dashed #2c2c2c;border-radius:20px;margin:11px 30px;padding:20px;max-width:70ch;}"
    "#drop-area.highlight{border-color:purple;}"
    "#fupldfrm{display:none;}"
    ".animate{animation:load 1.2s infinite 0s ease-in-out;animation-direction: alternate;}"
    "</style>";


static const char menu_html[] = "<menu id='menu'><ul>"      
    "<li onclick='archive();' id='archive_opt'>Compress</li>"
    "<li onclick='rename();'>Rename</li>"
    "<li onclick='del();'>Delete</li>"
    "</ul></menu>";

static const char menu_css[] = "<style>"
    "menu{display:none;z-index:100;position:absolute;margin:0;font-family:monospace;}"
    "ul{display:block;margin:0;padding:5px;border-radius:6px;"
    "background-color:#1E1E1E;color:#CECECE;list-style-type:none;opacity:0.9;}"
    "li{padding:6px 20px;border-radius:6px;}"
    "li:hover{background-color:#CECECE;color:#1E1E1E;cursor:default;opacity:1;}"
    "</style>";

static const char menu_js[] = "<script>"    
      "var ctxMenu=document.getElementById('menu');"
      "var archiveOpt=document.getElementById('archive_opt');"
      "function hide_menu(){ctxMenu.style.display='none';}"     
      "document.addEventListener('contextmenu',function(e){target=e.target.text;"
        "if(target){"      
            "ctxMenu.style.display='block';ctxMenu.style.left=(e.pageX-60)+'px';"
            "ctxMenu.style.top=(e.pageY-6)+'px';e.preventDefault();\n"
            "if(target.endsWith('/')){"
                "archiveOpt.style.display='block';"
            "}else{"
                "archiveOpt.style.display='none';"
            "}"
        "}"
        "else{hide_menu();}"
      "},false);\n"
      "document.addEventListener('click', function(){hide_menu();});"
      "ctxMenu.click(function(event){event.stopPropagation();});"
      "function request(cmd){var xhttp=new XMLHttpRequest();xhttp.open('GET', cmd, true);xhttp.send();"
      "xhttp.onreadystatechange=function(){"
        "if (xhttp.readyState == 4 && xhttp.status == 200){location.reload();"
        "}else if (xhttp.readyState == 4 && xhttp.status == 301){alert(xhttp.responseText);location.reload();}"
      "}}"
      "function execute(op){var cmd=window.location.href+'?'+op+'='+escape(target);if(op=='rename'){cmd+='&name='+name;}"
      "cmd+='&ts='+Date.now();request(cmd)}"
      "function archive(){execute('archive')}"
      "function del(){execute('delete')}\n"
      "function newdir(){target=prompt('Please input new folder name','New_dir');if(target!=null){execute('newdir')}}"
      "function rename(){name=prompt('Please input new name','New_name');if(name!=null){execute('rename')}}"      
      "</script>";  


static const char toolbar_css[] = "<style>"
    "#toolbar{width: 100vw;margin:0;padding:0 10px;display:flex;background-color:#dadada;color:#2c2c2c;position:fixed;top:0;left:0;}"
    ".tool-btn{margin: 2px;padding:0;opacity:0.8;cursor:pointer;text-decoration:none;}"
    ".tool-btn:hover{opacity:1;}"
    ".tool-btn img{width:24px;height:24px;}"
    "#loc-path{margin:1px 1px 1px 10px;font-size:24px;line-height:26px;padding:0;}"
    "</style>";

    

#endif
