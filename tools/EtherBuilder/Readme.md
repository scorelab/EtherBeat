# Etherbeat Builder

Tool to store ethereum blockchain in order to support analysis. SQLite database and Rocksdb has been used.

### SQLite Setup

install SQLite3 in your environment

create directory for sqlite database

`mkdir /tmp/dbsqlite`

### Rocksdb Setup

download the latest rocksdb source code from github

`git clone https://github.com/facebook/rocksdb.git`

go into rocksdb folder

`cd rocksdb`

build

`CXXFLAGS="-Wno-error" DEBUG_LEVEL=0 make shared_lib install-shared`

set to LD_LIBRARY_PATH (usually rocksdb installed in /usr/lib but some environments like arch it's installed in /usr/local/lib)

`export LD_LIBRARY_PATH=/usr/lib`

create a directory for rocksdb

`mkdir /tmp/dbrocks`


### Running Builder

`./run.sh`