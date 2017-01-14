# Learning To Play Wolfenstein3D

An ongoing implementation of the NEAT (NeuroEvolution of Augmenting Topologies) algorithm designed by Kenneth O.Stanley & Risto Miikulainen that progressively learns to play Wolfenstein3D as part of my final year computer science project. The algorithm is implemented in C++ in order to accompany to Wolf4SDL source port which is also written in C++

## Reasons why using this old version of C++ is hell
- Array instantiation isn't allowed in header files
- for (Element e : elements) { ....} isn't allowed...you gotta use for (int i = 0; i < elements.size(); i++) {..}
- for (int i = 0; i < 10; i++) std::cout << i << std::endl isn't allowed,,,, you HAVE to have the braces around it..
- There's no std::to_string(double) method, you gotta use str = static_cast<std::ostringstream*>(&(std::ostringstream() << mydouble))->str();
