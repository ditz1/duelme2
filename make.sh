if [ $# -eq 0 ]; then
    sudo rm /var/www/html/index.* 
    cd client/build
    make
    sudo cp index.* /var/www/html
    cd ../../server/build/
    make
    ./program 192.168.1.42
elif [ "$1" = "local" ]; then
    cd client/build
    make
    kill $(lsof -t -i:8080)
    python3 -m http.server 8080 &
    cd ../../server/build/
    make
    ./program 127.0.0.1
elif [ "$1" = "debug" ]; then
    echo "running on LOCAL"
    cd client/build
    make
    kill $(lsof -t -i:8080)
    python3 -m http.server 8080 &
    cd ../../server/build/
    make
    lldb ./program 127.0.0.1
elif [ "$1" = "app" ]; then
    echo "attempting to build standlone application"
    cd client/build
    make
    kill $(lsof -t -i:8080)
    cd ../../server/build/
    make
    ./program 127.0.0.1
fi