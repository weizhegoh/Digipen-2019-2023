Game Concept:
Sundown is a 3D top-down puzzle games where the player plays as a duo scientist brothers who have accidentally transformed themselves into rats and trapped by the lab security system.
The game follows the journey of the two brothers, Tom and John, as they try to solve puzzle to escape the lab security and turn themselves back into humans.



Game Name: Sundown
Team Name: Epiphany

Team Members:
Tan Tong Wee, Spencer (BSGD) - Producer
Lee Jun Jie (RTIS) - Co-Producer
Leok Boon Siang (BAGD) - Lead Designer
Lee Liang Ping (RTIS) - Graphics Director
Tan Wei Liang, Kenric (RTIS) - Graphic Programmer
Chng Yong Quan (BSGD) - Editor Director
Goh Wei Zhe (BSGD) - Editor Programmer
Zhang Hong Yao, Lilie (BSGD) - Logic Programmer
Law Zhi Xuan (BSGD) - Tools Programmer
Neo Jian Sheng, Kevin (BSGD) - Sound Director

Lecturers:
Jonathan Kwek
Keng Jin Poh
Holger Liebnitz
Tomas Arce



Project Building:
To generate the project solution, follow the steps below to build the project solution:
Open SundownEpiphany -> Open Scripts -> Run GenerateDependencies.bat -> Run GenerateProjects.bat
A "Build" folder under SundownEpiphany will be generated containing the project solution.

To compile asset using the asset compiler, follow the steps below:
Open SundownEpiphany -> Put the assets into the "Assets" Folder -> Open Scripts -> Run GenerateAssets.bat



Project Configuration:
- Debug (Debug build with console logging and all memory tracking and tracy visualiser)
- Development (Developmental build with console logging, optimisation-enabled, and tracy visualiser)
- Release (Release build with maximum optimisation, without any logging and tracy visauliser)

Project Platform:
- OpenGL (Game build)
- OpenGLEditor (Editor build)
- Vulkan (Unused)
- VulkanEditor (Unused)



Custom Engine Editor
  Shortcut Keys
    Most of the editor features can be toggle through the graphic settings panel. Below are some of the shortcut keys for utilisation:
     'Arrow Keys' - to control the movement of the camera
     'Hold Right-click + WASD' - to control the movement of the camera
     'Left Ctrl + N' - to create new level scene file
     'Left Ctrl + O' - to open the scene file
     'Left Ctrl + S' - to save the scene file
     'Left Ctrl + Left Shift + S' - to save the scene file as
     'Left Ctrl + Z' - undo 
     'Left Ctrl + Y' - redo 
     'Left Ctrl + C' - copy entity/file
     'Left Ctrl + V' - paste entity/file
     'Left Ctrl + D' - duplicate entity
     'DELETE` - delete selected entity/file
     'W' - to translate the selected object
     'E' - to rotate the selected object
     'R' - to scale the selected object
     'Double click or more' - To select other entities

  Drag and drop
    Create entity - drag and drop animation/static model from asset panel or archetype panel to viewport
    Link entity(parent-child) - drag a entity to a desired parent
    Prefab - drag and drop prefab file from asset panel to prefab panel
    Prefab Instance - drag and drop from prefab panel to viewport
    Texture - drag and drop a dds file to an entity to apply the texture to the entity

  Changing of component data
    Click on an entity/prefab to edit its component

  Create Prefab
    Right-click on entity and select create prefab

  Play/Pause/Stop
    Press play button to run simulation
    Press pause button to pause simulation
    Press stop button to stop simulation

  Graphics setting
    Settings like show wire frame, debug drawing and shadow can be changed in graphics setting panel

  Performance data/visualizer
    Performance data/visualizer will be shown in setting panel

  Enabling/Disabling Panels
    Enable and disable panel from the window tab on the menu bar

  Gizmo Snapping
    Enable snapping by checking the box and set the desired snapping value

  Grid Panel
    Controls for the grid map and the tiles placement or deletion



Custom Engine Game Controls
Keyboard:
  Player Controls:
  W key - Walk in front
  S key - Hop backwards
  A key - Rotate direction to the left
  D key - Rotate direction to the right
  F key - Interact (Grow to push cart/Leave clone)
  TAB key - Preview goal

  Camera Controls:
  LEFT ARROW key - Rotate camera to the left
  RIGHT ARROW key - Rotate camera to the right

  Cutscene Controls:
  ENTER key - Skip
  SPACE key - Next scene

Controller:
  Player Controls:
  DPAD UP - Walk in front
  DPAD DOWN - Hop backwards
  DPAD LEFT - Rotate direction to the left
  DPAD RIGHT - Rotate direction to the right
  WEST BUTTON(X/Square) - Interact (Grow to push cart/Leave clone)
  NORTH BUTTON(Y/Triangle) - Preview goal

  Camera Controls:
  LEFT SHOULDER BUTTON(L1)  - Rotate camera to the left
  RIGHT SHOULDER BUTTON(R1) - Rotate camera to the right

  Cutscene Controls:
  SOUTH BUTTON(A/X) - Skip
  EAST BUTTON(B/Circle) - Next scene

Walkthrough Story:
After the mess John had caused to transform them into rats and fortuitously trap them into an alternate dimension by the lab security.
Both Tom and John has their own set of puzzles to solve in order to deactivate the seucirty system, and breakout of the dimension to transform themselves back into human.