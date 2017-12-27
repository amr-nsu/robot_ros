class Targets:

    class Target:
        def __init__(self):
            self.x = None
            self.y = None

        def set(self, x, y):
            self.x = round(x, 2)
            self.y = round(y, 2)

        def __nonzero__(self):
            return None not in (self.x, self.y)

    def __init__(self):
        self.targets = []

    def add(self, x, y):
        target = Targets.Target()
        target.set(x, y)
        self.targets.append(target)

    def drop(self):
        del self.targets[-1]

    def clear(self):
        self.__init__()
