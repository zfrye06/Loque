
#Running the Server

```
cmake .

make

./server
```
# Viewing the Database

1. Download mySQL Community Server: https://dev.mysql.com/downloads/mysql/
2. When it asks you what you would like to install make sure mySQL workbench is selected.
3. After installing open mySQL workbench and add a connection.
4. Name the connection whatever you want.
5. Hostname: bernardcosgriff.com, Port: 3306, Username: teamaccess, Password: password
6. You should now be able to see the tables I created and make new ones if necessary.

Install mySQL and mySQL Connector
MySQL for ubuntu was:
```
$ sudo apt-get install mysql-server
```
MySQL Connector for ubuntu was:
```
$ apt-get install libmysqlcppconn-dev
```

# Schema

Please view the database in MySQL Workbench.

# Supported Actions:

- Admin disables certain maps for their class 
- Admin enables certain maps for their class
- Student completes map, generating score info
- Game menu displays which maps are enabled and which are disabled (by category?)
- Admin retrieves aggregate statistics for all students in their class 
- Total scores for all students
- Total time played for all students
- Highest level completed by any student in the class
- Average per-level score for students in class
- Highest scoring student for each level and his/her score
 -  Admin retrieves students’  stats for particular student in their class
  - total score
 - total time played
  - levels completed
  - highest score per level completed
  - best time taken per level completed
  - total time taken per level completed
 -  Student retrieves information about himself (note how similar this is to above!)
  - total score
  - total time played
  - levels completed
  - highest score per level completed
  - best time taken per level completed
  - total time taken per level completed
