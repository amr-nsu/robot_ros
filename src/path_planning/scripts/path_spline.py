import numpy as np
from math import hypot
from scipy import interpolate


class Path_Spline:

    def __init__(self, xp, yp, speed):
        self.xp = np.array(xp, dtype=float)
        self.yp = np.array(yp, dtype=float)
        self.speed = speed
        self.speed_normalize = speed
        self.position = 0.
        self.current_point_index = -1
        self.is_init = False

    def __nonzero__(self):
        return self.is_init

    def clear(self):
        self.__init__(xp=(), yp=(), speed=self.speed_normalize)

    def insert_point(self, point_index, (x, y)):
        self.xp = np.insert(self.xp, point_index, x)
        self.yp = np.insert(self.yp, point_index, y)

    def delete_point(self, point_index):
        self.xp = np.delete(self.xp, point_index)
        self.yp = np.delete(self.yp, point_index)

    def init_spline(self):

        def path_len(n):
            s = 0
            for i in xrange(n):
                s += hypot(self.xp[i+1] - self.xp[i], self.yp[i+1] - self.yp[i])
            return s

        delta_len = path_len(len(self.xp)-1) / path_len(len(self.xp)-2)

        self.speed = self.speed_normalize / path_len(len(self.xp)-1)
        self.position /= delta_len

        self.tck, _u = interpolate.splprep([self.xp, self.yp], s=0.0)
        self.is_init = True

    def get_path_point(self, position):
        x0,y0 = interpolate.splev(position, self.tck)
        return x0, y0

    def get_target_point(self, dt):
        """position from 0 to 1"""
        self.position += self.speed * dt
        if self.position > 1:
            self.position = 1
        x0,y0 = interpolate.splev(self.position, self.tck)
        dx0,dy0 = interpolate.splev(self.position, self.tck, der=1)
        return x0, y0, dx0, dy0

