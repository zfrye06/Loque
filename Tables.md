
# Tables:

- User
 - UserId (Auto Increment Int)
 - Username (String)
 - Password (salted hash String)
 - isAdmin (bool)
 - levelsCompleted (bit flag)
 - Total play time (Int)
 - Total Score (Int)
- UserAssociations
 - ClassID
 - UserID
- Score Info
 - StudentId
 - Lvl (Int)
 - highScore (Int)
 - Time to completion (Int)
- MapAssociations
 - ClassID
 - MapID
- Map
 - MapID
 - Name
 - Category
 - FilePath
 - DataURL 






# Actions:


- Admin disables certain maps for their class 
- Admin enables certain maps for their class
- Admin enables certain maps for one student
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
