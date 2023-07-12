# vc7proof
This program is intend to verify that all of the visibilities are correct in limited visibility terrain.

How to execute:

chmod +x ./run.sh

./run.sh

Note that input.txt stores the coordinates of guards and viewpoints, where A, B, C, ..., G represents g_1, g_2, g_3, ..., g_7, and rest of points are viewpoints (e.g. CEG represents vp_3,5,7). We omitted the viewpoint sees empty set and the viewpoint only see one guard, which can be realiazable easily.

The vc7coord.cpp will read the input.txt and verify every viewpoint sees the guards that suppose to see and the guards that not suppose to see. If this viewpoint sees the guard, then the distance between the viewpoint and the guard must not greater than rho, and there is no such point between this viewpoint and the guard above the line segment of this viewpoint and the guard.
