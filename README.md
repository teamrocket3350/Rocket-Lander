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

In file included from rocketlander.cpp:23:0:
nicholasP.h:110:16: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
   float fuel = 100;
                ^
                
nicholasP.h:111:19: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
   float fuelMax = 100;
                   ^
                   
rocketlander.cpp:151:36: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
  Goal goal = Goal(950, 740, 100, 10);
                                    ^
                                    
In file included from nicholasP.cpp:26:0:
nicholasP.h:110:16: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
   float fuel = 100;
                ^
                
nicholasP.h:111:19: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
   float fuelMax = 100;
                   ^
                   
In file included from abrahamA.h:18:0,
                 from abrahamA.cpp:18:
nicholasP.h:110:16: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
   float fuel = 100;
                ^
                
nicholasP.h:111:19: warning: non-static data member initializers only available with -std=c++11 or -std=gnu++11
   float fuelMax = 100;
