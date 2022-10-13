Welcome to Chiron!

This is a slightly more sophisticated 2D Physics Engine as compared to my previous one made in Java. It is quite a bit more efficient as it makes use of
dot products rather than trigonometric functions to detect vertex projection overlap between polygon pairs (this is specifically for the Hyperplane-Separation Theorem algorithm, also known as the Separation-Axis Theorem (SAT/HST), and also introduces the GJK (Gilbert ohnathan-Keerthi) algorithm for collision detection.

Controls:

; - Pause (you must pause before drawing polygons). \n
P - While paused, Draws a point for a polygon vertex. \n
Enter - While paused, Unpauses and adds the polygon to the engine. \n
V - While unpaused, deletes the last drawn polygon. \n
C - While unpaused, Clears the console window. \n
C - While paused, adds a circle (approximated with 20 edges). \n
D - While paused, deletes the last drawn polygon vertex. \n
Esc - While unpaused, deletes all polygons from the engine. \n
