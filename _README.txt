=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-= ROBOROBO! =-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=- 2008-2012 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
-=-=-=-= nicolas.bredeche(at)gmail.com -=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

latest version update: <launch roborobo! binary or check roborobo.cpp>
How to know about current version and target build? Launch "roborobo -v" in the console.

Check _INSTALL.TXT for installation guide.
Check _FAQ.TXT for trouble shooting.
Check this file for anything else.

==== DESCRIPTION ====

Roborobo! is a fast and simple 2D mobile robot simulator written in C++ based on a khepera/epuck model. It is targeted for fast single and multi-robots simulation (primary motivation is Evolutionary Robotics). 

Some random bits of information:
- Robot position and mouvement are real-valued (ie. precise)
- Collision is performed on a pixel-based information for fast speed (ie. fast but not precise)
- Robot-robot collision can be switched off (faster, less accurate)
- both GUI mode and batch mode are available. Note that batch mode is the fastest mode (gBatchMode=true, gVerbose=false).
- only one external dependencies: SDL library (multi-platform fast 2D library).
- most parameters are to be found in the config subdirectory (e.g. default.properties)
- full environment and robot specifications can set up directly with an image editor (check data directories). E.g. Robot proximity sensor positions are set up in the robot image. 
- the very first version of roborobo! can be found here: http://neuronik.free.fr/nemelith/?p=67 (it is quite different as it was first a game sort-of experience, eventhough i already had roborobo! in mind). An included html help file describes the overall idea behind parameterization through image files (some modifications may have occured since then).
- you can specify a properties file as command line option (see "./roborobo -h" for help)
 

==== CREDITS ====

roborobo!: 
 - main developper: nicolas.bredeche(at)lri.fr
 - contributor(s): Jean-Marc Montanier, Leo Cazenille
included contributions:
 - the Properties class from Evert Haasdijk (VU Amsterdam)
 - box-muller algorithm from Everett F. Carter Jr. (1994) - ref: http://www.taygeta.com/random/gaussian.html
 - other contribs are mentionned in the code (check: core/Misc.h and contrib/*)
 - past contributor(s): Pierre Delarboulas

=====================
=====================
==== QUICK START ====
=====================
=====================

================================
==== INTALLER'S QUICK START ====
================================

CHECK _INSTALL.TXT

=================================
==== DEVELOPER'S QUICK START ====
=================================


** QUICK OVERVIEW **

The philosophy is that in the standard case, the designer should be able to code the agent behavior (the ''Behavior Control Architecture'') and to access both the world and agents states in a seperate fashion (''World'' and ''Agent'' observers). The world and agents observers are run before the actual agent behavioral update, meaning that the observers do observe a ''stable'' snapshot of the environment. Then, agent behavioral update are performed in turn-based fashion, to deal with possible collision.


The only useful things to know in order to write your own evolutionary algorithms are:

Concepts and hints:
- WorldObserver: object dealing with world level, used for accessing state of the world, including all agents (methods: reset and step).
- AgentObserver: object dealing with agent level, used for accessing state of agent. All agent observers are called before behavior update (methods: reset and step)
	=> note that both observers object are really useful for monitoring, logging, computing fitness updates, managing off-line evolutionary optimization, etc.
- BehaviorControlArchitecture: object dealing with behavior at the robot level (useful methods: init and reset)
- RobotAgentWorldInterface:    implements the influence of robot dynamics (hardware and context-dependant), 
	- ie. map control architecture desired command to actual robot's internal update values (e.g. sensor/motor noise/defect, terrain slippery/ruggedness, etc.)
	- hints: this is where you should write your sensor/motor noise model
	- access robot WorldModel, static object.
	- by default: simply return unmodified control architecture output values (ie. no noise, standard environment)
- If you write new classes derived from the above:
	- references: check out roborobo.h to update the object references (ie. dont need to modify anything else).
	- includes  : check additional includes in RobotAgent.h (and possibly elsewhere)
- *WorldModel: object where the (robot) world model is stored. You may want to derive and extend it.
- From the *ControlArchitecture objects, you can access the world and robot(s) world-models. All the rest is simulator-specific implementation.
- of course, you may want to create a new properties file in the config sub-directory
- Lastly, dont forget to edit the "include/core/Config/config.h" file to ''parameterize'' your internal active class structure. You may also precise which properties file to load with command line option (see command line help with "-h" option).

About the simulation update method scheme:
	- AgentObserver is called N times per iterations (N = nb of agents)
	- WorldObserver is called once per iterations
	- Update method: turn-based, synchroneous and shuffle-ordered update method.
		- update method call sequence: WorldObserver => N_a * agentObservers => (N_e * energyPoints update) => N_a * agent steps => N_a agent moves
			- worldObserver is called *before* agentObservers, agentObservers are all called *before* agent.stepBehavior
			- environment physics and actual moving of agents are performed in a row *after* all agent.stepBehavior has been called
			- Agent position movements is performed using synchroneous position updates. Hence, solving collisions is robust to agent index ordering
		- the update ordering is shuffled for each new world iterations, ie. agent indexes are shuffled (to avoid update ordering nasty effects accross time)
			- a typical problem if shuffling is not done is that agents with low indexes always act and move first, hence a big survival advantage.
			- however, agentObservers and agent.stepBehavior are called in the same order (ie. no re-shuffling)
	- WorldModel: contains all information on on agent, ie. its representation of the outside world, including its own status (e.g. energy level).
		- in practical, neither controlarchitecture or observers should contain general agent-related information. All should be stored in the worldmodel.

The idea behing the ''Observers'':
	- WorldObserver 
		- WorldObserver is called once per world update, before anything else;
		- WorldObserver is typically used to prepare data for the world iterations
			- e.g.: updating energy points, global data, global logging, setting up current experimental setup, etc.
	- AgentObserver
		- AgentObserver is called once per agents, before updating controller and position
		- AgentObserver is typically used to manage various agent-related sub-routines such as radio broadcast, learning/adaptation, but also data logging
	- WorldObserver vs. AgentObserver:
		- The conceptual idea behing the two types of observers is:
			1 -	if:
					it is about the whole population and/or environment [e.g. switch between experimental setups or any ''god'' level actions]
					it is concerned with experimental constraints [e.g. set active/inactive agent depending on energy level]
				then:
					do it in WorldObserver
			2 - if:
					if it is about the agent
				then:
					do it in AgentObserver
			3 - HOWEVER, there may be ambiguous situations, especially if an agent-centered routines would highly benefits from factorization within the WorldObserver.
				- A typical example is broadcasting: N * broadcasting for each agent is N^2 ( - N), while only N*(N+1)/2 (- N) if it is centralized (as symmetry is naturaly handled)
				=> Now then, let's take into account two considerations:
					(a) the code written in roborobo is not intended to be directly transferred into another system (e.g. to a robot)
					(b) if the benefit of optimization is *truly* significant (e.g. order of magnitude), it should be of highest priority
				=> Now, the guideline:
					- if (b) is true and significant, then code should be written in WorldObserver (and a comment added to AgentObserver)
	- AgentObserver vs. ControlArchitecture
		- ControlArchitecture deals with acting in the environment (setting up motor control, updating the neural nets, etc.)
		- AgentObserver deals with the internal agent state (updating learning, performing optimization, etc.)
	

** QUICK START : SETTING UP YOUR PROJECT SPACE **

In a nutshell: Let's say you want to quickly start a project, what do you need? Here is a short step by step tutorial. Setting up your own project can be quickly achieved, follow the guide:

STEP 1. Use the createProjectFrom.sh script in the prj/ directory
  - this allows you to clone an existing project to start with. E.g. BasicProject provides the very basic needed to start

STEP 2. update the roborobo internal code
	To register a project within roboro, you must:
	 - create a new instance of ConfigurationLoader (.CPP/.H).
	    configuration loader files are in 'src/ext' :
	    start by duplicating an existing file. E.g.: BasicConfigurationLoader.cpp/h is a good start
		add/update the line at the beginning to match with your project name
			#if defined PRJ_PROJECT_NAME || !defined MODULAR
		dont forget to leave the #endif at the end of the file.
	    /!\ The Preprocessor variable 'PRJ_PROJECT_NAME' must be named as follows :
			If the project name is 'Project-Name', the resulting preprocessor variable will be 'PRJ_PROJECT_NAME'.
			If the project name is 'fOO-BaR_toTO', the resulting preprocessor variable will be 'PRJ_FOO_BAR_TOTO'.
			Only characters a-z A-Z and '-' and '_' are accepted for a project name.	
	 - update the file 'src/core/ConfigurationLoader.cpp' :
		in the method 'ConfigurationLoader::make_ConfigurationLoader', add a new 'if' block referring to your new configuration loader, as follow:
			#if defined PRJ_PROJECT_NAME || !defined MODULAR
				else if (configurationLoaderObjectName == "ProjectNameConfigurationLoader" )
				{
					return new ProjectNameConfigurationLoader();
				}
			#endif
			
STEP 3. update your building method (check _INSTALL.TXT for a reminder):
  - modular makefile method: use makefile-manager to activate your new project. Dont forget to activate AT LEAST one project (./makefile-manager -h). Then launch roborobo with the correct configuration file (./roborobo -h).
  - simple makefile method: nothing to do. just run 'make'
  - Xcode project: add the new files to your roborobo project

STEP 4. launching roborobo
  - at this point, you can already compile and launch ("./roborobo"), but your project wont be run unless you write a dedicated properties file.
  - in the 'config' directory, duplicate an existing properties file (e.g. 'default-mono.properties' or 'default-multi.properties') and update it.
  - to use it: "./roborobo -l config/mynewconfigfile.properties"

Now what? Here are some hints and entry points:

1. which files should I create/write?  The simplest way to start is to write you own controller, everything else can be left with default classes. If you followed the previous steps, Your newly created ConfigurationLoader file refers to methods and you should already have a new behavior class. Hence, you only have to update make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) function.
Let's take a closer at the items in the DefaultConfigurationLoader (src/ext/DefaultConfigurationLoader.cpp):
			make_WorldObserver(World* wm)	: [default is ok] access all of the world state, called inbetween two world updates.
			make_RobotAgentWorldModel() : [default is ok] - Sensori-motor (and more) agent interface with the world (e.g. getSensor and setMotor methods, etc.)
			make_AgentObserver(RobotAgentWorldModel* wm): [default is ok] - access an agent state, all agent observers are called once *before* behavior update.
			make_BehaviorControlArchitecture(RobotAgentWorldModel *wm) : [default is wall-avoidance] - "[!!] ENTRY POINT [!!]" - this is where you write your actual behavior code
			make_RobotAgentWorldInterface() : [default is ok] - implementation of robot dynamics (max. acceleration, etc.)

2. how can I modify simulation parameters? chech the properties file you created in the "config" sub-directory. Check roborobo command line option to launch roborobo with the correct properties file ("roborobo -l config/myConfigFile.propertie").

3. Where do i go from here? Check and learn from the other projects in the project directory.


** METHODOLOGY GUIDELINES **

- Any new project should start, *and be limited to*, a specific sub-directory in prj/, with both an include/ and src/ sub-directories. E.g. ./prj/MyProject/[include&src].
- Observers should be understood as "deals with everything that happens inbetween two lifetimes" while Behavior should be understood as "everything that happens during lifetime", disregarding if it is agent or expert viewpoint, ie. behavior class may also include some external environmental constraints (such as specific locomotion mode in some given zones that are not dependent of the robot motor decision). In this sense, the WorldModel classes are also more general than what is usually intended as it is mainly a container for all robot states, including those the actual robot controller may not access.
- In case you implement new functionalities, the ext sub-directory should be the place to write them. For user-specific code, i would suggest to add a new directory with you name (e.g. nicolas/, evert/, ...) to keep core/ext/contrib as simple as possible (ie. evolutionary robotics may not be the only future application of roborobo!) - Thus, avoid modification into the core of roborobo, so that it can be straightforwadly re-use in other contexts.
- When developping a new "fork" of roborobo, all configuration parameters should limited to the .properties file and the include/core/Config/config.h file. These two files along with your own instance if the class described just above should be enough to create new setups. It should be easy to switch from one experiment to another just by commenting blocks in the include/core/Config/config.h file (see config.h file for example).
- A last word: roborobo! is not exactly the paragon of Clean Coding philosophy as it was originaly, and still is, a single-coder-in-a-very-big-hurry short term project. While i tried to keep the code clean, there are several do-as-i-say-not-as-i-do features (lack of accessing methods, etc.). Hence, roborobo! may not be the best starting point for developping your homebrew robot simulator. However, it can still be pretty useful if you just intend to extend it for a particular application (e.g. a specific swarm mobile robotics environment and simulation).

- Minor comments: 
  - indentation assumes only tab character spacing, display is assumed to be 4 empty characters equivalent.


** Contents of directories **

- prj    : project locations, each project is indenpendant, and may be seen as a special instance of roborobo - parameterized through ConfigurationLoader and .properties file.
- core   : core roborobo! code. Contains the elementary elements for running roborobo.
- ext    : derived class and extensions for roborobo! code, which may be of global interest. It contains addition to roborobo core, including all code that can be factorised from projects (ie. re-usable code, such as e.g. neural nets, logger), as well as project specific code.
- contrib: imported code with useful features.
- log    : log files.
- data   : contains image files.
- config : contains properties files.


** Images description **

gAgentMaskFilename				: the agent as how it is displayed and used for collision.
gAgentSpecsFilename				: the agent specification regarding proximity sensor (location and orientation).
gForegroundImageFilename 		: the environment decorative image, ie. only used for displaying -- should more or less match environment image.
gEnvironmentImageFilename  		: the environment description image. 
									R component is unused
									G component is used to determine agent #G starting position (if more than one pixel with same G value, choose the most upper-left), 
									B component is used to determine object type.
									obstacle (and agent) Id ("type") are returned by proximity sensors. 0..255: #obstacle, 1000..1256..+++: #agent
									R=255 means "no obstacle", G=255 means nothing, ie.: pure white means "no obstacle"
									(!) use png to avoid pixel exact color loss
									note that the sensor beam lenght parameter is set in the config file.
gBackgroundImageFilename  		: this image is purely decorative. Change parralax factor for faster scrolling (if screen size is smaller that environment/foreground size).
gZoneImageFilename        		: this image specifies the "zone". It has two purposes:
									(a) The red component for each pixel determines the zone Id, which is sensed through the agent floor sensor
									(b) if caption display mode is on, a caption image may be displayed if exists (see below gZoneCaption*) -- value 00 is ignored.
gZoneCaptionPrefixFilename 		: image name prefix for caption (optional) - in display caption mode, corresponding image is displayed on screen whenever a robot is in a given zone.

** default images **

Some image files are available by default in the data subdirectory. Here's a quick summary (but you may add anything you like):
- agent*: a 32x32 robot image, with 8 sensors (khepera-like)
- miniagent*: a 5x5 robot image, with 8 sensors (sensor belt)

** EnergyPoint usage **
Description of a standard energy point is given in src/core/EnergyPoint.cpp. It is used in SwarmOnlineAgentObserver.cpp and EnergyWorldObserver.cpp.

Basically an energyPoint can be of two type : fixed or not. A fixed energy point will always re-appear at the same place on the map, while an unfixed energy point will always re-appear at a new random place on the map.
The differenciation is done at the creation of the energy point. If an id is given to the energy point, this one will be fixed. This id can be used to set the position of the fixed point. If one want to give the position of a fixed energy point, this position has to be given in the properties file :
energy[id].x = XX;
energy[id].y = XX;
If the position of a fixed point isn't described in the properties file, the initial position of this point will be choosen randomly. Still the energy point will always re-appear at its initial position.

Energy points could be displayed on the screen. In order to do this define the VisibleEnergyPoint property (in the propoerties file) to true. Beware, displaying energy points will slow down the simulator.

EnergyPoints can be inactivated (ie: no nore reachable by a robot). The function setInactivate() has just to be called. Inactivated EnergyPoints can be reactivated by calling the function setActivate(). If a respawn lag has been defined (time between the descativation and reactivation of an EnergyPoint), one can just call the step() function at each time step (this call will idealy done in a WorldObserver. This step function will take care to reactivate the EnergyPoint once the respawn lag is passed.

** Some conventions **

Other (important) conventions
- if agent_i proximity sensor meets agent_j, the object ID return value will be agent_j.id+1000. Values <1000 are used for object Id. (see also gEnvironmentImageFilename, above)


**Dev notes**
//Some notes on internal implementations - (i try to keep this list up-to-date, but you should double-check by browing though the code if anything seems "weird")//

- motor control: control architecture should set desired translation absolute speed (e.g. 2 pixels/steps) + desired rotational velocity (e.g. -13deg/steps)
- motor control: check values un Parameter file for maximum speed and maximum displacement/steps values (e.g. maximum update per steps for translation speed)
- sensor/motor noise model: should be implemented in RobotAgentWorldInterface*.cpp/h


===================
==== BENCHMARKING ====
===================


** Benchmarking **

[20120606] - BENCHMARK (updated)

IMPORTANT NOTE: As of June 6th 2012, a new collision method has been implemented. Speed is increased by ~x2.26 when using ''big'' robots, and ~x1.09 for ''small'' robots.

NEW BENCHMARKING RESULTS, DETAILED - 2012-06-06

Comparing:
- old collision method (active version before june 6th, 2012)
- new collision method, old mask (ie. mask corresponds to all the robot image)
- new collision method, new mask (ie. mask only represent to edges of the robot image - {mini}agent-mask-edgesOnly) -- the recommended option

Tested with:
- default-mas.properties  (100 mini-robots)
- default-mas-bigrobots.properties  (30 (big size) robots)

Options for running:
- no-display
- network is off
- ~10 successive fps values

Tested on:
- Macbook Air, 1.8Ghz intel Core i7, 4GO 1333Mhz DDR3

RESULTS:

- NEW collision method, EDGE-ONLY mask (recommended)
  - default-mas: 1216 fps
  - default-mas-bigrobots: 1023 fps

- NEW collision method, FULL mask
  - default-mas: 1235 fps
  - default-mas-bigrobots: 498 fps

- OLD collision method (deleted from 2012-6-6, for reference)
  - default-mas: 1123 fps
  - default-mas-bigrobots: 451 fps

SUMMARY:

The new collision method always outperforms the old one. Using edge-only masks is only relevant when using ''big'' robots (and dont change much with ''small'' robots). 

Details:
  - with ''small'' robots, the new collision method (using whatever masks) is x1.09 faster than the old one. Hence, it does not change much (this was expected).
  - with ''big'' robots, the new collision method (using edge-only mask) is **x2.26** faster that the old one. Hence, good news. 

Additional comments:
- The difference btw using edge-only or full mask for the new collision method is not significant.
- These conclusions of course hold for whatever number of robots you use, from 1 to +oo (linear relation wrt. number of robots).


=-= ------- =-=
=-= ------- =-=

[20091119] - BENCHMARK -- DEPRECATED as of 2012-06-06
(left here for information)

// display mode 2, with/without collision (note: no network, as it was not implemented at that time)
- test 1 : default.conf: one big robot
  - test a : collision-free
  - test b : collision
- test 2 : default-mas.conf: 106 small robots
  - test a : collision-free, no communication network
  - test b : collision, no communication network
  - test c : collision, with communication network

PC-Labo (dualcore2 3ghz, 4go, Nvidia GeForce 280 GTX) 
  - default.conf:     19230it/s   19230it/s    -
  - default-mas.conf: 1250it/s    1200it/s     1020it/s

Mac-Nicolas (dualcore2 2.9ghz, 4go, Nvidia 9400M or 9600M GT (no diff.))
  - default.conf:     13900it/s   13900it/s    -
  - default-mas.conf: 930it/s     915it/s      515it/s

Some comparaison (not exhaustive, just for info - no update with current version, data from 2009):
 single robot:
   Simbad: 15000 fps on PC-labo.
 multi-robots:
   Mason: env.1000it/s avec 106 agents on PC-labo



==================
======= MISC =======
==================


**List of future implementations**

Priority 1 (do it now!):
- (none)

Priority 2 (should do it, someday):
- sensor (range and orientation) and motor noise. (rate should be set by user in properties file)
-.makefile ability to compile only part of ext/prj files (projects independant wrt. compilation) [current: all projects are registered in ConfigurationLoader]

Priority 3 (maybe?)
-?convert into mm or cm. (well, it's a matter of conversion, e.g. 1 pix = 1 cm? depends on the ration envt size vs. robot size)
-?implement millisecond precision for creating the random seed (critical or not?) -- random seed is based on second precision, ie. two runs starting at the same second with identical properties/parameters are identical (may be relevant when using clusters, but also, maybe unlikely to happen).
- check fullscreen.
-.add controller library (MLP, ESN, ...)

=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 


**BUG TRACKER**

- 2010.03.??: solve the verbosing on the creation of robots in the selfRep mode
- 2010.04.??: placement of energy points away from any obstacles
-.2010.06.23: collision bug when user-control agent bump into other agent.
-.2010.06.17: sub-pixel translation value is ignored [solved]


=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 

