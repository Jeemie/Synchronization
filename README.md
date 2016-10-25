# Synchronization

In Project 3A, every student designed and proposed a synchronization problem that could be selected as the
synchronization problem for the class. Two of these problems were selected as candidates for the entire class
to implement in Project 3B. The details of the problems follow. You must select one of the two problems
and solve it using both of the following two different ways:
1. using mutexes and condition variables (e.g., pthread mutex lock, pthread mutex trylock,
pthread mutex unlock, pthread cond wait, and pthread cond signal)
2. using semaphores (e.g., sem wait and sem post)
By using two different synchronization methods to solve the same problem, you will likely realize that
the different methods actually provide the same functionality albeit with different syntax. Students must
solve the same problem using the two different primitive categories. Students should only work on one of the
two problems. No extra credit will be given for solving both problems. Further, students may not choose to
solve Problem 1 using one approach and Problem 2 using a different approach.
You must create threads for each of your actors. Each thread must use a degree of randomness for
your scheduling; however, you may want to use the same seed value for your randomness initially to create
reproducible code for debugging purposes.

# Problem 1: Autonomous Aircraft Landing
This project will explore autonomous aircraft that can safely and efficiently land at an airport without the
need for any Air Traffic Controllers.
# Need:

Air traffic controllers in the United States have one of the most stressful jobs,sometimes managing hundreds of planes during their shifts that all want to occupy a limited number of runways. This stress adds up, especially during the night shift. A recent article from NBC cites a 2011 report by the Federal Aviation Information that says in 2011 about two in ten controllers committed significant errors. It takes just one, tiny loss of focus to send two planes crashing in to one another, or to cause major delays. We need a way to
eliminate these human shortcomings in order to save and protect lives.

# Approach: 
We will eliminate human error by replacing human air traffic controllers with an automated
system aboard each plane, that will coordinate with other planes to provide the safe and efficient use of
limited runways. The planes will have access to a shared data buffer, which will consist of shared data fields
and structures. A semaphore-based approach and a mutex and condition variable-based approach will be
used to implement this system. For each runway that is available, mutual exclusion will be maintained, and
use of the runways will be as efficient as possible, while avoiding accidents and planes running out of fuel
while still in the air.

# Benefit: 
Not only will this automated system result in fewer near-misses, accidents, and delays, there will
also be a benefit for the environment, as planes will be directed in and out of the airport in the most efficient
manner. This approach will also be more financially viable, since the need for air traffic controllers will be
eliminated, and maintenance of the system can be performed along with the regular maintenance for the
aircraft. We will ensure that no plane never gets to land.
Competition: Our competition really only includes a centralized approach (air traffic controllers in an air
traffic control tower). We are better than this centralized approach because we ensure maximum parallelism
and flow of traffic coming into the airport, and we also better avoid accidents because we eliminate human
error.

For the purposes of this problem, we will consider only incoming air traffic. There will be a fixed number
of runways (R), and multiple planes waiting to land (P), where P >R, and there is one thread for each plane.
A plane cannot land in any occupied runway, and there is a fixed amount of time that must pass for each
plane that is currently occupying a runway to clear it and free up the runway. Also, each plane has a random
amount of fuel left when it enters the airspace around the airport, and uses up fuel at a fixed rate while
waiting to land. Once a plane has a certain amount of fuel left, it enters the “danger zone” and must land
within a certain time frame or it will crash. If a plane starts to land, another cannot land in front of it. The
amount of time required for the landing process should randomly vary in a fixed range for each plane. You
cannot change the number of runways, remaining fuel of any plane, or reduce the amount of time it takes a
plane to clear the runway.

When running your solution, each plane must announce: 1) when it arrives and wants to land, 2) when
the plane begins to land, 3) when it touches down on the runway, and 4) when it clears the runway. In each
stage, the plane must announce its fuel remaining. In stage 2-4, the plane must indicate its targeted runway.
If a plane enters the danger zone, it must create an announcement and indicate its remaining fuel. Finally,
if a plane runs out of fuel, it must indicate that it crashed and the simulation program should immediately
halt.

For the simulation, the airport will have 3 runways and 25 plane threads. The planes must enter the
airports airspace at a random time, but often enough so that they will compete for runways. Each plane
will start with a random amount of remaining fuel.

Occasionally, a plane may need to make an emergency landing due to equipment issues. When a plane
declares an emergency, all other planes will be blocked from beginning a landing. The plane with the
emergency must be the first to land at the first available runway. All planes that are already in descent
will continue to land and will clear the runways. Once the plane with the emergency lands and clears the
runway, normal operation will resume and planes may again begin landing. You may designate the plane
(or planes) with emergency landings in advance (e.g., when the thread is created), but it must be randomly
scheduled with the other planes.

None of the planes can be starved, parallelism should be maximized where possible (to minimize delays),
and the approach should be fair, where possible.
