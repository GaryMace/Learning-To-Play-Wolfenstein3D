# Learning To Play Wolfenstein3D

An ongoing implementation of the NEAT (NeuroEvolution of Augmenting Topologies) algorithm designed by Kenneth O.Stanley & Risto Miikulainen that progressively learns to play Wolfenstein3D as part of my final year computer science project. The algorithm is implemented in C++ in order to accompany to Wolf4SDL source port which is also written in C++

## Reasons why using this old version of C++ is hell
- Array instantiation isn't allowed in header files
- for (Element e : elements) { ....} isn't allowed...you gotta use for (int i = 0; i < elements.size(); i++) {..}
- There's no std::to_string(double) method, you gotta use str = static_cast<std::ostringstream*>(&(std::ostringstream() << mydouble))->str();


## THINGS THAT TOOK WAY TOO LONG TO FIND!
- tilemap[MAPSIZE][MAPSIZE] stores a byte map representation of all the walls on a level. 0 for walkable free space for a player and anything else is a byte from the (word) mapsegs[MAPPLANES] list. This will serve as an input to doop :D. It took an hour and a half of searching to find the setter for this stupid thing... Who names a wall data array TILEMAP???????? WHAT'S WRONG WITH WALLMAP[][]???
- More fantastic naming, statobjlist[MAXSTATS] is an array storing static object struct references. These include things like ammo, guns, health, treasure etc..
- AGAIN ID LMAO REALLY??? objlist[MAXACTORS] is an array of all actors (enemies, also the player) structs on the current map. Seriously if you're gonna consider enemies as actors then call the bloody array actorlist or something, where did objlist even come from..
