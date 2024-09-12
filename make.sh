sudo rm /var/www/html/index.* 
cd client/build
make
sudo cp index.* /var/www/html
cd ../../server/build/
make
./program