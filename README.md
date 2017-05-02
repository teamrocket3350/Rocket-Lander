# Rocket-Lander

Don't forget to use K&R indent style!
https://en.wikipedia.org/wiki/Indent_style#K.26R

Breakdown:
------------------------------------------------
Enemy AI & Obstacles            | Ramon
------------------------------------------------
Upgrade Store                   | Patrick
------------------------------------------------
Movement                        | Nick
------------------------------------------------
Save/Load Progress/Levels (XML) | Abraham
------------------------------------------------
Art + UI + Sound                | Patrick
------------------------------------------------
Collision                       | Nick
------------------------------------------------
Objectives                      | Nick?
------------------------------------------------

Note to Nick:

Make sure project size is below 5MB


Notes to Abraham:

abrahamA.h:

  in aPlatform:
  
  x & y should have array size of 100
  
  "struct aPlatform {
    int x[100];
    int y[100];
};"

Change loadLevel prototype to
"levelData loadLevel(int);"

In abrahamA.cpp:

  Change loadLevel function definition to
  "levelData loadLevel(int levelNumber)"

The level doesn't need to be passed to it.
Just add 

   "levelData level;"

below "int parCount = 1;"



