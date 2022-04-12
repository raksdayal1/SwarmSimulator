#!/usr/bin/env python

import sys
import numpy as np
import nvector as nv
import navpy

class Frame:
    def __init__(self):
        self.wgs84 = nv.FrameE(name='WGS84')

    def addRefLLA(self, LLAref):
        self.LLAref = LLAref
        self.pointref = self.wgs84.GeoPoint(latitude=self.LLAref[0], longitude=self.LLAref[1], z=self.LLAref[2], degrees=True)

        # pass in a 3XN numpy matrix containing N LLA points
    def ConvLLA2NED(self, LLA_mat):

        LLA_ned = np.zeros(LLA_mat.shape)

        for i in range(LLA_mat.shape[1]):
            point = self.wgs84.GeoPoint(latitude=LLA_mat[0,i], longitude=LLA_mat[1,i], z=LLA_mat[2,i], degrees=True)

            p_AB_E = nv.diff_positions(self.pointref, point)
            frame_N = nv.FrameN(self.pointref)
            p_AB_N = p_AB_E.change_frame(frame_N)
            p_AB_N = p_AB_N.pvector.ravel()

            LLA_ned[:,i] = p_AB_N

        return LLA_ned

    def ConvNED2LLA(self, NED):
        # NED should be 1X3 array
        lat, lon, alt = navpy.ned2lla(NED, self.LLAref[0], self.LLAref[1], self.LLAref[2])
        return lat,lon, alt