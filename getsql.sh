#! bash

sudo mkdir -p /var/run/mysqld
sudo chown mysql /var/run/mysqld/
sudo service mysql restart