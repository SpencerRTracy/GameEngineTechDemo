Spencer Tracy

CS 529 Game Engine Submission

CORE ENGINE FEATURES:

Basic Debug Drawing
By pressing the key 'U' in game, debug drawing becomes activated.  All active game objects become 
surrounded by their collision volumes.  All active game objects also have their direction of travel 
denoted by a green line from their center.  The length of this line is the speed at which the 
object is travelling.  ('GameLogic/GameLogic.cpp' Line: 805 DebugDraw() method)

Basic C++ Objects
Every .cpp file in the Visual Studio's Solution Explorer is a C++ class with the exception of the 
Project Explorer's 'Utils' folder.  The components are an example of C++ objects ('EngineComponents').

Basic Events
An event manager component is in the game engine ('EngineComponents/EventManager/EventManager.cpp').
This event manager handles all routing of messages between components.

Basic Text Serialization
Text Serialization is done by the ObjectCreator class ('EngineComponents/ObjectCreator/ObjectCreator.cpp' Line:23 CreateGameObject() method ).
All base game objects are defined in text files located in the 'GameEngine/GameObjects' folder.  
All objects that are going to be in the game derive from these bases only.

Rendering Textured Quads
Textured quads are used for the galaxy background, Pyramid mini boss, and others.

Basic Sprite Transformation
Transformation values are stored in the GameObject class ('EngineComponents/GameObject/GameObject.h'  Lines: 102-120).  
Once the object is ready to be rendered, ObjectModel creates transformation matrix to apply using the GameObject's values 
('EngineComponents/GameObject/GameObjectModel/ObjectModel.cpp' Lines: 26-30)

Basic Collision Detection Between Circles
The Asteroids in the game collide with each other.  ('EngineComponents/Physics/Physics.cpp Line: 85)

Basic Collision Response
The Asteroids in the game slide around each other in a delayed bounce fashion.  Originally I had a 
pure bounce in mind for the asteroids, but since asteroids are supposed to be massive, them bouncing 
off each other like ping pong balls did not seem appropriate.  They currently slide off each other when 
colliding to imitate conservation of momentum in a way. ('EngineComponents/Physics/Physics.cpp Line: 85 - 200)

Human Player Control of a Ship
The human player uses W,A,S,D to control the ship in the game.

Collision Between Projectiles, Player, and Enemies
Bullets colliding with asteroids hurt the asteroid and destroy the bullet.
Missiles colliding with asteroids have a similar effect.
Asteroids, Fire Asteroids, or Pyramid mini bosses colliding with the player kill the player.
('GameLogic/GameLogic.cpp' Line: 561 HandleCollision() method)

Controls Screen
The first screen when launching the game is a controls screen.  Pressing space on this screen starts the game.
('GameLogic/GameLogic.cpp' Lines: 955 - 992)

Win/Lose Condition
Level Manager is in 'GameLogic/GameLogic.cpp Line 890 NextLevel() method'
	Win Condition:
		The game has 5 levels.  Once level 5 is beat, the game proceeds to a victory screen, signifying a win.
		Levels can be skipped using the 'P' key
	Lose Condition:
		The player starts with 5 lives.  If all lives are lost the game ends.




----------------------------------------------

ADDITIONAL ENGINE FEATURES:

Advanced Debug Drawing
By pressing the key 'U' in-game, debug drawing becomes activated.  Text is used to display all active 
game objects' positions in the grid space. ('GameLogic/GameLogic.cpp' Line: 805 DebugDraw() method)

Real-Time Debug Drawing
Debug drawing does not require a recompile and can be activated at any time by pressing the 'U' key. 
('GameLogic/GameLogic.cpp' Line: 805 DebugDraw() method)

Component Based Design
All systems are split up into unique independent components in the Solution Explorer's 'EngineComponents' folder.
('EngineComponents/MainEngine/MainEngine.cpp')

Input Manager
An Input Manager is used to handle any key press (not just those enumerated explicitly) using a hash map.
('EngineComponents/InputManager/InputManager.cpp')

Game Object IDs
Every time a GameObject gets created, it gets a unique ID to identify it from other instances of 
the same type of game object.  This is done through the GameObject class's static instance counter 
that increments every time GameObject gets instantiated.
('EngineComponents/GameObject/GameObject.h' Line: 35)

Creation of Objects from Data Files
The text serialization from files creates a new GameObject type and fills it with the information 
read from the file.
('EngineComponents/ObjectCreator/ObjectCreator.cpp' Line:23 CreateGameObject() method )

Sprite Z Sorting
All sprites have a layer value on them that changes their sorting for draw calls based on z-value control.
('EngineComponents/GameObject/GameObject.h' Line: 53)

Collision Between Different Body Types
The collision between the Ship and the Asteroids is a Rectangle to Sphere collision.
('EngineComponents/Physics/Physics.cpp Line: 61)  Also, the Math2D library c file contains 
functions that handle a variety of type1 to type2 collision detections.  This is in the 
Project Explorer's 'EngineComponents/Utils' folder.

Type of Enemy (Asteroid)
The Asteroid is an enemy that spawns at the beginning of each level.  Each time it is hit it shrinks 
in size, until eventually it disappears.

Type of Enemy (Pyramid Mini Boss)
Pyramid Mini Boss is a fast enemy that shows up randomly, with a higher chance the higher the level.

Type of Enemy (Lava Asteroid)
Lava Asteroid is an enemy that only spawns when the player collides with a normal asteroid.  The collision 
response is to create a Fire Asteroid that has double the speed of the asteroid that was hit.

Type of Weapon (Bullet)
Pressing the Space Bar shoots a bullet in the direction the ship is facing.

Type of Weapon (Homing Missile)
Pressing the 'F' key shoots homing missiles that track to the nearest asteroid.  The speed and turn 
radius is small, but as a trade off they are unlimited as long as there is only 3 out at a time.  An 
indicator of the current number of missiles currently deployed is located on the player ship.

