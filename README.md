Charmington was the game my team submitted for the 3 day game jam, MelonJam Winter 2024 - https://itch.io/jam/winter-melonjam-2023

Charmington was built on the engine that I've been making over the last 2ish years. https://github.com/GDBobby/Eight-Winds-Engine

### The Team Included :
MaxBarken - https://github.com/maxbarken

AccusedMink - https://github.com/nastya2010gon - https://acussedmink.itch.io/



Max was in charge of music.
AccusedMink did models, and I believe one animation that made it into the final build.

I hadn't worked with models made in Maya before, and so unfortunately I was unable to import AccusedMink's animations to the engine. This is an issue I'll need to address in the future.

### Final Jam Commit
https://github.com/GDBobby/MelonJam/commit/84227eea82b98db22ead4ddca8e51bc134a0d5d9

Collaboration with Max and AccusedMink ended as of the jam's final commit. It was a pleasure working with both of them.

### Moving forward
Currently, I'm working on bringing back some features I created for the jam as permanent engine features. 

The current step for that is creating an in-engine tile based level editor, as I'm not a big fan of how Tiled does things. 
I believe it would be more pertinent to create my own version rather than make a Tiled branch, but maybe hindsight will see it differently.


In particular, I'm going to be changing the save format to a binary format instead of a json format. 
One additional bit is being reserved for checking if a tile is void, instead of shifting the values for all tile IDs. This does reduce the amount of available tiles slots to 268 million. Hardly manageable, I know.


One of the main reasons for implementing an in-engine level builder as opposed to branching Tiled, is I'd like to be able to do runtime level editting.
I haven't reached this point yet, but I'd also like an additional data layer for defining level exits, destinations, entrances, and entrance points.
I'd also like to be able to tile larger areas with seamless level transitions.
