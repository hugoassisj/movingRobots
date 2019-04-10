[![HitCount](http://hits.dwyl.io/hugoassisj/movingRobots.svg)](http://hits.dwyl.io/hugoassisj/movingRobots)
![Downloads](https://img.shields.io/github/downloads/hugoassisj/movingRobots/total.svg?style=flat-square)
# movingRobots

Threads concurrency and synchronization example, written in C++ using Qt.

# Instructions

Consider a system that receives the positions of three robots, which move in a room, from three different sources (for example: IMU, GPS and Odometry).

These sources spend a time between 100 ms and 4000 ms to capture the position of each robot, and then insert this information concurrently in a buffer of finite size N.

After the system receives the position of a robot from all three sources, it must process the average position and insert the result into a data structure that stores the position of each robot separately. Finally, the system must present the positions of the robots in a graphical user interface.

![asd](https://user-images.githubusercontent.com/45035051/48899050-9a0caf00-ee35-11e8-8cc5-0deca1e4bf04.png)

The goal is to create a C ++ program using threads and semaphores that allow the capture, processing, and presentation of each robot's movement to occur concurrently and synchronously.

The initial position of the robots can be generated randomly. In order to generate their trajectory, consider that each new position consists of the previous position plus a random value in a given interval ( -P < Position < P), for each component (x and y).

# Required

	ubuntu
	QtCreator

##  Install QtCreator
Open terminal and run:

```
sudo apt-get install build-essential

sudo apt-get install qtcreator

sudo apt-get install qt5-default
```

## Open Project

Open terminal and run:
  
```
qtcreator 
```
  
After that, select Open Project and then the _*.PRO_ file.
  

