import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import matplotlib.transforms as mtransforms
import matplotlib.text as mtext

class MyLine(lines.Line2D):
    def __init__(self, *args, **kwargs):
        lines.Line2D.__init__(self, *args, **kwargs)

    def set_figure(self, figure):
        lines.Line2D.set_figure(self, figure)

    def set_axes(self, axes):
        lines.Line2D.set_axes(self, axes)

    def set_transform(self, transform):
        lines.Line2D.set_transform(self, transform)

    def set_data(self, x, y):
        lines.Line2D.set_data(self, x, y)

    def draw(self, renderer):
        lines.Line2D.draw(self, renderer)

class Point():
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Solution():
    def __init__(self, filename):
        self.filename = filename
        self.coord = dict()
        self.trucks = dict()
        self.load_file()
        self.show_file()

    def load_file(self):
        f = open(self.filename, 'r')
        read_coord = True
        for l in f:
            if(read_coord):
                line = l.split()
                if(line[0] == "COORD"):
                    continue
                if(line[0] == "TRUCKS"):
                    read_coord = False
                    continue
                self.coord[int(line[0])] = Point(int(line[1]), int(line[2]))
            else:
                line = l.split()
                self.trucks[int(line[0])] = list()
                for i in range(1, len(line)):
                    if(line[i] != "\n"):
                        self.trucks[int(line[0])].append(int(line[i]))

    def show_file(self):
        colors = ['blue', 'green', 'orange', 'red', 'purple', 'black', 'yellow', 'cyan', 'magenta', 'grey']
        fig, ax = plt.subplots()
        color = 0
        for truck in self.trucks.keys():
            x = list()
            y = list()
            x.append(self.coord[1].x)
            y.append(self.coord[1].y)
            clients = self.trucks[truck]
            for c in clients:
                x.append(self.coord[c].x)
                y.append(self.coord[c].y)

            x.append(self.coord[1].x)
            y.append(self.coord[1].y)
            line = MyLine(x, y, mfc='red', ms=12)
            line.set_color(colors[color])
            ax.add_line(line)
            color = (color + 1) % 10
        ax.set_xlim(left=None, right=100)
        ax.set_ylim(bottom=None, top=100)
        plt.show()

def main():
    s = Solution("./A-n32-k5.txt")

main()
