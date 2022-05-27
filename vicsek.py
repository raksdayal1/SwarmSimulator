# Vicsek Position and Orientation Update Script
# POC: Christian Griffith
# Email: christian.griffith@okstate.edu
# 5/19/2022

# Objective: Updates the position and orientation of an agent based upon the Vicsek swarm model.
# Reference: "Novel Type of Phase Transition in a System of Self-Driven Particles" by Vicsek et. al. (1995)


import numpy as np
import matplotlib.pyplot as plt
import scipy.spatial.distance as dist


# Parameters
TMAX = 300      # Maximum simulation time
L = 500         # Domain size
noise = 0.01    # Noise
N = 25          # Number of Agents
r = 100         # Interacting radius
vel = 1         # Absolute velocity
dt = 1          # Time step

# Starting Location Spread
SSpd = 0.5
SPos = (L/2)-(0.5*SSpd*L)

# Initial Conditions
x = SSpd * L * np.random.rand(N) + SPos
y = SSpd * L * np.random.rand(N) + SPos
#x = L * np.random.rand(N)
#y = L * np.random.rand(N)
theta = 2 * np.pi * (np.random.rand(N) - 0.5)
avgTheta = np.zeros(N)

# Initial Condition Plot (if interested)
# plt.scatter(x, y)
# plt.show()

# Properly shape location data into arrays (matrix)
tempArr = np.array([[x], [y]])
shapeLoc = np.reshape(tempArr, (2, -1))
agentLoc = np.transpose(shapeLoc)

# For Loop to update at every iteration
for i in range(TMAX):
    # Calculate average distance between agents
    D = dist.pdist(agentLoc, 'euclidean')           # Euclidean distance between points (vector)
    M = dist.squareform(D)                          # Matrix form of distance vector

    # List of agents within range of interacting radius
    agentInRange = np.where(np.logical_and(M > 0, M < r))
    rowInd = agentInRange[0]
    colInd = agentInRange[1]

    # Calculate average direction
    for j in range(N):
        agentList = colInd[rowInd == j]     # List of agent coordinates that are in range of each other

        # If list of agents is not empty, take an average of direction. If empty, don't change agent direction.
        if len(agentList) > 0:
            angles = np.zeros(len(agentList))

            for k in range(len(agentList)):         # Loop for averaging multiple agent directions
                angles[k] = theta[agentList[k]]

            avgTheta[j] = np.arctan2(np.mean(np.sin(angles)), np.mean(np.cos(angles)))      # Average Direction Angle
        else:
            avgTheta[j] = theta[j]

    # Update Position and Orientation
    x = x + vel * np.cos(theta) * dt
    y = y + vel * np.sin(theta) * dt
    theta = avgTheta + noise * (np.random.rand(N) - 0.5)

    # Plotting agents at each iteration
    plt.scatter(x, y)
    plt.xlim([0, L])
    plt.ylim([0, L])
    plt.pause(0.06)
    plt.cla()
plt.show()


