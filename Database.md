# How to view the Database
1. Download mySQL Community Server: https://dev.mysql.com/downloads/mysql/
2. When it asks you what you would like to install make sure mySQL workbench is selected.
3. After installing open mySQL workbench and add a connection.
4. Name the connection whatever you want.
5. Hostname: bernardcosgriff.com, Port: 3306, Username: teamaccess, Password: password
6. You should now be able to see the tables I created and make new ones if necessary.

# Tables

## User
- UserId: userID
 - Int
 - Auto increment
 - Non-nullable
 - Primary key
- Username: username
 - String
 - Non-nullable
- Password: password
 - String 
 - Salted hash
 - Non-nullable
- isAdmin: isAdmin
 - Tinyint(bool)
 - Non-nullable 
- levelsCompleted: levelsCompleted
 - Varbinary(bit flag)
 - Non-nullable 
- Total Score: totalScore
 - Int
 - Non-nullable
- Total play time: totalTime
 - Int
 - Non-nullable
 
## UserAssociations
- UserID: userID
 - Int
 - Non-nullable
- ClassID: classID
 - Int
 - Non-nullable
 
## Score Info
- UserId: userID
 - Int
 - Non-nullable
- Map: mapID
 - Int
 - Non-nullable
- highScore: levelScore
 - Int
 - Non-nullable
- Time to completion: completionTime
 - Int
 - Non-nullable
 
## MapAssociations
- ClassID: classID
 - Int
 - Non-nullable
- MapID: mapID
 - Int
 - Non-nullable

## Map
- MapID: mapID
 - Int
 - Primary key
 - Non-nullable
- Name: mapName
 - String
 - Non-nullable
 - Unique
- Category: category
 - String
- FilePath: path
 - String
 - Non-nullable
 - Unique
- DataURL: url
 - String

# Actions:


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


# Launcher Responsibilities:
- Provide you with a username/password field :)
- Allow you to create a student account.
- Send the login information to the server in the form of a SQL query
- See if there’s an associated account. 
- If it’s an admin account, give them the choice of either playing the game (libsfml process) or opening the admin portal (Qt app)
- If it’s a user account, launch the game, passing the UserId as the only argument
