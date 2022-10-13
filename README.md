Welcome to Chiron!

This is a slightly more sophisticated 2D Physics Engine as compared to my previous one made in Java. It is quite a bit more efficient as it makes use of
dot products rather than trigonometric functions to detect vertex projection overlap between polygon pairs (this is specifically for the Hyperplane-Separation Theorem algorithm, also known as the Separation-Axis Theorem (SAT/HST), and also introduces the GJK (Gilbert ohnathan-Keerthi) algorithm for collision detection.

(Note, you can only add one shape at a time. Once you have drawn a convex shape, press enter)

Controls while paused:

Enter - While paused, Unpauses and adds the polygon to the engine.

C - While paused, draws a circle (approximated with 26 edges).

D - While paused, deletes the last drawn polygon vertex.

P - While paused, draws a point for a polygon vertex.

S - While paused, draws a square.

Controls while unpaused:

; - Pause (you must pause before drawing polygons).

V - While unpaused, deletes the last drawn polygon. 

C - While unpaused, clears the console window.

Esc - While unpaused, deletes all polygons from the engine.
