### install
boost is needed for server <br><br>
emscripten is needed for client<br><br>
<br><br>
install instructions from here <br><br>
https://github.com/emscripten-core/emscripten.git<br><br>
https://github.com/boostorg/beast.git<br><br>
<br><br>

to ensure that emscripten is setup correctly, run <br><br>
```bash
echo $EMSDK
```
<br><br>
when running build.sh, provide the /path/to/boost<br><br>
```bash
./build.sh web /Users/my_user/boost
```
<br><br>
build.sh has 3 options: web, local, and clean
<br><br>
web is used for deployment and will expect a /var/www/html/ folder to exist,<br><br>
whereas local is for testing and will spawn a python http server from the build folder<br><br>

after running build.sh, run make.sh with the same argument - local or web.<br><br>

